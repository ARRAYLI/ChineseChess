#include "MainWnd.h"
#include "Board.h"
#include "MultiGame.h"
#include "SingleGame.h"
#include "NetGame.h"
#include<QHBoxLayout>       // 水平布局

MainWnd::MainWnd(int gameType,QWidget *parent) : QWidget(parent)
{
    _gameType = gameType;

    // 人机对战
    if(_gameType == 0)
    {
        SingleGame *game = new SingleGame;
        CtrlPanel *panel = new CtrlPanel;

        // 水平布局
        QHBoxLayout *hLay  = new QHBoxLayout(this);
        hLay->addWidget(game,1);
        hLay->addWidget(panel);
        connect(panel,SIGNAL(sigBack()),game,SLOT(slotBack()));
    }
    // 双人对战
    else if(_gameType == 1)
    {
        MultiGame *game = new MultiGame;
        CtrlPanel *panel = new CtrlPanel;

        // 水平布局
        QHBoxLayout *hLay  = new QHBoxLayout(this);
        hLay->addWidget(game,1);
        hLay->addWidget(panel);
        connect(panel,SIGNAL(sigBack()),game,SLOT(slotBack()));
    }

    // 网络对战服务端
    else if(_gameType == 2)
    {
        NetGame *game = new NetGame(true);
        CtrlPanel *panel = new CtrlPanel;

        // 水平布局
        QHBoxLayout *hLay  = new QHBoxLayout(this);
        hLay->addWidget(game,1);
        hLay->addWidget(panel);
        connect(panel,SIGNAL(sigBack()),game,SLOT(slotBack()));

    }
    // 网络对战客户端
    else if(_gameType == 3)
    {
        NetGame *game = new NetGame(false);
        CtrlPanel *panel = new CtrlPanel;

        // 水平布局
        QHBoxLayout *hLay  = new QHBoxLayout(this);
        hLay->addWidget(game,1);
        hLay->addWidget(panel);
        connect(panel,SIGNAL(sigBack()),game,SLOT(slotBack()));
    }


}

MainWnd::~MainWnd()
{

}


