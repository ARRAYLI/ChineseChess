#include "ChooseDlg.h"
#include<QVBoxLayout>   // 垂直布局类
ChooseDlg::ChooseDlg(QDialog *parent) : QDialog(parent)
{
    // 设置游戏界面的种类选择
    QVBoxLayout *lay = new QVBoxLayout(this);
    lay->addWidget(_button[0] = new QPushButton("人机对战"));
    lay->addWidget(_button[1] = new QPushButton("人人对战"));
    lay->addWidget(_button[2] = new QPushButton("网络对战(服务端)"));
    lay->addWidget(_button[3] = new QPushButton("人机对战(客户端)"));

    // 把创建的四个按钮和槽函数关联
    for(int i=0;i<4;i++)
    {
        connect(_button[i],SIGNAL(clicked()),this,SLOT(slotClicked()));
    }
}

ChooseDlg::~ChooseDlg()
{

}

void ChooseDlg::slotClicked()
{
    // emit 发射信号后，sender() 捕捉到信号是谁发的，然后确定信号的来源
    QObject *s = sender();

    for(int i=0;i<4;i++)
    {
        if(_button[i] == s)
        {
            this->_selected = i;
            break;
        }
    }
    // 接收了这个事件，不在向下传递下去
    accept();
}

