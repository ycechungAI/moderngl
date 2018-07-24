#include "gl_context.hpp"
#include "../modules.hpp"

#include <Windows.h>

#define WGL_ACCELERATION 0x2003
#define WGL_FULL_ACCELERATION 0x2027
#define WGL_CONTEXT_MAJOR_VERSION 0x2091
#define WGL_CONTEXT_MINOR_VERSION 0x2092
#define WGL_CONTEXT_PROFILE_MASK 0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT 0x0001

typedef int (WINAPI * my_ChoosePixelFormat_type)(HDC hdc, const int * piAttribIList, const float * pfAttribFList, unsigned nMaxFormats, int * piFormats, unsigned * nNumFormats);
typedef HGLRC (WINAPI * my_CreateContextAttribs_type)(HDC hdc, HGLRC hglrc, const int * attribList);

my_ChoosePixelFormat_type my_ChoosePixelFormat;
my_CreateContextAttribs_type my_CreateContextAttribs;

PIXELFORMATDESCRIPTOR pfd = {
    sizeof(PIXELFORMATDESCRIPTOR),  // nSize
    1,                              // nVersion
    PFD_DRAW_TO_WINDOW |
    PFD_SUPPORT_OPENGL |
    PFD_GENERIC_ACCELERATED |
    PFD_DOUBLEBUFFER,               // dwFlags
    0,                              // iPixelType
    32,                             // cColorBits
    0,                              // cRedBits
    0,                              // cRedShift
    0,                              // cGreenBits
    0,                              // cGreenShift
    0,                              // cBlueBits
    0,                              // cBlueShift
    0,                              // cAlphaBits
    0,                              // cAlphaShift
    0,                              // cAccumBits
    0,                              // cAccumRedBits
    0,                              // cAccumGreenBits
    0,                              // cAccumBlueBits
    0,                              // cAccumAlphaBits
    24,                             // cDepthBits
    0,                              // cStencilBits
    0,                              // cAuxBuffers
    0,                              // iLayerType
    0,                              // bReserved
    0,                              // dwLayerMask
    0,                              // dwVisibleMask
    0,                              // dwDamageMask
};

void InitModernContext() {
    static bool initialized = false;

    if (initialized) {
        return;
    }

    initialized = true;

    HMODULE hinst = GetModuleHandle(0);

    if (!hinst) {
        return;
    }

    WNDCLASS extClass = {
        CS_OWNDC,                       // style
        DefWindowProc,                  // lpfnWndProc
        0,                              // cbClsExtra
        0,                              // cbWndExtra
        hinst,                          // hInstance
        0,                              // hIcon
        0,                              // hCursor
        0,                              // hbrBackground
        0,                              // lpszMenuName
        "ContextLoader",                // lpszClassName
    };

    if (!RegisterClass(&extClass)) {
        return;
    }

    HWND loader_hwnd = CreateWindow(
        "ContextLoader",                // lpClassName
        0,                              // lpWindowName
        0,                              // dwStyle
        0,                              // x
        0,                              // y
        0,                              // nWidth
        0,                              // nHeight
        0,                              // hWndParent
        0,                              // hMenu
        hinst,                          // hInstance
        0                               // lpParam
    );

    if (!loader_hwnd) {
        return;
    }

    HDC loader_hdc = GetDC(loader_hwnd);

    if (!loader_hdc) {
        return;
    }

    int loader_pixelformat = ChoosePixelFormat(loader_hdc, &pfd);

    if (!loader_pixelformat) {
        return;
    }

    if (!SetPixelFormat(loader_hdc, loader_pixelformat, &pfd)) {
        return;
    }

    HGLRC loader_hglrc = wglCreateContext(loader_hdc);

    if (!loader_hglrc) {
        return;
    }

    if (!wglMakeCurrent(loader_hdc, loader_hglrc)) {
        return;
    }

    my_CreateContextAttribs = (my_CreateContextAttribs_type)wglGetProcAddress("wglCreateContextAttribsARB");

    if (!my_CreateContextAttribs) {
        return;
    }

    my_ChoosePixelFormat = (my_ChoosePixelFormat_type)wglGetProcAddress("wglChoosePixelFormatARB");

    if (!my_ChoosePixelFormat) {
        return;
    }

    if (!wglMakeCurrent(0, 0)) {
        return;
    }

    if (!wglDeleteContext(loader_hglrc)) {
        return;
    }

    if (!ReleaseDC(loader_hwnd, loader_hdc)) {
        return;
    }

    if (!DestroyWindow(loader_hwnd)) {
        return;
    }

    if (!UnregisterClass("ContextLoader", hinst)) {
        return;
    }
}

