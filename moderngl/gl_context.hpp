#pragma once

struct GLContext {
    void * display;
    void * window;
    void * context;
    void * old_context;
    void * old_window;
    void * old_display;
    const char * error;
    bool standalone;
    int glversion;

    bool load(bool standalone, int glversion);
    void enter();
    void exit();
    bool active();
    void destroy();
};
