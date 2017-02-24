#include "thread.h"
#include <QDebug>
#include "backpanel.h"

Thread::Thread(QObject *parent) :
    QThread(parent)
{
    m_run = true;
    //休眠时间
    m_sleepTime = 1;

    m_suspend = false;
}

void Thread::suspendOrRun()
{
    setSuspend(!m_suspend);
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
qDebug() << "m_run:" << m_run;
    //线程运行
    while( m_run )
    {
//qDebug() << "run....";
        if( !m_suspend )
        {
            m_backPanel->shapeRun();
        }
//qDebug() << "m_backPanel:" << m_backPanel;

        sleep(m_sleepTime);
    }
}
