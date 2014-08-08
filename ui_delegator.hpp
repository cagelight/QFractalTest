#ifndef UI_DELEGATOR_HPP
#define UI_DELEGATOR_HPP

#include <QObject>

class FractUIDelegator : public QObject {
    Q_OBJECT
signals:
    void UpdateRenderView();
    void UpdateProgressBar();
};

#endif // UI_DELEGATOR_HPP
