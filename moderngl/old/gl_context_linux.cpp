#include "gl_context.hpp"

#include <dlfcn.h>
#include <GL/glx.h>
#include <GL/gl.h>

#define GLX_CONTEXT_MAJOR_VERSION 0x2091
#define GLX_CONTEXT_MINOR_VERSION 0x2092
#define GLX_CONTEXT_PROFILE_MASK 0x9126
#define GLX_CONTEXT_CORE_PROFILE_BIT 0x0001

typedef Display * (* MyXOpenDisplay)(const char *);
typedef int (* MyXDefaultScreen)(Display *);
typedef Window (* MyXRootWindow)(Display *, int);
typedef Colormap (* MyXCreateColormap)(Display *, Window, Visual *, int);
typedef Window (* MyXCreateWindow)(Display *, Window, int, int, unsigned int, unsigned int, unsigned int, int, unsigned int, Visual *, unsigned long, XSetWindowAttributes *);
typedef int (* MyXDestroyWindow)(Display *, Window);
typedef int (* MyXCloseDisplay)(Display *);
typedef XErrorHandler (* MyXSetErrorHandler)(XErrorHandler);

typedef GLXFBConfig * (* MyglXChooseFBConfig)(Display *, int, const int *, int *);
typedef XVisualInfo * (* MyglXChooseVisual)(Display *, int, int *);
typedef Display * (* MyglXGetCurrentDisplay)();
typedef GLXContext (* MyglXGetCurrentContext)();
typedef GLXDrawable (* MyglXGetCurrentDrawable)();
typedef Bool (* MyglXMakeCurrent)(Display *, GLXDrawable, GLXContext);
typedef void (* MyglXDestroyContext)(Display *, GLXContext);
typedef GLXContext (* MyglXCreateContext)(Display *, XVisualInfo *, GLXContext, Bool);
typedef void (*(* MyglXGetProcAddress)(const GLubyte *procname))();
typedef GLXContext (* MyglXCreateContextAttribsARB)(Display * display, GLXFBConfig config, GLXContext context, int direct, const int * attribs);

void * libgl = dlopen("libGL.so", RTLD_LAZY);
MyglXChooseFBConfig myglXChooseFBConfig = (MyglXChooseFBConfig)dlsym(libgl, "glXChooseFBConfig");
MyglXChooseVisual myglXChooseVisual = (MyglXChooseVisual)dlsym(libgl, "glXChooseVisual");
MyglXGetCurrentDisplay myglXGetCurrentDisplay = (MyglXGetCurrentDisplay)dlsym(libgl, "glXGetCurrentDisplay");
MyglXGetCurrentContext myglXGetCurrentContext = (MyglXGetCurrentContext)dlsym(libgl, "glXGetCurrentContext");
MyglXGetCurrentDrawable myglXGetCurrentDrawable = (MyglXGetCurrentDrawable)dlsym(libgl, "glXGetCurrentDrawable");
MyglXMakeCurrent myglXMakeCurrent = (MyglXMakeCurrent)dlsym(libgl, "glXMakeCurrent");
MyglXDestroyContext myglXDestroyContext = (MyglXDestroyContext)dlsym(libgl, "glXDestroyContext");
MyglXCreateContext myglXCreateContext = (MyglXCreateContext)dlsym(libgl, "glXCreateContext");
MyglXGetProcAddress myglXGetProcAddress = (MyglXGetProcAddress)dlsym(libgl, "glXGetProcAddress");

typedef unsigned int EGLenum;
typedef int EGLint;
typedef unsigned int EGLBoolean;
typedef Display * EGLNativeDisplayType;
typedef Pixmap EGLNativePixmapType;
typedef Window EGLNativeWindowType;
typedef void * EGLConfig;
typedef void * EGLSurface;
typedef void * EGLContext;

#define EGLAPIENTRYP *
#define EGL_DEFAULT_DISPLAY 0
#define EGL_NO_CONTEXT 0
#define EGL_NO_SURFACE 0

#define EGL_PBUFFER_BIT                   0x0001
#define EGL_RENDERABLE_TYPE               0x3040
#define EGL_NONE                          0x3038
#define EGL_OPENGL_BIT                    0x0008
#define EGL_BLUE_SIZE                     0x3022
#define EGL_DEPTH_SIZE                    0x3025
#define EGL_RED_SIZE                      0x3024
#define EGL_GREEN_SIZE                    0x3023
#define EGL_SURFACE_TYPE                  0x3033
#define EGL_OPENGL_API                    0x30A2
#define EGL_WIDTH                         0x3057
#define EGL_HEIGHT                        0x3056

