#ifndef MULTIGAME_H
#define MULTIGAME_H
#include "Board.h"
#include<QWidget>
// 多人游戏类
class MultiGame : public Board
{
    Q_OBJECT
public:
    MultiGame(QWidget *parent = 0);
    ~MultiGame();
};

#endif // MULTIGAME_H
