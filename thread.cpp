#include "thread.h"
#include <QDebug>
#include "backpanel.h"

Thread::Thread(QObject *parent) :
    QThread(parent)
{
    m_run = true;
    //休眠时间
    m_sleepTime = 1000;

    m_suspend = false;
}

void Thread::suspendOrRun()
{
    setSuspend(!m_suspend);
}

void Thread::setSleepTime(int s)
{
    m_sleepTime = s;
}

void Thread::setBackPanel(BackPanel *b)
{
    m_backPanel = b;
}

void Thread::setSuspend(bool b)
{
    m_suspend = b;
}

void Thread::setRun(bool r)
{
    m_run = r;
}

void Thread::run()
{
    //线程运行
    while( m_run )
    {
        msleep(m_sleepTime);
        if( !m_suspend )
        {
            m_backPanel->shapeRun();
        }
    }
}