typedef void * EGLDisplay;
typedef EGLDisplay (EGLAPIENTRYP PFNEGLGETDISPLAYPROC) (EGLNativeDisplayType display_id);
typedef EGLBoolean (EGLAPIENTRYP PFNEGLINITIALIZEPROC) (EGLDisplay dpy, EGLint *major, EGLint *minor);
typedef EGLBoolean (EGLAPIENTRYP PFNEGLCHOOSECONFIGPROC) (EGLDisplay dpy, const EGLint *attrib_list, EGLConfig *configs, EGLint config_size, EGLint *num_config);
typedef EGLSurface (EGLAPIENTRYP PFNEGLCREATEPBUFFERSURFACEPROC) (EGLDisplay dpy, EGLConfig config, const EGLint *attrib_list);
typedef EGLBoolean (EGLAPIENTRYP PFNEGLBINDAPIPROC) (EGLenum api);
typedef EGLContext (EGLAPIENTRYP PFNEGLCREATECONTEXTPROC) (EGLDisplay dpy, EGLConfig config, EGLContext share_context, const EGLint *attrib_list);
typedef EGLBoolean (EGLAPIENTRYP PFNEGLMAKECURRENTPROC) (EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx);

#define EGL_CONTEXT_MAJOR_VERSION         0x3098
#define EGL_CONTEXT_MINOR_VERSION         0x30FB
#define EGL_CONTEXT_OPENGL_PROFILE_MASK   0x30FD
#define EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT 0x00000001
#define EGL_CONTEXT_OPENGL_FORWARD_COMPATIBLE 0x31B1

EGLint configAttribs[] = {
    EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
    EGL_BLUE_SIZE, 8,
    EGL_GREEN_SIZE, 8,
    EGL_RED_SIZE, 8,
    EGL_DEPTH_SIZE, 8,
    EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
    EGL_NONE
};

int SilentXErrorHandler(Display * d, XErrorEvent * e) {
    return 0;
}

