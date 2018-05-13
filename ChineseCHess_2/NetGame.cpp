#include "NetGame.h"
#include<QDebug>
NetGame::NetGame(bool server,QWidget *parent):Board(parent)
{
    // 初始化
    _server = NULL;
    _socket = NULL;
    _bServer = server;
    // 如果是服务器端被创建
    if(_bServer)
    {
        _server = new QTcpServer(this);

        // 监听任何网卡的 9999 端口
        _server->listen(QHostAddress::Any,9999);

        // 如果收到客户端的连接请求的话，执行 slotNewConnection() 函数来处理链接请求
        connect(_server,SIGNAL(newConnection()),this,SLOT(slotNewConnection()));
    }
    // 作为客户端访问
    else
    {
        // 创建通信的 socket
        _socket = new QTcpSocket(this);
        // 连接服务器
        _socket->connectToHost(QHostAddress("127.0.0.1"),9999);

        connect(_socket,SIGNAL(readyRead()),this,SLOT(slotRecv()));
    }
}

void NetGame::slotNewConnection()
{
    // 只能两个人下棋，不能两个人以上玩
    if(_socket != NULL) return;
    // 接收客户端请求连接
    _socket = _server->nextPendingConnection();
    // 服务端接收客户端发送过来的数据
    connect(_socket,SIGNAL(readyRead()),this,SLOT(slotRecv()));

    qDebug()<<"connect";

    /* 产生随机数来决定谁走红色 */
    bool bRedSide = qrand()%2>0;
    init(bRedSide);

    /* 给对方发送数据*/
    QByteArray buf;
    buf.append(1);
    buf.append(bRedSide?0:1);

    //服务端给客户端发送，服务端执红棋还是黑棋
    _socket->write(buf);

    // init(buf[1] == 0);
}

// 重写基类中的悔棋函数
void NetGame::back()
{
    if(_bRedTurn != _bSide)
        return;
    backOne();
    backOne();

    QByteArray buf;
    buf.append(3);
    _socket->write(buf);
}

// 重写基类中的鼠标点击事件
void NetGame::click(int id, int row, int col)
{
    // 如果不是红棋在下方，则退出
    if(_bRedTurn != _bSide)
        return;
    Board::click(id,row,col);

    // 给对方发送数据
    QByteArray buf;
    buf.append(2);
    buf.append(id);
    buf.append(row);
    buf.append(col);
    _socket->write(buf);
}


// 网络对战中悔棋
void NetGame::backFromNetwork(QByteArray /*buf*/)
{
    backOne();
    backOne();
}
// 网络对战中点击棋子
void NetGame::clickFromNetwork(QByteArray buf)
{
  //  Board::click(buf[1],9-buf[2],8-buf[3]);
    Board::click(buf[1],9-buf[2],8-buf[3]);
}
// 初始化网络对战环境
void NetGame::initFromNetwork(QByteArray buf)
{
    // 根据网络中传过来的第一个字节判断，是服务器执行黑棋子，在下方。客户端执行红棋，在下方
    bool bRedSide = buf.at(1)>0?true:false;
    init(bRedSide);
}

void NetGame::slotRecv()
{
    QByteArray buf = _socket->readAll();
    // 根据接收到的数据 的第一个字节判断是
    switch(buf.at(0))
    {
    case 1:
        initFromNetwork(buf);
        break;
    case 2:
        clickFromNetwork(buf);
        break;
    case 3:
        backFromNetwork(buf);
        break;
    default:
        break;

    }
}


NetGame::~NetGame()
{

}
