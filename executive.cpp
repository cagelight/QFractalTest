#include "executive.hpp"
#include "render.hpp"
#include "ui.hpp"

void executive::initialize(int argc, char *argv[]) {
    render::initialize();
    ui::initialize(argc, argv);
}

int executive::operate() {
    return ui::execute();
}

void executive::terminate() {
    ui::terminate();
}
