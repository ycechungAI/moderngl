#include "gl_context.hpp"
#include "modules.hpp"

struct GLVersion {
	int major;
	int minor;
};

GLVersion version[] = {
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

int versions = sizeof(version) / sizeof(GLVersion);

#if defined(_WIN32) || defined(_WIN64)

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

#elif defined(__APPLE__)

// TODO: restore

#else

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
			PyErr_Format(moderngl_error, "cannot detect the display");
			return context;
		}

		int nelements = 0;

		GLXFBConfig * fbc = glXChooseFBConfig(dpy, DefaultScreen(dpy), 0, &nelements);

		if (!fbc) {
			PyErr_Format(moderngl_error, "cannot read the display configuration");
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

			PyErr_Format(moderngl_error, "cannot choose a visual info");
			return context;
		}

		XSetWindowAttributes swa;
		swa.colormap = XCreateColormap(dpy, RootWindow(dpy, vi->screen), vi->visual, AllocNone);
		swa.border_pixel = 0;
		swa.event_mask = StructureNotifyMask;

		Window win = XCreateWindow(dpy, RootWindow(dpy, vi->screen), 0, 0, width, height, 0, vi->depth, InputOutput, vi->visual, CWBorderPixel | CWColormap | CWEventMask, &swa);

		if (!win) {
			XCloseDisplay(dpy);

			PyErr_Format(moderngl_error, "cannot create window");
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

			PyErr_Format(moderngl_error, "cannot create OpenGL context");
			return context;
		}

		XSetErrorHandler(0);

		int make_current = glXMakeCurrent(dpy, win, ctx);

		if (!make_current) {
			glXDestroyContext(dpy, ctx);
			XDestroyWindow(dpy, win);
			XCloseDisplay(dpy);

			PyErr_Format(moderngl_error, "cannot select OpenGL context");
			return context;
		}
	}

	Display * dpy = glXGetCurrentDisplay();

	if (!dpy) {
		PyErr_Format(moderngl_error, "cannot detect display");
		return false;
	}

	Window win = glXGetCurrentDrawable();

	if (!win) {
		PyErr_Format(moderngl_error, "cannot detect window");
		return false;
	}

	GLXContext ctx = glXGetCurrentContext();

	if (!ctx) {
		PyErr_Format(moderngl_error, "cannot detect OpenGL context");
		return false;
	}

	this->window = (void *)win;
	this->display = (void *)dpy;
	this->context = (void *)ctx;

	return true;
}

void DestroyGLContext(const GLContext & context) {
	if (!context.standalone) {
		return;
	}

	if (context.display) {
		glXMakeCurrent((Display *)context.display, 0, 0);

		if (context.context) {
			glXDestroyContext((Display *)context.display, (GLXContext)context.context);
			// context.context = 0;
		}

		if (context.window) {
			XDestroyWindow((Display *)context.display, (Window)context.window);
			// context.window = 0;
		}

		XCloseDisplay((Display *)context.display);
		// context.display = 0;
	}
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

#endif
