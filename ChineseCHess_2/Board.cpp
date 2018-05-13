#include "Board.h"
#include<QVector>
#include<QDebug>
#include<QMessageBox>

//定义宏，通过宏来获取 id 棋子的 row 和 col
#define GetRowCol(__row,__col,__id)    \
    int __row = _s[__id]._row;\
    int __col = _s[__id]._col;


Board::Board(QWidget *parent):QFrame(parent)
{
    // 初始化棋子半径
    this->_r = 20;
    // 固定窗口大小
    setMinimumSize(_r*18+1, _r*20+1);
    // 初始化 32 颗棋子
    init(true);

    // 初始化输赢
    _isRedWin = -1;
}

Board::~Board()
{

}
// 初始化 32 颗棋子
void Board::init(bool bRedSide)
{
    for(int i=0;i<32;i++)
    {
        _s[i].init(i);
    }

    if(bRedSide)
    {
        // 如果红色棋子在下面的话，则反转棋盘
        for(int i=0;i<32;i++)
        {
            _s[i].rotate();
        }
    }
    // 初始化被选中的棋子为 -1 ,即刚开始没有棋子被选中
    _selectid = -1;
    // 红棋子被返回
    _bRedTurn = true;
    // 红色棋子在下面
    _bSide = bRedSide;
    // 重绘界面
    update();
}
// 绘制函数
void Board::paintEvent(QPaintEvent *)
{
    int r = height()/20;
    // 获取棋子的半径
    _r = r;
    // 偏移量
    _off = QPoint(r+1,r+1);

    QPainter p(this);
    p.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    // 绘制棋盘
    p.save();
    drawPlate(p);
    p.restore();

    // 绘制田字格函数
    p.save();
    drawPlace(p);
    p.restore();

    // 绘制炮眼和兵眼
    p.save();
    drawInitPosition(p);
    p.restore();
    // 绘制棋子
    p.save();
    drawStone(p);
    p.restore();

}
// 画棋盘函数
void Board::drawPlate(QPainter &p)
{
    for(int i=0;i<10;i++)
    {
        // 如果画棋盘的第一行和最后一行的话，线条颜色加粗
        if(i == 0 || i == 9)
        {
            p.setPen(QPen(Qt::black, 3, Qt::SolidLine));
        }
        else
        {
            p.setPen(QPen(Qt::black, 1, Qt::SolidLine));
        }
        // 画线条
        p.drawLine(center(i,0),center(i,8));
    }

    // 画棋盘的 9 列
    for(int i=0;i<9;i++)
    {
        // 如果是第一列或者第九列，线条加粗
        if(i == 0|| i == 8)
        {
            p.setPen(QPen(Qt::black, 3, Qt::SolidLine));
            p.drawLine(center(0, i), center(9, i));
        }
        // 如果是其他的列，则在第 4 和 第 5 行之间不画竖线
        else
        {
            p.setPen(QPen(Qt::black, 1, Qt::SolidLine));
            p.drawLine(center(0, i), center(4, i));
            p.drawLine(center(5, i), center(9, i));
        }

    }

}
// 画田字格函数
void Board::drawPlace(QPainter &p)
{
    // 黑棋的田字格
    p.drawLine(center(0,3),center(2,5));
    p.drawLine(center(0,5),center(2,3));

    // 红棋的田字格
    p.drawLine(center(7,3),center(9,5));
    p.drawLine(center(9,3),center(7,5));
}
// 画棋子函数
void Board::drawStone(QPainter &p)
{
    // 画 32 颗棋子
    for(int i=0;i<32;i++)
    {
        drawStone(p,i);
    }
}

