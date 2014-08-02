#ifndef UI_HPP
#define UI_HPP

#include <QWidget>

namespace ui {
    void initialize(int, char*[]);
    int execute();
    void close();
    void terminate();
    void update_render_view();

    namespace classes {
        class MainWindow : public QWidget {
            Q_OBJECT
        public:
            MainWindow();
            QSize sizeHint() const;
            void closeEvent(QCloseEvent*);
        public slots:
            void Render();
        };
        class RenderWindow : public QWidget {
            Q_OBJECT
        public:
            RenderWindow();
            QSize sizeHint() const;
            void closeEvent(QCloseEvent*);
            void resizeEvent(QResizeEvent*);
        public slots:
            void SetRenderViewMode(int);
        };
    }
}

#endif // UI_HPP
