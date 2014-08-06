#include "qwidget_2dnavigator.hpp"

#include <QtWidgets>

Q2DNavigator::Q2DNavigator(QWidget *parent) :
    QWidget(parent),
    layout(new QGridLayout(this)),
    buttonUp(new QPushButton(this)),
    buttonDown(new QPushButton(this)),
    buttonLeft(new QPushButton(this)),
    buttonRight(new QPushButton(this))
{
    layout->addWidget(buttonUp, 0, 0);
    layout->addWidget(buttonDown, 2, 0);
    layout->addWidget(buttonLeft, 1, 0);
    layout->addWidget(buttonRight, 1, 1);
    this->setLayout(layout);
}