void Board::drawStone(QPainter &p, int id)
{
    // 判断棋子是否死亡
    if(isDead(id))   return;

    QColor color;
    if(red(id)) color = Qt::red;
    else    color = Qt::black;

    // 设置画刷
    p.setPen(QPen(QBrush(color),2));
    if(id == _selectid) p.setBrush(Qt::gray);
    else    p.setBrush(Qt::yellow);

    // 画棋子
    p.drawEllipse(cell(id));

    // 设置字体
    p.setFont(QFont("system", _r*1.2, 700));
    // 把棋子的汉子写上去
    p.drawText(cell(id),name(id),QTextOption(Qt::AlignCenter));
}
// 画炮口和兵眼的符号
void Board::drawInitPosition(QPainter &p)
{
    // 黑棋的炮眼
    drawInitPosition(p,2,1);
    drawInitPosition(p,2,7);
    // 黑棋的兵眼
    drawInitPosition(p,3,0);
    drawInitPosition(p,3,2);
    drawInitPosition(p,3,4);
    drawInitPosition(p,3,6);
    drawInitPosition(p,3,8);

    // 红棋的炮眼
    drawInitPosition(p,7,1);
    drawInitPosition(p,7,7);
    // 红棋的兵眼
    drawInitPosition(p,6,0);
    drawInitPosition(p,6,2);
    drawInitPosition(p,6,4);
    drawInitPosition(p,6,6);
    drawInitPosition(p,6,8);
}

void Board::drawInitPosition(QPainter &p, int row, int col)
{
    // 获取当前行和列的坐标
    QPoint pt = center(row,col);
    // 偏移量
    QPoint off = QPoint(_r/6,_r/6);
    int len = _r/3;

    QPoint pStart;
    QPoint pEnd;

    // 如果不是第一列的话
    if(col != 0)
    {
        // 画炮眼和兵眼的左上角
        pStart = QPoint(pt.x()-off.x(),pt.y()-off.y());
        pEnd = pStart + QPoint(-len,0);
        p.drawLine(pStart,pEnd);
        pEnd = pStart + QPoint(0,-len);
        p.drawLine(pStart,pEnd);

        // 画炮眼和兵眼的左下角
        pStart = QPoint(pt.x()-off.x(),pt.y()+off.y());
        pEnd = pStart + QPoint(-len,0);
        p.drawLine(pStart,pEnd);
        pEnd = pStart + QPoint(0,len);
        p.drawLine(pStart,pEnd);
    }
    // 如果不是最后一列的话
    if(col != 8)
    {
        // 画炮眼和兵眼的右上角
        pStart = QPoint(pt.x()+off.x(),pt.y()-off.y());
        pEnd = pStart + QPoint(len,0);
        p.drawLine(pStart,pEnd);
        pEnd = pStart + QPoint(0,-len);
        p.drawLine(pStart,pEnd);

        // 画炮眼和兵眼的右下角
        pStart = QPoint(pt.x()+off.x(),pt.y()+off.y());
        pEnd = pStart + QPoint(len,0);
        p.drawLine(pStart,pEnd);
        pEnd = pStart + QPoint(0,len);
        p.drawLine(pStart,pEnd);
    }
}

QPoint Board::center(int row, int col)
{
    QPoint pt(_r*2*col,_r*2*row);
    return pt+_off;
}

QPoint Board::center(int id)
{
    return center(_s[id]._row,_s[id]._col);
}

QPoint Board::topLeft(int row, int col)
{
    return center(row,col)-QPoint(_r,_r);
}

QPoint Board::topLeft(int id)
{
    return center(id)-QPoint(_r,_r);
}

QRect Board::cell(int row, int col)
{
    return QRect(topLeft(row,col),QSize(_r*2-1,_r*2-1));
}

