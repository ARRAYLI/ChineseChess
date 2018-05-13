#ifndef BOARD_H
#define BOARD_H

#include <QObject>
#include <QWidget>
#include<QFrame>
#include<QPaintEvent>
#include<QMouseEvent>

#include<QVector>
#include"Stone.h"
#include"Step.h"


class Board : public QFrame
{
    Q_OBJECT
public:
    Board(QWidget *parent = 0);
    ~Board();
public:
    // 游戏数据
    Stone _s[32];       // 32颗象棋子
    int _r;             // 棋子的半径
    QPoint _off;        // 棋子的偏移量
    bool _bSide;       // 判断红棋子在上方还是在下方

    //悔棋的步骤
    QVector<Step*> _steps;

    // 游戏状态
    int _selectid;      // 被选中棋子的 id
    bool _bRedTurn;     // 是否红色棋子被返回
    // 初始化 32 颗棋子
    void init(bool bRedSide);

    // 绘制函数
    void paintEvent(QPaintEvent *);

    // 画棋盘函数
    void drawPlate(QPainter &p);
    // 画田字格函数
    void drawPlace(QPainter &p);
    // 画棋子函数
    void drawStone(QPainter &p);
    void drawStone(QPainter &p,int id);
    // 画炮口和兵眼的符号
    void drawInitPosition(QPainter &p);
    void drawInitPosition(QPainter &p,int row,int col);

    // 功能或坐标
    // 棋子中心坐标
    QPoint center(int row,int col);
    QPoint center(int id);

    QPoint topLeft(int row,int col);
    QPoint topLeft(int id);

    QRect cell(int row,int col);
    QRect cell(int id);

    // 获取点击坐标的行和列
    bool getClickRowCol(QPoint pt, int& row, int& col);

    // 帮助函数

    // 获取棋子的名字
    QString name(int id);
    //判断棋子是否是红色棋子
    bool red(int id);
    //判断棋子是否是相同颜色
    bool sameColor(int id1,int id2);
    // 获取棋子 id
    int getStoneId(int row,int col);
    // 杀死棋子
    void killStone(int id);
    // 复活棋子
    void reliveStone(int id);
    //移动棋子
    void moveStone(int moveid,int row,int col);
    //判断棋子是否死亡
    bool isDead(int id);


    // 棋子移动函数

    // 鼠标释放函数
    void mouseReleaseEvent(QMouseEvent *ev);
    // 鼠标点击事件
    void click(QPoint pt);
    // 虚函数鼠标点击事件
    virtual void click(int id,int row,int col);
    // 尝试选中棋子
    void trySelectStone(int id);
    // 尝试移动棋子
    void tryMoveStone(int killid,int row,int col);
    // 移动棋子
    void moveStone(int moveid,int killid,int row,int col);
    // 保存悔棋的步骤
    void saveStep(int moveid,int killid,int row,int col,QVector<Step*> &steps);
    // 后退一步
    void backOne();
    // 悔棋
    void back(Step *step);
    // 虚函数悔棋
    virtual void back();

    // 走棋规则

    // 判断棋子是否可以移动
    bool canMove(int moveid,int killid,int row,int col);
    // 判断车是否可以移动
    bool canMoveChe(int moveid,int killid,int row,int col);
    // 判断马是否可以移动
    bool canMoveMa(int moveid,int killid,int row,int col);
    // 判断炮是否可以移动
    bool canMovePao(int moveid,int killid,int row,int col);
    // 判断兵是否可以移动
    bool canMoveBing(int moveid,int killid,int row,int col);
    // 判断将是否可以移动
    bool canMoveJiang(int moveid,int killid,int row,int col);
    // 判断士是否可以移动
    bool canMoveShi(int moveid,int killid,int row,int col);
    // 判断象是否可以移动
    bool canMoveXiang(int moveid,int killid,int row,int col);
    // 判断是否可能被选择
    bool canSelect(int id);

    // 规则助手函数

    // 判断棋子是否在直线上可以移动
    int getStoneCountAtLine(int row1,int col1,int row2,int col2);
    // 判断位置之间的关系
    int relation(int row1,int col1,int row2,int col2);
    // 判断棋子是否在底边
    bool isBottomSide(int id);

    // 判断游戏是否结束

    int _isRedWin;     // -1 表示平局   0 表示红棋输了  1 表示红棋赢了
    // 判断红棋是否获胜
    int isRedWin();
    // 显示游戏是否结束
    void isGameOver();

public slots:
    // 返回按钮的槽函数
     void slotBack();
signals:
   //  void sigClose();
};

#endif // BOARD_H
