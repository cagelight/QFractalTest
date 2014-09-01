#ifndef QWIDGET_2DNAVIGATOR_HPP
#define QWIDGET_2DNAVIGATOR_HPP

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>

class QNavigator2D : public QWidget {
    Q_OBJECT
public:
    enum Options {
        NAVIGATOR2_NONE   = 0,
        NAVIGATOR2_CENTER = 1 << 0,
        NAVIGATOR2_ZOOM   = 1 << 1,
        NAVIGATOR2_MARKSV = 1 << 2,
    };
    enum Direction {
        NAV_UP,
        NAV_DOWN,
        NAV_LEFT,
        NAV_RIGHT,
        NAV_CENTER,
        NAV_INC,
        NAV_DEC,
    };
public:
    QNavigator2D(QWidget *parent = 0, QNavigator2D::Options options = NAVIGATOR2_CENTER);
signals:
    void directionReleased(QNavigator2D::Direction);
private slots:
    void navUp() {emit directionReleased(NAV_UP);}
    void navDown() {emit directionReleased(NAV_DOWN);}
    void navLeft() {emit directionReleased(NAV_LEFT);}
    void navRight() {emit directionReleased(NAV_RIGHT);}
    void navCenter() {emit directionReleased(NAV_CENTER);}
    void navInc() {emit directionReleased(NAV_INC);}
    void navDec() {emit directionReleased(NAV_DEC);}
private:
    QGridLayout* layout;
    QPushButton *buttonUp, *buttonDown, *buttonLeft, *buttonRight, *buttonCenter, *buttonIncrement, *buttonDecrement;
};

#endif // QWIDGET_2DNAVIGATOR_HPP
