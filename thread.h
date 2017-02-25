#ifndef THREAD_H
#define THREAD_H

#include <QThread>

class BackPanel;

/**
 * 使Shape下落
 */
class Thread : public QThread
{
    Q_OBJECT
public:
    explicit Thread(QObject *parent = 0);
    //设置游戏是否运行
    void setRun(bool r);
    //保存BackPanel的引用
    void setBackPanel(BackPanel *b);
    //是否暂停
    void setSuspend(bool b);
    //设置暂停或者运行
    void suspendOrRun();
    //设置线程休眠时间
    void setSleepTime(int s);

    void run();

signals:

public slots:

private :
    bool m_run;       //线程是否运行

    bool m_suspend;   //游戏暂停

    long m_sleepTime; //线程休眠时间

    BackPanel *m_backPanel;

};

#endif // THREAD_H
