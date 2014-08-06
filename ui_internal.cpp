#include "ui_internal.hpp"

QImage ui::internal::GetArrowIcon(Qt::ArrowType direction, QColor color) {
    QImage img(32, 32, QImage::Format_ARGB32_Premultiplied);
    img.fill(0x00000000);
    QPainter paint(&img);
    QPainterPath tri;
    tri.moveTo(0.0f, 32.0f);
    tri.lineTo(32.0f, 32.0f);
    tri.lineTo(16.0f, 0.0f);
    tri.lineTo(0.0f, 32.0f);
    paint.setPen(Qt::NoPen);
    paint.fillPath(tri, QBrush(color));
    return img;
}
