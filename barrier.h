#ifndef BARRIER_H
#define BARRIER_H

#include <QObject>

class QPainter;
class Shape;

class Barrier : public QObject
{
    Q_OBJECT
public:
    explicit Barrier(QObject *parent = 0);



    void drawMe(QPainter &paint);

    void accept(Shape *shape);

    //判断y方向有没有障碍物
    bool hasBrrerFromY(int x, int y, Shape *s);
    //判断x方向有没有障碍物
    bool hasBarrerFromX(int x, int y, Shape *s);

    void clear();

private:
    void init();

signals:

public slots:

private :

    static int ROW;
    static int COLUMN;

    int m_barrers[19][15];


};

#endif // BARRIER_H
