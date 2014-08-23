#include "qwidget_2dnavigator.hpp"

#include <QtWidgets>
#include <QtGui>

static QImage GetArrowIcon(Qt::ArrowType direction = Qt::UpArrow, QSize size = QSize(32, 32), QColor color = QColor(0, 0, 0, 255)) {
    QImage img(size, QImage::Format_ARGB32_Premultiplied);
    img.fill(0x00000000);
    QPainter paint(&img);
    QSizeF sizeF(size);
    paint.translate(sizeF.width()/2, sizeF.width()/2);
    switch (direction) {
    case Qt::DownArrow:
        paint.rotate(180);
        break;
    case Qt::LeftArrow:
        paint.rotate(-90);
        break;
    case Qt::RightArrow:
        paint.rotate(90);
        break;
    default:
        break;
    }
    QPainterPath tri;
    tri.moveTo(-sizeF.width()/2, sizeF.height()/2);
    tri.lineTo(sizeF.width()/2, sizeF.height()/2);
    tri.lineTo(0.0f, -sizeF.height()/2);
    tri.lineTo(-sizeF.width()/2, sizeF.height()/2);
    paint.setPen(Qt::NoPen);
    paint.fillPath(tri, QBrush(color));
    return img;
}

static QImage GetCircleIcon(QSize size = QSize(32, 32), QColor color = QColor(0, 0, 0, 255)) {
    QImage img(size, QImage::Format_ARGB32_Premultiplied);
    img.fill(0x00000000);
    QPainter paint(&img);
    QSizeF sizeF(size);
    paint.translate(sizeF.width()/2, sizeF.width()/2);
    paint.setPen(Qt::NoPen);
    paint.setBrush(QBrush(color));
    paint.drawEllipse(QPointF(0.0f, 0.0f), sizeF.width()/2, sizeF.height()/2);
    return img;
}

const float iconRectRatio = 0.25f;

static QImage GetIncIcon(QSize size = QSize(32, 32), QColor color = QColor(0, 0, 0, 255)) {
    QImage img(size, QImage::Format_ARGB32_Premultiplied);
    img.fill(0x00000000);
    QPainter paint(&img);
    QSizeF sizeF(size);
    QSizeF RsizeF(iconRectRatio * sizeF);
    paint.translate(sizeF.width()/2, sizeF.width()/2);
    paint.setPen(Qt::NoPen);
    paint.setBrush(QBrush(color));
    QRectF horzbar = QRectF(-sizeF.width()/2, -RsizeF.height()/2, sizeF.width(), RsizeF.height());
    QRectF vertbar = QRectF(-RsizeF.width()/2, -sizeF.height()/2, RsizeF.width(), sizeF.height());
    paint.drawRect(horzbar);
    paint.drawRect(vertbar);
    return img;
}

static QImage GetDecIcon(QSize size = QSize(32, 32), QColor color = QColor(0, 0, 0, 255)) {
    QImage img(size, QImage::Format_ARGB32_Premultiplied);
    img.fill(0x00000000);
    QPainter paint(&img);
    QSizeF sizeF(size);
    QSizeF RsizeF(iconRectRatio * sizeF);
    paint.translate(sizeF.width()/2, sizeF.width()/2);
    paint.setPen(Qt::NoPen);
    paint.setBrush(QBrush(color));
    QRectF horzbar = QRectF(-sizeF.width()/2, -RsizeF.height()/2, sizeF.width(), RsizeF.height());
    paint.drawRect(horzbar);
    return img;
}

Q2DNavigator::Q2DNavigator(QWidget *parent, Q2DNavigatorOptions options) :
    QWidget(parent),
    layout(new QGridLayout(this)),
    buttonUp(new QPushButton(this)),
    buttonDown(new QPushButton(this)),
    buttonLeft(new QPushButton(this)),
    buttonRight(new QPushButton(this))
{
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    QSize iconSize(64, 64);
    QSize buttonSize(32, 32);
    buttonUp->setIcon(QIcon(QPixmap::fromImage(GetArrowIcon(Qt::UpArrow, iconSize))));
    buttonDown->setIcon(QIcon(QPixmap::fromImage(GetArrowIcon(Qt::DownArrow, iconSize))));
    buttonLeft->setIcon(QIcon(QPixmap::fromImage(GetArrowIcon(Qt::LeftArrow, iconSize))));
    buttonRight->setIcon(QIcon(QPixmap::fromImage(GetArrowIcon(Qt::RightArrow, iconSize))));
    buttonUp->setFixedSize(buttonSize);
    buttonDown->setFixedSize(buttonSize);
    buttonLeft->setFixedSize(buttonSize);
    buttonRight->setFixedSize(buttonSize);
    QObject::connect(buttonUp, SIGNAL(released()), this, SIGNAL(upReleased()));
    QObject::connect(buttonDown, SIGNAL(released()), this, SIGNAL(downReleased()));
    QObject::connect(buttonLeft, SIGNAL(released()), this, SIGNAL(leftReleased()));
    QObject::connect(buttonRight, SIGNAL(released()), this, SIGNAL(rightReleased()));
    layout->addWidget(buttonUp, 0, 1);
    layout->addWidget(buttonDown, 2, 1);
    layout->addWidget(buttonLeft, 1, 0);
    layout->addWidget(buttonRight, 1, 2);
    if (options & NAVIGATOR2_CENTER) {
        buttonCenter = new QPushButton(this);
        buttonCenter->setIcon(QIcon(QPixmap::fromImage(GetCircleIcon(iconSize))));
        buttonCenter->setFixedSize(buttonSize);
        QObject::connect(buttonCenter, SIGNAL(released()), this, SIGNAL(centerReleased()));
        layout->addWidget(buttonCenter, 1, 1);
    }
    if (options & NAVIGATOR2_ZOOM) {
        buttonIncrement = new QPushButton(this);
        buttonDecrement = new QPushButton(this);
        buttonIncrement->setIcon(QIcon(QPixmap::fromImage(GetIncIcon(iconSize))));
        buttonDecrement->setIcon(QIcon(QPixmap::fromImage(GetDecIcon(iconSize))));
        buttonIncrement->setFixedSize(buttonSize);
        buttonDecrement->setFixedSize(buttonSize);
        QObject::connect(buttonIncrement, SIGNAL(released()), this, SIGNAL(incrementReleased()));
        QObject::connect(buttonDecrement, SIGNAL(released()), this, SIGNAL(decrementReleased()));
        layout->addWidget(buttonIncrement, 0, 2);
        layout->addWidget(buttonDecrement, 2, 2);
    }
    if (options & NAVIGATOR2_MARKSV) {
        buttonCenter = new QPushButton(this);
        buttonCenter->setIcon(QIcon(QPixmap::fromImage(GetCircleIcon(iconSize))));
        buttonCenter->setFixedSize(buttonSize);
        QObject::connect(buttonCenter, SIGNAL(released()), this, SIGNAL(centerReleased()));
        layout->addWidget(buttonCenter, 1, 1);
    }
    this->setLayout(layout);
}
