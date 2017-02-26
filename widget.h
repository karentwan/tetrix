#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "backpanel.h"

namespace Ui {
class Widget;
}

/**
 * 按钮类型
 */
enum ColorBtnType {
    BACK,
    NET,
    FULLLINE
};

/**
 * 控制器，操作面板与俄罗斯方块之间的操作面板
 */
class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);

    void setScore(int score);

    void suspend();

    ~Widget();

private :
    void init();

public slots:
    void openColorDialog();

private slots:
    void on_backCheck_toggled(bool checked);

    void on_netCheck_toggled(bool checked);

    void on_netColorBtn_clicked();

    void on_resumeBtn_clicked();

    void on_startGameBtn_clicked();

    void on_suspendGameBtn_clicked();

    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::Widget *ui;
    BackPanel *m_backPanel;
    ColorBtnType m_colorBtn;
};

#endif // WIDGET_H
