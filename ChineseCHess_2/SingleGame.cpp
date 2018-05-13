#include "SingleGame.h"
#include "Step.h"
#include<QTimer>

SingleGame::SingleGame(QWidget *parent):Board(parent)
{
    _level = 4;
}

SingleGame::~SingleGame()
{

}
// 重写基类的悔棋函数
void SingleGame::back()
{
    // 如果
    if(_bRedTurn)
    {
        backOne();
        backOne();
    }
}
// 重写基类的鼠标点击事件
void SingleGame::click(int id, int row, int col)
{
    // 如果是黑棋走棋，点击了也没有用
    if(!this->_bRedTurn)  return;

    // 直接调用父类的 click(int id,int row,int col) 函数
    Board::click(id,row,col);
    //轮到黑棋走棋
    if(!this->_bRedTurn)
    {
        /* 启动 0.1s 定时器,在 0.1s 后电脑开始思考 */
        QTimer::singleShot(100,this,SLOT(computerMove()));

    }
}


// 电脑走棋
void SingleGame::computerMove()
{
    // 电脑获取最好的走棋步骤
    Step *step = getBestMove();
    // 电脑开始走棋
    moveStone(step->_moveid,step->_killid,step->_rowTo,step->_colTo);

    delete step;
    update();
}

// 电脑获取最好的走棋步骤
Step *SingleGame::getBestMove()
{
    /*
     *  1.看看有哪些步骤可以走
     *  2.试着走一下
     *  3.评估走的结果
     *  4.取最好的结果作为参考
     */

     Step *ret = NULL;
    // 1.看看有哪些步骤可以走
    // 定义容器，保存棋子的走棋步骤
    QVector<Step*> steps;

    // 获取所有的走棋步骤
    getAllPossibleMove(steps);

    int maxInAllMinScore = -300000;
    //2.试着走一下
    //3.评估走的结果
    while(steps.count())
    {
        // 从容器中获取当前的走棋步骤
        Step *step = steps.last();
        steps.removeLast();
        // 电脑假装走棋，就像老人下棋时的指指点点
        fakeMove(step);

        // 获取电脑假装走棋后的局面得分
        int minScore = getMinScore(this->_level-1,maxInAllMinScore);
        // 走棋后，回退到之前的棋子位置
        unfakeMove(step);
        if(minScore > maxInAllMinScore)
        {
            if(ret)  delete ret;
            ret = step;
            maxInAllMinScore = minScore;

        }
        else
        {
            delete step;
        }

    }
    // 取最好的结果作为参考
    return ret;

    // 电脑获取最好的走棋步骤
    //  Step *step = getBestMove();
    // 电脑开始走棋
    // moveStone(step->_moveid,step->_killid,step->_rowTo,step->_colTo);
}

// 获取得分
int SingleGame::calcSocre()
{
    //  enum TYPE{CHE,MA,PAO,BING,JIANG,SHI,XIANG};

    // 设置静态的棋子分数
    static int s[] = {1000,499,501,200,15000,100,100};

    // 黑棋得分
    int scoreBlack = 0;
    // 红棋得分
    int scoreRed = 0;

    for(int i=0;i<16;i++)
    {
        // 如果棋子是死棋的话，继续循环
        if(_s[i]._dead)  continue;
        // 计算红棋的所有棋子的总分
        scoreRed += s[_s[i]._type];
    }
    for(int i=16;i<32;i++)
    {
        // 如果棋子是死棋的话，继续循环
        if(_s[i]._dead)  continue;
        // 计算黑棋的所有棋子的总分
        scoreBlack += s[_s[i]._type];
    }

    // 计算黑棋得分和红棋得分之差
    return scoreBlack- scoreRed;
}

