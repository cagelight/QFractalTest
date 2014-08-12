#ifndef QWIDGET_2DNAVIGATOR_HPP
#define QWIDGET_2DNAVIGATOR_HPP

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>

class Q2DNavigator : public QWidget {
    Q_OBJECT
public:
    Q2DNavigator(QWidget *parent = 0);
private:
    QGridLayout* layout;
    QPushButton *buttonUp, *buttonDown, *buttonLeft, *buttonRight, *buttonCenter;
};

#endif // QWIDGET_2DNAVIGATOR_HPP
