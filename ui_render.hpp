#ifndef UI_RENDER_HPP
#define UI_RENDER_HPP

#include <QtWidgets>

class FractUIRender : public QWidget {
    Q_OBJECT
public:
    FractUIRender();
    QSize sizeHint() const;
    void closeEvent(QCloseEvent*);
    void resizeEvent(QResizeEvent*);
public slots:
    void UpdateView();
private:
    //vars
    QGridLayout *layoutOverworld = new QGridLayout(this);
    QCheckBox *cbFit = new QCheckBox(this);
    QLabel *labelRenderView = new QLabel(this);
    QScrollArea *areaRender = new QScrollArea(this);
};

#endif // UI_RENDER_HPP
