#ifndef QOBJECT_FRACTALRENDERER_HPP
#define QOBJECT_FRACTALRENDERER_HPP

#include <atomic>
#include <mutex>
#include <queue>
#include <thread>

#include <QObject>
#include <QImage>

#include "qfractalmeta.hpp"

struct fract_settings;

class QFractalRenderer : public QObject { //THIS CLASS __IS NOT__ THREAD SAFE OR COPYABLE. THREADS FOR INTERNAL USE ONLY.
    Q_OBJECT
public:
    QFractalRenderer(QFractalMeta F);
    ~QFractalRenderer();
    QImage getImage();
    const QImage *getImagePtr() const;
    void setSettings(QFractalMeta F); //Stops an active render and then resets the state.
signals:
    void started();
    void paused();
    void stopped();
    void wasreset();
    void progress(int, int);
    void finished();
public slots:
    void start();
    void pause();
    void stop();
    void reset(); //Stops an active render.
private:
    ///Variables
    QFractalMeta fract;
    fract_settings *fractC = nullptr;
    QImage image;
    std::atomic_bool dopause, dostop, isactive;
    std::thread *delegateThread;
    std::queue<std::thread> workerThreads;
    std::atomic_int delegateCur;
    std::mutex delegateLock;
    ///Methods
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
