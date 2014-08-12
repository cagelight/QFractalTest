#ifndef UI_HPP
#define UI_HPP

namespace ui {
    void initialize(int, char*[]);
    int execute();
    void close();
    void terminate();
    void update_render_view();
    void update_render_progress();
}

#endif // UI_HPP
