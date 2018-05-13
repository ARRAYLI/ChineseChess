#include "Stone.h"

Stone::Stone()
{

}

Stone::~Stone()
{

}
// 初始化棋子
void Stone::init(int id)
{
    struct {
        int row;        // 棋子的行
        int col;        // 棋子的列
        Stone::TYPE type;       // 棋子的类型
    }pos[16]={
    {0,0,Stone::CHE},
    {0,1,Stone::MA},
    {0,2,Stone::XIANG},
    {0,3,Stone::SHI},
    {0,4,Stone::JIANG},
    {0,5,Stone::SHI},
    {0,6,Stone::XIANG},
    {0,7,Stone::MA},
    {0,8,Stone::CHE},

    {2,1,Stone::PAO},
    {2,7,Stone::PAO},
    {3,0,Stone::BING},
    {3,2,Stone::BING},
    {3,4,Stone::BING},
    {3,6,Stone::BING},
    {3,8,Stone::BING},
};
    // 如果棋子的 id<16 的话，那么棋子在棋盘的上方
    if(id<16)
    {
        this->_row = pos[id].row;
        this->_col = pos[id].col;
        this->_type = pos[id].type;
    }
    else
    {
        // 如果棋子的 id >16 ，那么棋子在棋盘的下部分
        // 注意这里 列是 9 列，行是 9 行
        this->_row = 9-pos[id-16].row;
        this->_col = 8-pos[id-16].col;
        this->_type = pos[id-16].type;
    }

    // 每个棋子初始化为或者，不是死棋子
    this->_dead = false;
    // 判断当前棋子是否是红棋子， id < 16 位红色棋子， id > 16 为黑色棋子
    this->_red = id<16;
}
// 获取棋子的名字
QString Stone::name()
{
    switch (this->_type) {
    case Stone::CHE:
        return "车";
    case Stone::MA:
        return "马";
    case Stone::PAO:
        return "炮";
    case Stone::BING:
        return "兵";
    case Stone::JIANG:
        return "将";
    case Stone::SHI:
        return "仕";
    case Stone::XIANG:
        return "象";
    }
    return "错误";
}
 // 棋盘反转
void Stone::rotate()
{
    this->_row = 9-this->_row;
    this->_col = 8 -this->_col;
}

