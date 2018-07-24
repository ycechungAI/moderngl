#pragma once

struct GLContext {
    void * display;
    void * window;
    void * context;
    void * old_context;
    void * old_window;
    void * old_display;
    bool standalone;

    bool load(bool standalone);
    void enter();
    void exit();
    bool active();
    void destroy();
};

struct GLVersion {
    int major;
    int minor;
};

static GLVersion version[] = {
    {4, 6},
    {4, 5},
    {4, 4},
    {4, 3},
    {4, 2},
    {4, 1},
    {4, 0},
    {3, 3},
    {3, 2},
    {3, 1},
    {0, 0},
};

const int versions = sizeof(version) / sizeof(GLVersion);
