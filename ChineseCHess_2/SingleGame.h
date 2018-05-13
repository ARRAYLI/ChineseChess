/*
#ifndef SINGLEGAME_H
#define SINGLEGAME_H
#include "Board.h"

class SingleGame : public Board
{
    Q_OBJECT
public:
    explicit SingleGame(QWidget *parent = 0);
    ~SingleGame();

    int _level;

    void back();
    void click(int id, int row, int col);

    Step* getBestMove();
    void getAllPossibleMove(QVector<Step*>& steps);
    int getMinScore(int level, int curMin);
    int getMaxScore(int level, int curMax);
    int score();

    void fakeMove(Step* step);
    void unfakeMove(Step* step);

signals:

public slots:
    void computerMove();
};

#endif // SINGLEGAME_H

*/

#ifndef SINGLEGAME_H
#define SINGLEGAME_H

#include "Board.h"
#include "Step.h"


class SingleGame : public Board
{
    Q_OBJECT
public:
    SingleGame(QWidget *parent =0);
    ~SingleGame();

public:

    // 递归层数（告诉电脑考虑走棋的层次）
    int _level;
    // 重写基类的悔棋函数
    void back();
    // 重写基类的鼠标点击事件
    virtual void click(int id,int row,int col);

    // 电脑获取最好的走棋步骤
    Step* getBestMove();
    // 获取所有的走棋步骤
    void getAllPossibleMove(QVector<Step*> &steps);
    // 获取得分
    int calcSocre();
    // 获取电脑假假移动后，人也假假移动,获取移动后的最好局面得分，即获取电脑的最小得分
    int getMinScore(int level,int curMin);
    int getMaxScore(int level,int cutMax);

    // 电脑假装移动
    void fakeMove(Step* step);
    // 电脑假装回退
    void unfakeMove(Step* step);

public slots:
    void computerMove();

};

#endif // SINGLEGAME_H
