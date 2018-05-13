#ifndef STEP_H
#define STEP_H
#include<QObject>
#include<QWidget>

// 悔棋类,移动棋子类
// 步长类

class Step:public QObject
{
  Q_OBJECT
public:
    Step(QObject *parent = 0);
    ~Step();

public:
    // 移动棋子的 id
    int _moveid;
    // 被杀死棋子的 id
    int _killid;
    // 棋子从哪一行移动过来
    int _rowFrom;
    // 棋子从哪一列移动过来
    int _colFrom;
    // 棋子移动到哪一行
    int _rowTo;
    // 棋子移动到那一列
    int _colTo;

public slots:

};

#endif // STEP_H
