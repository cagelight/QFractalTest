#ifndef QWIDGET_2DNAVIGATOR_HPP
#define QWIDGET_2DNAVIGATOR_HPP

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>

enum Q2DNavigatorOptions {
    NAVIGATOR2_NONE   = 0,
    NAVIGATOR2_CENTER = 1 << 0,
    NAVIGATOR2_ZOOM   = 1 << 1,
    NAVIGATOR2_MARKSV = 1 << 2,
};

class Q2DNavigator : public QWidget {
    Q_OBJECT
public:
    Q2DNavigator(QWidget *parent = 0, Q2DNavigatorOptions options = NAVIGATOR2_CENTER);
signals:
    void upReleased();
    void downReleased();
    void leftReleased();
    void rightReleased();
    void centerReleased();
    void incrementReleased();
    void decrementReleased();
private:
    QGridLayout* layout;
    QPushButton *buttonUp, *buttonDown, *buttonLeft, *buttonRight, *buttonCenter, *buttonIncrement, *buttonDecrement;
};

#endif // QWIDGET_2DNAVIGATOR_HPP
