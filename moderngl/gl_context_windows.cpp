#include "gl_context.hpp"

#define UNICODE
#include <Windows.h>

#define WGL_CONTEXT_PROFILE_MASK 0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT 0x0001
#define WGL_CONTEXT_MAJOR_VERSION 0x2091
#define WGL_CONTEXT_MINOR_VERSION 0x2092

typedef HGLRC (WINAPI * PFNWGLCREATECONTEXTATTRIBSARBPROC)(HDC hDC, HGLRC hShareContext, const int * attribList);
typedef BOOL (WINAPI * PFNWGLSWAPINTERVALEXTPROC)(int interval);

HINSTANCE hinst;

bool GLContext::load(bool standalone, int glversion) {
    this->standalone = standalone;
    this->glversion = glversion;

    if (!hinst) {
        this->error = "hinstance is NULL";
        return false;
    }

    HWND hwnd;
    HDC hdc;
    HGLRC hrc;

    if (standalone) {
        WNDCLASSEXW wnd_class = {
            sizeof(WNDCLASSEXW), CS_OWNDC, DefWindowProc, 0, 0, hinst, NULL, NULL, NULL, NULL, L"moderngl", NULL,
        };

        if (!RegisterClassEx(&wnd_class)) {
            this->error = "RegisterClassEx failed";
            return false;
        }

        hwnd = CreateWindowEx(NULL, L"moderngl", NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, hinst, NULL);
        if (!hwnd) {
            this->error = "CreateWindowEx failed";
            return false;
        }

        hdc = GetDC(hwnd);
        if (!hdc) {
            this->error = "GetDC failed";
            return false;
        }

        PIXELFORMATDESCRIPTOR pfd = {
            sizeof(PIXELFORMATDESCRIPTOR),
            1,
            PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_GENERIC_ACCELERATED | PFD_DOUBLEBUFFER,
            0,
            24,
        };

        int pixelformat = ChoosePixelFormat(hdc, &pfd);
        if (!pixelformat) {
            this->error = "ChoosePixelFormat failed";
            return false;
        }

        if (!SetPixelFormat(hdc, pixelformat, &pfd)) {
            this->error = "SetPixelFormat failed";
            return false;
        }

        hrc = wglCreateContext(hdc);
        if (!hrc) {
            this->error = "wglCreateContext failed";
            return false;
        }

        if (!wglMakeCurrent(hdc, hrc)) {
            this->error = "wglMakeCurrent failed";
            return false;
        }
    } else {
        hdc = wglGetCurrentDC();
        if (!hdc) {
            this->error = "wglGetCurrentDC failed";
            return false;
        }
        hrc = wglGetCurrentContext();
        if (!hrc) {
            this->error = "wglGetCurrentContext failed";
            return false;
        }
    }

    if (glversion) {
        FARPROC proc = wglGetProcAddress("wglCreateContextAttribsARB");
        PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)proc;
        if (!wglCreateContextAttribsARB) {
            this->error = "wglCreateContextAttribsARB is missing";
            return false;
        }

        if (!wglMakeCurrent(NULL, NULL)) {
            this->error = "wglMakeCurrent failed";
            return false;
        }

        if (standalone) {
            if (!wglDeleteContext(hrc)) {
                this->error = "wglDeleteContext failed";
                return false;
            }
        }

        int attribs[] = {
            WGL_CONTEXT_PROFILE_MASK, WGL_CONTEXT_CORE_PROFILE_BIT,
            WGL_CONTEXT_MAJOR_VERSION, glversion / 100 % 10,
            WGL_CONTEXT_MINOR_VERSION, glversion / 10 % 10,
            0, 0,
        };

        hrc = wglCreateContextAttribsARB(hdc, standalone ? NULL : hrc, attribs);
        if (!hrc) {
            this->error = "wglCreateContextAttribsARB failed";
            return false;
        }
    }

    if (!wglMakeCurrent(hdc, hrc)) {
        this->error = "wglMakeCurrent failed";
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

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    if (fdwReason == DLL_PROCESS_ATTACH) {
        hinst = hinstDLL;
    }
    return true;
}
