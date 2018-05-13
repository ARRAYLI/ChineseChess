#ifndef CHOOSEDLG_H
#define CHOOSEDLG_H

#include <QDialog>
#include<QPushButton>
// 选择游戏按钮类(界面)

class ChooseDlg : public QDialog
{
    Q_OBJECT
public:
    explicit ChooseDlg(QDialog *parent = 0);
    ~ChooseDlg();
    QPushButton *_button[4];
    int _selected;
signals:

public slots:
    void slotClicked();
};

#endif // CHOOSEDLG_H
