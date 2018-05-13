#ifndef STONE_H
#define STONE_H
#include<QRect>
#include<QPainter>
#include<QString>

//棋子类
class Stone
{
public:
    Stone();
    ~Stone();
public:
    // 棋子类型的枚举
    enum TYPE{CHE,MA,PAO,BING,JIANG,SHI,XIANG};

    // 初始化棋子
    void init(int id);

    // 获取棋子的名字
    QString name();

    // 棋盘反转
    void rotate();

    // 棋子的行
    int _row;
    // 棋子的列
    int _col;
    // 棋子的类型
    TYPE _type;
    // 棋子是否死亡
    bool _dead;
    // 棋子是否是红棋子
    bool _red;
    // 棋子的 Id
    int _id;

};

#endif // STONE_H