// 获取棋子所在矩形的左上角坐标
QRect Board::cell(int id)
{
    return   QRect(topLeft(id),QSize(_r*2-1,_r*2-1));
}
// 获取点击坐标的行和列
bool Board::getClickRowCol(QPoint pt, int &row, int &col)
{
    for(row = 0;row<10;row++)
    {
        for(col = 0;col<9;col++)
        {
            QPoint distance = center(row,col) - pt;
            // 如果鼠标点中的位置在棋子的范围之内，则返回 true;
            if(distance.x()*distance.x()+distance.y()*distance.y()<_r*_r)
                return true;
        }
    }
    // 否则，返回 false;
    return false;
}
// 获取棋子的名字
QString Board::name(int id)
{
    return _s[id].name();
}
//判断棋子是否是红色棋子
bool Board::red(int id)
{
    return _s[id]._red;
}
//判断棋子是否是相同颜色
bool Board::sameColor(int id1, int id2)
{
    if(id1 == -1 || id2 == -1) return false;
    return red(id1) == red(id2);
}
// 获取棋子 id
int Board::getStoneId(int row, int col)
{
    for(int i=0;i<32;i++)
    {
        if(_s[i]._row == row && _s[i]._col == col && !isDead(i))
            return i;
    }
    return -1;
}
// 杀死棋子
void Board::killStone(int id)
{
    if(id == -1) return;
    _s[id]._dead = true;
}
// 复活棋子
void Board::reliveStone(int id)
{
    // 如果棋子的 id 不存在的话，退出
    if(id == -1)    return;
    // 把棋子置活
    _s[id]._dead = false;
}
//移动棋子
void Board::moveStone(int moveid, int row, int col)
{
    _s[moveid]._row = row;
    _s[moveid]._col  = col;

    _bRedTurn = !_bRedTurn;
}
//判断棋子是否死亡
bool Board::isDead(int id)
{
    if(id == -1)
        return true;
    return _s[id]._dead;
}
// 鼠标释放函数
void Board::mouseReleaseEvent(QMouseEvent *ev)
{  // 判断是否是鼠标左键释放

    if(ev->button() != Qt::LeftButton)
    {
        return;
    }
    click(ev->pos());

}