// 获取电脑假假移动后，人也假假移动,获取移动后的最好局面得分，即获取电脑的最小得分
int SingleGame::getMinScore(int level,int curMin)
{
    if(level == 0)
        return calcSocre();
    // 1.看看有哪些步骤可以走
    // 定义容器，保存棋子的走棋步骤
    QVector<Step*> steps;

    // 获取所有的走棋步骤
    getAllPossibleMove(steps);   // 是红棋的 possibleMove()
    int minInAllMaxScore = 300000;
    //2.试着走一下
    //3.评估走的结果
    while(steps.count())
    {
        // 从容器中获取当前的走棋步骤
        Step *step = steps.last();
        steps.removeLast();

        // 人假装走棋，就像老人下棋时的指指点点
        fakeMove(step);
        // 获取人假装走棋后的局面得分
        int maxScore = getMaxScore(level-1,minInAllMaxScore);

      //  int maxScore = calcSocre();
        // 走棋后，回退到之前的棋子位置
        unfakeMove(step);
        delete step;

        // 获取电脑最小得分
        if(maxScore <= curMin)
        {
            while(steps.count())
            {
                Step* step = steps.last();
                steps.removeLast();
                delete step;

            }
            return  maxScore;
        }

        if(maxScore < minInAllMaxScore)
        {
            minInAllMaxScore = maxScore;
        }
    }
    return minInAllMaxScore;
}

int SingleGame::getMaxScore(int level,int curMax)
{
    if(level == 0)
        return calcSocre();
    // 1.看看有哪些步骤可以走
    // 定义容器，保存棋子的走棋步骤
    QVector<Step*> steps;

    // 获取所有的走棋步骤
    getAllPossibleMove(steps);   // 是红棋的 possibleMove()
    int maxInAllMinScore = -300000;
    //2.试着走一下
    //3.评估走的结果
    while(steps.count())
    {
        // 从容器中获取当前的走棋步骤
        Step *step = steps.last();
        steps.removeLast();

        // 人假装走棋，就像老人下棋时的指指点点
        fakeMove(step);
        // 获取人假装走棋后的局面得分
        int MinScore = getMinScore(level-1,maxInAllMinScore);
        // 走棋后，回退到之前的棋子位置
        unfakeMove(step);
        delete step;

        // 获取电脑最小得分
        if( MinScore >= curMax)
        {
            while(steps.count())
            {
                Step *step = steps.last();
                steps.removeLast();
                delete step;
            }
            return MinScore;
        }
        if(MinScore > maxInAllMinScore)
        {
            maxInAllMinScore = MinScore;
        }
    }
    return maxInAllMinScore;
}

// 电脑假装移动
void SingleGame::fakeMove(Step *step)
{
    // 杀死棋子
    killStone(step->_killid);
    // 移动棋子
    //moveStone(step->_moveid,step->_killid,step->_rowTo,step->_colTo);
    moveStone(step->_moveid,step->_rowTo,step->_colTo);
}
// 电脑假装回退
void SingleGame::unfakeMove(Step * step)
{
    //复活棋子
    reliveStone(step->_killid);
    // 把棋子移回到原来的位置
   // moveStone(step->_moveid,step->_killid,step->_rowFrom,step->_colFrom);
    moveStone(step->_moveid,step->_rowFrom,step->_colFrom);
}

// 获取所有的走棋步骤
void SingleGame::getAllPossibleMove(QVector<Step *> &steps)
{
    // 如果是黑棋走棋
    int min,max;
    // 如果是红棋走棋
    if(this->_bRedTurn)
    {
        min = 0;
        max = 16;
    }
    // 如果是黑棋
    else
    {
        min = 16;
        max = 32;
    }
    // 循环遍历所有的黑棋，获取所有的走棋步骤
    for(int i=min;i<max;i++)
    {
        // 如果棋子是死棋，则结束当前循环，进入下一个棋子的判断
        if(this->_s[i]._dead)  continue;
        for(int row=0;row<10;row++)
        {
            for(int col=0;col<9;col++)
            {
                // 获取被杀棋子的 id
                int killid = this->getStoneId(row,col);
                // 如果发现被杀棋子和自己棋子的颜色相同，则继续循环
                if(sameColor(i,killid)) continue;

                // 如果棋子能移动的话，则存入容器中
                if(canMove(i,killid,row,col))
                {
                    // steps 保存所有当前棋子的走棋步骤
                    saveStep(i,killid,row,col,steps);
                }
            }
        }

    }
}


