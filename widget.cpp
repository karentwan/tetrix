#include "widget.h"
#include "ui_widget.h"
#include <QLayout>
#include <QHBoxLayout>
#include "configuration.h"
#include <QDebug>
#include <QPushButton>
#include <QColorDialog>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    init();
    setWindowTitle("俄罗斯方块");


}

void Widget::setScore(int score)
{
    ui->score->setText(QString("分数:%1").arg(score));
}


void Widget::init()
{
    //设置边框
    ui->BackPanel->setStyleSheet("QWidget#BackPanel{border:1px solid #ccc;}");
    m_backPanel = new BackPanel(this);
    //设置固定的大小
//    setFixedSize(Configuration::GRID_WIDTH * ( Configuration::COLUMN * 2 - 1),
//                 Configuration::GRID_WIDTH * Configuration::ROW);
    setSizeIncrement(Configuration::GRID_WIDTH * ( Configuration::COLUMN * 2 - 1),
                        Configuration::GRID_WIDTH * Configuration::ROW);
    QHBoxLayout *rightLay = new QHBoxLayout(ui->BackPanel);
    rightLay->setContentsMargins(0, 0, 0, 0);
    rightLay->setSpacing(0);
    rightLay->addWidget(m_backPanel);
    ui->BackPanel->setLayout(rightLay);
    //设置固定值
    m_backPanel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    //修改按钮的样式
//    ui->startGameBtn->setStyleSheet("background-color:#a49ef0; border-color:#a49ef0;color:#fff;border-radius:5px;line-height:20px;");

    //连接信号和槽
    connect(ui->setBackgroundBtn, SIGNAL(clicked()), this, SLOT(openColorDialog()));
}

void Widget::openColorDialog()
{
    QColor c = QColorDialog::getColor(QColor::fromRgb(205, 219, 255),NULL, "选择背景颜色", QColorDialog::ShowAlphaChannel);
qDebug() << "c:" << c;
    m_backPanel->setBackColor(c);

}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_backCheck_toggled(bool checked)
{
    qDebug() << "checked:" << checked;
    m_backPanel->setBack(checked);
}

void Widget::on_netCheck_toggled(bool checked)
{
    m_backPanel->setNet(checked);
}

void Widget::on_netColorBtn_clicked()
{
    QColor c = QColorDialog::getColor(QColor::fromRgb(205, 219, 255),NULL, "选择网格颜色", QColorDialog::ShowAlphaChannel);
    m_backPanel->setNetColor(c);
}

void Widget::on_resumeBtn_clicked()
{
    m_backPanel->reset();
    //设置CheckBox的默认值
    ui->netCheck->setChecked(true);
    ui->backCheck->setChecked(true);
}

void Widget::on_startGameBtn_clicked()
{
    m_backPanel->startGame();
    ui->startGameBtn->setEnabled(false);

    m_play = new QMediaPlayer(this);
    m_play->setMedia(QUrl::fromLocalFile(":/res/paopao.mp3"));
  //  m_play->setVolume(50);
    m_play->play();
}

void Widget::suspend()
{
    QString s = ui->suspendGameBtn->text();
    if( s.contains("运行游戏"))
    {
        ui->suspendGameBtn->setText("暂停游戏");
    } else
    {
        ui->suspendGameBtn->setText("运行游戏");
    }
    m_backPanel->suspendOrRun();
}

void Widget::on_suspendGameBtn_clicked()
{
    suspend();
}

void Widget::on_comboBox_currentIndexChanged(int index)
{
//qDebug() << "level :" << index;
    int g = 7 - index;
    if( g <= 0 || g > 7 )
        return;
qDebug() << "g:" << g;
    m_backPanel->setGameLevel(g * 100);
}
