#include "executive.hpp"

int main(int argc, char *argv[]) {
    executive::initialize(argc, argv);
    int exec = executive::operate();
    executive::terminate();
    return exec;
}
