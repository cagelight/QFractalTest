#ifndef UI_HPP
#define UI_HPP

#include <QApplication>
#include <QObject>

#include "qfractalmeta.hpp"

class FractUIMain;
class FractUIRender;
class QFractalRenderer;

class FractUIManager : public QApplication {
    Q_OBJECT
public:
    FractUIManager(int argc, char *argv[]);
    ~FractUIManager();
private slots:
    void beginCoreRender(QFractalMeta);
    void stopCoreRender();
private:
    FractUIMain *mainWindow;
    FractUIRender *renderWindow;
    QFractalRenderer *coreRender = nullptr;
};

#endif // UI_HPP
