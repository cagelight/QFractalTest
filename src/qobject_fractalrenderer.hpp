#ifndef QOBJECT_FRACTALRENDERER_HPP
#define QOBJECT_FRACTALRENDERER_HPP

#include <mutex>
#include <thread>
#include <queue>

#include <QObject>
#include <QImage>

#include "qfractalmeta.hpp"

struct fract_settings;

class QFractalRenderer : public QObject { //THIS CLASS __IS NOT__ THREAD SAFE OR COPYABLE. ALWAYS USE IN MAIN QT THREAD!
    Q_OBJECT
public:
    QFractalRenderer(QFractalMeta F);
    QImage getImage();
    void setSettings(QFractalMeta F); //Stops an active render and then resets the state.
signals:
    void started();
    void paused();
    void stopped();
    void wasreset();
    void progressUpdate(int max, int cur);
    void finished();
public slots:
    void start();
    void pause();
    void stop();
    void reset(); //Stops an active render.
private:
    QFractalMeta fract;
    fract_settings *fractC = nullptr;
    QImage image;
    bool dopause, dostop, isactive;
    std::thread *delegateThread;
    std::queue<std::thread> workerThreads;
    int delegateCur = 0;
    std::mutex delegateLock;
    void setupFract();
    void resume();
    void completeDelegate();
    void delegate();
    void work();
    bool delegateLine(unsigned int &lineNum, unsigned int*& rowPtr);
    QFractalRenderer(const QFractalRenderer&) = delete;
    QFractalRenderer& operator=(const QFractalRenderer&) = delete;
};

#endif // QOBJECT_FRACTALRENDERER_HPP
