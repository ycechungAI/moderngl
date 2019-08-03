#include "gl_context.hpp"

#include <OpenGL/OpenGL.h>
#include <ApplicationServices/ApplicationServices.h>

#include <OpenGL/gl.h>
#include <OpenGL/glext.h>

bool GLContext::load(bool standalone) {
    this->standalone = standalone;

    if (standalone) {
        // CGDirectDisplayID display = CGMainDisplayID();
        // CGOpenGLDisplayMask cglDisplayMask = CGDisplayIDToOpenGLDisplayMask(display);
        // CGLContextObj curr_ctx = CGLGetCurrentContext();

        GLint num_pixelformats = 0;
        CGLPixelFormatObj pixelformat = 0;

        // kCGLPFAAccelerated
        CGLPixelFormatAttribute attribs[] = {
            kCGLPFAOpenGLProfile,
            (CGLPixelFormatAttribute)kCGLOGLPVersion_GL4_Core,
            (CGLPixelFormatAttribute)0,
        };

        CGLChoosePixelFormat(attribs, &pixelformat, &num_pixelformats);

        if (!pixelformat) {

            CGLPixelFormatAttribute attribs[] = {
                kCGLPFAOpenGLProfile,
                (CGLPixelFormatAttribute)kCGLOGLPVersion_GL3_Core,
                (CGLPixelFormatAttribute)0,
            };

            CGLChoosePixelFormat(attribs, &pixelformat, &num_pixelformats);

            if (!pixelformat) {
                CGLPixelFormatAttribute attribs[] = {
                    (CGLPixelFormatAttribute)0,
                };

                CGLChoosePixelFormat(attribs, &pixelformat, &num_pixelformats);
            }
        }

        if (!pixelformat) {
            this->error = "cannot choose pixel format";
            return false;
        }

        CGLContextObj cgl_context = 0;

        CGLCreateContext(pixelformat, 0, &cgl_context);
        CGLDestroyPixelFormat(pixelformat);

        if (!cgl_context) {
            this->error = "cannot create OpenGL context";
            return false;
        }

        CGLSetCurrentContext(cgl_context);

        int fbo = 0;
        int color_rbo = 0;
        int depth_rbo = 0;

        glGenFramebuffers(1, (GLuint *)&fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        glGenRenderbuffers(1, (GLuint *)&color_rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, color_rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, 1, 1);

        glGenRenderbuffers(1, (GLuint *)&depth_rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, depth_rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 1, 1);

        glFramebufferRenderbuffer(
            GL_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0,
            GL_RENDERBUFFER,
            color_rbo
        );

        glFramebufferRenderbuffer(
            GL_FRAMEBUFFER,
            GL_DEPTH_ATTACHMENT,
            GL_RENDERBUFFER,
            depth_rbo
        );

        int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

        if (status != GL_FRAMEBUFFER_COMPLETE) {
            this->error = "the framebuffer is not complete";

            switch (status) {
                case GL_FRAMEBUFFER_UNDEFINED:
                    this->error = "the framebuffer is not complete (UNDEFINED)";
                    break;

                case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
                    this->error = "the framebuffer is not complete (INCOMPLETE_ATTACHMENT)";
                    break;

                case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
                    this->error = "the framebuffer is not complete (INCOMPLETE_MISSING_ATTACHMENT)";
                    break;

                case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
                    this->error = "the framebuffer is not complete (INCOMPLETE_DRAW_BUFFER)";
                    break;

                case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
                    this->error = "the framebuffer is not complete (INCOMPLETE_READ_BUFFER)";
                    break;

                case GL_FRAMEBUFFER_UNSUPPORTED:
                    this->error = "the framebuffer is not complete (UNSUPPORTED)";
                    break;

                case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
                    this->error = "the framebuffer is not complete (INCOMPLETE_MULTISAMPLE)";
                    break;
            }

            return false;
        }
    }

    CGLContextObj ctx = CGLGetCurrentContext();

    if (!ctx) {
        this->error = "cannot detect OpenGL context";
        return false;
    }

    this->window = 0;
    this->display = 0;
    this->context = (void *)ctx;

    return true;
}

void GLContext::enter() {
    this->old_context = (void *)CGLGetCurrentContext();
    CGLSetCurrentContext((CGLContextObj)this->context);
}

void GLContext::exit() {
    CGLSetCurrentContext((CGLContextObj)this->old_context);
}

bool GLContext::active() {
    return this->context == CGLGetCurrentContext();
}

// void DestroyGLContext(const GLContext & context) {
// 	if (!context.standalone) {
// 		return;
// 	}

// 	if (context.context) {
// 		CGLDestroyContext((CGLContextObj)context.context);
// 		// context.context = 0;
// 	}
// }
