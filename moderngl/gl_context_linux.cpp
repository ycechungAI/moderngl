#include "gl_context.hpp"

#include <GL/glx.h>
#include <GL/gl.h>

#define GLX_CONTEXT_MAJOR_VERSION 0x2091
#define GLX_CONTEXT_MINOR_VERSION 0x2092
#define GLX_CONTEXT_PROFILE_MASK 0x9126
#define GLX_CONTEXT_CORE_PROFILE_BIT 0x0001

typedef GLXContext (* GLXCREATECONTEXTATTRIBSARBPROC)(Display * display, GLXFBConfig config, GLXContext context, Bool direct, const int * attribs);

int SilentXErrorHandler(Display * d, XErrorEvent * e) {
    return 0;
}

bool GLContext::load(bool standalone) {
    this->standalone = standalone;

    if (standalone) {
        int width = 1;
        int height = 1;

        Display * dpy = XOpenDisplay(0);

        if (!dpy) {
            dpy = XOpenDisplay(":0.0");
        }

        if (!dpy) {
            this->error = "cannot detect the display";
            return context;
        }

        int nelements = 0;

        GLXFBConfig * fbc = glXChooseFBConfig(dpy, DefaultScreen(dpy), 0, &nelements);

        if (!fbc) {
            this->error = "cannot read the display configuration";
            XCloseDisplay(dpy);
            return context;
        }

        static int attributeList[] = {
            GLX_RGBA,
            GLX_DOUBLEBUFFER,
            GLX_RED_SIZE, 8,
            GLX_GREEN_SIZE, 8,
            GLX_BLUE_SIZE, 8,
            GLX_DEPTH_SIZE, 24,
            None,
        };

        XVisualInfo * vi = glXChooseVisual(dpy, DefaultScreen(dpy), attributeList);

        if (!vi) {
            XCloseDisplay(dpy);

            this->error = "cannot choose a visual info";
            return context;
        }

        XSetWindowAttributes swa;
        swa.colormap = XCreateColormap(dpy, RootWindow(dpy, vi->screen), vi->visual, AllocNone);
        swa.border_pixel = 0;
        swa.event_mask = StructureNotifyMask;

        Window win = XCreateWindow(dpy, RootWindow(dpy, vi->screen), 0, 0, width, height, 0, vi->depth, InputOutput, vi->visual, CWBorderPixel | CWColormap | CWEventMask, &swa);

        if (!win) {
            XCloseDisplay(dpy);

            this->error = "cannot create window";
            return context;
        }

        // XMapWindow(dpy, win);

        GLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribsARB = (GLXCREATECONTEXTATTRIBSARBPROC)glXGetProcAddress((const GLubyte *)"glXCreateContextAttribsARB");

        GLXContext ctx = 0;

        XSetErrorHandler(SilentXErrorHandler);

        if (glXCreateContextAttribsARB) {
            for (int i = 0; i < versions; ++i) {
                int attribs[] = {
                    GLX_CONTEXT_PROFILE_MASK, GLX_CONTEXT_CORE_PROFILE_BIT,
                    GLX_CONTEXT_MAJOR_VERSION, version[i].major,
                    GLX_CONTEXT_MINOR_VERSION, version[i].minor,
                    0, 0,
                };

                ctx = glXCreateContextAttribsARB(dpy, *fbc, 0, true, attribs);

                if (ctx) {
                    break;
                }
            }
        }

        if (!ctx) {
            ctx = glXCreateContext(dpy, vi, 0, GL_TRUE);
        }

        if (!ctx) {
            XDestroyWindow(dpy, win);
            XCloseDisplay(dpy);

            this->error = "cannot create OpenGL context";
            return context;
        }

        XSetErrorHandler(0);

        int make_current = glXMakeCurrent(dpy, win, ctx);

        if (!make_current) {
            glXDestroyContext(dpy, ctx);
            XDestroyWindow(dpy, win);
            XCloseDisplay(dpy);

            this->error = "cannot select OpenGL context";
            return context;
        }
    }

    Display * dpy = glXGetCurrentDisplay();

    if (!dpy) {
        this->error = "cannot detect display";
        return false;
    }

    Window win = glXGetCurrentDrawable();

    if (!win) {
        this->error = "cannot detect window";
        return false;
    }

    GLXContext ctx = glXGetCurrentContext();

    if (!ctx) {
        this->error = "cannot detect OpenGL context";
        return false;
    }

    this->window = (void *)win;
    this->display = (void *)dpy;
    this->context = (void *)ctx;

    return true;
}

void GLContext::enter() {
    this->old_display = (void *)glXGetCurrentDisplay();
    this->old_window = (void *)glXGetCurrentDrawable();
    this->old_context = (void *)glXGetCurrentContext();
    glXMakeCurrent((Display *)this->display, (Window)this->window, (GLXContext)this->context);
}

void GLContext::exit() {
    glXMakeCurrent((Display *)this->old_display, (Window)this->old_window, (GLXContext)this->old_context);
}

bool GLContext::active() {
    return this->context == glXGetCurrentContext();
}
