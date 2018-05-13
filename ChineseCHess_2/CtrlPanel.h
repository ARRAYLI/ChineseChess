#ifndef CTRLPANEL_H
#define CTRLPANEL_H

#include <QWidget>
#include<QPushButton>
// 控制面板类

class CtrlPanel : public QWidget
{
    Q_OBJECT
public:
    explicit CtrlPanel(QWidget *parent = 0);
    ~CtrlPanel();

    // 返回按钮
    QPushButton *_back;
signals:
    void sigBack();
public slots:
  //  void CloseWnd();
};

#endif // CTRLPANEL_H