// 鼠标点击事件
void Board::click(QPoint pt)
{
    int row,col;
    // 通过鼠标点击的坐标来判断棋子是否被点击中
    bool bClicked = getClickRowCol(pt,row,col);
    // 如果棋子没有被选中
    if(!bClicked)  return;

    // 获取棋子的 id
    int id = getStoneId(row,col);

    click(id,row,col);
}
// 虚函数鼠标点击事件
void Board::click(int id, int row, int col)
{
    // 如果没有选中的棋子
    if(this->_selectid == -1)
    {
        // 则尝试着选中棋子
        trySelectStone(id);
    }
    // 如果有选中的棋子，则尝试着移动棋子
    else
    {
        tryMoveStone(id,row,col);
    }
}
// 尝试选中棋子
void Board::trySelectStone(int id)
{
    // 如果没有选中棋子
    if(id == -1)
        return;
    // 判断棋子是否肯能被选中
    if(!canSelect(id)) return;

    _selectid = id;
    update();

}
// 尝试移动棋子
void Board::tryMoveStone(int killid, int row, int col)
{
    // 如果被杀死的棋子存在并且 棋子的颜色相同，则重新选择棋子
    if(killid != -1 && sameColor(killid,_selectid))
    {
        trySelectStone(killid);
        return;
    }
    // 判断棋子是否可以移动
    bool ret = canMove(_selectid,killid,row,col);
    if(ret)
    {
        moveStone(_selectid,killid,row,col);
        _selectid = -1;
        update();
    }

}
// 移动棋子
void Board::moveStone(int moveid, int killid, int row, int col)
{
    // 保存走棋不中欧
    saveStep(moveid,killid,row,col,_steps);

    //   isGameOver();
    // 杀死棋子
    killStone(killid);
    // 移动棋子
    moveStone(moveid,row,col);
    isGameOver();
}
// 保存悔棋的步骤
void Board::saveStep(int moveid, int killid, int row, int col, QVector<Step *> &steps)
{
    //获取要移动棋子的行和列
    GetRowCol(row1,col1,moveid);

    Step* step = new Step;
    step->_colFrom = col1;
    step->_colTo = col;

    step->_rowFrom = row1;
    step->_rowTo = row;

    step->_killid = killid;
    step->_moveid = moveid;

    steps.append(step);
}
// 后退一步
void Board::backOne()
{
    //  qDebug()<<"backOne() 333 start";
    // qDebug()<<"_steps.size = "<<_steps.size();
    // 如果容器中没有悔棋步骤的话
    if( this->_steps.size() == 0) return;

    // 获取容器中最后一不走棋
    Step *step = _steps.last();
    // 并删除容器中最后一步悔棋步骤
    _steps.removeLast();

    //   qDebug()<<"setp->_moveid = "<<step->_moveid<<",step->_killid = "<<step->_killid<<","
    //   "step->_rowTo = "<<step->_rowTo<<",step->_colTo = "<<step->_colTo<<",step->_rowFrom = "
    //   <<step->_rowFrom<<"step->_rowCol" << step->_colTo;
    // 按照悔棋的步骤，进行悔棋
    back(step);

    // 更新界面
    update();

    // 释放悔棋后的悔棋步骤
    delete step;
    //    qDebug()<<"backOne() 333 end";
}
// 悔棋
void Board::back(Step *step)
{
    // qDebug()<<"back 22222";
    // 复活别杀死的棋子
    reliveStone(step->_killid);
    //移动棋子到原来的位置
    moveStone(step->_moveid,step->_rowFrom,step->_colFrom);
}
// 虚函数悔棋
void Board::back()
{
    // qDebug()<<"back 222222222";
    // 悔一步棋
    backOne();
}
// 判断棋子是否可以移动
bool Board::canMove(int moveid, int killid, int row, int col)
{
    // 如果移动和被杀死的棋子相同的话，则 返回 true
    if(sameColor(moveid,killid)) return false;

    switch(_s[moveid]._type)
    {
    case Stone::CHE:
        return canMoveChe(moveid,killid,row,col);
    case Stone::MA:
        return canMoveMa(moveid,killid,row,col);
    case Stone::PAO:
        return canMovePao(moveid,killid,row,col);
    case Stone::BING:
        return canMoveBing(moveid,killid,row,col);
    case Stone::JIANG:
        return canMoveJiang(moveid,killid,row,col);
    case Stone::SHI:
        return canMoveShi(moveid,killid,row,col);
    case Stone::XIANG:
        return canMoveXiang(moveid,killid,row,col);
    }
    return false;
}
// 判断车是否可以移动
bool Board::canMoveChe(int moveid, int killid, int row, int col)
{
    // 获取 id 处棋子的 行和列
    GetRowCol(row1,col1,moveid);

    // 判断棋子当前的行和列移动到鼠标点中位置的行和列是否在一行上。如果在一行上可以移动，
    // 不在一行上不可以移动

    int ret = getStoneCountAtLine(row1,col1,row,col);
    if(ret == 0)
        return true;
    return false;
}
// 判断马是否可以移动
bool Board::canMoveMa(int moveid, int killid, int row, int col)
{
    // 获取 id 处棋子的 行和列
    GetRowCol(row1,col1,moveid);
    // 获取走棋规则的值
    int r = relation(row1,col1,row,col);
    // 马移动的规则是：1.马走斜日
    //               2.马走斜日换算成距离的话，是 12 或者 21
    if(r != 12 && r != 21)
        return false;

    if(r == 12)
    {
        // 判断是否別马腿(横着跳马)
        if(getStoneId(row1,(col1+col)/2) != -1)
            return false;
    }
    else
    {
        // 判断是否別马腿(竖着跳马)
        if(getStoneId((row1+row)/2,col1) != -1)
            return false;
    }
    return true;
}
// 判断炮是否可以移动
bool Board::canMovePao(int moveid, int killid, int row, int col)
{
    // 获取 id 处棋子的 行和列
    GetRowCol(row1,col1,moveid);

    // 判断炮在直线上移动时，有几个棋子在移动方向上的前面
    int ret = getStoneCountAtLine(row,col,row1,col1);

    // 如果被杀的棋子没有死的话
    if(killid != -1)
    {
        // 如果炮杀死对方棋子的直线上有一个棋子在前面，则跳过这个棋子
        if(ret == 1) return  true;
    }
    else
    {
        // 如果目标棋子已经是死亡状态，并且炮的移动方向上没有棋子，可以直线移动
        if(ret == 0) return true;
    }
    return false;
}
// 判断兵是否可以移动
bool Board::canMoveBing(int moveid, int killid, int row, int col)
{
    // 获取 id 处棋子的 行和列
    GetRowCol(row1,col1,moveid);
    // 获取走棋规则的值
    int ret = relation(row1,col1,row,col);
    //兵的走棋规则：1.兵一次只能走一步
    //            2.兵不能后退，指向向前走
    //            3.兵如果没有过河的话，只能向前走，过河后可以左右移动，但不能后退 1 或者 10

    if(ret != 1 && ret != 10) return false;

    // 如果是下方的棋子
    if(isBottomSide(moveid))
    {
        // 棋子不能向后移动
        if(row>row1) return false;
        // 棋子不能左右移动
        if(row1>=5 && row == row1) return false;
    }
    // 如果是上方的棋子
    else
    {
        // 棋子不能向后移动
        if(row<row1) return false;
        // 上方的棋子不能向左右移动
        if(row1<=4 && row1 == row) return false;
    }
    return true;
}
// 判断将是否可以移动
bool Board::canMoveJiang(int moveid, int killid, int row, int col)
{

    //首先判断将是否活着,如果两个向相互见面，按车的走棋规则吃掉对方的将
    if(killid != -1 && _s[killid]._type == Stone::JIANG)
        return canMoveChe(moveid,killid,row,col);
    // 连个将之间有棋子挡着
    GetRowCol(row1,col1,moveid);
    // 获取走棋规则的值
    int ret = relation(row1,col1,row,col);
    // 将的走棋规则：1.只能在田字格内走棋
    //             2.每次只能走一步，并且不走走斜线 1 或者 10
    if( ret != 1 && ret != 10) return false;
    // 判断棋子是否在田字格内
    if(col <3 || col>5) return false;

    // 棋子在下面的话
    if(isBottomSide(moveid))
    {
        // 如果棋子在下方，并且棋子所在的行不能小于 7
        if(row<7) return false;
    }
    else
    {
        // 如果棋子在上方，并且棋子所在的行不能大于 2
        if(row>2) return false;
    }
    return true;
}
// 判断士是否可以移动
bool Board::canMoveShi(int moveid, int killid, int row, int col)
{
    // 获取 id 处棋子的 行和列
    GetRowCol(row1,col1,moveid);

    // 获取走棋规则的值
    int ret = relation(row1,col1,row,col);
    // 仕的走棋规则： 1.仕一次只能走一步
    //              2.仕只能在田字格内走棋，并且只能走斜线 11
    if(ret != 11) return false;

    if(col<3||col>5) return false;
    // 判断棋子是否在下方
    if(isBottomSide(moveid))
    {
        if(row<7) return false;

    }
    else
    {
        if(row>2) return false;
    }

    return true;
}
// 判断象是否可以移动
bool Board::canMoveXiang(int moveid, int killid, int row, int col)
{
    // 获取 id 处棋子的 行和列
    GetRowCol(row1,col1,moveid);

    // 获取走棋规则的值
    int ret = relation(row1,col1,row,col);
    // 象的走棋规则是: 1.象飞田字格  22
    //               2.判断象眼是否有棋子
    //               3. 象不能过河
    // 判断象是否是飞田字格
    if( ret != 22) return false;
    // 判断象眼是否有棋子
    int rEye = (row+row1)/2;
    int cEye = (col+col1)/2;
    if(getStoneId(rEye,cEye) != -1)
        return false;

    // 如果象在下边的话，象的行不能小于 4
    if(isBottomSide(moveid))
    {
        if(row<4) return false;

    }
    // 如果象在上边的话，象的行不能大于 5
    else
    {
        if(row>5) return false;
    }


    return true;

}
// 判断是否可能被选择
bool Board::canSelect(int id)
{
    return _bRedTurn == _s[id]._red;
}
// 判断棋子是否在直线上可以移动
int Board::getStoneCountAtLine(int row1, int col1, int row2, int col2)
{
    int ret =0;
    // 如果棋子所在的行和列，和目标位置的行和列不在一条直线上则 出错，返回 -1
    if(row1 != row2 && col1 != col2)
        return -1;
    // 如果当前棋子的行和列，和目标位置的行和列一样，则出错，返回 -1
    if(row1 == row2 && col1 == col2)
        return -1;
    // 如果棋子所在的行和目标位置的行相同
    if(row1 == row2)
    {
        // 获取棋子所在列和目标位置所在列的最小列值
        int min = col1 < col2 ? col1 : col2;

        // 获取棋子所在列和目标位置所在列的最大列值
        int max = col1 < col2 ? col2 : col1;

        // 循环遍历最小列值和最大列值之间是否有棋子或者障碍
        for(int col = min+1;col<max;col++)
        {
            if(getStoneId(row1,col) != -1) ++ret;
        }

    }
    else
    {
        // 获取棋子所在列和目标位置所在列的最小列值
        int min = row1 < row2 ? row1 : row2;

        // 获取棋子所在列和目标位置所在列的最大列值
        int max = row1 < row2 ? row2 : row1;

        // 循环遍历最小列值和最大列值之间是否有棋子或者障碍
        for(int row = min+1;row<max;row++)
        {
            if(getStoneId(row,col1) != -1) ++ret;
        }
    }
    return ret;
}
// 判断位置之间的关系
int Board::relation(int row1, int col1, int row2, int col2)
{
    return qAbs(row1-row2)*10+qAbs(col1-col2);
}
// 判断棋子是否在底边
bool Board::isBottomSide(int id)
{
    return _bSide == _s[id]._red;
}
// 判断红棋是否获胜
int Board::isRedWin()
{
    int ret = -1;           // -1 表示平局     0 表示红棋输了      1 表示红棋赢了
    int _Red = 1;
    int _Black = 1;
    // 判断红棋的 将是否还在 ，若不在，则红棋输了
    for(int i=0;i<16;i++)
    {
        if(_s[i]._type == Stone::JIANG && isDead(i))
        {
            // 返回红棋失败
            _Red = 0;
            break;
        }
        else
            continue;
    }

    // 如果红棋的将没有死亡的话，判断黑棋的将有没有死亡
    for(int i=16;i<32;i++)
    {
        if(_s[i]._type == Stone::JIANG && isDead(i))
        {
            _Black = 0;
            break;
        }
        else
            continue;

    }
    // 0 代表红棋输了，黑棋赢了
    if(_Red == 0 && _Black == 1)
    {
        ret = 0;
    }
    else if(_Red == 1 && _Black == 0)
    {
        ret = 1;
    }
    else
    {
        ret = -1;
    }
    return ret;
}
// 显示游戏是否结束
void Board::isGameOver()
{
    int ret = isRedWin();
    /*
    if(ret == 0 || ret == 1)
    {
        if( ret == 0)
            QMessageBox::information(this,"游戏结果","红棋赢了");
        else
            QMessageBox::information(this,"游戏结果","黑棋赢了");
        // 退出程序
        this->close();
    }
    else
        return;
        */
    // 红棋输了
    if(ret == 0)
    {
        QMessageBox::information(NULL,"游戏结果","黑棋赢了了，红棋输了"/*,QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes*/);

        //connect(this,SIGNAL(close()),this,SLOT(CloseWnd()));
        // 点击关闭按钮退出程序
        //this->close();

    }
    // 红棋赢了
    else if(ret == 1)
    {
        QMessageBox::information(NULL,"游戏结果","红棋棋赢了，黑棋输了"/*,QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes*/);
        // connect(this,SIGNAL(close()),this,SLOT(CloseWnd()));
        //this->close();
    }
    // 平局
    else
    {
        return;
    }
}
// 返回按钮的槽函数
void Board::slotBack()
{
    //  qDebug()<<"111111111111";
    back();
}