bool GLContext::load(bool standalone) {
    this->standalone = standalone;

    if (standalone) {
        int width = 1;
        int height = 1;

        InitModernContext();

        HINSTANCE inst = GetModuleHandle(0);

        if (!inst) {
            PyErr_Format(moderngl_error, "module handle is null");
            return false;
        }

        static bool registered = false;

        if (!registered) {
            WNDCLASS wndClass = {
                CS_OWNDC,                            // style
                DefWindowProc,                       // lpfnWndProc
                0,                                   // cbClsExtra
                0,                                   // cbWndExtra
                inst,                                // hInstance
                0,                                   // hIcon
                0,                                   // hCursor
                0,                                   // hbrBackground
                0,                                   // lpszMenuName
                "StandaloneContext",                 // lpszClassName
            };

            if (!RegisterClass(&wndClass)) {
                PyErr_Format(moderngl_error, "cannot register window class");
                return false;
            }

            registered = true;
        }

        HWND hwnd = CreateWindowEx(
            0,                                   // exStyle
            "StandaloneContext",                 // lpClassName
            0,                                   // lpWindowName
            0,                                   // dwStyle
            0,                                   // x
            0,                                   // y
            width,                               // nWidth
            height,                              // nHeight
            0,                                   // hWndParent
            0,                                   // hMenu
            inst,                                // hInstance
            0                                    // lpParam
        );

        if (!hwnd) {
            PyErr_Format(moderngl_error, "cannot create window");
            return false;
        }

        HDC hdc = GetDC(hwnd);

        if (!hdc) {
            PyErr_Format(moderngl_error, "cannot create device content");
            return false;
        }

        HGLRC hrc = 0;

        if (my_CreateContextAttribs && my_ChoosePixelFormat) {
            int pixelformat = 0;
            unsigned num_formats = 0;

            int empty1[4] = {};
            float empty2[4] = {};

            // WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB

            if (!my_ChoosePixelFormat(hdc, empty1, empty2, 1, &pixelformat, &num_formats)) {
                PyErr_Format(moderngl_error, "cannot choose pixel format");
                return false;
            }

            if (!num_formats) {
                PyErr_Format(moderngl_error, "no pixel formats available");
                return false;
            }

            if (!SetPixelFormat(hdc, pixelformat, &pfd)) {
                PyErr_Format(moderngl_error, "cannot set pixel format");
                return false;
            }

            for (int i = 0; i < versions; ++i) {
                int attribs[] = {
                    WGL_CONTEXT_PROFILE_MASK, WGL_CONTEXT_CORE_PROFILE_BIT,
                    WGL_CONTEXT_MAJOR_VERSION, version[i].major,
                    WGL_CONTEXT_MINOR_VERSION, version[i].minor,
                    0, 0,
                };

                hrc = my_CreateContextAttribs(hdc, 0, attribs);

                if (hrc) {
                    break;
                }
            }
        } else {
            int pf = ChoosePixelFormat(hdc, &pfd);

            if (!pf) {
                PyErr_Format(moderngl_error, "cannot choose pixel format");
                return false;
            }

            int set_pixel_format = SetPixelFormat(hdc, pf, &pfd);

            if (!set_pixel_format) {
                PyErr_Format(moderngl_error, "cannot set pixel format");
                return false;
            }

            hrc = wglCreateContext(hdc);
        }

        if (!hrc) {
            PyErr_Format(moderngl_error, "cannot create OpenGL context");
            return false;
        }

        int make_current = wglMakeCurrent(hdc, hrc);

        if (!make_current) {
            PyErr_Format(moderngl_error, "cannot select OpenGL context");
            return false;
        }
    }

    HGLRC hrc = wglGetCurrentContext();

    if (!hrc) {
        PyErr_Format(moderngl_error, "cannot detect context");
        return false;
    }

    HDC hdc = wglGetCurrentDC();

    if (!hdc) {
        PyErr_Format(moderngl_error, "cannot detect device content");
        return false;
    }

    HWND hwnd = WindowFromDC(hdc);

    if (!hwnd) {
        PyErr_Format(moderngl_error, "cannot detect window");
        return false;
    }

    this->window = (void *)hwnd;
    this->display = (void *)hdc;
    this->context = (void *)hrc;

    return true;
}

void GLContext::enter() {
    this->old_context = (void *)wglGetCurrentContext();
    this->old_display = (void *)wglGetCurrentDC();
    wglMakeCurrent((HDC)this->display, (HGLRC)this->context);
}

void GLContext::exit() {
    wglMakeCurrent((HDC)this->old_display, (HGLRC)this->old_context);
}

bool GLContext::active() {
    return this->context == wglGetCurrentContext();
}
