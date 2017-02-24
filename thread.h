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

    void setRun(bool r);

    void setBackPanel(BackPanel *b);

    void setSuspend(bool b);

    void suspendOrRun();

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