bool GLContext::load(bool standalone, bool egl) {
    this->standalone = standalone;

    if (standalone && egl) {
        void * libegl = dlopen("libEGL.so", RTLD_LAZY);
        PFNEGLGETDISPLAYPROC eglGetDisplay = (PFNEGLGETDISPLAYPROC)dlsym(libegl, "eglGetDisplay");
        PFNEGLINITIALIZEPROC eglInitialize = (PFNEGLINITIALIZEPROC)dlsym(libegl, "eglInitialize");
        PFNEGLCHOOSECONFIGPROC eglChooseConfig = (PFNEGLCHOOSECONFIGPROC)dlsym(libegl, "eglChooseConfig");
        PFNEGLBINDAPIPROC eglBindAPI = (PFNEGLBINDAPIPROC)dlsym(libegl, "eglBindAPI");
        PFNEGLCREATECONTEXTPROC eglCreateContext = (PFNEGLCREATECONTEXTPROC)dlsym(libegl, "eglCreateContext");
        PFNEGLMAKECURRENTPROC eglMakeCurrent = (PFNEGLMAKECURRENTPROC)dlsym(libegl, "eglMakeCurrent");

        EGLDisplay eglDpy = eglGetDisplay(EGL_DEFAULT_DISPLAY);

        EGLint major, minor;
        eglInitialize(eglDpy, &major, &minor);

        EGLint numConfigs;
        EGLConfig eglCfg;

        eglChooseConfig(eglDpy, configAttribs, &eglCfg, 1, &numConfigs);

        eglBindAPI(EGL_OPENGL_API);

        int ctxattribs[] = {
            EGL_CONTEXT_MAJOR_VERSION, 3,
            EGL_CONTEXT_MINOR_VERSION, 3,
            EGL_CONTEXT_OPENGL_PROFILE_MASK, EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT,
            // EGL_CONTEXT_OPENGL_FORWARD_COMPATIBLE, 1,
            EGL_NONE,
        };

        EGLContext eglCtx = eglCreateContext(eglDpy, eglCfg, EGL_NO_CONTEXT, ctxattribs);
        eglMakeCurrent(eglDpy, EGL_NO_SURFACE, EGL_NO_SURFACE, eglCtx);
    } else if (standalone) {
        void * libx11 = dlopen("libX11.so", RTLD_LAZY);
        MyXOpenDisplay myXOpenDisplay = (MyXOpenDisplay)dlsym(libx11, "XOpenDisplay");
        MyXDefaultScreen myXDefaultScreen = (MyXDefaultScreen)dlsym(libx11, "XDefaultScreen");
        MyXRootWindow myXRootWindow = (MyXRootWindow)dlsym(libx11, "XRootWindow");
        MyXCreateColormap myXCreateColormap = (MyXCreateColormap)dlsym(libx11, "XCreateColormap");
        MyXCreateWindow myXCreateWindow = (MyXCreateWindow)dlsym(libx11, "XCreateWindow");
        MyXDestroyWindow myXDestroyWindow = (MyXDestroyWindow)dlsym(libx11, "XDestroyWindow");
        MyXCloseDisplay myXCloseDisplay = (MyXCloseDisplay)dlsym(libx11, "XCloseDisplay");
        MyXSetErrorHandler myXSetErrorHandler = (MyXSetErrorHandler)dlsym(libx11, "XSetErrorHandler");

        Display * dpy = myXOpenDisplay(0);

        if (!dpy) {
            dpy = myXOpenDisplay(":0.0");
        }

        if (!dpy) {
            this->error = "cannot detect the display";
            return context;
        }

        int nelements = 0;

        GLXFBConfig * fbc = myglXChooseFBConfig(dpy, myXDefaultScreen(dpy), 0, &nelements);

        if (!fbc) {
            this->error = "cannot read the display configuration";
            myXCloseDisplay(dpy);
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

        XVisualInfo * vi = myglXChooseVisual(dpy, myXDefaultScreen(dpy), attributeList);

        if (!vi) {
            myXCloseDisplay(dpy);

            this->error = "cannot choose a visual info";
            return context;
        }

        XSetWindowAttributes swa;
        swa.colormap = myXCreateColormap(dpy, myXRootWindow(dpy, vi->screen), vi->visual, AllocNone);
        swa.border_pixel = 0;
        swa.event_mask = StructureNotifyMask;

        Window win = myXCreateWindow(dpy, myXRootWindow(dpy, vi->screen), 0, 0, 1, 1, 0, vi->depth, InputOutput, vi->visual, CWBorderPixel | CWColormap | CWEventMask, &swa);

        if (!win) {
            myXCloseDisplay(dpy);

            this->error = "cannot create window";
            return context;
        }

        MyglXCreateContextAttribsARB myglXCreateContextAttribsARB = (MyglXCreateContextAttribsARB)myglXGetProcAddress((const GLubyte *)"glXCreateContextAttribsARB");

        GLXContext ctx = 0;

        myXSetErrorHandler(SilentXErrorHandler);

        if (myglXCreateContextAttribsARB) {
            for (int i = 0; i < versions; ++i) {
                int attribs[] = {
                    GLX_CONTEXT_PROFILE_MASK, GLX_CONTEXT_CORE_PROFILE_BIT,
                    GLX_CONTEXT_MAJOR_VERSION, version[i].major,
                    GLX_CONTEXT_MINOR_VERSION, version[i].minor,
                    0, 0,
                };

                ctx = myglXCreateContextAttribsARB(dpy, *fbc, 0, true, attribs);

                if (ctx) {
                    break;
                }
            }
        }

        if (!ctx) {
            ctx = myglXCreateContext(dpy, vi, 0, GL_TRUE);
        }

        if (!ctx) {
            myXDestroyWindow(dpy, win);
            myXCloseDisplay(dpy);

            this->error = "cannot create OpenGL context";
            return context;
        }

        myXSetErrorHandler(0);

        int make_current = myglXMakeCurrent(dpy, win, ctx);

        if (!make_current) {
            myglXDestroyContext(dpy, ctx);
            myXDestroyWindow(dpy, win);
            myXCloseDisplay(dpy);

            this->error = "cannot select OpenGL context";
            return context;
        }
    } else {
        this->display = (void *)myglXGetCurrentDisplay();

        if (!this->display) {
            this->error = "cannot detect display";
            return false;
        }

        this->window = (void *)myglXGetCurrentDrawable();

        if (!this->window) {
            this->error = "cannot detect window";
            return false;
        }

        this->context = (void *)myglXGetCurrentContext();

        if (!this->context) {
            this->error = "cannot detect OpenGL context";
            return false;
        }
    }

    return true;
}

void GLContext::enter() {
    // this->old_display = (void *)myglXGetCurrentDisplay();
    // this->old_window = (void *)myglXGetCurrentDrawable();
    // this->old_context = (void *)myglXGetCurrentContext();
    // myglXMakeCurrent((Display *)this->display, (Window)this->window, (GLXContext)this->context);
}

void GLContext::exit() {
    // myglXMakeCurrent((Display *)this->old_display, (Window)this->old_window, (GLXContext)this->old_context);
}

bool GLContext::active() {
    // return this->context == myglXGetCurrentContext();
    return true;
}
