#include<QApplication>

#include<QMessageBox>
#include "MainWnd.h"
#include"ChooseDlg.h"
#include<QTime>

int main(int argc,char *argv[])
{
    QApplication app(argc,argv);

    // 获取当前时间
    QTime t = QTime::currentTime();
    // 设置随机数种子
    qsrand(t.msec()+t.second()*1000);

    ChooseDlg dlg;
    // 选择游戏模式界面
    if(dlg.exec() != QDialog::Accepted)
           return 0;
    // 选择象棋对战模式
    MainWnd wnd(dlg._selected);
    wnd.show();
    return app.exec();
}
