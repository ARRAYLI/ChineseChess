#ifndef NETGAME_H
#define NETGAME_H
#include "Board.h"

#include<QTcpServer>
#include<QTcpSocket>

/*
    1.执红方还是黑方，这个信息由服务器发出，客户单接收

        第一个字节固定是 1，第二个字节是 1 或者是 0，1表示接收方走红棋，0表示接收方走黑棋

    2.点击信息，
        第一个字节固定是 2 ，第二个字节是坐标行，第三个字节是坐标列，第四个字节是点击的 棋子 id
*/

class NetGame : public Board
{
    Q_OBJECT
public:
    NetGame(bool server,QWidget *parent = 0);
    ~NetGame();
public slots:
    // 处理新的连接请求
    void slotNewConnection();
    // 接收数据
   void slotRecv();

 //   void slotDataArrive();

public:
    // 重写基类中的悔棋函数
    void back();
    // 重写基类中的鼠标点击事件
    void click(int id, int row, int col);

    // 网络对战中悔棋
    void backFromNetwork(QByteArray buf);
    // 网络对战中点击棋子
    void clickFromNetwork(QByteArray buf);
    // 初始化网络对战环境
    void initFromNetwork(QByteArray buf);

public:
    QTcpServer *_server;         // 作为服务器，_server 和 _socket 都要使用
    QTcpSocket *_socket;          // 作为客户端只需要使用 _socket
    bool    _bServer;            // 判断本程序时作为服务端启动还是以客户端启动
};

#endif // NETGAME_H
