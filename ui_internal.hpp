#ifndef UI_INTERNAL_HPP
#define UI_INTERNAL_HPP

#include <QtGui>

namespace ui {
    namespace internal {
        QImage GetArrowIcon(Qt::ArrowType direction, QColor color = QColor(0, 0, 0, 0xFF));
    }
}

#endif // UI_INTERNAL_HPP
