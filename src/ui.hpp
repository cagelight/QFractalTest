#ifndef UI_HPP
#define UI_HPP

#include <QApplication>
#include <QObject>

class FractUIMain;
class FractUIRender;

class FractUIManager : public QApplication {
    Q_OBJECT
public:
    FractUIManager(int argc, char *argv[]);
    ~FractUIManager();
private:
    FractUIMain *mainWindow;
    FractUIRender *renderWindow;
};

#endif // UI_HPP
