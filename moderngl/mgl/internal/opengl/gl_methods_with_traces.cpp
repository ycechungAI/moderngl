#include "gl_methods_with_traces.hpp"
#include "gl_tracer.hpp"

#if defined(_WIN32) || defined(_WIN64)

#define PREFIX ""

#include <Windows.h>

#ifdef MemoryBarrier
#undef MemoryBarrier
#endif

void * LoadMethod(const char * method) {
    static HMODULE opengl32 = LoadLibrary("opengl32.dll");

    void * proc = (void *)GetProcAddress(opengl32, method);

    if (proc) {
        return proc;
    }

    proc = (void *)wglGetProcAddress(method);

    if (proc) {
        return proc;
    }

    return 0;
}

#elif defined(__APPLE__)

#define PREFIX "_"

#import <mach-o/dyld.h>
#import <stdlib.h>
#import <string.h>

void * LoadMethod(const char * method) {
    NSSymbol symbol = 0;

    if (NSIsSymbolNameDefined(method)) {
        symbol = NSLookupAndBindSymbol(method);
    }

    return symbol ? NSAddressOfSymbol(symbol) : 0;
}

#else

#define PREFIX ""

#include <dlfcn.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

typedef const void * (* PROC_glXGetProcAddress)(const char *);

void * LoadMethod(const char * method) {
    static void * libgl = dlopen("libGL.so.1", RTLD_LAZY);
    static PROC_glXGetProcAddress glXGetProcAddress = (PROC_glXGetProcAddress)dlsym(libgl, "glXGetProcAddress");

    void * proc = (void *)dlsym(libgl, method);

    if (proc) {
        return proc;
    }

    proc = (void *)glXGetProcAddress(method);

    if (proc) {
        return proc;
    }

    return 0;
}

#endif

#define TRACE(name, ...) trace_gl_method(_GetError(), name, ##__VA_ARGS__);

void GLMethods::CullFace(GLenum mode) const {
    _CullFace(mode);
    TRACE("glCullFace");
}

void GLMethods::FrontFace(GLenum mode) const {
    _FrontFace(mode);
    TRACE("glFrontFace");
}

void GLMethods::Hint(GLenum target, GLenum mode) const {
    _Hint(target, mode);
    TRACE("glHint");
}

void GLMethods::LineWidth(GLfloat width) const {
    _LineWidth(width);
    TRACE("glLineWidth");
}

void GLMethods::PointSize(GLfloat size) const {
    _PointSize(size);
    TRACE("glPointSize");
}

void GLMethods::PolygonMode(GLenum face, GLenum mode) const {
    _PolygonMode(face, mode);
    TRACE("glPolygonMode");
}

void GLMethods::Scissor(GLint x, GLint y, GLsizei width, GLsizei height) const {
    _Scissor(x, y, width, height);
    TRACE("glScissor");
}

void GLMethods::TexParameterf(GLenum target, GLenum pname, GLfloat param) const {
    _TexParameterf(target, pname, param);
    TRACE("glTexParameterf");
}

void GLMethods::TexParameterfv(GLenum target, GLenum pname, const GLfloat * params) const {
    _TexParameterfv(target, pname, params);
    TRACE("glTexParameterfv");
}

void GLMethods::TexParameteri(GLenum target, GLenum pname, GLint param) const {
    _TexParameteri(target, pname, param);
    TRACE("glTexParameteri");
}

void GLMethods::TexParameteriv(GLenum target, GLenum pname, const GLint * params) const {
    _TexParameteriv(target, pname, params);
    TRACE("glTexParameteriv");
}

void GLMethods::TexImage1D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void * pixels) const {
    _TexImage1D(target, level, internalformat, width, border, format, type, pixels);
    TRACE("glTexImage1D");
}

void GLMethods::TexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void * pixels) const {
    _TexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
    TRACE("glTexImage2D");
}

void GLMethods::DrawBuffer(GLenum buf) const {
    _DrawBuffer(buf);
    TRACE("glDrawBuffer");
}

void GLMethods::Clear(GLbitfield mask) const {
    _Clear(mask);
    TRACE("glClear");
}

void GLMethods::ClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) const {
    _ClearColor(red, green, blue, alpha);
    TRACE("glClearColor");
}

void GLMethods::ClearStencil(GLint s) const {
    _ClearStencil(s);
    TRACE("glClearStencil");
}

void GLMethods::ClearDepth(GLdouble depth) const {
    _ClearDepth(depth);
    TRACE("glClearDepth");
}

void GLMethods::StencilMask(GLuint mask) const {
    _StencilMask(mask);
    TRACE("glStencilMask");
}

void GLMethods::ColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha) const {
    _ColorMask(red, green, blue, alpha);
    TRACE("glColorMask");
}

void GLMethods::DepthMask(GLboolean flag) const {
    _DepthMask(flag);
    TRACE("glDepthMask");
}

void GLMethods::Disable(GLenum cap) const {
    _Disable(cap);
    TRACE("glDisable");
}

void GLMethods::Enable(GLenum cap) const {
    _Enable(cap);
    TRACE("glEnable");
}

void GLMethods::Finish() const {
    _Finish();
    TRACE("glFinish");
}

void GLMethods::Flush() const {
    _Flush();
    TRACE("glFlush");
}

void GLMethods::BlendFunc(GLenum sfactor, GLenum dfactor) const {
    _BlendFunc(sfactor, dfactor);
    TRACE("glBlendFunc");
}

void GLMethods::LogicOp(GLenum opcode) const {
    _LogicOp(opcode);
    TRACE("glLogicOp");
}

void GLMethods::StencilFunc(GLenum func, GLint ref, GLuint mask) const {
    _StencilFunc(func, ref, mask);
    TRACE("glStencilFunc");
}

void GLMethods::StencilOp(GLenum fail, GLenum zfail, GLenum zpass) const {
    _StencilOp(fail, zfail, zpass);
    TRACE("glStencilOp");
}

void GLMethods::DepthFunc(GLenum func) const {
    _DepthFunc(func);
    TRACE("glDepthFunc");
}

void GLMethods::PixelStoref(GLenum pname, GLfloat param) const {
    _PixelStoref(pname, param);
    TRACE("glPixelStoref");
}

void GLMethods::PixelStorei(GLenum pname, GLint param) const {
    _PixelStorei(pname, param);
    TRACE("glPixelStorei");
}

void GLMethods::ReadBuffer(GLenum src) const {
    _ReadBuffer(src);
    TRACE("glReadBuffer");
}

void GLMethods::ReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void * pixels) const {
    _ReadPixels(x, y, width, height, format, type, pixels);
    TRACE("glReadPixels");
}

void GLMethods::GetBooleanv(GLenum pname, GLboolean * data) const {
    _GetBooleanv(pname, data);
    TRACE("glGetBooleanv");
}

void GLMethods::GetDoublev(GLenum pname, GLdouble * data) const {
    _GetDoublev(pname, data);
    TRACE("glGetDoublev");
}

GLenum GLMethods::GetError() const {
    GLenum __result = _GetError();
    TRACE("glGetError");
    return __result;
}

void GLMethods::GetFloatv(GLenum pname, GLfloat * data) const {
    _GetFloatv(pname, data);
    TRACE("glGetFloatv");
}

void GLMethods::GetIntegerv(GLenum pname, GLint * data) const {
    _GetIntegerv(pname, data);
    TRACE("glGetIntegerv");
}

const GLubyte * GLMethods::GetString(GLenum name) const {
    const GLubyte * __result = _GetString(name);
    TRACE("glGetString");
    return __result;
}

void GLMethods::GetTexImage(GLenum target, GLint level, GLenum format, GLenum type, void * pixels) const {
    _GetTexImage(target, level, format, type, pixels);
    TRACE("glGetTexImage");
}

void GLMethods::GetTexParameterfv(GLenum target, GLenum pname, GLfloat * params) const {
    _GetTexParameterfv(target, pname, params);
    TRACE("glGetTexParameterfv");
}

void GLMethods::GetTexParameteriv(GLenum target, GLenum pname, GLint * params) const {
    _GetTexParameteriv(target, pname, params);
    TRACE("glGetTexParameteriv");
}

void GLMethods::GetTexLevelParameterfv(GLenum target, GLint level, GLenum pname, GLfloat * params) const {
    _GetTexLevelParameterfv(target, level, pname, params);
    TRACE("glGetTexLevelParameterfv");
}

void GLMethods::GetTexLevelParameteriv(GLenum target, GLint level, GLenum pname, GLint * params) const {
    _GetTexLevelParameteriv(target, level, pname, params);
    TRACE("glGetTexLevelParameteriv");
}

GLboolean GLMethods::IsEnabled(GLenum cap) const {
    GLboolean __result = _IsEnabled(cap);
    TRACE("glIsEnabled");
    return __result;
}

void GLMethods::DepthRange(GLdouble n, GLdouble f) const {
    _DepthRange(n, f);
    TRACE("glDepthRange");
}

void GLMethods::Viewport(GLint x, GLint y, GLsizei width, GLsizei height) const {
    _Viewport(x, y, width, height);
    TRACE("glViewport");
}

void GLMethods::DrawArrays(GLenum mode, GLint first, GLsizei count) const {
    _DrawArrays(mode, first, count);
    TRACE("glDrawArrays");
}

void GLMethods::DrawElements(GLenum mode, GLsizei count, GLenum type, const void * indices) const {
    _DrawElements(mode, count, type, indices);
    TRACE("glDrawElements");
}

void GLMethods::GetPointerv(GLenum pname, void ** params) const {
    _GetPointerv(pname, params);
    TRACE("glGetPointerv");
}

void GLMethods::PolygonOffset(GLfloat factor, GLfloat units) const {
    _PolygonOffset(factor, units);
    TRACE("glPolygonOffset");
}

void GLMethods::CopyTexImage1D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border) const {
    _CopyTexImage1D(target, level, internalformat, x, y, width, border);
    TRACE("glCopyTexImage1D");
}

void GLMethods::CopyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border) const {
    _CopyTexImage2D(target, level, internalformat, x, y, width, height, border);
    TRACE("glCopyTexImage2D");
}

void GLMethods::CopyTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width) const {
    _CopyTexSubImage1D(target, level, xoffset, x, y, width);
    TRACE("glCopyTexSubImage1D");
}

void GLMethods::CopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height) const {
    _CopyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height);
    TRACE("glCopyTexSubImage2D");
}

void GLMethods::TexSubImage1D(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void * pixels) const {
    _TexSubImage1D(target, level, xoffset, width, format, type, pixels);
    TRACE("glTexSubImage1D");
}

void GLMethods::TexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void * pixels) const {
    _TexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels);
    TRACE("glTexSubImage2D");
}

void GLMethods::BindTexture(GLenum target, GLuint texture) const {
    _BindTexture(target, texture);
    TRACE("glBindTexture");
}

void GLMethods::DeleteTextures(GLsizei n, const GLuint * textures) const {
    _DeleteTextures(n, textures);
    TRACE("glDeleteTextures");
}

void GLMethods::GenTextures(GLsizei n, GLuint * textures) const {
    _GenTextures(n, textures);
    TRACE("glGenTextures");
}

GLboolean GLMethods::IsTexture(GLuint texture) const {
    GLboolean __result = _IsTexture(texture);
    TRACE("glIsTexture");
    return __result;
}

void GLMethods::DrawRangeElements(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void * indices) const {
    _DrawRangeElements(mode, start, end, count, type, indices);
    TRACE("glDrawRangeElements");
}

void GLMethods::TexImage3D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void * pixels) const {
    _TexImage3D(target, level, internalformat, width, height, depth, border, format, type, pixels);
    TRACE("glTexImage3D");
}

void GLMethods::TexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void * pixels) const {
    _TexSubImage3D(target, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels);
    TRACE("glTexSubImage3D");
}

void GLMethods::CopyTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height) const {
    _CopyTexSubImage3D(target, level, xoffset, yoffset, zoffset, x, y, width, height);
    TRACE("glCopyTexSubImage3D");
}

void GLMethods::ActiveTexture(GLenum texture) const {
    _ActiveTexture(texture);
    TRACE("glActiveTexture");
}

void GLMethods::SampleCoverage(GLfloat value, GLboolean invert) const {
    _SampleCoverage(value, invert);
    TRACE("glSampleCoverage");
}

void GLMethods::CompressedTexImage3D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void * data) const {
    _CompressedTexImage3D(target, level, internalformat, width, height, depth, border, imageSize, data);
    TRACE("glCompressedTexImage3D");
}

void GLMethods::CompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void * data) const {
    _CompressedTexImage2D(target, level, internalformat, width, height, border, imageSize, data);
    TRACE("glCompressedTexImage2D");
}

void GLMethods::CompressedTexImage1D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void * data) const {
    _CompressedTexImage1D(target, level, internalformat, width, border, imageSize, data);
    TRACE("glCompressedTexImage1D");
}

void GLMethods::CompressedTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void * data) const {
    _CompressedTexSubImage3D(target, level, xoffset, yoffset, zoffset, width, height, depth, format, imageSize, data);
    TRACE("glCompressedTexSubImage3D");
}

void GLMethods::CompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void * data) const {
    _CompressedTexSubImage2D(target, level, xoffset, yoffset, width, height, format, imageSize, data);
    TRACE("glCompressedTexSubImage2D");
}

void GLMethods::CompressedTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void * data) const {
    _CompressedTexSubImage1D(target, level, xoffset, width, format, imageSize, data);
    TRACE("glCompressedTexSubImage1D");
}

void GLMethods::GetCompressedTexImage(GLenum target, GLint level, void * img) const {
    _GetCompressedTexImage(target, level, img);
    TRACE("glGetCompressedTexImage");
}

void GLMethods::BlendFuncSeparate(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha) const {
    _BlendFuncSeparate(sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha);
    TRACE("glBlendFuncSeparate");
}

void GLMethods::MultiDrawArrays(GLenum mode, const GLint * first, const GLsizei * count, GLsizei drawcount) const {
    _MultiDrawArrays(mode, first, count, drawcount);
    TRACE("glMultiDrawArrays");
}

void GLMethods::MultiDrawElements(GLenum mode, const GLsizei * count, GLenum type, const void * const * indices, GLsizei drawcount) const {
    _MultiDrawElements(mode, count, type, indices, drawcount);
    TRACE("glMultiDrawElements");
}

void GLMethods::PointParameterf(GLenum pname, GLfloat param) const {
    _PointParameterf(pname, param);
    TRACE("glPointParameterf");
}

void GLMethods::PointParameterfv(GLenum pname, const GLfloat * params) const {
    _PointParameterfv(pname, params);
    TRACE("glPointParameterfv");
}

void GLMethods::PointParameteri(GLenum pname, GLint param) const {
    _PointParameteri(pname, param);
    TRACE("glPointParameteri");
}

void GLMethods::PointParameteriv(GLenum pname, const GLint * params) const {
    _PointParameteriv(pname, params);
    TRACE("glPointParameteriv");
}

void GLMethods::BlendColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) const {
    _BlendColor(red, green, blue, alpha);
    TRACE("glBlendColor");
}

void GLMethods::BlendEquation(GLenum mode) const {
    _BlendEquation(mode);
    TRACE("glBlendEquation");
}

void GLMethods::GenQueries(GLsizei n, GLuint * ids) const {
    _GenQueries(n, ids);
    TRACE("glGenQueries");
}

void GLMethods::DeleteQueries(GLsizei n, const GLuint * ids) const {
    _DeleteQueries(n, ids);
    TRACE("glDeleteQueries");
}

GLboolean GLMethods::IsQuery(GLuint id) const {
    GLboolean __result = _IsQuery(id);
    TRACE("glIsQuery");
    return __result;
}

void GLMethods::BeginQuery(GLenum target, GLuint id) const {
    _BeginQuery(target, id);
    TRACE("glBeginQuery");
}

void GLMethods::EndQuery(GLenum target) const {
    _EndQuery(target);
    TRACE("glEndQuery");
}

void GLMethods::GetQueryiv(GLenum target, GLenum pname, GLint * params) const {
    _GetQueryiv(target, pname, params);
    TRACE("glGetQueryiv");
}

void GLMethods::GetQueryObjectiv(GLuint id, GLenum pname, GLint * params) const {
    _GetQueryObjectiv(id, pname, params);
    TRACE("glGetQueryObjectiv");
}

void GLMethods::GetQueryObjectuiv(GLuint id, GLenum pname, GLuint * params) const {
    _GetQueryObjectuiv(id, pname, params);
    TRACE("glGetQueryObjectuiv");
}

void GLMethods::BindBuffer(GLenum target, GLuint buffer) const {
    _BindBuffer(target, buffer);
    TRACE("glBindBuffer(%x, %d)", target, buffer);
}

void GLMethods::DeleteBuffers(GLsizei n, const GLuint * buffers) const {
    _DeleteBuffers(n, buffers);
    TRACE("glDeleteBuffers");
}

void GLMethods::GenBuffers(GLsizei n, GLuint * buffers) const {
    _GenBuffers(n, buffers);
    TRACE("glGenBuffers");
}

GLboolean GLMethods::IsBuffer(GLuint buffer) const {
    GLboolean __result = _IsBuffer(buffer);
    TRACE("glIsBuffer");
    return __result;
}

void GLMethods::BufferData(GLenum target, GLsizeiptr size, const void * data, GLenum usage) const {
    _BufferData(target, size, data, usage);
    TRACE("glBufferData");
}

void GLMethods::BufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const void * data) const {
    _BufferSubData(target, offset, size, data);
    TRACE("glBufferSubData");
}

void GLMethods::GetBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, void * data) const {
    _GetBufferSubData(target, offset, size, data);
    TRACE("glGetBufferSubData");
}

void * GLMethods::MapBuffer(GLenum target, GLenum access) const {
    void * __result = _MapBuffer(target, access);
    TRACE("glMapBuffer");
    return __result;
}

GLboolean GLMethods::UnmapBuffer(GLenum target) const {
    GLboolean __result = _UnmapBuffer(target);
    TRACE("glUnmapBuffer");
    return __result;
}

void GLMethods::GetBufferParameteriv(GLenum target, GLenum pname, GLint * params) const {
    _GetBufferParameteriv(target, pname, params);
    TRACE("glGetBufferParameteriv");
}

void GLMethods::GetBufferPointerv(GLenum target, GLenum pname, void ** params) const {
    _GetBufferPointerv(target, pname, params);
    TRACE("glGetBufferPointerv");
}

void GLMethods::BlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha) const {
    _BlendEquationSeparate(modeRGB, modeAlpha);
    TRACE("glBlendEquationSeparate");
}

void GLMethods::DrawBuffers(GLsizei n, const GLenum * bufs) const {
    _DrawBuffers(n, bufs);
    TRACE("glDrawBuffers");
}

void GLMethods::StencilOpSeparate(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass) const {
    _StencilOpSeparate(face, sfail, dpfail, dppass);
    TRACE("glStencilOpSeparate");
}

void GLMethods::StencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint mask) const {
    _StencilFuncSeparate(face, func, ref, mask);
    TRACE("glStencilFuncSeparate");
}

void GLMethods::StencilMaskSeparate(GLenum face, GLuint mask) const {
    _StencilMaskSeparate(face, mask);
    TRACE("glStencilMaskSeparate");
}

void GLMethods::AttachShader(GLuint program, GLuint shader) const {
    _AttachShader(program, shader);
    TRACE("glAttachShader");
}

void GLMethods::BindAttribLocation(GLuint program, GLuint index, const GLchar * name) const {
    _BindAttribLocation(program, index, name);
    TRACE("glBindAttribLocation");
}

void GLMethods::CompileShader(GLuint shader) const {
    _CompileShader(shader);
    TRACE("glCompileShader");
}

GLuint GLMethods::CreateProgram() const {
    GLuint __result = _CreateProgram();
    TRACE("glCreateProgram");
    return __result;
}

GLuint GLMethods::CreateShader(GLenum type) const {
    GLuint __result = _CreateShader(type);
    TRACE("glCreateShader");
    return __result;
}

void GLMethods::DeleteProgram(GLuint program) const {
    _DeleteProgram(program);
    TRACE("glDeleteProgram");
}

void GLMethods::DeleteShader(GLuint shader) const {
    _DeleteShader(shader);
    TRACE("glDeleteShader");
}

void GLMethods::DetachShader(GLuint program, GLuint shader) const {
    _DetachShader(program, shader);
    TRACE("glDetachShader");
}

void GLMethods::DisableVertexAttribArray(GLuint index) const {
    _DisableVertexAttribArray(index);
    TRACE("glDisableVertexAttribArray");
}

void GLMethods::EnableVertexAttribArray(GLuint index) const {
    _EnableVertexAttribArray(index);
    TRACE("glEnableVertexAttribArray");
}

void GLMethods::GetActiveAttrib(GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLint * size, GLenum * type, GLchar * name) const {
    _GetActiveAttrib(program, index, bufSize, length, size, type, name);
    TRACE("glGetActiveAttrib");
}

void GLMethods::GetActiveUniform(GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLint * size, GLenum * type, GLchar * name) const {
    _GetActiveUniform(program, index, bufSize, length, size, type, name);
    TRACE("glGetActiveUniform");
}

void GLMethods::GetAttachedShaders(GLuint program, GLsizei maxCount, GLsizei * count, GLuint * shaders) const {
    _GetAttachedShaders(program, maxCount, count, shaders);
    TRACE("glGetAttachedShaders");
}

GLint GLMethods::GetAttribLocation(GLuint program, const GLchar * name) const {
    GLint __result = _GetAttribLocation(program, name);
    TRACE("glGetAttribLocation");
    return __result;
}

void GLMethods::GetProgramiv(GLuint program, GLenum pname, GLint * params) const {
    _GetProgramiv(program, pname, params);
    TRACE("glGetProgramiv");
}

void GLMethods::GetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei * length, GLchar * infoLog) const {
    _GetProgramInfoLog(program, bufSize, length, infoLog);
    TRACE("glGetProgramInfoLog");
}

void GLMethods::GetShaderiv(GLuint shader, GLenum pname, GLint * params) const {
    _GetShaderiv(shader, pname, params);
    TRACE("glGetShaderiv");
}

void GLMethods::GetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei * length, GLchar * infoLog) const {
    _GetShaderInfoLog(shader, bufSize, length, infoLog);
    TRACE("glGetShaderInfoLog");
}

void GLMethods::GetShaderSource(GLuint shader, GLsizei bufSize, GLsizei * length, GLchar * source) const {
    _GetShaderSource(shader, bufSize, length, source);
    TRACE("glGetShaderSource");
}

GLint GLMethods::GetUniformLocation(GLuint program, const GLchar * name) const {
    GLint __result = _GetUniformLocation(program, name);
    TRACE("glGetUniformLocation");
    return __result;
}

void GLMethods::GetUniformfv(GLuint program, GLint location, GLfloat * params) const {
    _GetUniformfv(program, location, params);
    TRACE("glGetUniformfv");
}

void GLMethods::GetUniformiv(GLuint program, GLint location, GLint * params) const {
    _GetUniformiv(program, location, params);
    TRACE("glGetUniformiv");
}

void GLMethods::GetVertexAttribdv(GLuint index, GLenum pname, GLdouble * params) const {
    _GetVertexAttribdv(index, pname, params);
    TRACE("glGetVertexAttribdv");
}

void GLMethods::GetVertexAttribfv(GLuint index, GLenum pname, GLfloat * params) const {
    _GetVertexAttribfv(index, pname, params);
    TRACE("glGetVertexAttribfv");
}

void GLMethods::GetVertexAttribiv(GLuint index, GLenum pname, GLint * params) const {
    _GetVertexAttribiv(index, pname, params);
    TRACE("glGetVertexAttribiv");
}

void GLMethods::GetVertexAttribPointerv(GLuint index, GLenum pname, void ** pointer) const {
    _GetVertexAttribPointerv(index, pname, pointer);
    TRACE("glGetVertexAttribPointerv");
}

GLboolean GLMethods::IsProgram(GLuint program) const {
    GLboolean __result = _IsProgram(program);
    TRACE("glIsProgram");
    return __result;
}

GLboolean GLMethods::IsShader(GLuint shader) const {
    GLboolean __result = _IsShader(shader);
    TRACE("glIsShader");
    return __result;
}

void GLMethods::LinkProgram(GLuint program) const {
    _LinkProgram(program);
    TRACE("glLinkProgram");
}

void GLMethods::ShaderSource(GLuint shader, GLsizei count, const GLchar * const * string, const GLint * length) const {
    _ShaderSource(shader, count, string, length);
    TRACE("glShaderSource");
}

void GLMethods::UseProgram(GLuint program) const {
    _UseProgram(program);
    TRACE("glUseProgram");
}

void GLMethods::Uniform1f(GLint location, GLfloat v0) const {
    _Uniform1f(location, v0);
    TRACE("glUniform1f");
}

void GLMethods::Uniform2f(GLint location, GLfloat v0, GLfloat v1) const {
    _Uniform2f(location, v0, v1);
    TRACE("glUniform2f");
}

void GLMethods::Uniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2) const {
    _Uniform3f(location, v0, v1, v2);
    TRACE("glUniform3f");
}

void GLMethods::Uniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) const {
    _Uniform4f(location, v0, v1, v2, v3);
    TRACE("glUniform4f");
}

void GLMethods::Uniform1i(GLint location, GLint v0) const {
    _Uniform1i(location, v0);
    TRACE("glUniform1i");
}

void GLMethods::Uniform2i(GLint location, GLint v0, GLint v1) const {
    _Uniform2i(location, v0, v1);
    TRACE("glUniform2i");
}

void GLMethods::Uniform3i(GLint location, GLint v0, GLint v1, GLint v2) const {
    _Uniform3i(location, v0, v1, v2);
    TRACE("glUniform3i");
}

void GLMethods::Uniform4i(GLint location, GLint v0, GLint v1, GLint v2, GLint v3) const {
    _Uniform4i(location, v0, v1, v2, v3);
    TRACE("glUniform4i");
}

void GLMethods::Uniform1fv(GLint location, GLsizei count, const GLfloat * value) const {
    _Uniform1fv(location, count, value);
    TRACE("glUniform1fv");
}

void GLMethods::Uniform2fv(GLint location, GLsizei count, const GLfloat * value) const {
    _Uniform2fv(location, count, value);
    TRACE("glUniform2fv");
}

void GLMethods::Uniform3fv(GLint location, GLsizei count, const GLfloat * value) const {
    _Uniform3fv(location, count, value);
    TRACE("glUniform3fv");
}

void GLMethods::Uniform4fv(GLint location, GLsizei count, const GLfloat * value) const {
    _Uniform4fv(location, count, value);
    TRACE("glUniform4fv");
}

void GLMethods::Uniform1iv(GLint location, GLsizei count, const GLint * value) const {
    _Uniform1iv(location, count, value);
    TRACE("glUniform1iv");
}

void GLMethods::Uniform2iv(GLint location, GLsizei count, const GLint * value) const {
    _Uniform2iv(location, count, value);
    TRACE("glUniform2iv");
}

void GLMethods::Uniform3iv(GLint location, GLsizei count, const GLint * value) const {
    _Uniform3iv(location, count, value);
    TRACE("glUniform3iv");
}

void GLMethods::Uniform4iv(GLint location, GLsizei count, const GLint * value) const {
    _Uniform4iv(location, count, value);
    TRACE("glUniform4iv");
}

void GLMethods::UniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value) const {
    _UniformMatrix2fv(location, count, transpose, value);
    TRACE("glUniformMatrix2fv");
}

void GLMethods::UniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value) const {
    _UniformMatrix3fv(location, count, transpose, value);
    TRACE("glUniformMatrix3fv");
}

void GLMethods::UniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value) const {
    _UniformMatrix4fv(location, count, transpose, value);
    TRACE("glUniformMatrix4fv");
}

void GLMethods::ValidateProgram(GLuint program) const {
    _ValidateProgram(program);
    TRACE("glValidateProgram");
}

void GLMethods::VertexAttrib1d(GLuint index, GLdouble x) const {
    _VertexAttrib1d(index, x);
    TRACE("glVertexAttrib1d");
}

void GLMethods::VertexAttrib1dv(GLuint index, const GLdouble * v) const {
    _VertexAttrib1dv(index, v);
    TRACE("glVertexAttrib1dv");
}

void GLMethods::VertexAttrib1f(GLuint index, GLfloat x) const {
    _VertexAttrib1f(index, x);
    TRACE("glVertexAttrib1f");
}

void GLMethods::VertexAttrib1fv(GLuint index, const GLfloat * v) const {
    _VertexAttrib1fv(index, v);
    TRACE("glVertexAttrib1fv");
}

void GLMethods::VertexAttrib1s(GLuint index, GLshort x) const {
    _VertexAttrib1s(index, x);
    TRACE("glVertexAttrib1s");
}

void GLMethods::VertexAttrib1sv(GLuint index, const GLshort * v) const {
    _VertexAttrib1sv(index, v);
    TRACE("glVertexAttrib1sv");
}

void GLMethods::VertexAttrib2d(GLuint index, GLdouble x, GLdouble y) const {
    _VertexAttrib2d(index, x, y);
    TRACE("glVertexAttrib2d");
}

void GLMethods::VertexAttrib2dv(GLuint index, const GLdouble * v) const {
    _VertexAttrib2dv(index, v);
    TRACE("glVertexAttrib2dv");
}

void GLMethods::VertexAttrib2f(GLuint index, GLfloat x, GLfloat y) const {
    _VertexAttrib2f(index, x, y);
    TRACE("glVertexAttrib2f");
}

void GLMethods::VertexAttrib2fv(GLuint index, const GLfloat * v) const {
    _VertexAttrib2fv(index, v);
    TRACE("glVertexAttrib2fv");
}

void GLMethods::VertexAttrib2s(GLuint index, GLshort x, GLshort y) const {
    _VertexAttrib2s(index, x, y);
    TRACE("glVertexAttrib2s");
}

void GLMethods::VertexAttrib2sv(GLuint index, const GLshort * v) const {
    _VertexAttrib2sv(index, v);
    TRACE("glVertexAttrib2sv");
}

void GLMethods::VertexAttrib3d(GLuint index, GLdouble x, GLdouble y, GLdouble z) const {
    _VertexAttrib3d(index, x, y, z);
    TRACE("glVertexAttrib3d");
}

void GLMethods::VertexAttrib3dv(GLuint index, const GLdouble * v) const {
    _VertexAttrib3dv(index, v);
    TRACE("glVertexAttrib3dv");
}

void GLMethods::VertexAttrib3f(GLuint index, GLfloat x, GLfloat y, GLfloat z) const {
    _VertexAttrib3f(index, x, y, z);
    TRACE("glVertexAttrib3f");
}

void GLMethods::VertexAttrib3fv(GLuint index, const GLfloat * v) const {
    _VertexAttrib3fv(index, v);
    TRACE("glVertexAttrib3fv");
}

void GLMethods::VertexAttrib3s(GLuint index, GLshort x, GLshort y, GLshort z) const {
    _VertexAttrib3s(index, x, y, z);
    TRACE("glVertexAttrib3s");
}

void GLMethods::VertexAttrib3sv(GLuint index, const GLshort * v) const {
    _VertexAttrib3sv(index, v);
    TRACE("glVertexAttrib3sv");
}

void GLMethods::VertexAttrib4Nbv(GLuint index, const GLbyte * v) const {
    _VertexAttrib4Nbv(index, v);
    TRACE("glVertexAttrib4Nbv");
}

void GLMethods::VertexAttrib4Niv(GLuint index, const GLint * v) const {
    _VertexAttrib4Niv(index, v);
    TRACE("glVertexAttrib4Niv");
}

void GLMethods::VertexAttrib4Nsv(GLuint index, const GLshort * v) const {
    _VertexAttrib4Nsv(index, v);
    TRACE("glVertexAttrib4Nsv");
}

void GLMethods::VertexAttrib4Nub(GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w) const {
    _VertexAttrib4Nub(index, x, y, z, w);
    TRACE("glVertexAttrib4Nub");
}

void GLMethods::VertexAttrib4Nubv(GLuint index, const GLubyte * v) const {
    _VertexAttrib4Nubv(index, v);
    TRACE("glVertexAttrib4Nubv");
}

void GLMethods::VertexAttrib4Nuiv(GLuint index, const GLuint * v) const {
    _VertexAttrib4Nuiv(index, v);
    TRACE("glVertexAttrib4Nuiv");
}

void GLMethods::VertexAttrib4Nusv(GLuint index, const GLushort * v) const {
    _VertexAttrib4Nusv(index, v);
    TRACE("glVertexAttrib4Nusv");
}

void GLMethods::VertexAttrib4bv(GLuint index, const GLbyte * v) const {
    _VertexAttrib4bv(index, v);
    TRACE("glVertexAttrib4bv");
}

void GLMethods::VertexAttrib4d(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w) const {
    _VertexAttrib4d(index, x, y, z, w);
    TRACE("glVertexAttrib4d");
}

void GLMethods::VertexAttrib4dv(GLuint index, const GLdouble * v) const {
    _VertexAttrib4dv(index, v);
    TRACE("glVertexAttrib4dv");
}

void GLMethods::VertexAttrib4f(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w) const {
    _VertexAttrib4f(index, x, y, z, w);
    TRACE("glVertexAttrib4f");
}

void GLMethods::VertexAttrib4fv(GLuint index, const GLfloat * v) const {
    _VertexAttrib4fv(index, v);
    TRACE("glVertexAttrib4fv");
}

void GLMethods::VertexAttrib4iv(GLuint index, const GLint * v) const {
    _VertexAttrib4iv(index, v);
    TRACE("glVertexAttrib4iv");
}

void GLMethods::VertexAttrib4s(GLuint index, GLshort x, GLshort y, GLshort z, GLshort w) const {
    _VertexAttrib4s(index, x, y, z, w);
    TRACE("glVertexAttrib4s");
}

void GLMethods::VertexAttrib4sv(GLuint index, const GLshort * v) const {
    _VertexAttrib4sv(index, v);
    TRACE("glVertexAttrib4sv");
}

void GLMethods::VertexAttrib4ubv(GLuint index, const GLubyte * v) const {
    _VertexAttrib4ubv(index, v);
    TRACE("glVertexAttrib4ubv");
}

void GLMethods::VertexAttrib4uiv(GLuint index, const GLuint * v) const {
    _VertexAttrib4uiv(index, v);
    TRACE("glVertexAttrib4uiv");
}

void GLMethods::VertexAttrib4usv(GLuint index, const GLushort * v) const {
    _VertexAttrib4usv(index, v);
    TRACE("glVertexAttrib4usv");
}

void GLMethods::VertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void * pointer) const {
    _VertexAttribPointer(index, size, type, normalized, stride, pointer);
    TRACE("glVertexAttribPointer");
}

void GLMethods::UniformMatrix2x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value) const {
    _UniformMatrix2x3fv(location, count, transpose, value);
    TRACE("glUniformMatrix2x3fv");
}

void GLMethods::UniformMatrix3x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value) const {
    _UniformMatrix3x2fv(location, count, transpose, value);
    TRACE("glUniformMatrix3x2fv");
}

void GLMethods::UniformMatrix2x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value) const {
    _UniformMatrix2x4fv(location, count, transpose, value);
    TRACE("glUniformMatrix2x4fv");
}

void GLMethods::UniformMatrix4x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value) const {
    _UniformMatrix4x2fv(location, count, transpose, value);
    TRACE("glUniformMatrix4x2fv");
}

void GLMethods::UniformMatrix3x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value) const {
    _UniformMatrix3x4fv(location, count, transpose, value);
    TRACE("glUniformMatrix3x4fv");
}

void GLMethods::UniformMatrix4x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value) const {
    _UniformMatrix4x3fv(location, count, transpose, value);
    TRACE("glUniformMatrix4x3fv");
}

void GLMethods::ColorMaski(GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a) const {
    _ColorMaski(index, r, g, b, a);
    TRACE("glColorMaski");
}

void GLMethods::GetBooleani_v(GLenum target, GLuint index, GLboolean * data) const {
    _GetBooleani_v(target, index, data);
    TRACE("glGetBooleani_v");
}

void GLMethods::GetIntegeri_v(GLenum target, GLuint index, GLint * data) const {
    _GetIntegeri_v(target, index, data);
    TRACE("glGetIntegeri_v");
}

void GLMethods::Enablei(GLenum target, GLuint index) const {
    _Enablei(target, index);
    TRACE("glEnablei");
}

void GLMethods::Disablei(GLenum target, GLuint index) const {
    _Disablei(target, index);
    TRACE("glDisablei");
}

GLboolean GLMethods::IsEnabledi(GLenum target, GLuint index) const {
    GLboolean __result = _IsEnabledi(target, index);
    TRACE("glIsEnabledi");
    return __result;
}

void GLMethods::BeginTransformFeedback(GLenum primitiveMode) const {
    _BeginTransformFeedback(primitiveMode);
    TRACE("glBeginTransformFeedback");
}

void GLMethods::EndTransformFeedback() const {
    _EndTransformFeedback();
    TRACE("glEndTransformFeedback");
}

void GLMethods::BindBufferRange(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size) const {
    _BindBufferRange(target, index, buffer, offset, size);
    TRACE("glBindBufferRange");
}

void GLMethods::BindBufferBase(GLenum target, GLuint index, GLuint buffer) const {
    _BindBufferBase(target, index, buffer);
    TRACE("glBindBufferBase");
}

void GLMethods::TransformFeedbackVaryings(GLuint program, GLsizei count, const GLchar * const * varyings, GLenum bufferMode) const {
    _TransformFeedbackVaryings(program, count, varyings, bufferMode);
    TRACE("glTransformFeedbackVaryings");
}

void GLMethods::GetTransformFeedbackVarying(GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLsizei * size, GLenum * type, GLchar * name) const {
    _GetTransformFeedbackVarying(program, index, bufSize, length, size, type, name);
    TRACE("glGetTransformFeedbackVarying");
}

void GLMethods::ClampColor(GLenum target, GLenum clamp) const {
    _ClampColor(target, clamp);
    TRACE("glClampColor");
}

void GLMethods::BeginConditionalRender(GLuint id, GLenum mode) const {
    _BeginConditionalRender(id, mode);
    TRACE("glBeginConditionalRender");
}

void GLMethods::EndConditionalRender() const {
    _EndConditionalRender();
    TRACE("glEndConditionalRender");
}

void GLMethods::VertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const void * pointer) const {
    _VertexAttribIPointer(index, size, type, stride, pointer);
    TRACE("glVertexAttribIPointer");
}

void GLMethods::GetVertexAttribIiv(GLuint index, GLenum pname, GLint * params) const {
    _GetVertexAttribIiv(index, pname, params);
    TRACE("glGetVertexAttribIiv");
}

void GLMethods::GetVertexAttribIuiv(GLuint index, GLenum pname, GLuint * params) const {
    _GetVertexAttribIuiv(index, pname, params);
    TRACE("glGetVertexAttribIuiv");
}

void GLMethods::VertexAttribI1i(GLuint index, GLint x) const {
    _VertexAttribI1i(index, x);
    TRACE("glVertexAttribI1i");
}

void GLMethods::VertexAttribI2i(GLuint index, GLint x, GLint y) const {
    _VertexAttribI2i(index, x, y);
    TRACE("glVertexAttribI2i");
}

void GLMethods::VertexAttribI3i(GLuint index, GLint x, GLint y, GLint z) const {
    _VertexAttribI3i(index, x, y, z);
    TRACE("glVertexAttribI3i");
}

void GLMethods::VertexAttribI4i(GLuint index, GLint x, GLint y, GLint z, GLint w) const {
    _VertexAttribI4i(index, x, y, z, w);
    TRACE("glVertexAttribI4i");
}

void GLMethods::VertexAttribI1ui(GLuint index, GLuint x) const {
    _VertexAttribI1ui(index, x);
    TRACE("glVertexAttribI1ui");
}

void GLMethods::VertexAttribI2ui(GLuint index, GLuint x, GLuint y) const {
    _VertexAttribI2ui(index, x, y);
    TRACE("glVertexAttribI2ui");
}

void GLMethods::VertexAttribI3ui(GLuint index, GLuint x, GLuint y, GLuint z) const {
    _VertexAttribI3ui(index, x, y, z);
    TRACE("glVertexAttribI3ui");
}

void GLMethods::VertexAttribI4ui(GLuint index, GLuint x, GLuint y, GLuint z, GLuint w) const {
    _VertexAttribI4ui(index, x, y, z, w);
    TRACE("glVertexAttribI4ui");
}

void GLMethods::VertexAttribI1iv(GLuint index, const GLint * v) const {
    _VertexAttribI1iv(index, v);
    TRACE("glVertexAttribI1iv");
}

void GLMethods::VertexAttribI2iv(GLuint index, const GLint * v) const {
    _VertexAttribI2iv(index, v);
    TRACE("glVertexAttribI2iv");
}

void GLMethods::VertexAttribI3iv(GLuint index, const GLint * v) const {
    _VertexAttribI3iv(index, v);
    TRACE("glVertexAttribI3iv");
}

void GLMethods::VertexAttribI4iv(GLuint index, const GLint * v) const {
    _VertexAttribI4iv(index, v);
    TRACE("glVertexAttribI4iv");
}

void GLMethods::VertexAttribI1uiv(GLuint index, const GLuint * v) const {
    _VertexAttribI1uiv(index, v);
    TRACE("glVertexAttribI1uiv");
}

void GLMethods::VertexAttribI2uiv(GLuint index, const GLuint * v) const {
    _VertexAttribI2uiv(index, v);
    TRACE("glVertexAttribI2uiv");
}

void GLMethods::VertexAttribI3uiv(GLuint index, const GLuint * v) const {
    _VertexAttribI3uiv(index, v);
    TRACE("glVertexAttribI3uiv");
}

void GLMethods::VertexAttribI4uiv(GLuint index, const GLuint * v) const {
    _VertexAttribI4uiv(index, v);
    TRACE("glVertexAttribI4uiv");
}

void GLMethods::VertexAttribI4bv(GLuint index, const GLbyte * v) const {
    _VertexAttribI4bv(index, v);
    TRACE("glVertexAttribI4bv");
}

void GLMethods::VertexAttribI4sv(GLuint index, const GLshort * v) const {
    _VertexAttribI4sv(index, v);
    TRACE("glVertexAttribI4sv");
}

void GLMethods::VertexAttribI4ubv(GLuint index, const GLubyte * v) const {
    _VertexAttribI4ubv(index, v);
    TRACE("glVertexAttribI4ubv");
}

void GLMethods::VertexAttribI4usv(GLuint index, const GLushort * v) const {
    _VertexAttribI4usv(index, v);
    TRACE("glVertexAttribI4usv");
}

void GLMethods::GetUniformuiv(GLuint program, GLint location, GLuint * params) const {
    _GetUniformuiv(program, location, params);
    TRACE("glGetUniformuiv");
}

void GLMethods::BindFragDataLocation(GLuint program, GLuint color, const GLchar * name) const {
    _BindFragDataLocation(program, color, name);
    TRACE("glBindFragDataLocation");
}

GLint GLMethods::GetFragDataLocation(GLuint program, const GLchar * name) const {
    GLint __result = _GetFragDataLocation(program, name);
    TRACE("glGetFragDataLocation");
    return __result;
}

void GLMethods::Uniform1ui(GLint location, GLuint v0) const {
    _Uniform1ui(location, v0);
    TRACE("glUniform1ui");
}

void GLMethods::Uniform2ui(GLint location, GLuint v0, GLuint v1) const {
    _Uniform2ui(location, v0, v1);
    TRACE("glUniform2ui");
}

void GLMethods::Uniform3ui(GLint location, GLuint v0, GLuint v1, GLuint v2) const {
    _Uniform3ui(location, v0, v1, v2);
    TRACE("glUniform3ui");
}

void GLMethods::Uniform4ui(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3) const {
    _Uniform4ui(location, v0, v1, v2, v3);
    TRACE("glUniform4ui");
}

void GLMethods::Uniform1uiv(GLint location, GLsizei count, const GLuint * value) const {
    _Uniform1uiv(location, count, value);
    TRACE("glUniform1uiv");
}

void GLMethods::Uniform2uiv(GLint location, GLsizei count, const GLuint * value) const {
    _Uniform2uiv(location, count, value);
    TRACE("glUniform2uiv");
}

void GLMethods::Uniform3uiv(GLint location, GLsizei count, const GLuint * value) const {
    _Uniform3uiv(location, count, value);
    TRACE("glUniform3uiv");
}

void GLMethods::Uniform4uiv(GLint location, GLsizei count, const GLuint * value) const {
    _Uniform4uiv(location, count, value);
    TRACE("glUniform4uiv");
}

void GLMethods::TexParameterIiv(GLenum target, GLenum pname, const GLint * params) const {
    _TexParameterIiv(target, pname, params);
    TRACE("glTexParameterIiv");
}

void GLMethods::TexParameterIuiv(GLenum target, GLenum pname, const GLuint * params) const {
    _TexParameterIuiv(target, pname, params);
    TRACE("glTexParameterIuiv");
}

void GLMethods::GetTexParameterIiv(GLenum target, GLenum pname, GLint * params) const {
    _GetTexParameterIiv(target, pname, params);
    TRACE("glGetTexParameterIiv");
}

void GLMethods::GetTexParameterIuiv(GLenum target, GLenum pname, GLuint * params) const {
    _GetTexParameterIuiv(target, pname, params);
    TRACE("glGetTexParameterIuiv");
}

void GLMethods::ClearBufferiv(GLenum buffer, GLint drawbuffer, const GLint * value) const {
    _ClearBufferiv(buffer, drawbuffer, value);
    TRACE("glClearBufferiv");
}

void GLMethods::ClearBufferuiv(GLenum buffer, GLint drawbuffer, const GLuint * value) const {
    _ClearBufferuiv(buffer, drawbuffer, value);
    TRACE("glClearBufferuiv");
}

void GLMethods::ClearBufferfv(GLenum buffer, GLint drawbuffer, const GLfloat * value) const {
    _ClearBufferfv(buffer, drawbuffer, value);
    TRACE("glClearBufferfv");
}

void GLMethods::ClearBufferfi(GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil) const {
    _ClearBufferfi(buffer, drawbuffer, depth, stencil);
    TRACE("glClearBufferfi");
}

const GLubyte * GLMethods::GetStringi(GLenum name, GLuint index) const {
    const GLubyte * __result = _GetStringi(name, index);
    TRACE("glGetStringi");
    return __result;
}

GLboolean GLMethods::IsRenderbuffer(GLuint renderbuffer) const {
    GLboolean __result = _IsRenderbuffer(renderbuffer);
    TRACE("glIsRenderbuffer");
    return __result;
}

void GLMethods::BindRenderbuffer(GLenum target, GLuint renderbuffer) const {
    _BindRenderbuffer(target, renderbuffer);
    TRACE("glBindRenderbuffer");
}

void GLMethods::DeleteRenderbuffers(GLsizei n, const GLuint * renderbuffers) const {
    _DeleteRenderbuffers(n, renderbuffers);
    TRACE("glDeleteRenderbuffers");
}

void GLMethods::GenRenderbuffers(GLsizei n, GLuint * renderbuffers) const {
    _GenRenderbuffers(n, renderbuffers);
    TRACE("glGenRenderbuffers");
}

void GLMethods::RenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height) const {
    _RenderbufferStorage(target, internalformat, width, height);
    TRACE("glRenderbufferStorage");
}

void GLMethods::GetRenderbufferParameteriv(GLenum target, GLenum pname, GLint * params) const {
    _GetRenderbufferParameteriv(target, pname, params);
    TRACE("glGetRenderbufferParameteriv");
}

GLboolean GLMethods::IsFramebuffer(GLuint framebuffer) const {
    GLboolean __result = _IsFramebuffer(framebuffer);
    TRACE("glIsFramebuffer");
    return __result;
}

void GLMethods::BindFramebuffer(GLenum target, GLuint framebuffer) const {
    _BindFramebuffer(target, framebuffer);
    TRACE("glBindFramebuffer");
}

void GLMethods::DeleteFramebuffers(GLsizei n, const GLuint * framebuffers) const {
    _DeleteFramebuffers(n, framebuffers);
    TRACE("glDeleteFramebuffers");
}

void GLMethods::GenFramebuffers(GLsizei n, GLuint * framebuffers) const {
    _GenFramebuffers(n, framebuffers);
    TRACE("glGenFramebuffers");
}

GLenum GLMethods::CheckFramebufferStatus(GLenum target) const {
    GLenum __result = _CheckFramebufferStatus(target);
    TRACE("glCheckFramebufferStatus");
    return __result;
}

void GLMethods::FramebufferTexture1D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level) const {
    _FramebufferTexture1D(target, attachment, textarget, texture, level);
    TRACE("glFramebufferTexture1D");
}

void GLMethods::FramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level) const {
    _FramebufferTexture2D(target, attachment, textarget, texture, level);
    TRACE("glFramebufferTexture2D");
}

void GLMethods::FramebufferTexture3D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset) const {
    _FramebufferTexture3D(target, attachment, textarget, texture, level, zoffset);
    TRACE("glFramebufferTexture3D");
}

void GLMethods::FramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer) const {
    _FramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer);
    TRACE("glFramebufferRenderbuffer");
}

void GLMethods::GetFramebufferAttachmentParameteriv(GLenum target, GLenum attachment, GLenum pname, GLint * params) const {
    _GetFramebufferAttachmentParameteriv(target, attachment, pname, params);
    TRACE("glGetFramebufferAttachmentParameteriv");
}

void GLMethods::GenerateMipmap(GLenum target) const {
    _GenerateMipmap(target);
    TRACE("glGenerateMipmap");
}

void GLMethods::BlitFramebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter) const {
    _BlitFramebuffer(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);
    TRACE("glBlitFramebuffer");
}

void GLMethods::RenderbufferStorageMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height) const {
    _RenderbufferStorageMultisample(target, samples, internalformat, width, height);
    TRACE("glRenderbufferStorageMultisample");
}

void GLMethods::FramebufferTextureLayer(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer) const {
    _FramebufferTextureLayer(target, attachment, texture, level, layer);
    TRACE("glFramebufferTextureLayer");
}

void * GLMethods::MapBufferRange(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access) const {
    void * __result = _MapBufferRange(target, offset, length, access);
    TRACE("glMapBufferRange");
    return __result;
}

void GLMethods::FlushMappedBufferRange(GLenum target, GLintptr offset, GLsizeiptr length) const {
    _FlushMappedBufferRange(target, offset, length);
    TRACE("glFlushMappedBufferRange");
}

void GLMethods::BindVertexArray(GLuint array) const {
    _BindVertexArray(array);
    TRACE("glBindVertexArray");
}

void GLMethods::DeleteVertexArrays(GLsizei n, const GLuint * arrays) const {
    _DeleteVertexArrays(n, arrays);
    TRACE("glDeleteVertexArrays");
}

void GLMethods::GenVertexArrays(GLsizei n, GLuint * arrays) const {
    _GenVertexArrays(n, arrays);
    TRACE("glGenVertexArrays");
}

GLboolean GLMethods::IsVertexArray(GLuint array) const {
    GLboolean __result = _IsVertexArray(array);
    TRACE("glIsVertexArray");
    return __result;
}

void GLMethods::DrawArraysInstanced(GLenum mode, GLint first, GLsizei count, GLsizei instancecount) const {
    _DrawArraysInstanced(mode, first, count, instancecount);
    TRACE("glDrawArraysInstanced");
}

void GLMethods::DrawElementsInstanced(GLenum mode, GLsizei count, GLenum type, const void * indices, GLsizei instancecount) const {
    _DrawElementsInstanced(mode, count, type, indices, instancecount);
    TRACE("glDrawElementsInstanced");
}

void GLMethods::TexBuffer(GLenum target, GLenum internalformat, GLuint buffer) const {
    _TexBuffer(target, internalformat, buffer);
    TRACE("glTexBuffer");
}

void GLMethods::PrimitiveRestartIndex(GLuint index) const {
    _PrimitiveRestartIndex(index);
    TRACE("glPrimitiveRestartIndex");
}

void GLMethods::CopyBufferSubData(GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size) const {
    _CopyBufferSubData(readTarget, writeTarget, readOffset, writeOffset, size);
    TRACE("glCopyBufferSubData");
}

void GLMethods::GetUniformIndices(GLuint program, GLsizei uniformCount, const GLchar * const * uniformNames, GLuint * uniformIndices) const {
    _GetUniformIndices(program, uniformCount, uniformNames, uniformIndices);
    TRACE("glGetUniformIndices");
}

void GLMethods::GetActiveUniformsiv(GLuint program, GLsizei uniformCount, const GLuint * uniformIndices, GLenum pname, GLint * params) const {
    _GetActiveUniformsiv(program, uniformCount, uniformIndices, pname, params);
    TRACE("glGetActiveUniformsiv");
}

void GLMethods::GetActiveUniformName(GLuint program, GLuint uniformIndex, GLsizei bufSize, GLsizei * length, GLchar * uniformName) const {
    _GetActiveUniformName(program, uniformIndex, bufSize, length, uniformName);
    TRACE("glGetActiveUniformName");
}

GLuint GLMethods::GetUniformBlockIndex(GLuint program, const GLchar * uniformBlockName) const {
    GLuint __result = _GetUniformBlockIndex(program, uniformBlockName);
    TRACE("glGetUniformBlockIndex");
    return __result;
}

void GLMethods::GetActiveUniformBlockiv(GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint * params) const {
    _GetActiveUniformBlockiv(program, uniformBlockIndex, pname, params);
    TRACE("glGetActiveUniformBlockiv");
}

void GLMethods::GetActiveUniformBlockName(GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei * length, GLchar * uniformBlockName) const {
    _GetActiveUniformBlockName(program, uniformBlockIndex, bufSize, length, uniformBlockName);
    TRACE("glGetActiveUniformBlockName");
}

void GLMethods::UniformBlockBinding(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding) const {
    _UniformBlockBinding(program, uniformBlockIndex, uniformBlockBinding);
    TRACE("glUniformBlockBinding");
}

void GLMethods::DrawElementsBaseVertex(GLenum mode, GLsizei count, GLenum type, const void * indices, GLint basevertex) const {
    _DrawElementsBaseVertex(mode, count, type, indices, basevertex);
    TRACE("glDrawElementsBaseVertex");
}

void GLMethods::DrawRangeElementsBaseVertex(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void * indices, GLint basevertex) const {
    _DrawRangeElementsBaseVertex(mode, start, end, count, type, indices, basevertex);
    TRACE("glDrawRangeElementsBaseVertex");
}

void GLMethods::DrawElementsInstancedBaseVertex(GLenum mode, GLsizei count, GLenum type, const void * indices, GLsizei instancecount, GLint basevertex) const {
    _DrawElementsInstancedBaseVertex(mode, count, type, indices, instancecount, basevertex);
    TRACE("glDrawElementsInstancedBaseVertex");
}

void GLMethods::MultiDrawElementsBaseVertex(GLenum mode, const GLsizei * count, GLenum type, const void * const * indices, GLsizei drawcount, const GLint * basevertex) const {
    _MultiDrawElementsBaseVertex(mode, count, type, indices, drawcount, basevertex);
    TRACE("glMultiDrawElementsBaseVertex");
}

void GLMethods::ProvokingVertex(GLenum mode) const {
    _ProvokingVertex(mode);
    TRACE("glProvokingVertex");
}

GLsync GLMethods::FenceSync(GLenum condition, GLbitfield flags) const {
    GLsync __result = _FenceSync(condition, flags);
    TRACE("glFenceSync");
    return __result;
}

GLboolean GLMethods::IsSync(GLsync sync) const {
    GLboolean __result = _IsSync(sync);
    TRACE("glIsSync");
    return __result;
}

void GLMethods::DeleteSync(GLsync sync) const {
    _DeleteSync(sync);
    TRACE("glDeleteSync");
}

GLenum GLMethods::ClientWaitSync(GLsync sync, GLbitfield flags, GLuint64 timeout) const {
    GLenum __result = _ClientWaitSync(sync, flags, timeout);
    TRACE("glClientWaitSync");
    return __result;
}

void GLMethods::WaitSync(GLsync sync, GLbitfield flags, GLuint64 timeout) const {
    _WaitSync(sync, flags, timeout);
    TRACE("glWaitSync");
}

void GLMethods::GetInteger64v(GLenum pname, GLint64 * data) const {
    _GetInteger64v(pname, data);
    TRACE("glGetInteger64v");
}

void GLMethods::GetSynciv(GLsync sync, GLenum pname, GLsizei bufSize, GLsizei * length, GLint * values) const {
    _GetSynciv(sync, pname, bufSize, length, values);
    TRACE("glGetSynciv");
}

void GLMethods::GetInteger64i_v(GLenum target, GLuint index, GLint64 * data) const {
    _GetInteger64i_v(target, index, data);
    TRACE("glGetInteger64i_v");
}

void GLMethods::GetBufferParameteri64v(GLenum target, GLenum pname, GLint64 * params) const {
    _GetBufferParameteri64v(target, pname, params);
    TRACE("glGetBufferParameteri64v");
}

void GLMethods::FramebufferTexture(GLenum target, GLenum attachment, GLuint texture, GLint level) const {
    _FramebufferTexture(target, attachment, texture, level);
    TRACE("glFramebufferTexture");
}

void GLMethods::TexImage2DMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations) const {
    _TexImage2DMultisample(target, samples, internalformat, width, height, fixedsamplelocations);
    TRACE("glTexImage2DMultisample");
}

void GLMethods::TexImage3DMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations) const {
    _TexImage3DMultisample(target, samples, internalformat, width, height, depth, fixedsamplelocations);
    TRACE("glTexImage3DMultisample");
}

void GLMethods::GetMultisamplefv(GLenum pname, GLuint index, GLfloat * val) const {
    _GetMultisamplefv(pname, index, val);
    TRACE("glGetMultisamplefv");
}

void GLMethods::SampleMaski(GLuint maskNumber, GLbitfield mask) const {
    _SampleMaski(maskNumber, mask);
    TRACE("glSampleMaski");
}

void GLMethods::BindFragDataLocationIndexed(GLuint program, GLuint colorNumber, GLuint index, const GLchar * name) const {
    _BindFragDataLocationIndexed(program, colorNumber, index, name);
    TRACE("glBindFragDataLocationIndexed");
}

GLint GLMethods::GetFragDataIndex(GLuint program, const GLchar * name) const {
    GLint __result = _GetFragDataIndex(program, name);
    TRACE("glGetFragDataIndex");
    return __result;
}

void GLMethods::GenSamplers(GLsizei count, GLuint * samplers) const {
    _GenSamplers(count, samplers);
    TRACE("glGenSamplers");
}

void GLMethods::DeleteSamplers(GLsizei count, const GLuint * samplers) const {
    _DeleteSamplers(count, samplers);
    TRACE("glDeleteSamplers");
}

GLboolean GLMethods::IsSampler(GLuint sampler) const {
    GLboolean __result = _IsSampler(sampler);
    TRACE("glIsSampler");
    return __result;
}

void GLMethods::BindSampler(GLuint unit, GLuint sampler) const {
    _BindSampler(unit, sampler);
    TRACE("glBindSampler");
}

void GLMethods::SamplerParameteri(GLuint sampler, GLenum pname, GLint param) const {
    _SamplerParameteri(sampler, pname, param);
    TRACE("glSamplerParameteri");
}

void GLMethods::SamplerParameteriv(GLuint sampler, GLenum pname, const GLint * param) const {
    _SamplerParameteriv(sampler, pname, param);
    TRACE("glSamplerParameteriv");
}

void GLMethods::SamplerParameterf(GLuint sampler, GLenum pname, GLfloat param) const {
    _SamplerParameterf(sampler, pname, param);
    TRACE("glSamplerParameterf");
}

void GLMethods::SamplerParameterfv(GLuint sampler, GLenum pname, const GLfloat * param) const {
    _SamplerParameterfv(sampler, pname, param);
    TRACE("glSamplerParameterfv");
}

void GLMethods::SamplerParameterIiv(GLuint sampler, GLenum pname, const GLint * param) const {
    _SamplerParameterIiv(sampler, pname, param);
    TRACE("glSamplerParameterIiv");
}

void GLMethods::SamplerParameterIuiv(GLuint sampler, GLenum pname, const GLuint * param) const {
    _SamplerParameterIuiv(sampler, pname, param);
    TRACE("glSamplerParameterIuiv");
}

void GLMethods::GetSamplerParameteriv(GLuint sampler, GLenum pname, GLint * params) const {
    _GetSamplerParameteriv(sampler, pname, params);
    TRACE("glGetSamplerParameteriv");
}

void GLMethods::GetSamplerParameterIiv(GLuint sampler, GLenum pname, GLint * params) const {
    _GetSamplerParameterIiv(sampler, pname, params);
    TRACE("glGetSamplerParameterIiv");
}

void GLMethods::GetSamplerParameterfv(GLuint sampler, GLenum pname, GLfloat * params) const {
    _GetSamplerParameterfv(sampler, pname, params);
    TRACE("glGetSamplerParameterfv");
}

void GLMethods::GetSamplerParameterIuiv(GLuint sampler, GLenum pname, GLuint * params) const {
    _GetSamplerParameterIuiv(sampler, pname, params);
    TRACE("glGetSamplerParameterIuiv");
}

void GLMethods::QueryCounter(GLuint id, GLenum target) const {
    _QueryCounter(id, target);
    TRACE("glQueryCounter");
}

void GLMethods::GetQueryObjecti64v(GLuint id, GLenum pname, GLint64 * params) const {
    _GetQueryObjecti64v(id, pname, params);
    TRACE("glGetQueryObjecti64v");
}

void GLMethods::GetQueryObjectui64v(GLuint id, GLenum pname, GLuint64 * params) const {
    _GetQueryObjectui64v(id, pname, params);
    TRACE("glGetQueryObjectui64v");
}

void GLMethods::VertexAttribDivisor(GLuint index, GLuint divisor) const {
    _VertexAttribDivisor(index, divisor);
    TRACE("glVertexAttribDivisor");
}

void GLMethods::VertexAttribP1ui(GLuint index, GLenum type, GLboolean normalized, GLuint value) const {
    _VertexAttribP1ui(index, type, normalized, value);
    TRACE("glVertexAttribP1ui");
}

void GLMethods::VertexAttribP1uiv(GLuint index, GLenum type, GLboolean normalized, const GLuint * value) const {
    _VertexAttribP1uiv(index, type, normalized, value);
    TRACE("glVertexAttribP1uiv");
}

void GLMethods::VertexAttribP2ui(GLuint index, GLenum type, GLboolean normalized, GLuint value) const {
    _VertexAttribP2ui(index, type, normalized, value);
    TRACE("glVertexAttribP2ui");
}

void GLMethods::VertexAttribP2uiv(GLuint index, GLenum type, GLboolean normalized, const GLuint * value) const {
    _VertexAttribP2uiv(index, type, normalized, value);
    TRACE("glVertexAttribP2uiv");
}

void GLMethods::VertexAttribP3ui(GLuint index, GLenum type, GLboolean normalized, GLuint value) const {
    _VertexAttribP3ui(index, type, normalized, value);
    TRACE("glVertexAttribP3ui");
}

void GLMethods::VertexAttribP3uiv(GLuint index, GLenum type, GLboolean normalized, const GLuint * value) const {
    _VertexAttribP3uiv(index, type, normalized, value);
    TRACE("glVertexAttribP3uiv");
}

void GLMethods::VertexAttribP4ui(GLuint index, GLenum type, GLboolean normalized, GLuint value) const {
    _VertexAttribP4ui(index, type, normalized, value);
    TRACE("glVertexAttribP4ui");
}

void GLMethods::VertexAttribP4uiv(GLuint index, GLenum type, GLboolean normalized, const GLuint * value) const {
    _VertexAttribP4uiv(index, type, normalized, value);
    TRACE("glVertexAttribP4uiv");
}

void GLMethods::MinSampleShading(GLfloat value) const {
    _MinSampleShading(value);
    TRACE("glMinSampleShading");
}

void GLMethods::BlendEquationi(GLuint buf, GLenum mode) const {
    _BlendEquationi(buf, mode);
    TRACE("glBlendEquationi");
}

void GLMethods::BlendEquationSeparatei(GLuint buf, GLenum modeRGB, GLenum modeAlpha) const {
    _BlendEquationSeparatei(buf, modeRGB, modeAlpha);
    TRACE("glBlendEquationSeparatei");
}

void GLMethods::BlendFunci(GLuint buf, GLenum src, GLenum dst) const {
    _BlendFunci(buf, src, dst);
    TRACE("glBlendFunci");
}

void GLMethods::BlendFuncSeparatei(GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha) const {
    _BlendFuncSeparatei(buf, srcRGB, dstRGB, srcAlpha, dstAlpha);
    TRACE("glBlendFuncSeparatei");
}

void GLMethods::DrawArraysIndirect(GLenum mode, const void * indirect) const {
    _DrawArraysIndirect(mode, indirect);
    TRACE("glDrawArraysIndirect");
}

void GLMethods::DrawElementsIndirect(GLenum mode, GLenum type, const void * indirect) const {
    _DrawElementsIndirect(mode, type, indirect);
    TRACE("glDrawElementsIndirect");
}

void GLMethods::Uniform1d(GLint location, GLdouble x) const {
    _Uniform1d(location, x);
    TRACE("glUniform1d");
}

void GLMethods::Uniform2d(GLint location, GLdouble x, GLdouble y) const {
    _Uniform2d(location, x, y);
    TRACE("glUniform2d");
}

void GLMethods::Uniform3d(GLint location, GLdouble x, GLdouble y, GLdouble z) const {
    _Uniform3d(location, x, y, z);
    TRACE("glUniform3d");
}

void GLMethods::Uniform4d(GLint location, GLdouble x, GLdouble y, GLdouble z, GLdouble w) const {
    _Uniform4d(location, x, y, z, w);
    TRACE("glUniform4d");
}

void GLMethods::Uniform1dv(GLint location, GLsizei count, const GLdouble * value) const {
    _Uniform1dv(location, count, value);
    TRACE("glUniform1dv");
}

void GLMethods::Uniform2dv(GLint location, GLsizei count, const GLdouble * value) const {
    _Uniform2dv(location, count, value);
    TRACE("glUniform2dv");
}

void GLMethods::Uniform3dv(GLint location, GLsizei count, const GLdouble * value) const {
    _Uniform3dv(location, count, value);
    TRACE("glUniform3dv");
}

void GLMethods::Uniform4dv(GLint location, GLsizei count, const GLdouble * value) const {
    _Uniform4dv(location, count, value);
    TRACE("glUniform4dv");
}

void GLMethods::UniformMatrix2dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value) const {
    _UniformMatrix2dv(location, count, transpose, value);
    TRACE("glUniformMatrix2dv");
}

void GLMethods::UniformMatrix3dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value) const {
    _UniformMatrix3dv(location, count, transpose, value);
    TRACE("glUniformMatrix3dv");
}

void GLMethods::UniformMatrix4dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value) const {
    _UniformMatrix4dv(location, count, transpose, value);
    TRACE("glUniformMatrix4dv");
}

void GLMethods::UniformMatrix2x3dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value) const {
    _UniformMatrix2x3dv(location, count, transpose, value);
    TRACE("glUniformMatrix2x3dv");
}

void GLMethods::UniformMatrix2x4dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value) const {
    _UniformMatrix2x4dv(location, count, transpose, value);
    TRACE("glUniformMatrix2x4dv");
}

void GLMethods::UniformMatrix3x2dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value) const {
    _UniformMatrix3x2dv(location, count, transpose, value);
    TRACE("glUniformMatrix3x2dv");
}

void GLMethods::UniformMatrix3x4dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value) const {
    _UniformMatrix3x4dv(location, count, transpose, value);
    TRACE("glUniformMatrix3x4dv");
}

void GLMethods::UniformMatrix4x2dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value) const {
    _UniformMatrix4x2dv(location, count, transpose, value);
    TRACE("glUniformMatrix4x2dv");
}

void GLMethods::UniformMatrix4x3dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value) const {
    _UniformMatrix4x3dv(location, count, transpose, value);
    TRACE("glUniformMatrix4x3dv");
}

void GLMethods::GetUniformdv(GLuint program, GLint location, GLdouble * params) const {
    _GetUniformdv(program, location, params);
    TRACE("glGetUniformdv");
}

GLint GLMethods::GetSubroutineUniformLocation(GLuint program, GLenum shadertype, const GLchar * name) const {
    GLint __result = _GetSubroutineUniformLocation(program, shadertype, name);
    TRACE("glGetSubroutineUniformLocation");
    return __result;
}

GLuint GLMethods::GetSubroutineIndex(GLuint program, GLenum shadertype, const GLchar * name) const {
    GLuint __result = _GetSubroutineIndex(program, shadertype, name);
    TRACE("glGetSubroutineIndex");
    return __result;
}

void GLMethods::GetActiveSubroutineUniformiv(GLuint program, GLenum shadertype, GLuint index, GLenum pname, GLint * values) const {
    _GetActiveSubroutineUniformiv(program, shadertype, index, pname, values);
    TRACE("glGetActiveSubroutineUniformiv");
}

void GLMethods::GetActiveSubroutineUniformName(GLuint program, GLenum shadertype, GLuint index, GLsizei bufsize, GLsizei * length, GLchar * name) const {
    _GetActiveSubroutineUniformName(program, shadertype, index, bufsize, length, name);
    TRACE("glGetActiveSubroutineUniformName");
}

void GLMethods::GetActiveSubroutineName(GLuint program, GLenum shadertype, GLuint index, GLsizei bufsize, GLsizei * length, GLchar * name) const {
    _GetActiveSubroutineName(program, shadertype, index, bufsize, length, name);
    TRACE("glGetActiveSubroutineName");
}

void GLMethods::UniformSubroutinesuiv(GLenum shadertype, GLsizei count, const GLuint * indices) const {
    _UniformSubroutinesuiv(shadertype, count, indices);
    TRACE("glUniformSubroutinesuiv");
}

void GLMethods::GetUniformSubroutineuiv(GLenum shadertype, GLint location, GLuint * params) const {
    _GetUniformSubroutineuiv(shadertype, location, params);
    TRACE("glGetUniformSubroutineuiv");
}

void GLMethods::GetProgramStageiv(GLuint program, GLenum shadertype, GLenum pname, GLint * values) const {
    _GetProgramStageiv(program, shadertype, pname, values);
    TRACE("glGetProgramStageiv");
}

void GLMethods::PatchParameteri(GLenum pname, GLint value) const {
    _PatchParameteri(pname, value);
    TRACE("glPatchParameteri");
}

void GLMethods::PatchParameterfv(GLenum pname, const GLfloat * values) const {
    _PatchParameterfv(pname, values);
    TRACE("glPatchParameterfv");
}

void GLMethods::BindTransformFeedback(GLenum target, GLuint id) const {
    _BindTransformFeedback(target, id);
    TRACE("glBindTransformFeedback");
}

void GLMethods::DeleteTransformFeedbacks(GLsizei n, const GLuint * ids) const {
    _DeleteTransformFeedbacks(n, ids);
    TRACE("glDeleteTransformFeedbacks");
}

void GLMethods::GenTransformFeedbacks(GLsizei n, GLuint * ids) const {
    _GenTransformFeedbacks(n, ids);
    TRACE("glGenTransformFeedbacks");
}

GLboolean GLMethods::IsTransformFeedback(GLuint id) const {
    GLboolean __result = _IsTransformFeedback(id);
    TRACE("glIsTransformFeedback");
    return __result;
}

void GLMethods::PauseTransformFeedback() const {
    _PauseTransformFeedback();
    TRACE("glPauseTransformFeedback");
}

void GLMethods::ResumeTransformFeedback() const {
    _ResumeTransformFeedback();
    TRACE("glResumeTransformFeedback");
}

void GLMethods::DrawTransformFeedback(GLenum mode, GLuint id) const {
    _DrawTransformFeedback(mode, id);
    TRACE("glDrawTransformFeedback");
}

void GLMethods::DrawTransformFeedbackStream(GLenum mode, GLuint id, GLuint stream) const {
    _DrawTransformFeedbackStream(mode, id, stream);
    TRACE("glDrawTransformFeedbackStream");
}

void GLMethods::BeginQueryIndexed(GLenum target, GLuint index, GLuint id) const {
    _BeginQueryIndexed(target, index, id);
    TRACE("glBeginQueryIndexed");
}

void GLMethods::EndQueryIndexed(GLenum target, GLuint index) const {
    _EndQueryIndexed(target, index);
    TRACE("glEndQueryIndexed");
}

void GLMethods::GetQueryIndexediv(GLenum target, GLuint index, GLenum pname, GLint * params) const {
    _GetQueryIndexediv(target, index, pname, params);
    TRACE("glGetQueryIndexediv");
}

void GLMethods::ReleaseShaderCompiler() const {
    _ReleaseShaderCompiler();
    TRACE("glReleaseShaderCompiler");
}

void GLMethods::ShaderBinary(GLsizei count, const GLuint * shaders, GLenum binaryformat, const void * binary, GLsizei length) const {
    _ShaderBinary(count, shaders, binaryformat, binary, length);
    TRACE("glShaderBinary");
}

void GLMethods::GetShaderPrecisionFormat(GLenum shadertype, GLenum precisiontype, GLint * range, GLint * precision) const {
    _GetShaderPrecisionFormat(shadertype, precisiontype, range, precision);
    TRACE("glGetShaderPrecisionFormat");
}

void GLMethods::DepthRangef(GLfloat n, GLfloat f) const {
    _DepthRangef(n, f);
    TRACE("glDepthRangef");
}

void GLMethods::ClearDepthf(GLfloat d) const {
    _ClearDepthf(d);
    TRACE("glClearDepthf");
}

void GLMethods::GetProgramBinary(GLuint program, GLsizei bufSize, GLsizei * length, GLenum * binaryFormat, void * binary) const {
    _GetProgramBinary(program, bufSize, length, binaryFormat, binary);
    TRACE("glGetProgramBinary");
}

void GLMethods::ProgramBinary(GLuint program, GLenum binaryFormat, const void * binary, GLsizei length) const {
    _ProgramBinary(program, binaryFormat, binary, length);
    TRACE("glProgramBinary");
}

void GLMethods::ProgramParameteri(GLuint program, GLenum pname, GLint value) const {
    _ProgramParameteri(program, pname, value);
    TRACE("glProgramParameteri");
}

void GLMethods::UseProgramStages(GLuint pipeline, GLbitfield stages, GLuint program) const {
    _UseProgramStages(pipeline, stages, program);
    TRACE("glUseProgramStages");
}

void GLMethods::ActiveShaderProgram(GLuint pipeline, GLuint program) const {
    _ActiveShaderProgram(pipeline, program);
    TRACE("glActiveShaderProgram");
}

GLuint GLMethods::CreateShaderProgramv(GLenum type, GLsizei count, const GLchar * const * strings) const {
    GLuint __result = _CreateShaderProgramv(type, count, strings);
    TRACE("glCreateShaderProgramv");
    return __result;
}

void GLMethods::BindProgramPipeline(GLuint pipeline) const {
    _BindProgramPipeline(pipeline);
    TRACE("glBindProgramPipeline");
}

void GLMethods::DeleteProgramPipelines(GLsizei n, const GLuint * pipelines) const {
    _DeleteProgramPipelines(n, pipelines);
    TRACE("glDeleteProgramPipelines");
}

void GLMethods::GenProgramPipelines(GLsizei n, GLuint * pipelines) const {
    _GenProgramPipelines(n, pipelines);
    TRACE("glGenProgramPipelines");
}

GLboolean GLMethods::IsProgramPipeline(GLuint pipeline) const {
    GLboolean __result = _IsProgramPipeline(pipeline);
    TRACE("glIsProgramPipeline");
    return __result;
}

void GLMethods::GetProgramPipelineiv(GLuint pipeline, GLenum pname, GLint * params) const {
    _GetProgramPipelineiv(pipeline, pname, params);
    TRACE("glGetProgramPipelineiv");
}

void GLMethods::ProgramUniform1i(GLuint program, GLint location, GLint v0) const {
    _ProgramUniform1i(program, location, v0);
    TRACE("glProgramUniform1i");
}

void GLMethods::ProgramUniform1iv(GLuint program, GLint location, GLsizei count, const GLint * value) const {
    _ProgramUniform1iv(program, location, count, value);
    TRACE("glProgramUniform1iv");
}

void GLMethods::ProgramUniform1f(GLuint program, GLint location, GLfloat v0) const {
    _ProgramUniform1f(program, location, v0);
    TRACE("glProgramUniform1f");
}

void GLMethods::ProgramUniform1fv(GLuint program, GLint location, GLsizei count, const GLfloat * value) const {
    _ProgramUniform1fv(program, location, count, value);
    TRACE("glProgramUniform1fv");
}

void GLMethods::ProgramUniform1d(GLuint program, GLint location, GLdouble v0) const {
    _ProgramUniform1d(program, location, v0);
    TRACE("glProgramUniform1d");
}

void GLMethods::ProgramUniform1dv(GLuint program, GLint location, GLsizei count, const GLdouble * value) const {
    _ProgramUniform1dv(program, location, count, value);
    TRACE("glProgramUniform1dv");
}

void GLMethods::ProgramUniform1ui(GLuint program, GLint location, GLuint v0) const {
    _ProgramUniform1ui(program, location, v0);
    TRACE("glProgramUniform1ui");
}

void GLMethods::ProgramUniform1uiv(GLuint program, GLint location, GLsizei count, const GLuint * value) const {
    _ProgramUniform1uiv(program, location, count, value);
    TRACE("glProgramUniform1uiv");
}

void GLMethods::ProgramUniform2i(GLuint program, GLint location, GLint v0, GLint v1) const {
    _ProgramUniform2i(program, location, v0, v1);
    TRACE("glProgramUniform2i");
}

void GLMethods::ProgramUniform2iv(GLuint program, GLint location, GLsizei count, const GLint * value) const {
    _ProgramUniform2iv(program, location, count, value);
    TRACE("glProgramUniform2iv");
}

void GLMethods::ProgramUniform2f(GLuint program, GLint location, GLfloat v0, GLfloat v1) const {
    _ProgramUniform2f(program, location, v0, v1);
    TRACE("glProgramUniform2f");
}

void GLMethods::ProgramUniform2fv(GLuint program, GLint location, GLsizei count, const GLfloat * value) const {
    _ProgramUniform2fv(program, location, count, value);
    TRACE("glProgramUniform2fv");
}

void GLMethods::ProgramUniform2d(GLuint program, GLint location, GLdouble v0, GLdouble v1) const {
    _ProgramUniform2d(program, location, v0, v1);
    TRACE("glProgramUniform2d");
}

void GLMethods::ProgramUniform2dv(GLuint program, GLint location, GLsizei count, const GLdouble * value) const {
    _ProgramUniform2dv(program, location, count, value);
    TRACE("glProgramUniform2dv");
}

void GLMethods::ProgramUniform2ui(GLuint program, GLint location, GLuint v0, GLuint v1) const {
    _ProgramUniform2ui(program, location, v0, v1);
    TRACE("glProgramUniform2ui");
}

void GLMethods::ProgramUniform2uiv(GLuint program, GLint location, GLsizei count, const GLuint * value) const {
    _ProgramUniform2uiv(program, location, count, value);
    TRACE("glProgramUniform2uiv");
}

void GLMethods::ProgramUniform3i(GLuint program, GLint location, GLint v0, GLint v1, GLint v2) const {
    _ProgramUniform3i(program, location, v0, v1, v2);
    TRACE("glProgramUniform3i");
}

void GLMethods::ProgramUniform3iv(GLuint program, GLint location, GLsizei count, const GLint * value) const {
    _ProgramUniform3iv(program, location, count, value);
    TRACE("glProgramUniform3iv");
}

void GLMethods::ProgramUniform3f(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2) const {
    _ProgramUniform3f(program, location, v0, v1, v2);
    TRACE("glProgramUniform3f");
}

void GLMethods::ProgramUniform3fv(GLuint program, GLint location, GLsizei count, const GLfloat * value) const {
    _ProgramUniform3fv(program, location, count, value);
    TRACE("glProgramUniform3fv");
}

void GLMethods::ProgramUniform3d(GLuint program, GLint location, GLdouble v0, GLdouble v1, GLdouble v2) const {
    _ProgramUniform3d(program, location, v0, v1, v2);
    TRACE("glProgramUniform3d");
}

void GLMethods::ProgramUniform3dv(GLuint program, GLint location, GLsizei count, const GLdouble * value) const {
    _ProgramUniform3dv(program, location, count, value);
    TRACE("glProgramUniform3dv");
}

void GLMethods::ProgramUniform3ui(GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2) const {
    _ProgramUniform3ui(program, location, v0, v1, v2);
    TRACE("glProgramUniform3ui");
}

void GLMethods::ProgramUniform3uiv(GLuint program, GLint location, GLsizei count, const GLuint * value) const {
    _ProgramUniform3uiv(program, location, count, value);
    TRACE("glProgramUniform3uiv");
}

void GLMethods::ProgramUniform4i(GLuint program, GLint location, GLint v0, GLint v1, GLint v2, GLint v3) const {
    _ProgramUniform4i(program, location, v0, v1, v2, v3);
    TRACE("glProgramUniform4i");
}

void GLMethods::ProgramUniform4iv(GLuint program, GLint location, GLsizei count, const GLint * value) const {
    _ProgramUniform4iv(program, location, count, value);
    TRACE("glProgramUniform4iv");
}

void GLMethods::ProgramUniform4f(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) const {
    _ProgramUniform4f(program, location, v0, v1, v2, v3);
    TRACE("glProgramUniform4f");
}

void GLMethods::ProgramUniform4fv(GLuint program, GLint location, GLsizei count, const GLfloat * value) const {
    _ProgramUniform4fv(program, location, count, value);
    TRACE("glProgramUniform4fv");
}

void GLMethods::ProgramUniform4d(GLuint program, GLint location, GLdouble v0, GLdouble v1, GLdouble v2, GLdouble v3) const {
    _ProgramUniform4d(program, location, v0, v1, v2, v3);
    TRACE("glProgramUniform4d");
}

void GLMethods::ProgramUniform4dv(GLuint program, GLint location, GLsizei count, const GLdouble * value) const {
    _ProgramUniform4dv(program, location, count, value);
    TRACE("glProgramUniform4dv");
}

void GLMethods::ProgramUniform4ui(GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3) const {
    _ProgramUniform4ui(program, location, v0, v1, v2, v3);
    TRACE("glProgramUniform4ui");
}

void GLMethods::ProgramUniform4uiv(GLuint program, GLint location, GLsizei count, const GLuint * value) const {
    _ProgramUniform4uiv(program, location, count, value);
    TRACE("glProgramUniform4uiv");
}

void GLMethods::ProgramUniformMatrix2fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value) const {
    _ProgramUniformMatrix2fv(program, location, count, transpose, value);
    TRACE("glProgramUniformMatrix2fv");
}

void GLMethods::ProgramUniformMatrix3fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value) const {
    _ProgramUniformMatrix3fv(program, location, count, transpose, value);
    TRACE("glProgramUniformMatrix3fv");
}

void GLMethods::ProgramUniformMatrix4fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value) const {
    _ProgramUniformMatrix4fv(program, location, count, transpose, value);
    TRACE("glProgramUniformMatrix4fv");
}

void GLMethods::ProgramUniformMatrix2dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value) const {
    _ProgramUniformMatrix2dv(program, location, count, transpose, value);
    TRACE("glProgramUniformMatrix2dv");
}

void GLMethods::ProgramUniformMatrix3dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value) const {
    _ProgramUniformMatrix3dv(program, location, count, transpose, value);
    TRACE("glProgramUniformMatrix3dv");
}

void GLMethods::ProgramUniformMatrix4dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value) const {
    _ProgramUniformMatrix4dv(program, location, count, transpose, value);
    TRACE("glProgramUniformMatrix4dv");
}

void GLMethods::ProgramUniformMatrix2x3fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value) const {
    _ProgramUniformMatrix2x3fv(program, location, count, transpose, value);
    TRACE("glProgramUniformMatrix2x3fv");
}

void GLMethods::ProgramUniformMatrix3x2fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value) const {
    _ProgramUniformMatrix3x2fv(program, location, count, transpose, value);
    TRACE("glProgramUniformMatrix3x2fv");
}

void GLMethods::ProgramUniformMatrix2x4fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value) const {
    _ProgramUniformMatrix2x4fv(program, location, count, transpose, value);
    TRACE("glProgramUniformMatrix2x4fv");
}

void GLMethods::ProgramUniformMatrix4x2fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value) const {
    _ProgramUniformMatrix4x2fv(program, location, count, transpose, value);
    TRACE("glProgramUniformMatrix4x2fv");
}

void GLMethods::ProgramUniformMatrix3x4fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value) const {
    _ProgramUniformMatrix3x4fv(program, location, count, transpose, value);
    TRACE("glProgramUniformMatrix3x4fv");
}

void GLMethods::ProgramUniformMatrix4x3fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value) const {
    _ProgramUniformMatrix4x3fv(program, location, count, transpose, value);
    TRACE("glProgramUniformMatrix4x3fv");
}

void GLMethods::ProgramUniformMatrix2x3dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value) const {
    _ProgramUniformMatrix2x3dv(program, location, count, transpose, value);
    TRACE("glProgramUniformMatrix2x3dv");
}

void GLMethods::ProgramUniformMatrix3x2dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value) const {
    _ProgramUniformMatrix3x2dv(program, location, count, transpose, value);
    TRACE("glProgramUniformMatrix3x2dv");
}

void GLMethods::ProgramUniformMatrix2x4dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value) const {
    _ProgramUniformMatrix2x4dv(program, location, count, transpose, value);
    TRACE("glProgramUniformMatrix2x4dv");
}

void GLMethods::ProgramUniformMatrix4x2dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value) const {
    _ProgramUniformMatrix4x2dv(program, location, count, transpose, value);
    TRACE("glProgramUniformMatrix4x2dv");
}

void GLMethods::ProgramUniformMatrix3x4dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value) const {
    _ProgramUniformMatrix3x4dv(program, location, count, transpose, value);
    TRACE("glProgramUniformMatrix3x4dv");
}

void GLMethods::ProgramUniformMatrix4x3dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value) const {
    _ProgramUniformMatrix4x3dv(program, location, count, transpose, value);
    TRACE("glProgramUniformMatrix4x3dv");
}

void GLMethods::ValidateProgramPipeline(GLuint pipeline) const {
    _ValidateProgramPipeline(pipeline);
    TRACE("glValidateProgramPipeline");
}

void GLMethods::GetProgramPipelineInfoLog(GLuint pipeline, GLsizei bufSize, GLsizei * length, GLchar * infoLog) const {
    _GetProgramPipelineInfoLog(pipeline, bufSize, length, infoLog);
    TRACE("glGetProgramPipelineInfoLog");
}

void GLMethods::VertexAttribL1d(GLuint index, GLdouble x) const {
    _VertexAttribL1d(index, x);
    TRACE("glVertexAttribL1d");
}

void GLMethods::VertexAttribL2d(GLuint index, GLdouble x, GLdouble y) const {
    _VertexAttribL2d(index, x, y);
    TRACE("glVertexAttribL2d");
}

void GLMethods::VertexAttribL3d(GLuint index, GLdouble x, GLdouble y, GLdouble z) const {
    _VertexAttribL3d(index, x, y, z);
    TRACE("glVertexAttribL3d");
}

void GLMethods::VertexAttribL4d(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w) const {
    _VertexAttribL4d(index, x, y, z, w);
    TRACE("glVertexAttribL4d");
}

void GLMethods::VertexAttribL1dv(GLuint index, const GLdouble * v) const {
    _VertexAttribL1dv(index, v);
    TRACE("glVertexAttribL1dv");
}

void GLMethods::VertexAttribL2dv(GLuint index, const GLdouble * v) const {
    _VertexAttribL2dv(index, v);
    TRACE("glVertexAttribL2dv");
}

void GLMethods::VertexAttribL3dv(GLuint index, const GLdouble * v) const {
    _VertexAttribL3dv(index, v);
    TRACE("glVertexAttribL3dv");
}

void GLMethods::VertexAttribL4dv(GLuint index, const GLdouble * v) const {
    _VertexAttribL4dv(index, v);
    TRACE("glVertexAttribL4dv");
}

void GLMethods::VertexAttribLPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const void * pointer) const {
    _VertexAttribLPointer(index, size, type, stride, pointer);
    TRACE("glVertexAttribLPointer");
}

void GLMethods::GetVertexAttribLdv(GLuint index, GLenum pname, GLdouble * params) const {
    _GetVertexAttribLdv(index, pname, params);
    TRACE("glGetVertexAttribLdv");
}

void GLMethods::ViewportArrayv(GLuint first, GLsizei count, const GLfloat * v) const {
    _ViewportArrayv(first, count, v);
    TRACE("glViewportArrayv");
}

void GLMethods::ViewportIndexedf(GLuint index, GLfloat x, GLfloat y, GLfloat w, GLfloat h) const {
    _ViewportIndexedf(index, x, y, w, h);
    TRACE("glViewportIndexedf");
}

void GLMethods::ViewportIndexedfv(GLuint index, const GLfloat * v) const {
    _ViewportIndexedfv(index, v);
    TRACE("glViewportIndexedfv");
}

void GLMethods::ScissorArrayv(GLuint first, GLsizei count, const GLint * v) const {
    _ScissorArrayv(first, count, v);
    TRACE("glScissorArrayv");
}

void GLMethods::ScissorIndexed(GLuint index, GLint left, GLint bottom, GLsizei width, GLsizei height) const {
    _ScissorIndexed(index, left, bottom, width, height);
    TRACE("glScissorIndexed");
}

void GLMethods::ScissorIndexedv(GLuint index, const GLint * v) const {
    _ScissorIndexedv(index, v);
    TRACE("glScissorIndexedv");
}

void GLMethods::DepthRangeArrayv(GLuint first, GLsizei count, const GLdouble * v) const {
    _DepthRangeArrayv(first, count, v);
    TRACE("glDepthRangeArrayv");
}

void GLMethods::DepthRangeIndexed(GLuint index, GLdouble n, GLdouble f) const {
    _DepthRangeIndexed(index, n, f);
    TRACE("glDepthRangeIndexed");
}

void GLMethods::GetFloati_v(GLenum target, GLuint index, GLfloat * data) const {
    _GetFloati_v(target, index, data);
    TRACE("glGetFloati_v");
}

void GLMethods::GetDoublei_v(GLenum target, GLuint index, GLdouble * data) const {
    _GetDoublei_v(target, index, data);
    TRACE("glGetDoublei_v");
}

void GLMethods::DrawArraysInstancedBaseInstance(GLenum mode, GLint first, GLsizei count, GLsizei instancecount, GLuint baseinstance) const {
    _DrawArraysInstancedBaseInstance(mode, first, count, instancecount, baseinstance);
    TRACE("glDrawArraysInstancedBaseInstance");
}

void GLMethods::DrawElementsInstancedBaseInstance(GLenum mode, GLsizei count, GLenum type, const void * indices, GLsizei instancecount, GLuint baseinstance) const {
    _DrawElementsInstancedBaseInstance(mode, count, type, indices, instancecount, baseinstance);
    TRACE("glDrawElementsInstancedBaseInstance");
}

void GLMethods::DrawElementsInstancedBaseVertexBaseInstance(GLenum mode, GLsizei count, GLenum type, const void * indices, GLsizei instancecount, GLint basevertex, GLuint baseinstance) const {
    _DrawElementsInstancedBaseVertexBaseInstance(mode, count, type, indices, instancecount, basevertex, baseinstance);
    TRACE("glDrawElementsInstancedBaseVertexBaseInstance");
}

void GLMethods::GetInternalformativ(GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint * params) const {
    _GetInternalformativ(target, internalformat, pname, bufSize, params);
    TRACE("glGetInternalformativ");
}

void GLMethods::GetActiveAtomicCounterBufferiv(GLuint program, GLuint bufferIndex, GLenum pname, GLint * params) const {
    _GetActiveAtomicCounterBufferiv(program, bufferIndex, pname, params);
    TRACE("glGetActiveAtomicCounterBufferiv");
}

void GLMethods::BindImageTexture(GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format) const {
    _BindImageTexture(unit, texture, level, layered, layer, access, format);
    TRACE("glBindImageTexture");
}

void GLMethods::MemoryBarrier(GLbitfield barriers) const {
    _MemoryBarrier(barriers);
    TRACE("glMemoryBarrier");
}

void GLMethods::TexStorage1D(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width) const {
    _TexStorage1D(target, levels, internalformat, width);
    TRACE("glTexStorage1D");
}

void GLMethods::TexStorage2D(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height) const {
    _TexStorage2D(target, levels, internalformat, width, height);
    TRACE("glTexStorage2D");
}

void GLMethods::TexStorage3D(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth) const {
    _TexStorage3D(target, levels, internalformat, width, height, depth);
    TRACE("glTexStorage3D");
}

void GLMethods::DrawTransformFeedbackInstanced(GLenum mode, GLuint id, GLsizei instancecount) const {
    _DrawTransformFeedbackInstanced(mode, id, instancecount);
    TRACE("glDrawTransformFeedbackInstanced");
}

void GLMethods::DrawTransformFeedbackStreamInstanced(GLenum mode, GLuint id, GLuint stream, GLsizei instancecount) const {
    _DrawTransformFeedbackStreamInstanced(mode, id, stream, instancecount);
    TRACE("glDrawTransformFeedbackStreamInstanced");
}

void GLMethods::ClearBufferData(GLenum target, GLenum internalformat, GLenum format, GLenum type, const void * data) const {
    _ClearBufferData(target, internalformat, format, type, data);
    TRACE("glClearBufferData");
}

void GLMethods::ClearBufferSubData(GLenum target, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void * data) const {
    _ClearBufferSubData(target, internalformat, offset, size, format, type, data);
    TRACE("glClearBufferSubData");
}

void GLMethods::DispatchCompute(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z) const {
    _DispatchCompute(num_groups_x, num_groups_y, num_groups_z);
    TRACE("glDispatchCompute");
}

void GLMethods::DispatchComputeIndirect(GLintptr indirect) const {
    _DispatchComputeIndirect(indirect);
    TRACE("glDispatchComputeIndirect");
}

void GLMethods::CopyImageSubData(GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth) const {
    _CopyImageSubData(srcName, srcTarget, srcLevel, srcX, srcY, srcZ, dstName, dstTarget, dstLevel, dstX, dstY, dstZ, srcWidth, srcHeight, srcDepth);
    TRACE("glCopyImageSubData");
}

void GLMethods::FramebufferParameteri(GLenum target, GLenum pname, GLint param) const {
    _FramebufferParameteri(target, pname, param);
    TRACE("glFramebufferParameteri");
}

void GLMethods::GetFramebufferParameteriv(GLenum target, GLenum pname, GLint * params) const {
    _GetFramebufferParameteriv(target, pname, params);
    TRACE("glGetFramebufferParameteriv");
}

void GLMethods::GetInternalformati64v(GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint64 * params) const {
    _GetInternalformati64v(target, internalformat, pname, bufSize, params);
    TRACE("glGetInternalformati64v");
}

void GLMethods::InvalidateTexSubImage(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth) const {
    _InvalidateTexSubImage(texture, level, xoffset, yoffset, zoffset, width, height, depth);
    TRACE("glInvalidateTexSubImage");
}

void GLMethods::InvalidateTexImage(GLuint texture, GLint level) const {
    _InvalidateTexImage(texture, level);
    TRACE("glInvalidateTexImage");
}

void GLMethods::InvalidateBufferSubData(GLuint buffer, GLintptr offset, GLsizeiptr length) const {
    _InvalidateBufferSubData(buffer, offset, length);
    TRACE("glInvalidateBufferSubData");
}

void GLMethods::InvalidateBufferData(GLuint buffer) const {
    _InvalidateBufferData(buffer);
    TRACE("glInvalidateBufferData");
}

void GLMethods::InvalidateFramebuffer(GLenum target, GLsizei numAttachments, const GLenum * attachments) const {
    _InvalidateFramebuffer(target, numAttachments, attachments);
    TRACE("glInvalidateFramebuffer");
}

void GLMethods::InvalidateSubFramebuffer(GLenum target, GLsizei numAttachments, const GLenum * attachments, GLint x, GLint y, GLsizei width, GLsizei height) const {
    _InvalidateSubFramebuffer(target, numAttachments, attachments, x, y, width, height);
    TRACE("glInvalidateSubFramebuffer");
}

void GLMethods::MultiDrawArraysIndirect(GLenum mode, const void * indirect, GLsizei drawcount, GLsizei stride) const {
    _MultiDrawArraysIndirect(mode, indirect, drawcount, stride);
    TRACE("glMultiDrawArraysIndirect");
}

void GLMethods::MultiDrawElementsIndirect(GLenum mode, GLenum type, const void * indirect, GLsizei drawcount, GLsizei stride) const {
    _MultiDrawElementsIndirect(mode, type, indirect, drawcount, stride);
    TRACE("glMultiDrawElementsIndirect");
}

void GLMethods::GetProgramInterfaceiv(GLuint program, GLenum programInterface, GLenum pname, GLint * params) const {
    _GetProgramInterfaceiv(program, programInterface, pname, params);
    TRACE("glGetProgramInterfaceiv");
}

GLuint GLMethods::GetProgramResourceIndex(GLuint program, GLenum programInterface, const GLchar * name) const {
    GLuint __result = _GetProgramResourceIndex(program, programInterface, name);
    TRACE("glGetProgramResourceIndex");
    return __result;
}

void GLMethods::GetProgramResourceName(GLuint program, GLenum programInterface, GLuint index, GLsizei bufSize, GLsizei * length, GLchar * name) const {
    _GetProgramResourceName(program, programInterface, index, bufSize, length, name);
    TRACE("glGetProgramResourceName");
}

void GLMethods::GetProgramResourceiv(GLuint program, GLenum programInterface, GLuint index, GLsizei propCount, const GLenum * props, GLsizei bufSize, GLsizei * length, GLint * params) const {
    _GetProgramResourceiv(program, programInterface, index, propCount, props, bufSize, length, params);
    TRACE("glGetProgramResourceiv");
}

GLint GLMethods::GetProgramResourceLocation(GLuint program, GLenum programInterface, const GLchar * name) const {
    GLint __result = _GetProgramResourceLocation(program, programInterface, name);
    TRACE("glGetProgramResourceLocation");
    return __result;
}

GLint GLMethods::GetProgramResourceLocationIndex(GLuint program, GLenum programInterface, const GLchar * name) const {
    GLint __result = _GetProgramResourceLocationIndex(program, programInterface, name);
    TRACE("glGetProgramResourceLocationIndex");
    return __result;
}

void GLMethods::ShaderStorageBlockBinding(GLuint program, GLuint storageBlockIndex, GLuint storageBlockBinding) const {
    _ShaderStorageBlockBinding(program, storageBlockIndex, storageBlockBinding);
    TRACE("glShaderStorageBlockBinding");
}

void GLMethods::TexBufferRange(GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size) const {
    _TexBufferRange(target, internalformat, buffer, offset, size);
    TRACE("glTexBufferRange");
}

void GLMethods::TexStorage2DMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations) const {
    _TexStorage2DMultisample(target, samples, internalformat, width, height, fixedsamplelocations);
    TRACE("glTexStorage2DMultisample");
}

void GLMethods::TexStorage3DMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations) const {
    _TexStorage3DMultisample(target, samples, internalformat, width, height, depth, fixedsamplelocations);
    TRACE("glTexStorage3DMultisample");
}

void GLMethods::TextureView(GLuint texture, GLenum target, GLuint origtexture, GLenum internalformat, GLuint minlevel, GLuint numlevels, GLuint minlayer, GLuint numlayers) const {
    _TextureView(texture, target, origtexture, internalformat, minlevel, numlevels, minlayer, numlayers);
    TRACE("glTextureView");
}

void GLMethods::BindVertexBuffer(GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride) const {
    _BindVertexBuffer(bindingindex, buffer, offset, stride);
    TRACE("glBindVertexBuffer");
}

void GLMethods::VertexAttribFormat(GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset) const {
    _VertexAttribFormat(attribindex, size, type, normalized, relativeoffset);
    TRACE("glVertexAttribFormat");
}

void GLMethods::VertexAttribIFormat(GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset) const {
    _VertexAttribIFormat(attribindex, size, type, relativeoffset);
    TRACE("glVertexAttribIFormat");
}

void GLMethods::VertexAttribLFormat(GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset) const {
    _VertexAttribLFormat(attribindex, size, type, relativeoffset);
    TRACE("glVertexAttribLFormat");
}

void GLMethods::VertexAttribBinding(GLuint attribindex, GLuint bindingindex) const {
    _VertexAttribBinding(attribindex, bindingindex);
    TRACE("glVertexAttribBinding");
}

void GLMethods::VertexBindingDivisor(GLuint bindingindex, GLuint divisor) const {
    _VertexBindingDivisor(bindingindex, divisor);
    TRACE("glVertexBindingDivisor");
}

void GLMethods::DebugMessageControl(GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint * ids, GLboolean enabled) const {
    _DebugMessageControl(source, type, severity, count, ids, enabled);
    TRACE("glDebugMessageControl");
}

void GLMethods::DebugMessageInsert(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * buf) const {
    _DebugMessageInsert(source, type, id, severity, length, buf);
    TRACE("glDebugMessageInsert");
}

void GLMethods::DebugMessageCallback(GLDEBUGPROC callback, const void * userParam) const {
    _DebugMessageCallback(callback, userParam);
    TRACE("glDebugMessageCallback");
}

GLuint GLMethods::GetDebugMessageLog(GLuint count, GLsizei bufSize, GLenum * sources, GLenum * types, GLuint * ids, GLenum * severities, GLsizei * lengths, GLchar * messageLog) const {
    GLuint __result = _GetDebugMessageLog(count, bufSize, sources, types, ids, severities, lengths, messageLog);
    TRACE("glGetDebugMessageLog");
    return __result;
}

void GLMethods::PushDebugGroup(GLenum source, GLuint id, GLsizei length, const GLchar * message) const {
    _PushDebugGroup(source, id, length, message);
    TRACE("glPushDebugGroup");
}

void GLMethods::PopDebugGroup() const {
    _PopDebugGroup();
    TRACE("glPopDebugGroup");
}

void GLMethods::ObjectLabel(GLenum identifier, GLuint name, GLsizei length, const GLchar * label) const {
    _ObjectLabel(identifier, name, length, label);
    TRACE("glObjectLabel");
}

void GLMethods::GetObjectLabel(GLenum identifier, GLuint name, GLsizei bufSize, GLsizei * length, GLchar * label) const {
    _GetObjectLabel(identifier, name, bufSize, length, label);
    TRACE("glGetObjectLabel");
}

void GLMethods::ObjectPtrLabel(const void * ptr, GLsizei length, const GLchar * label) const {
    _ObjectPtrLabel(ptr, length, label);
    TRACE("glObjectPtrLabel");
}

void GLMethods::GetObjectPtrLabel(const void * ptr, GLsizei bufSize, GLsizei * length, GLchar * label) const {
    _GetObjectPtrLabel(ptr, bufSize, length, label);
    TRACE("glGetObjectPtrLabel");
}

void GLMethods::BufferStorage(GLenum target, GLsizeiptr size, const void * data, GLbitfield flags) const {
    _BufferStorage(target, size, data, flags);
    TRACE("glBufferStorage(%x, %d, %p, %x)", target, size, data, flags);
}

void GLMethods::ClearTexImage(GLuint texture, GLint level, GLenum format, GLenum type, const void * data) const {
    _ClearTexImage(texture, level, format, type, data);
    TRACE("glClearTexImage");
}

void GLMethods::ClearTexSubImage(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void * data) const {
    _ClearTexSubImage(texture, level, xoffset, yoffset, zoffset, width, height, depth, format, type, data);
    TRACE("glClearTexSubImage");
}

void GLMethods::BindBuffersBase(GLenum target, GLuint first, GLsizei count, const GLuint * buffers) const {
    _BindBuffersBase(target, first, count, buffers);
    TRACE("glBindBuffersBase");
}

void GLMethods::BindBuffersRange(GLenum target, GLuint first, GLsizei count, const GLuint * buffers, const GLintptr * offsets, const GLsizeiptr * sizes) const {
    _BindBuffersRange(target, first, count, buffers, offsets, sizes);
    TRACE("glBindBuffersRange");
}

void GLMethods::BindTextures(GLuint first, GLsizei count, const GLuint * textures) const {
    _BindTextures(first, count, textures);
    TRACE("glBindTextures");
}

void GLMethods::BindSamplers(GLuint first, GLsizei count, const GLuint * samplers) const {
    _BindSamplers(first, count, samplers);
    TRACE("glBindSamplers");
}

void GLMethods::BindImageTextures(GLuint first, GLsizei count, const GLuint * textures) const {
    _BindImageTextures(first, count, textures);
    TRACE("glBindImageTextures");
}

void GLMethods::BindVertexBuffers(GLuint first, GLsizei count, const GLuint * buffers, const GLintptr * offsets, const GLsizei * strides) const {
    _BindVertexBuffers(first, count, buffers, offsets, strides);
    TRACE("glBindVertexBuffers");
}

void GLMethods::ClipControl(GLenum origin, GLenum depth) const {
    _ClipControl(origin, depth);
    TRACE("glClipControl");
}

void GLMethods::CreateTransformFeedbacks(GLsizei n, GLuint * ids) const {
    _CreateTransformFeedbacks(n, ids);
    TRACE("glCreateTransformFeedbacks");
}

void GLMethods::TransformFeedbackBufferBase(GLuint xfb, GLuint index, GLuint buffer) const {
    _TransformFeedbackBufferBase(xfb, index, buffer);
    TRACE("glTransformFeedbackBufferBase");
}

void GLMethods::TransformFeedbackBufferRange(GLuint xfb, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size) const {
    _TransformFeedbackBufferRange(xfb, index, buffer, offset, size);
    TRACE("glTransformFeedbackBufferRange");
}

void GLMethods::GetTransformFeedbackiv(GLuint xfb, GLenum pname, GLint * param) const {
    _GetTransformFeedbackiv(xfb, pname, param);
    TRACE("glGetTransformFeedbackiv");
}

void GLMethods::GetTransformFeedbacki_v(GLuint xfb, GLenum pname, GLuint index, GLint * param) const {
    _GetTransformFeedbacki_v(xfb, pname, index, param);
    TRACE("glGetTransformFeedbacki_v");
}

void GLMethods::GetTransformFeedbacki64_v(GLuint xfb, GLenum pname, GLuint index, GLint64 * param) const {
    _GetTransformFeedbacki64_v(xfb, pname, index, param);
    TRACE("glGetTransformFeedbacki64_v");
}

void GLMethods::CreateBuffers(GLsizei n, GLuint * buffers) const {
    _CreateBuffers(n, buffers);
    TRACE("glCreateBuffers");
}

void GLMethods::NamedBufferStorage(GLuint buffer, GLsizeiptr size, const void * data, GLbitfield flags) const {
    _NamedBufferStorage(buffer, size, data, flags);
    TRACE("glNamedBufferStorage");
}

void GLMethods::NamedBufferData(GLuint buffer, GLsizeiptr size, const void * data, GLenum usage) const {
    _NamedBufferData(buffer, size, data, usage);
    TRACE("glNamedBufferData");
}

void GLMethods::NamedBufferSubData(GLuint buffer, GLintptr offset, GLsizeiptr size, const void * data) const {
    _NamedBufferSubData(buffer, offset, size, data);
    TRACE("glNamedBufferSubData");
}

void GLMethods::CopyNamedBufferSubData(GLuint readBuffer, GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size) const {
    _CopyNamedBufferSubData(readBuffer, writeBuffer, readOffset, writeOffset, size);
    TRACE("glCopyNamedBufferSubData");
}

void GLMethods::ClearNamedBufferData(GLuint buffer, GLenum internalformat, GLenum format, GLenum type, const void * data) const {
    _ClearNamedBufferData(buffer, internalformat, format, type, data);
    TRACE("glClearNamedBufferData");
}

void GLMethods::ClearNamedBufferSubData(GLuint buffer, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void * data) const {
    _ClearNamedBufferSubData(buffer, internalformat, offset, size, format, type, data);
    TRACE("glClearNamedBufferSubData");
}

void * GLMethods::MapNamedBuffer(GLuint buffer, GLenum access) const {
    void * __result = _MapNamedBuffer(buffer, access);
    TRACE("glMapNamedBuffer");
    return __result;
}

void * GLMethods::MapNamedBufferRange(GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access) const {
    void * __result = _MapNamedBufferRange(buffer, offset, length, access);
    TRACE("glMapNamedBufferRange");
    return __result;
}

GLboolean GLMethods::UnmapNamedBuffer(GLuint buffer) const {
    GLboolean __result = _UnmapNamedBuffer(buffer);
    TRACE("glUnmapNamedBuffer");
    return __result;
}

void GLMethods::FlushMappedNamedBufferRange(GLuint buffer, GLintptr offset, GLsizeiptr length) const {
    _FlushMappedNamedBufferRange(buffer, offset, length);
    TRACE("glFlushMappedNamedBufferRange");
}

void GLMethods::GetNamedBufferParameteriv(GLuint buffer, GLenum pname, GLint * params) const {
    _GetNamedBufferParameteriv(buffer, pname, params);
    TRACE("glGetNamedBufferParameteriv");
}

void GLMethods::GetNamedBufferParameteri64v(GLuint buffer, GLenum pname, GLint64 * params) const {
    _GetNamedBufferParameteri64v(buffer, pname, params);
    TRACE("glGetNamedBufferParameteri64v");
}

void GLMethods::GetNamedBufferPointerv(GLuint buffer, GLenum pname, void ** params) const {
    _GetNamedBufferPointerv(buffer, pname, params);
    TRACE("glGetNamedBufferPointerv");
}

void GLMethods::GetNamedBufferSubData(GLuint buffer, GLintptr offset, GLsizeiptr size, void * data) const {
    _GetNamedBufferSubData(buffer, offset, size, data);
    TRACE("glGetNamedBufferSubData");
}

void GLMethods::CreateFramebuffers(GLsizei n, GLuint * framebuffers) const {
    _CreateFramebuffers(n, framebuffers);
    TRACE("glCreateFramebuffers");
}

void GLMethods::NamedFramebufferRenderbuffer(GLuint framebuffer, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer) const {
    _NamedFramebufferRenderbuffer(framebuffer, attachment, renderbuffertarget, renderbuffer);
    TRACE("glNamedFramebufferRenderbuffer");
}

void GLMethods::NamedFramebufferParameteri(GLuint framebuffer, GLenum pname, GLint param) const {
    _NamedFramebufferParameteri(framebuffer, pname, param);
    TRACE("glNamedFramebufferParameteri");
}

void GLMethods::NamedFramebufferTexture(GLuint framebuffer, GLenum attachment, GLuint texture, GLint level) const {
    _NamedFramebufferTexture(framebuffer, attachment, texture, level);
    TRACE("glNamedFramebufferTexture");
}

void GLMethods::NamedFramebufferTextureLayer(GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLint layer) const {
    _NamedFramebufferTextureLayer(framebuffer, attachment, texture, level, layer);
    TRACE("glNamedFramebufferTextureLayer");
}

void GLMethods::NamedFramebufferDrawBuffer(GLuint framebuffer, GLenum buf) const {
    _NamedFramebufferDrawBuffer(framebuffer, buf);
    TRACE("glNamedFramebufferDrawBuffer");
}

void GLMethods::NamedFramebufferDrawBuffers(GLuint framebuffer, GLsizei n, const GLenum * bufs) const {
    _NamedFramebufferDrawBuffers(framebuffer, n, bufs);
    TRACE("glNamedFramebufferDrawBuffers");
}

void GLMethods::NamedFramebufferReadBuffer(GLuint framebuffer, GLenum src) const {
    _NamedFramebufferReadBuffer(framebuffer, src);
    TRACE("glNamedFramebufferReadBuffer");
}

void GLMethods::InvalidateNamedFramebufferData(GLuint framebuffer, GLsizei numAttachments, const GLenum * attachments) const {
    _InvalidateNamedFramebufferData(framebuffer, numAttachments, attachments);
    TRACE("glInvalidateNamedFramebufferData");
}

void GLMethods::InvalidateNamedFramebufferSubData(GLuint framebuffer, GLsizei numAttachments, const GLenum * attachments, GLint x, GLint y, GLsizei width, GLsizei height) const {
    _InvalidateNamedFramebufferSubData(framebuffer, numAttachments, attachments, x, y, width, height);
    TRACE("glInvalidateNamedFramebufferSubData");
}

void GLMethods::ClearNamedFramebufferiv(GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLint * value) const {
    _ClearNamedFramebufferiv(framebuffer, buffer, drawbuffer, value);
    TRACE("glClearNamedFramebufferiv");
}

void GLMethods::ClearNamedFramebufferuiv(GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLuint * value) const {
    _ClearNamedFramebufferuiv(framebuffer, buffer, drawbuffer, value);
    TRACE("glClearNamedFramebufferuiv");
}

void GLMethods::ClearNamedFramebufferfv(GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLfloat * value) const {
    _ClearNamedFramebufferfv(framebuffer, buffer, drawbuffer, value);
    TRACE("glClearNamedFramebufferfv");
}

void GLMethods::ClearNamedFramebufferfi(GLuint framebuffer, GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil) const {
    _ClearNamedFramebufferfi(framebuffer, buffer, drawbuffer, depth, stencil);
    TRACE("glClearNamedFramebufferfi");
}

void GLMethods::BlitNamedFramebuffer(GLuint readFramebuffer, GLuint drawFramebuffer, GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter) const {
    _BlitNamedFramebuffer(readFramebuffer, drawFramebuffer, srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);
    TRACE("glBlitNamedFramebuffer");
}

GLenum GLMethods::CheckNamedFramebufferStatus(GLuint framebuffer, GLenum target) const {
    GLenum __result = _CheckNamedFramebufferStatus(framebuffer, target);
    TRACE("glCheckNamedFramebufferStatus");
    return __result;
}

void GLMethods::GetNamedFramebufferParameteriv(GLuint framebuffer, GLenum pname, GLint * param) const {
    _GetNamedFramebufferParameteriv(framebuffer, pname, param);
    TRACE("glGetNamedFramebufferParameteriv");
}

void GLMethods::GetNamedFramebufferAttachmentParameteriv(GLuint framebuffer, GLenum attachment, GLenum pname, GLint * params) const {
    _GetNamedFramebufferAttachmentParameteriv(framebuffer, attachment, pname, params);
    TRACE("glGetNamedFramebufferAttachmentParameteriv");
}

void GLMethods::CreateRenderbuffers(GLsizei n, GLuint * renderbuffers) const {
    _CreateRenderbuffers(n, renderbuffers);
    TRACE("glCreateRenderbuffers");
}

void GLMethods::NamedRenderbufferStorage(GLuint renderbuffer, GLenum internalformat, GLsizei width, GLsizei height) const {
    _NamedRenderbufferStorage(renderbuffer, internalformat, width, height);
    TRACE("glNamedRenderbufferStorage");
}

void GLMethods::NamedRenderbufferStorageMultisample(GLuint renderbuffer, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height) const {
    _NamedRenderbufferStorageMultisample(renderbuffer, samples, internalformat, width, height);
    TRACE("glNamedRenderbufferStorageMultisample");
}

void GLMethods::GetNamedRenderbufferParameteriv(GLuint renderbuffer, GLenum pname, GLint * params) const {
    _GetNamedRenderbufferParameteriv(renderbuffer, pname, params);
    TRACE("glGetNamedRenderbufferParameteriv");
}

void GLMethods::CreateTextures(GLenum target, GLsizei n, GLuint * textures) const {
    _CreateTextures(target, n, textures);
    TRACE("glCreateTextures");
}

void GLMethods::TextureBuffer(GLuint texture, GLenum internalformat, GLuint buffer) const {
    _TextureBuffer(texture, internalformat, buffer);
    TRACE("glTextureBuffer");
}

void GLMethods::TextureBufferRange(GLuint texture, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size) const {
    _TextureBufferRange(texture, internalformat, buffer, offset, size);
    TRACE("glTextureBufferRange");
}

void GLMethods::TextureStorage1D(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width) const {
    _TextureStorage1D(texture, levels, internalformat, width);
    TRACE("glTextureStorage1D");
}

void GLMethods::TextureStorage2D(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height) const {
    _TextureStorage2D(texture, levels, internalformat, width, height);
    TRACE("glTextureStorage2D");
}

void GLMethods::TextureStorage3D(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth) const {
    _TextureStorage3D(texture, levels, internalformat, width, height, depth);
    TRACE("glTextureStorage3D");
}

void GLMethods::TextureStorage2DMultisample(GLuint texture, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations) const {
    _TextureStorage2DMultisample(texture, samples, internalformat, width, height, fixedsamplelocations);
    TRACE("glTextureStorage2DMultisample");
}

void GLMethods::TextureStorage3DMultisample(GLuint texture, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations) const {
    _TextureStorage3DMultisample(texture, samples, internalformat, width, height, depth, fixedsamplelocations);
    TRACE("glTextureStorage3DMultisample");
}

void GLMethods::TextureSubImage1D(GLuint texture, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void * pixels) const {
    _TextureSubImage1D(texture, level, xoffset, width, format, type, pixels);
    TRACE("glTextureSubImage1D");
}

void GLMethods::TextureSubImage2D(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void * pixels) const {
    _TextureSubImage2D(texture, level, xoffset, yoffset, width, height, format, type, pixels);
    TRACE("glTextureSubImage2D");
}

void GLMethods::TextureSubImage3D(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void * pixels) const {
    _TextureSubImage3D(texture, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels);
    TRACE("glTextureSubImage3D");
}

void GLMethods::CompressedTextureSubImage1D(GLuint texture, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void * data) const {
    _CompressedTextureSubImage1D(texture, level, xoffset, width, format, imageSize, data);
    TRACE("glCompressedTextureSubImage1D");
}

void GLMethods::CompressedTextureSubImage2D(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void * data) const {
    _CompressedTextureSubImage2D(texture, level, xoffset, yoffset, width, height, format, imageSize, data);
    TRACE("glCompressedTextureSubImage2D");
}

void GLMethods::CompressedTextureSubImage3D(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void * data) const {
    _CompressedTextureSubImage3D(texture, level, xoffset, yoffset, zoffset, width, height, depth, format, imageSize, data);
    TRACE("glCompressedTextureSubImage3D");
}

void GLMethods::CopyTextureSubImage1D(GLuint texture, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width) const {
    _CopyTextureSubImage1D(texture, level, xoffset, x, y, width);
    TRACE("glCopyTextureSubImage1D");
}

void GLMethods::CopyTextureSubImage2D(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height) const {
    _CopyTextureSubImage2D(texture, level, xoffset, yoffset, x, y, width, height);
    TRACE("glCopyTextureSubImage2D");
}

void GLMethods::CopyTextureSubImage3D(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height) const {
    _CopyTextureSubImage3D(texture, level, xoffset, yoffset, zoffset, x, y, width, height);
    TRACE("glCopyTextureSubImage3D");
}

void GLMethods::TextureParameterf(GLuint texture, GLenum pname, GLfloat param) const {
    _TextureParameterf(texture, pname, param);
    TRACE("glTextureParameterf");
}

void GLMethods::TextureParameterfv(GLuint texture, GLenum pname, const GLfloat * param) const {
    _TextureParameterfv(texture, pname, param);
    TRACE("glTextureParameterfv");
}

void GLMethods::TextureParameteri(GLuint texture, GLenum pname, GLint param) const {
    _TextureParameteri(texture, pname, param);
    TRACE("glTextureParameteri");
}

void GLMethods::TextureParameterIiv(GLuint texture, GLenum pname, const GLint * params) const {
    _TextureParameterIiv(texture, pname, params);
    TRACE("glTextureParameterIiv");
}

void GLMethods::TextureParameterIuiv(GLuint texture, GLenum pname, const GLuint * params) const {
    _TextureParameterIuiv(texture, pname, params);
    TRACE("glTextureParameterIuiv");
}

void GLMethods::TextureParameteriv(GLuint texture, GLenum pname, const GLint * param) const {
    _TextureParameteriv(texture, pname, param);
    TRACE("glTextureParameteriv");
}

void GLMethods::GenerateTextureMipmap(GLuint texture) const {
    _GenerateTextureMipmap(texture);
    TRACE("glGenerateTextureMipmap");
}

void GLMethods::BindTextureUnit(GLuint unit, GLuint texture) const {
    _BindTextureUnit(unit, texture);
    TRACE("glBindTextureUnit");
}

void GLMethods::GetTextureImage(GLuint texture, GLint level, GLenum format, GLenum type, GLsizei bufSize, void * pixels) const {
    _GetTextureImage(texture, level, format, type, bufSize, pixels);
    TRACE("glGetTextureImage");
}

void GLMethods::GetCompressedTextureImage(GLuint texture, GLint level, GLsizei bufSize, void * pixels) const {
    _GetCompressedTextureImage(texture, level, bufSize, pixels);
    TRACE("glGetCompressedTextureImage");
}

void GLMethods::GetTextureLevelParameterfv(GLuint texture, GLint level, GLenum pname, GLfloat * params) const {
    _GetTextureLevelParameterfv(texture, level, pname, params);
    TRACE("glGetTextureLevelParameterfv");
}

void GLMethods::GetTextureLevelParameteriv(GLuint texture, GLint level, GLenum pname, GLint * params) const {
    _GetTextureLevelParameteriv(texture, level, pname, params);
    TRACE("glGetTextureLevelParameteriv");
}

void GLMethods::GetTextureParameterfv(GLuint texture, GLenum pname, GLfloat * params) const {
    _GetTextureParameterfv(texture, pname, params);
    TRACE("glGetTextureParameterfv");
}

void GLMethods::GetTextureParameterIiv(GLuint texture, GLenum pname, GLint * params) const {
    _GetTextureParameterIiv(texture, pname, params);
    TRACE("glGetTextureParameterIiv");
}

void GLMethods::GetTextureParameterIuiv(GLuint texture, GLenum pname, GLuint * params) const {
    _GetTextureParameterIuiv(texture, pname, params);
    TRACE("glGetTextureParameterIuiv");
}

void GLMethods::GetTextureParameteriv(GLuint texture, GLenum pname, GLint * params) const {
    _GetTextureParameteriv(texture, pname, params);
    TRACE("glGetTextureParameteriv");
}

void GLMethods::CreateVertexArrays(GLsizei n, GLuint * arrays) const {
    _CreateVertexArrays(n, arrays);
    TRACE("glCreateVertexArrays");
}

void GLMethods::DisableVertexArrayAttrib(GLuint vaobj, GLuint index) const {
    _DisableVertexArrayAttrib(vaobj, index);
    TRACE("glDisableVertexArrayAttrib");
}

void GLMethods::EnableVertexArrayAttrib(GLuint vaobj, GLuint index) const {
    _EnableVertexArrayAttrib(vaobj, index);
    TRACE("glEnableVertexArrayAttrib");
}

void GLMethods::VertexArrayElementBuffer(GLuint vaobj, GLuint buffer) const {
    _VertexArrayElementBuffer(vaobj, buffer);
    TRACE("glVertexArrayElementBuffer");
}

void GLMethods::VertexArrayVertexBuffer(GLuint vaobj, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride) const {
    _VertexArrayVertexBuffer(vaobj, bindingindex, buffer, offset, stride);
    TRACE("glVertexArrayVertexBuffer");
}

void GLMethods::VertexArrayVertexBuffers(GLuint vaobj, GLuint first, GLsizei count, const GLuint * buffers, const GLintptr * offsets, const GLsizei * strides) const {
    _VertexArrayVertexBuffers(vaobj, first, count, buffers, offsets, strides);
    TRACE("glVertexArrayVertexBuffers");
}

void GLMethods::VertexArrayAttribBinding(GLuint vaobj, GLuint attribindex, GLuint bindingindex) const {
    _VertexArrayAttribBinding(vaobj, attribindex, bindingindex);
    TRACE("glVertexArrayAttribBinding");
}

void GLMethods::VertexArrayAttribFormat(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset) const {
    _VertexArrayAttribFormat(vaobj, attribindex, size, type, normalized, relativeoffset);
    TRACE("glVertexArrayAttribFormat");
}

void GLMethods::VertexArrayAttribIFormat(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset) const {
    _VertexArrayAttribIFormat(vaobj, attribindex, size, type, relativeoffset);
    TRACE("glVertexArrayAttribIFormat");
}

void GLMethods::VertexArrayAttribLFormat(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset) const {
    _VertexArrayAttribLFormat(vaobj, attribindex, size, type, relativeoffset);
    TRACE("glVertexArrayAttribLFormat");
}

void GLMethods::VertexArrayBindingDivisor(GLuint vaobj, GLuint bindingindex, GLuint divisor) const {
    _VertexArrayBindingDivisor(vaobj, bindingindex, divisor);
    TRACE("glVertexArrayBindingDivisor");
}

void GLMethods::GetVertexArrayiv(GLuint vaobj, GLenum pname, GLint * param) const {
    _GetVertexArrayiv(vaobj, pname, param);
    TRACE("glGetVertexArrayiv");
}

void GLMethods::GetVertexArrayIndexediv(GLuint vaobj, GLuint index, GLenum pname, GLint * param) const {
    _GetVertexArrayIndexediv(vaobj, index, pname, param);
    TRACE("glGetVertexArrayIndexediv");
}

void GLMethods::GetVertexArrayIndexed64iv(GLuint vaobj, GLuint index, GLenum pname, GLint64 * param) const {
    _GetVertexArrayIndexed64iv(vaobj, index, pname, param);
    TRACE("glGetVertexArrayIndexed64iv");
}

void GLMethods::CreateSamplers(GLsizei n, GLuint * samplers) const {
    _CreateSamplers(n, samplers);
    TRACE("glCreateSamplers");
}

void GLMethods::CreateProgramPipelines(GLsizei n, GLuint * pipelines) const {
    _CreateProgramPipelines(n, pipelines);
    TRACE("glCreateProgramPipelines");
}

void GLMethods::CreateQueries(GLenum target, GLsizei n, GLuint * ids) const {
    _CreateQueries(target, n, ids);
    TRACE("glCreateQueries");
}

void GLMethods::GetQueryBufferObjecti64v(GLuint id, GLuint buffer, GLenum pname, GLintptr offset) const {
    _GetQueryBufferObjecti64v(id, buffer, pname, offset);
    TRACE("glGetQueryBufferObjecti64v");
}

void GLMethods::GetQueryBufferObjectiv(GLuint id, GLuint buffer, GLenum pname, GLintptr offset) const {
    _GetQueryBufferObjectiv(id, buffer, pname, offset);
    TRACE("glGetQueryBufferObjectiv");
}

void GLMethods::GetQueryBufferObjectui64v(GLuint id, GLuint buffer, GLenum pname, GLintptr offset) const {
    _GetQueryBufferObjectui64v(id, buffer, pname, offset);
    TRACE("glGetQueryBufferObjectui64v");
}

void GLMethods::GetQueryBufferObjectuiv(GLuint id, GLuint buffer, GLenum pname, GLintptr offset) const {
    _GetQueryBufferObjectuiv(id, buffer, pname, offset);
    TRACE("glGetQueryBufferObjectuiv");
}

void GLMethods::MemoryBarrierByRegion(GLbitfield barriers) const {
    _MemoryBarrierByRegion(barriers);
    TRACE("glMemoryBarrierByRegion");
}

void GLMethods::GetTextureSubImage(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, GLsizei bufSize, void * pixels) const {
    _GetTextureSubImage(texture, level, xoffset, yoffset, zoffset, width, height, depth, format, type, bufSize, pixels);
    TRACE("glGetTextureSubImage");
}

void GLMethods::GetCompressedTextureSubImage(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLsizei bufSize, void * pixels) const {
    _GetCompressedTextureSubImage(texture, level, xoffset, yoffset, zoffset, width, height, depth, bufSize, pixels);
    TRACE("glGetCompressedTextureSubImage");
}

GLenum GLMethods::GetGraphicsResetStatus() const {
    GLenum __result = _GetGraphicsResetStatus();
    TRACE("glGetGraphicsResetStatus");
    return __result;
}

void GLMethods::GetnCompressedTexImage(GLenum target, GLint lod, GLsizei bufSize, void * pixels) const {
    _GetnCompressedTexImage(target, lod, bufSize, pixels);
    TRACE("glGetnCompressedTexImage");
}

void GLMethods::GetnTexImage(GLenum target, GLint level, GLenum format, GLenum type, GLsizei bufSize, void * pixels) const {
    _GetnTexImage(target, level, format, type, bufSize, pixels);
    TRACE("glGetnTexImage");
}

void GLMethods::GetnUniformdv(GLuint program, GLint location, GLsizei bufSize, GLdouble * params) const {
    _GetnUniformdv(program, location, bufSize, params);
    TRACE("glGetnUniformdv");
}

void GLMethods::GetnUniformfv(GLuint program, GLint location, GLsizei bufSize, GLfloat * params) const {
    _GetnUniformfv(program, location, bufSize, params);
    TRACE("glGetnUniformfv");
}

void GLMethods::GetnUniformiv(GLuint program, GLint location, GLsizei bufSize, GLint * params) const {
    _GetnUniformiv(program, location, bufSize, params);
    TRACE("glGetnUniformiv");
}

void GLMethods::GetnUniformuiv(GLuint program, GLint location, GLsizei bufSize, GLuint * params) const {
    _GetnUniformuiv(program, location, bufSize, params);
    TRACE("glGetnUniformuiv");
}

void GLMethods::ReadnPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, void * data) const {
    _ReadnPixels(x, y, width, height, format, type, bufSize, data);
    TRACE("glReadnPixels");
}

void GLMethods::TextureBarrier() const {
    _TextureBarrier();
    TRACE("glTextureBarrier");
}

void GLMethods::SpecializeShader(GLuint shader, const GLchar * pEntryPoint, GLuint numSpecializationConstants, const GLuint * pConstantIndex, const GLuint * pConstantValue) const {
    _SpecializeShader(shader, pEntryPoint, numSpecializationConstants, pConstantIndex, pConstantValue);
    TRACE("glSpecializeShader");
}

void GLMethods::MultiDrawArraysIndirectCount(GLenum mode, const void * indirect, GLintptr drawcount, GLsizei maxdrawcount, GLsizei stride) const {
    _MultiDrawArraysIndirectCount(mode, indirect, drawcount, maxdrawcount, stride);
    TRACE("glMultiDrawArraysIndirectCount");
}

void GLMethods::MultiDrawElementsIndirectCount(GLenum mode, GLenum type, const void * indirect, GLintptr drawcount, GLsizei maxdrawcount, GLsizei stride) const {
    _MultiDrawElementsIndirectCount(mode, type, indirect, drawcount, maxdrawcount, stride);
    TRACE("glMultiDrawElementsIndirectCount");
}

void GLMethods::PolygonOffsetClamp(GLfloat factor, GLfloat units, GLfloat clamp) const {
    _PolygonOffsetClamp(factor, units, clamp);
    TRACE("glPolygonOffsetClamp");
}

bool GLMethods::load() {
    this->_CullFace = (PFNGLCULLFACEPROC)LoadMethod(PREFIX "glCullFace");
    this->_FrontFace = (PFNGLFRONTFACEPROC)LoadMethod(PREFIX "glFrontFace");
    this->_Hint = (PFNGLHINTPROC)LoadMethod(PREFIX "glHint");
    this->_LineWidth = (PFNGLLINEWIDTHPROC)LoadMethod(PREFIX "glLineWidth");
    this->_PointSize = (PFNGLPOINTSIZEPROC)LoadMethod(PREFIX "glPointSize");
    this->_PolygonMode = (PFNGLPOLYGONMODEPROC)LoadMethod(PREFIX "glPolygonMode");
    this->_Scissor = (PFNGLSCISSORPROC)LoadMethod(PREFIX "glScissor");
    this->_TexParameterf = (PFNGLTEXPARAMETERFPROC)LoadMethod(PREFIX "glTexParameterf");
    this->_TexParameterfv = (PFNGLTEXPARAMETERFVPROC)LoadMethod(PREFIX "glTexParameterfv");
    this->_TexParameteri = (PFNGLTEXPARAMETERIPROC)LoadMethod(PREFIX "glTexParameteri");
    this->_TexParameteriv = (PFNGLTEXPARAMETERIVPROC)LoadMethod(PREFIX "glTexParameteriv");
    this->_TexImage1D = (PFNGLTEXIMAGE1DPROC)LoadMethod(PREFIX "glTexImage1D");
    this->_TexImage2D = (PFNGLTEXIMAGE2DPROC)LoadMethod(PREFIX "glTexImage2D");
    this->_DrawBuffer = (PFNGLDRAWBUFFERPROC)LoadMethod(PREFIX "glDrawBuffer");
    this->_Clear = (PFNGLCLEARPROC)LoadMethod(PREFIX "glClear");
    this->_ClearColor = (PFNGLCLEARCOLORPROC)LoadMethod(PREFIX "glClearColor");
    this->_ClearStencil = (PFNGLCLEARSTENCILPROC)LoadMethod(PREFIX "glClearStencil");
    this->_ClearDepth = (PFNGLCLEARDEPTHPROC)LoadMethod(PREFIX "glClearDepth");
    this->_StencilMask = (PFNGLSTENCILMASKPROC)LoadMethod(PREFIX "glStencilMask");
    this->_ColorMask = (PFNGLCOLORMASKPROC)LoadMethod(PREFIX "glColorMask");
    this->_DepthMask = (PFNGLDEPTHMASKPROC)LoadMethod(PREFIX "glDepthMask");
    this->_Disable = (PFNGLDISABLEPROC)LoadMethod(PREFIX "glDisable");
    this->_Enable = (PFNGLENABLEPROC)LoadMethod(PREFIX "glEnable");
    this->_Finish = (PFNGLFINISHPROC)LoadMethod(PREFIX "glFinish");
    this->_Flush = (PFNGLFLUSHPROC)LoadMethod(PREFIX "glFlush");
    this->_BlendFunc = (PFNGLBLENDFUNCPROC)LoadMethod(PREFIX "glBlendFunc");
    this->_LogicOp = (PFNGLLOGICOPPROC)LoadMethod(PREFIX "glLogicOp");
    this->_StencilFunc = (PFNGLSTENCILFUNCPROC)LoadMethod(PREFIX "glStencilFunc");
    this->_StencilOp = (PFNGLSTENCILOPPROC)LoadMethod(PREFIX "glStencilOp");
    this->_DepthFunc = (PFNGLDEPTHFUNCPROC)LoadMethod(PREFIX "glDepthFunc");
    this->_PixelStoref = (PFNGLPIXELSTOREFPROC)LoadMethod(PREFIX "glPixelStoref");
    this->_PixelStorei = (PFNGLPIXELSTOREIPROC)LoadMethod(PREFIX "glPixelStorei");
    this->_ReadBuffer = (PFNGLREADBUFFERPROC)LoadMethod(PREFIX "glReadBuffer");
    this->_ReadPixels = (PFNGLREADPIXELSPROC)LoadMethod(PREFIX "glReadPixels");
    this->_GetBooleanv = (PFNGLGETBOOLEANVPROC)LoadMethod(PREFIX "glGetBooleanv");
    this->_GetDoublev = (PFNGLGETDOUBLEVPROC)LoadMethod(PREFIX "glGetDoublev");
    this->_GetError = (PFNGLGETERRORPROC)LoadMethod(PREFIX "glGetError");
    this->_GetFloatv = (PFNGLGETFLOATVPROC)LoadMethod(PREFIX "glGetFloatv");
    this->_GetIntegerv = (PFNGLGETINTEGERVPROC)LoadMethod(PREFIX "glGetIntegerv");
    this->_GetString = (PFNGLGETSTRINGPROC)LoadMethod(PREFIX "glGetString");
    this->_GetTexImage = (PFNGLGETTEXIMAGEPROC)LoadMethod(PREFIX "glGetTexImage");
    this->_GetTexParameterfv = (PFNGLGETTEXPARAMETERFVPROC)LoadMethod(PREFIX "glGetTexParameterfv");
    this->_GetTexParameteriv = (PFNGLGETTEXPARAMETERIVPROC)LoadMethod(PREFIX "glGetTexParameteriv");
    this->_GetTexLevelParameterfv = (PFNGLGETTEXLEVELPARAMETERFVPROC)LoadMethod(PREFIX "glGetTexLevelParameterfv");
    this->_GetTexLevelParameteriv = (PFNGLGETTEXLEVELPARAMETERIVPROC)LoadMethod(PREFIX "glGetTexLevelParameteriv");
    this->_IsEnabled = (PFNGLISENABLEDPROC)LoadMethod(PREFIX "glIsEnabled");
    this->_DepthRange = (PFNGLDEPTHRANGEPROC)LoadMethod(PREFIX "glDepthRange");
    this->_Viewport = (PFNGLVIEWPORTPROC)LoadMethod(PREFIX "glViewport");
    this->_DrawArrays = (PFNGLDRAWARRAYSPROC)LoadMethod(PREFIX "glDrawArrays");
    this->_DrawElements = (PFNGLDRAWELEMENTSPROC)LoadMethod(PREFIX "glDrawElements");
    this->_GetPointerv = (PFNGLGETPOINTERVPROC)LoadMethod(PREFIX "glGetPointerv");
    this->_PolygonOffset = (PFNGLPOLYGONOFFSETPROC)LoadMethod(PREFIX "glPolygonOffset");
    this->_CopyTexImage1D = (PFNGLCOPYTEXIMAGE1DPROC)LoadMethod(PREFIX "glCopyTexImage1D");
    this->_CopyTexImage2D = (PFNGLCOPYTEXIMAGE2DPROC)LoadMethod(PREFIX "glCopyTexImage2D");
    this->_CopyTexSubImage1D = (PFNGLCOPYTEXSUBIMAGE1DPROC)LoadMethod(PREFIX "glCopyTexSubImage1D");
    this->_CopyTexSubImage2D = (PFNGLCOPYTEXSUBIMAGE2DPROC)LoadMethod(PREFIX "glCopyTexSubImage2D");
    this->_TexSubImage1D = (PFNGLTEXSUBIMAGE1DPROC)LoadMethod(PREFIX "glTexSubImage1D");
    this->_TexSubImage2D = (PFNGLTEXSUBIMAGE2DPROC)LoadMethod(PREFIX "glTexSubImage2D");
    this->_BindTexture = (PFNGLBINDTEXTUREPROC)LoadMethod(PREFIX "glBindTexture");
    this->_DeleteTextures = (PFNGLDELETETEXTURESPROC)LoadMethod(PREFIX "glDeleteTextures");
    this->_GenTextures = (PFNGLGENTEXTURESPROC)LoadMethod(PREFIX "glGenTextures");
    this->_IsTexture = (PFNGLISTEXTUREPROC)LoadMethod(PREFIX "glIsTexture");
    this->_DrawRangeElements = (PFNGLDRAWRANGEELEMENTSPROC)LoadMethod(PREFIX "glDrawRangeElements");
    this->_TexImage3D = (PFNGLTEXIMAGE3DPROC)LoadMethod(PREFIX "glTexImage3D");
    this->_TexSubImage3D = (PFNGLTEXSUBIMAGE3DPROC)LoadMethod(PREFIX "glTexSubImage3D");
    this->_CopyTexSubImage3D = (PFNGLCOPYTEXSUBIMAGE3DPROC)LoadMethod(PREFIX "glCopyTexSubImage3D");
    this->_ActiveTexture = (PFNGLACTIVETEXTUREPROC)LoadMethod(PREFIX "glActiveTexture");
    this->_SampleCoverage = (PFNGLSAMPLECOVERAGEPROC)LoadMethod(PREFIX "glSampleCoverage");
    this->_CompressedTexImage3D = (PFNGLCOMPRESSEDTEXIMAGE3DPROC)LoadMethod(PREFIX "glCompressedTexImage3D");
    this->_CompressedTexImage2D = (PFNGLCOMPRESSEDTEXIMAGE2DPROC)LoadMethod(PREFIX "glCompressedTexImage2D");
    this->_CompressedTexImage1D = (PFNGLCOMPRESSEDTEXIMAGE1DPROC)LoadMethod(PREFIX "glCompressedTexImage1D");
    this->_CompressedTexSubImage3D = (PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC)LoadMethod(PREFIX "glCompressedTexSubImage3D");
    this->_CompressedTexSubImage2D = (PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC)LoadMethod(PREFIX "glCompressedTexSubImage2D");
    this->_CompressedTexSubImage1D = (PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC)LoadMethod(PREFIX "glCompressedTexSubImage1D");
    this->_GetCompressedTexImage = (PFNGLGETCOMPRESSEDTEXIMAGEPROC)LoadMethod(PREFIX "glGetCompressedTexImage");
    this->_BlendFuncSeparate = (PFNGLBLENDFUNCSEPARATEPROC)LoadMethod(PREFIX "glBlendFuncSeparate");
    this->_MultiDrawArrays = (PFNGLMULTIDRAWARRAYSPROC)LoadMethod(PREFIX "glMultiDrawArrays");
    this->_MultiDrawElements = (PFNGLMULTIDRAWELEMENTSPROC)LoadMethod(PREFIX "glMultiDrawElements");
    this->_PointParameterf = (PFNGLPOINTPARAMETERFPROC)LoadMethod(PREFIX "glPointParameterf");
    this->_PointParameterfv = (PFNGLPOINTPARAMETERFVPROC)LoadMethod(PREFIX "glPointParameterfv");
    this->_PointParameteri = (PFNGLPOINTPARAMETERIPROC)LoadMethod(PREFIX "glPointParameteri");
    this->_PointParameteriv = (PFNGLPOINTPARAMETERIVPROC)LoadMethod(PREFIX "glPointParameteriv");
    this->_BlendColor = (PFNGLBLENDCOLORPROC)LoadMethod(PREFIX "glBlendColor");
    this->_BlendEquation = (PFNGLBLENDEQUATIONPROC)LoadMethod(PREFIX "glBlendEquation");
    this->_GenQueries = (PFNGLGENQUERIESPROC)LoadMethod(PREFIX "glGenQueries");
    this->_DeleteQueries = (PFNGLDELETEQUERIESPROC)LoadMethod(PREFIX "glDeleteQueries");
    this->_IsQuery = (PFNGLISQUERYPROC)LoadMethod(PREFIX "glIsQuery");
    this->_BeginQuery = (PFNGLBEGINQUERYPROC)LoadMethod(PREFIX "glBeginQuery");
    this->_EndQuery = (PFNGLENDQUERYPROC)LoadMethod(PREFIX "glEndQuery");
    this->_GetQueryiv = (PFNGLGETQUERYIVPROC)LoadMethod(PREFIX "glGetQueryiv");
    this->_GetQueryObjectiv = (PFNGLGETQUERYOBJECTIVPROC)LoadMethod(PREFIX "glGetQueryObjectiv");
    this->_GetQueryObjectuiv = (PFNGLGETQUERYOBJECTUIVPROC)LoadMethod(PREFIX "glGetQueryObjectuiv");
    this->_BindBuffer = (PFNGLBINDBUFFERPROC)LoadMethod(PREFIX "glBindBuffer");
    this->_DeleteBuffers = (PFNGLDELETEBUFFERSPROC)LoadMethod(PREFIX "glDeleteBuffers");
    this->_GenBuffers = (PFNGLGENBUFFERSPROC)LoadMethod(PREFIX "glGenBuffers");
    this->_IsBuffer = (PFNGLISBUFFERPROC)LoadMethod(PREFIX "glIsBuffer");
    this->_BufferData = (PFNGLBUFFERDATAPROC)LoadMethod(PREFIX "glBufferData");
    this->_BufferSubData = (PFNGLBUFFERSUBDATAPROC)LoadMethod(PREFIX "glBufferSubData");
    this->_GetBufferSubData = (PFNGLGETBUFFERSUBDATAPROC)LoadMethod(PREFIX "glGetBufferSubData");
    this->_MapBuffer = (PFNGLMAPBUFFERPROC)LoadMethod(PREFIX "glMapBuffer");
    this->_UnmapBuffer = (PFNGLUNMAPBUFFERPROC)LoadMethod(PREFIX "glUnmapBuffer");
    this->_GetBufferParameteriv = (PFNGLGETBUFFERPARAMETERIVPROC)LoadMethod(PREFIX "glGetBufferParameteriv");
    this->_GetBufferPointerv = (PFNGLGETBUFFERPOINTERVPROC)LoadMethod(PREFIX "glGetBufferPointerv");
    this->_BlendEquationSeparate = (PFNGLBLENDEQUATIONSEPARATEPROC)LoadMethod(PREFIX "glBlendEquationSeparate");
    this->_DrawBuffers = (PFNGLDRAWBUFFERSPROC)LoadMethod(PREFIX "glDrawBuffers");
    this->_StencilOpSeparate = (PFNGLSTENCILOPSEPARATEPROC)LoadMethod(PREFIX "glStencilOpSeparate");
    this->_StencilFuncSeparate = (PFNGLSTENCILFUNCSEPARATEPROC)LoadMethod(PREFIX "glStencilFuncSeparate");
    this->_StencilMaskSeparate = (PFNGLSTENCILMASKSEPARATEPROC)LoadMethod(PREFIX "glStencilMaskSeparate");
    this->_AttachShader = (PFNGLATTACHSHADERPROC)LoadMethod(PREFIX "glAttachShader");
    this->_BindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)LoadMethod(PREFIX "glBindAttribLocation");
    this->_CompileShader = (PFNGLCOMPILESHADERPROC)LoadMethod(PREFIX "glCompileShader");
    this->_CreateProgram = (PFNGLCREATEPROGRAMPROC)LoadMethod(PREFIX "glCreateProgram");
    this->_CreateShader = (PFNGLCREATESHADERPROC)LoadMethod(PREFIX "glCreateShader");
    this->_DeleteProgram = (PFNGLDELETEPROGRAMPROC)LoadMethod(PREFIX "glDeleteProgram");
    this->_DeleteShader = (PFNGLDELETESHADERPROC)LoadMethod(PREFIX "glDeleteShader");
    this->_DetachShader = (PFNGLDETACHSHADERPROC)LoadMethod(PREFIX "glDetachShader");
    this->_DisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)LoadMethod(PREFIX "glDisableVertexAttribArray");
    this->_EnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)LoadMethod(PREFIX "glEnableVertexAttribArray");
    this->_GetActiveAttrib = (PFNGLGETACTIVEATTRIBPROC)LoadMethod(PREFIX "glGetActiveAttrib");
    this->_GetActiveUniform = (PFNGLGETACTIVEUNIFORMPROC)LoadMethod(PREFIX "glGetActiveUniform");
    this->_GetAttachedShaders = (PFNGLGETATTACHEDSHADERSPROC)LoadMethod(PREFIX "glGetAttachedShaders");
    this->_GetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)LoadMethod(PREFIX "glGetAttribLocation");
    this->_GetProgramiv = (PFNGLGETPROGRAMIVPROC)LoadMethod(PREFIX "glGetProgramiv");
    this->_GetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)LoadMethod(PREFIX "glGetProgramInfoLog");
    this->_GetShaderiv = (PFNGLGETSHADERIVPROC)LoadMethod(PREFIX "glGetShaderiv");
    this->_GetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)LoadMethod(PREFIX "glGetShaderInfoLog");
    this->_GetShaderSource = (PFNGLGETSHADERSOURCEPROC)LoadMethod(PREFIX "glGetShaderSource");
    this->_GetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)LoadMethod(PREFIX "glGetUniformLocation");
    this->_GetUniformfv = (PFNGLGETUNIFORMFVPROC)LoadMethod(PREFIX "glGetUniformfv");
    this->_GetUniformiv = (PFNGLGETUNIFORMIVPROC)LoadMethod(PREFIX "glGetUniformiv");
    this->_GetVertexAttribdv = (PFNGLGETVERTEXATTRIBDVPROC)LoadMethod(PREFIX "glGetVertexAttribdv");
    this->_GetVertexAttribfv = (PFNGLGETVERTEXATTRIBFVPROC)LoadMethod(PREFIX "glGetVertexAttribfv");
    this->_GetVertexAttribiv = (PFNGLGETVERTEXATTRIBIVPROC)LoadMethod(PREFIX "glGetVertexAttribiv");
    this->_GetVertexAttribPointerv = (PFNGLGETVERTEXATTRIBPOINTERVPROC)LoadMethod(PREFIX "glGetVertexAttribPointerv");
    this->_IsProgram = (PFNGLISPROGRAMPROC)LoadMethod(PREFIX "glIsProgram");
    this->_IsShader = (PFNGLISSHADERPROC)LoadMethod(PREFIX "glIsShader");
    this->_LinkProgram = (PFNGLLINKPROGRAMPROC)LoadMethod(PREFIX "glLinkProgram");
    this->_ShaderSource = (PFNGLSHADERSOURCEPROC)LoadMethod(PREFIX "glShaderSource");
    this->_UseProgram = (PFNGLUSEPROGRAMPROC)LoadMethod(PREFIX "glUseProgram");
    this->_Uniform1f = (PFNGLUNIFORM1FPROC)LoadMethod(PREFIX "glUniform1f");
    this->_Uniform2f = (PFNGLUNIFORM2FPROC)LoadMethod(PREFIX "glUniform2f");
    this->_Uniform3f = (PFNGLUNIFORM3FPROC)LoadMethod(PREFIX "glUniform3f");
    this->_Uniform4f = (PFNGLUNIFORM4FPROC)LoadMethod(PREFIX "glUniform4f");
    this->_Uniform1i = (PFNGLUNIFORM1IPROC)LoadMethod(PREFIX "glUniform1i");
    this->_Uniform2i = (PFNGLUNIFORM2IPROC)LoadMethod(PREFIX "glUniform2i");
    this->_Uniform3i = (PFNGLUNIFORM3IPROC)LoadMethod(PREFIX "glUniform3i");
    this->_Uniform4i = (PFNGLUNIFORM4IPROC)LoadMethod(PREFIX "glUniform4i");
    this->_Uniform1fv = (PFNGLUNIFORM1FVPROC)LoadMethod(PREFIX "glUniform1fv");
    this->_Uniform2fv = (PFNGLUNIFORM2FVPROC)LoadMethod(PREFIX "glUniform2fv");
    this->_Uniform3fv = (PFNGLUNIFORM3FVPROC)LoadMethod(PREFIX "glUniform3fv");
    this->_Uniform4fv = (PFNGLUNIFORM4FVPROC)LoadMethod(PREFIX "glUniform4fv");
    this->_Uniform1iv = (PFNGLUNIFORM1IVPROC)LoadMethod(PREFIX "glUniform1iv");
    this->_Uniform2iv = (PFNGLUNIFORM2IVPROC)LoadMethod(PREFIX "glUniform2iv");
    this->_Uniform3iv = (PFNGLUNIFORM3IVPROC)LoadMethod(PREFIX "glUniform3iv");
    this->_Uniform4iv = (PFNGLUNIFORM4IVPROC)LoadMethod(PREFIX "glUniform4iv");
    this->_UniformMatrix2fv = (PFNGLUNIFORMMATRIX2FVPROC)LoadMethod(PREFIX "glUniformMatrix2fv");
    this->_UniformMatrix3fv = (PFNGLUNIFORMMATRIX3FVPROC)LoadMethod(PREFIX "glUniformMatrix3fv");
    this->_UniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)LoadMethod(PREFIX "glUniformMatrix4fv");
    this->_ValidateProgram = (PFNGLVALIDATEPROGRAMPROC)LoadMethod(PREFIX "glValidateProgram");
    this->_VertexAttrib1d = (PFNGLVERTEXATTRIB1DPROC)LoadMethod(PREFIX "glVertexAttrib1d");
    this->_VertexAttrib1dv = (PFNGLVERTEXATTRIB1DVPROC)LoadMethod(PREFIX "glVertexAttrib1dv");
    this->_VertexAttrib1f = (PFNGLVERTEXATTRIB1FPROC)LoadMethod(PREFIX "glVertexAttrib1f");
    this->_VertexAttrib1fv = (PFNGLVERTEXATTRIB1FVPROC)LoadMethod(PREFIX "glVertexAttrib1fv");
    this->_VertexAttrib1s = (PFNGLVERTEXATTRIB1SPROC)LoadMethod(PREFIX "glVertexAttrib1s");
    this->_VertexAttrib1sv = (PFNGLVERTEXATTRIB1SVPROC)LoadMethod(PREFIX "glVertexAttrib1sv");
    this->_VertexAttrib2d = (PFNGLVERTEXATTRIB2DPROC)LoadMethod(PREFIX "glVertexAttrib2d");
    this->_VertexAttrib2dv = (PFNGLVERTEXATTRIB2DVPROC)LoadMethod(PREFIX "glVertexAttrib2dv");
    this->_VertexAttrib2f = (PFNGLVERTEXATTRIB2FPROC)LoadMethod(PREFIX "glVertexAttrib2f");
    this->_VertexAttrib2fv = (PFNGLVERTEXATTRIB2FVPROC)LoadMethod(PREFIX "glVertexAttrib2fv");
    this->_VertexAttrib2s = (PFNGLVERTEXATTRIB2SPROC)LoadMethod(PREFIX "glVertexAttrib2s");
    this->_VertexAttrib2sv = (PFNGLVERTEXATTRIB2SVPROC)LoadMethod(PREFIX "glVertexAttrib2sv");
    this->_VertexAttrib3d = (PFNGLVERTEXATTRIB3DPROC)LoadMethod(PREFIX "glVertexAttrib3d");
    this->_VertexAttrib3dv = (PFNGLVERTEXATTRIB3DVPROC)LoadMethod(PREFIX "glVertexAttrib3dv");
    this->_VertexAttrib3f = (PFNGLVERTEXATTRIB3FPROC)LoadMethod(PREFIX "glVertexAttrib3f");
    this->_VertexAttrib3fv = (PFNGLVERTEXATTRIB3FVPROC)LoadMethod(PREFIX "glVertexAttrib3fv");
    this->_VertexAttrib3s = (PFNGLVERTEXATTRIB3SPROC)LoadMethod(PREFIX "glVertexAttrib3s");
    this->_VertexAttrib3sv = (PFNGLVERTEXATTRIB3SVPROC)LoadMethod(PREFIX "glVertexAttrib3sv");
    this->_VertexAttrib4Nbv = (PFNGLVERTEXATTRIB4NBVPROC)LoadMethod(PREFIX "glVertexAttrib4Nbv");
    this->_VertexAttrib4Niv = (PFNGLVERTEXATTRIB4NIVPROC)LoadMethod(PREFIX "glVertexAttrib4Niv");
    this->_VertexAttrib4Nsv = (PFNGLVERTEXATTRIB4NSVPROC)LoadMethod(PREFIX "glVertexAttrib4Nsv");
    this->_VertexAttrib4Nub = (PFNGLVERTEXATTRIB4NUBPROC)LoadMethod(PREFIX "glVertexAttrib4Nub");
    this->_VertexAttrib4Nubv = (PFNGLVERTEXATTRIB4NUBVPROC)LoadMethod(PREFIX "glVertexAttrib4Nubv");
    this->_VertexAttrib4Nuiv = (PFNGLVERTEXATTRIB4NUIVPROC)LoadMethod(PREFIX "glVertexAttrib4Nuiv");
    this->_VertexAttrib4Nusv = (PFNGLVERTEXATTRIB4NUSVPROC)LoadMethod(PREFIX "glVertexAttrib4Nusv");
    this->_VertexAttrib4bv = (PFNGLVERTEXATTRIB4BVPROC)LoadMethod(PREFIX "glVertexAttrib4bv");
    this->_VertexAttrib4d = (PFNGLVERTEXATTRIB4DPROC)LoadMethod(PREFIX "glVertexAttrib4d");
    this->_VertexAttrib4dv = (PFNGLVERTEXATTRIB4DVPROC)LoadMethod(PREFIX "glVertexAttrib4dv");
    this->_VertexAttrib4f = (PFNGLVERTEXATTRIB4FPROC)LoadMethod(PREFIX "glVertexAttrib4f");
    this->_VertexAttrib4fv = (PFNGLVERTEXATTRIB4FVPROC)LoadMethod(PREFIX "glVertexAttrib4fv");
    this->_VertexAttrib4iv = (PFNGLVERTEXATTRIB4IVPROC)LoadMethod(PREFIX "glVertexAttrib4iv");
    this->_VertexAttrib4s = (PFNGLVERTEXATTRIB4SPROC)LoadMethod(PREFIX "glVertexAttrib4s");
    this->_VertexAttrib4sv = (PFNGLVERTEXATTRIB4SVPROC)LoadMethod(PREFIX "glVertexAttrib4sv");
    this->_VertexAttrib4ubv = (PFNGLVERTEXATTRIB4UBVPROC)LoadMethod(PREFIX "glVertexAttrib4ubv");
    this->_VertexAttrib4uiv = (PFNGLVERTEXATTRIB4UIVPROC)LoadMethod(PREFIX "glVertexAttrib4uiv");
    this->_VertexAttrib4usv = (PFNGLVERTEXATTRIB4USVPROC)LoadMethod(PREFIX "glVertexAttrib4usv");
    this->_VertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)LoadMethod(PREFIX "glVertexAttribPointer");
    this->_UniformMatrix2x3fv = (PFNGLUNIFORMMATRIX2X3FVPROC)LoadMethod(PREFIX "glUniformMatrix2x3fv");
    this->_UniformMatrix3x2fv = (PFNGLUNIFORMMATRIX3X2FVPROC)LoadMethod(PREFIX "glUniformMatrix3x2fv");
    this->_UniformMatrix2x4fv = (PFNGLUNIFORMMATRIX2X4FVPROC)LoadMethod(PREFIX "glUniformMatrix2x4fv");
    this->_UniformMatrix4x2fv = (PFNGLUNIFORMMATRIX4X2FVPROC)LoadMethod(PREFIX "glUniformMatrix4x2fv");
    this->_UniformMatrix3x4fv = (PFNGLUNIFORMMATRIX3X4FVPROC)LoadMethod(PREFIX "glUniformMatrix3x4fv");
    this->_UniformMatrix4x3fv = (PFNGLUNIFORMMATRIX4X3FVPROC)LoadMethod(PREFIX "glUniformMatrix4x3fv");
    this->_ColorMaski = (PFNGLCOLORMASKIPROC)LoadMethod(PREFIX "glColorMaski");
    this->_GetBooleani_v = (PFNGLGETBOOLEANI_VPROC)LoadMethod(PREFIX "glGetBooleani_v");
    this->_GetIntegeri_v = (PFNGLGETINTEGERI_VPROC)LoadMethod(PREFIX "glGetIntegeri_v");
    this->_Enablei = (PFNGLENABLEIPROC)LoadMethod(PREFIX "glEnablei");
    this->_Disablei = (PFNGLDISABLEIPROC)LoadMethod(PREFIX "glDisablei");
    this->_IsEnabledi = (PFNGLISENABLEDIPROC)LoadMethod(PREFIX "glIsEnabledi");
    this->_BeginTransformFeedback = (PFNGLBEGINTRANSFORMFEEDBACKPROC)LoadMethod(PREFIX "glBeginTransformFeedback");
    this->_EndTransformFeedback = (PFNGLENDTRANSFORMFEEDBACKPROC)LoadMethod(PREFIX "glEndTransformFeedback");
    this->_BindBufferRange = (PFNGLBINDBUFFERRANGEPROC)LoadMethod(PREFIX "glBindBufferRange");
    this->_BindBufferBase = (PFNGLBINDBUFFERBASEPROC)LoadMethod(PREFIX "glBindBufferBase");
    this->_TransformFeedbackVaryings = (PFNGLTRANSFORMFEEDBACKVARYINGSPROC)LoadMethod(PREFIX "glTransformFeedbackVaryings");
    this->_GetTransformFeedbackVarying = (PFNGLGETTRANSFORMFEEDBACKVARYINGPROC)LoadMethod(PREFIX "glGetTransformFeedbackVarying");
    this->_ClampColor = (PFNGLCLAMPCOLORPROC)LoadMethod(PREFIX "glClampColor");
    this->_BeginConditionalRender = (PFNGLBEGINCONDITIONALRENDERPROC)LoadMethod(PREFIX "glBeginConditionalRender");
    this->_EndConditionalRender = (PFNGLENDCONDITIONALRENDERPROC)LoadMethod(PREFIX "glEndConditionalRender");
    this->_VertexAttribIPointer = (PFNGLVERTEXATTRIBIPOINTERPROC)LoadMethod(PREFIX "glVertexAttribIPointer");
    this->_GetVertexAttribIiv = (PFNGLGETVERTEXATTRIBIIVPROC)LoadMethod(PREFIX "glGetVertexAttribIiv");
    this->_GetVertexAttribIuiv = (PFNGLGETVERTEXATTRIBIUIVPROC)LoadMethod(PREFIX "glGetVertexAttribIuiv");
    this->_VertexAttribI1i = (PFNGLVERTEXATTRIBI1IPROC)LoadMethod(PREFIX "glVertexAttribI1i");
    this->_VertexAttribI2i = (PFNGLVERTEXATTRIBI2IPROC)LoadMethod(PREFIX "glVertexAttribI2i");
    this->_VertexAttribI3i = (PFNGLVERTEXATTRIBI3IPROC)LoadMethod(PREFIX "glVertexAttribI3i");
    this->_VertexAttribI4i = (PFNGLVERTEXATTRIBI4IPROC)LoadMethod(PREFIX "glVertexAttribI4i");
    this->_VertexAttribI1ui = (PFNGLVERTEXATTRIBI1UIPROC)LoadMethod(PREFIX "glVertexAttribI1ui");
    this->_VertexAttribI2ui = (PFNGLVERTEXATTRIBI2UIPROC)LoadMethod(PREFIX "glVertexAttribI2ui");
    this->_VertexAttribI3ui = (PFNGLVERTEXATTRIBI3UIPROC)LoadMethod(PREFIX "glVertexAttribI3ui");
    this->_VertexAttribI4ui = (PFNGLVERTEXATTRIBI4UIPROC)LoadMethod(PREFIX "glVertexAttribI4ui");
    this->_VertexAttribI1iv = (PFNGLVERTEXATTRIBI1IVPROC)LoadMethod(PREFIX "glVertexAttribI1iv");
    this->_VertexAttribI2iv = (PFNGLVERTEXATTRIBI2IVPROC)LoadMethod(PREFIX "glVertexAttribI2iv");
    this->_VertexAttribI3iv = (PFNGLVERTEXATTRIBI3IVPROC)LoadMethod(PREFIX "glVertexAttribI3iv");
    this->_VertexAttribI4iv = (PFNGLVERTEXATTRIBI4IVPROC)LoadMethod(PREFIX "glVertexAttribI4iv");
    this->_VertexAttribI1uiv = (PFNGLVERTEXATTRIBI1UIVPROC)LoadMethod(PREFIX "glVertexAttribI1uiv");
    this->_VertexAttribI2uiv = (PFNGLVERTEXATTRIBI2UIVPROC)LoadMethod(PREFIX "glVertexAttribI2uiv");
    this->_VertexAttribI3uiv = (PFNGLVERTEXATTRIBI3UIVPROC)LoadMethod(PREFIX "glVertexAttribI3uiv");
    this->_VertexAttribI4uiv = (PFNGLVERTEXATTRIBI4UIVPROC)LoadMethod(PREFIX "glVertexAttribI4uiv");
    this->_VertexAttribI4bv = (PFNGLVERTEXATTRIBI4BVPROC)LoadMethod(PREFIX "glVertexAttribI4bv");
    this->_VertexAttribI4sv = (PFNGLVERTEXATTRIBI4SVPROC)LoadMethod(PREFIX "glVertexAttribI4sv");
    this->_VertexAttribI4ubv = (PFNGLVERTEXATTRIBI4UBVPROC)LoadMethod(PREFIX "glVertexAttribI4ubv");
    this->_VertexAttribI4usv = (PFNGLVERTEXATTRIBI4USVPROC)LoadMethod(PREFIX "glVertexAttribI4usv");
    this->_GetUniformuiv = (PFNGLGETUNIFORMUIVPROC)LoadMethod(PREFIX "glGetUniformuiv");
    this->_BindFragDataLocation = (PFNGLBINDFRAGDATALOCATIONPROC)LoadMethod(PREFIX "glBindFragDataLocation");
    this->_GetFragDataLocation = (PFNGLGETFRAGDATALOCATIONPROC)LoadMethod(PREFIX "glGetFragDataLocation");
    this->_Uniform1ui = (PFNGLUNIFORM1UIPROC)LoadMethod(PREFIX "glUniform1ui");
    this->_Uniform2ui = (PFNGLUNIFORM2UIPROC)LoadMethod(PREFIX "glUniform2ui");
    this->_Uniform3ui = (PFNGLUNIFORM3UIPROC)LoadMethod(PREFIX "glUniform3ui");
    this->_Uniform4ui = (PFNGLUNIFORM4UIPROC)LoadMethod(PREFIX "glUniform4ui");
    this->_Uniform1uiv = (PFNGLUNIFORM1UIVPROC)LoadMethod(PREFIX "glUniform1uiv");
    this->_Uniform2uiv = (PFNGLUNIFORM2UIVPROC)LoadMethod(PREFIX "glUniform2uiv");
    this->_Uniform3uiv = (PFNGLUNIFORM3UIVPROC)LoadMethod(PREFIX "glUniform3uiv");
    this->_Uniform4uiv = (PFNGLUNIFORM4UIVPROC)LoadMethod(PREFIX "glUniform4uiv");
    this->_TexParameterIiv = (PFNGLTEXPARAMETERIIVPROC)LoadMethod(PREFIX "glTexParameterIiv");
    this->_TexParameterIuiv = (PFNGLTEXPARAMETERIUIVPROC)LoadMethod(PREFIX "glTexParameterIuiv");
    this->_GetTexParameterIiv = (PFNGLGETTEXPARAMETERIIVPROC)LoadMethod(PREFIX "glGetTexParameterIiv");
    this->_GetTexParameterIuiv = (PFNGLGETTEXPARAMETERIUIVPROC)LoadMethod(PREFIX "glGetTexParameterIuiv");
    this->_ClearBufferiv = (PFNGLCLEARBUFFERIVPROC)LoadMethod(PREFIX "glClearBufferiv");
    this->_ClearBufferuiv = (PFNGLCLEARBUFFERUIVPROC)LoadMethod(PREFIX "glClearBufferuiv");
    this->_ClearBufferfv = (PFNGLCLEARBUFFERFVPROC)LoadMethod(PREFIX "glClearBufferfv");
    this->_ClearBufferfi = (PFNGLCLEARBUFFERFIPROC)LoadMethod(PREFIX "glClearBufferfi");
    this->_GetStringi = (PFNGLGETSTRINGIPROC)LoadMethod(PREFIX "glGetStringi");
    this->_IsRenderbuffer = (PFNGLISRENDERBUFFERPROC)LoadMethod(PREFIX "glIsRenderbuffer");
    this->_BindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC)LoadMethod(PREFIX "glBindRenderbuffer");
    this->_DeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC)LoadMethod(PREFIX "glDeleteRenderbuffers");
    this->_GenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC)LoadMethod(PREFIX "glGenRenderbuffers");
    this->_RenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC)LoadMethod(PREFIX "glRenderbufferStorage");
    this->_GetRenderbufferParameteriv = (PFNGLGETRENDERBUFFERPARAMETERIVPROC)LoadMethod(PREFIX "glGetRenderbufferParameteriv");
    this->_IsFramebuffer = (PFNGLISFRAMEBUFFERPROC)LoadMethod(PREFIX "glIsFramebuffer");
    this->_BindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)LoadMethod(PREFIX "glBindFramebuffer");
    this->_DeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC)LoadMethod(PREFIX "glDeleteFramebuffers");
    this->_GenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)LoadMethod(PREFIX "glGenFramebuffers");
    this->_CheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)LoadMethod(PREFIX "glCheckFramebufferStatus");
    this->_FramebufferTexture1D = (PFNGLFRAMEBUFFERTEXTURE1DPROC)LoadMethod(PREFIX "glFramebufferTexture1D");
    this->_FramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)LoadMethod(PREFIX "glFramebufferTexture2D");
    this->_FramebufferTexture3D = (PFNGLFRAMEBUFFERTEXTURE3DPROC)LoadMethod(PREFIX "glFramebufferTexture3D");
    this->_FramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)LoadMethod(PREFIX "glFramebufferRenderbuffer");
    this->_GetFramebufferAttachmentParameteriv = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC)LoadMethod(PREFIX "glGetFramebufferAttachmentParameteriv");
    this->_GenerateMipmap = (PFNGLGENERATEMIPMAPPROC)LoadMethod(PREFIX "glGenerateMipmap");
    this->_BlitFramebuffer = (PFNGLBLITFRAMEBUFFERPROC)LoadMethod(PREFIX "glBlitFramebuffer");
    this->_RenderbufferStorageMultisample = (PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC)LoadMethod(PREFIX "glRenderbufferStorageMultisample");
    this->_FramebufferTextureLayer = (PFNGLFRAMEBUFFERTEXTURELAYERPROC)LoadMethod(PREFIX "glFramebufferTextureLayer");
    this->_MapBufferRange = (PFNGLMAPBUFFERRANGEPROC)LoadMethod(PREFIX "glMapBufferRange");
    this->_FlushMappedBufferRange = (PFNGLFLUSHMAPPEDBUFFERRANGEPROC)LoadMethod(PREFIX "glFlushMappedBufferRange");
    this->_BindVertexArray = (PFNGLBINDVERTEXARRAYPROC)LoadMethod(PREFIX "glBindVertexArray");
    this->_DeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)LoadMethod(PREFIX "glDeleteVertexArrays");
    this->_GenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)LoadMethod(PREFIX "glGenVertexArrays");
    this->_IsVertexArray = (PFNGLISVERTEXARRAYPROC)LoadMethod(PREFIX "glIsVertexArray");
    this->_DrawArraysInstanced = (PFNGLDRAWARRAYSINSTANCEDPROC)LoadMethod(PREFIX "glDrawArraysInstanced");
    this->_DrawElementsInstanced = (PFNGLDRAWELEMENTSINSTANCEDPROC)LoadMethod(PREFIX "glDrawElementsInstanced");
    this->_TexBuffer = (PFNGLTEXBUFFERPROC)LoadMethod(PREFIX "glTexBuffer");
    this->_PrimitiveRestartIndex = (PFNGLPRIMITIVERESTARTINDEXPROC)LoadMethod(PREFIX "glPrimitiveRestartIndex");
    this->_CopyBufferSubData = (PFNGLCOPYBUFFERSUBDATAPROC)LoadMethod(PREFIX "glCopyBufferSubData");
    this->_GetUniformIndices = (PFNGLGETUNIFORMINDICESPROC)LoadMethod(PREFIX "glGetUniformIndices");
    this->_GetActiveUniformsiv = (PFNGLGETACTIVEUNIFORMSIVPROC)LoadMethod(PREFIX "glGetActiveUniformsiv");
    this->_GetActiveUniformName = (PFNGLGETACTIVEUNIFORMNAMEPROC)LoadMethod(PREFIX "glGetActiveUniformName");
    this->_GetUniformBlockIndex = (PFNGLGETUNIFORMBLOCKINDEXPROC)LoadMethod(PREFIX "glGetUniformBlockIndex");
    this->_GetActiveUniformBlockiv = (PFNGLGETACTIVEUNIFORMBLOCKIVPROC)LoadMethod(PREFIX "glGetActiveUniformBlockiv");
    this->_GetActiveUniformBlockName = (PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC)LoadMethod(PREFIX "glGetActiveUniformBlockName");
    this->_UniformBlockBinding = (PFNGLUNIFORMBLOCKBINDINGPROC)LoadMethod(PREFIX "glUniformBlockBinding");
    this->_DrawElementsBaseVertex = (PFNGLDRAWELEMENTSBASEVERTEXPROC)LoadMethod(PREFIX "glDrawElementsBaseVertex");
    this->_DrawRangeElementsBaseVertex = (PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC)LoadMethod(PREFIX "glDrawRangeElementsBaseVertex");
    this->_DrawElementsInstancedBaseVertex = (PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC)LoadMethod(PREFIX "glDrawElementsInstancedBaseVertex");
    this->_MultiDrawElementsBaseVertex = (PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC)LoadMethod(PREFIX "glMultiDrawElementsBaseVertex");
    this->_ProvokingVertex = (PFNGLPROVOKINGVERTEXPROC)LoadMethod(PREFIX "glProvokingVertex");
    this->_FenceSync = (PFNGLFENCESYNCPROC)LoadMethod(PREFIX "glFenceSync");
    this->_IsSync = (PFNGLISSYNCPROC)LoadMethod(PREFIX "glIsSync");
    this->_DeleteSync = (PFNGLDELETESYNCPROC)LoadMethod(PREFIX "glDeleteSync");
    this->_ClientWaitSync = (PFNGLCLIENTWAITSYNCPROC)LoadMethod(PREFIX "glClientWaitSync");
    this->_WaitSync = (PFNGLWAITSYNCPROC)LoadMethod(PREFIX "glWaitSync");
    this->_GetInteger64v = (PFNGLGETINTEGER64VPROC)LoadMethod(PREFIX "glGetInteger64v");
    this->_GetSynciv = (PFNGLGETSYNCIVPROC)LoadMethod(PREFIX "glGetSynciv");
    this->_GetInteger64i_v = (PFNGLGETINTEGER64I_VPROC)LoadMethod(PREFIX "glGetInteger64i_v");
    this->_GetBufferParameteri64v = (PFNGLGETBUFFERPARAMETERI64VPROC)LoadMethod(PREFIX "glGetBufferParameteri64v");
    this->_FramebufferTexture = (PFNGLFRAMEBUFFERTEXTUREPROC)LoadMethod(PREFIX "glFramebufferTexture");
    this->_TexImage2DMultisample = (PFNGLTEXIMAGE2DMULTISAMPLEPROC)LoadMethod(PREFIX "glTexImage2DMultisample");
    this->_TexImage3DMultisample = (PFNGLTEXIMAGE3DMULTISAMPLEPROC)LoadMethod(PREFIX "glTexImage3DMultisample");
    this->_GetMultisamplefv = (PFNGLGETMULTISAMPLEFVPROC)LoadMethod(PREFIX "glGetMultisamplefv");
    this->_SampleMaski = (PFNGLSAMPLEMASKIPROC)LoadMethod(PREFIX "glSampleMaski");
    this->_BindFragDataLocationIndexed = (PFNGLBINDFRAGDATALOCATIONINDEXEDPROC)LoadMethod(PREFIX "glBindFragDataLocationIndexed");
    this->_GetFragDataIndex = (PFNGLGETFRAGDATAINDEXPROC)LoadMethod(PREFIX "glGetFragDataIndex");
    this->_GenSamplers = (PFNGLGENSAMPLERSPROC)LoadMethod(PREFIX "glGenSamplers");
    this->_DeleteSamplers = (PFNGLDELETESAMPLERSPROC)LoadMethod(PREFIX "glDeleteSamplers");
    this->_IsSampler = (PFNGLISSAMPLERPROC)LoadMethod(PREFIX "glIsSampler");
    this->_BindSampler = (PFNGLBINDSAMPLERPROC)LoadMethod(PREFIX "glBindSampler");
    this->_SamplerParameteri = (PFNGLSAMPLERPARAMETERIPROC)LoadMethod(PREFIX "glSamplerParameteri");
    this->_SamplerParameteriv = (PFNGLSAMPLERPARAMETERIVPROC)LoadMethod(PREFIX "glSamplerParameteriv");
    this->_SamplerParameterf = (PFNGLSAMPLERPARAMETERFPROC)LoadMethod(PREFIX "glSamplerParameterf");
    this->_SamplerParameterfv = (PFNGLSAMPLERPARAMETERFVPROC)LoadMethod(PREFIX "glSamplerParameterfv");
    this->_SamplerParameterIiv = (PFNGLSAMPLERPARAMETERIIVPROC)LoadMethod(PREFIX "glSamplerParameterIiv");
    this->_SamplerParameterIuiv = (PFNGLSAMPLERPARAMETERIUIVPROC)LoadMethod(PREFIX "glSamplerParameterIuiv");
    this->_GetSamplerParameteriv = (PFNGLGETSAMPLERPARAMETERIVPROC)LoadMethod(PREFIX "glGetSamplerParameteriv");
    this->_GetSamplerParameterIiv = (PFNGLGETSAMPLERPARAMETERIIVPROC)LoadMethod(PREFIX "glGetSamplerParameterIiv");
    this->_GetSamplerParameterfv = (PFNGLGETSAMPLERPARAMETERFVPROC)LoadMethod(PREFIX "glGetSamplerParameterfv");
    this->_GetSamplerParameterIuiv = (PFNGLGETSAMPLERPARAMETERIUIVPROC)LoadMethod(PREFIX "glGetSamplerParameterIuiv");
    this->_QueryCounter = (PFNGLQUERYCOUNTERPROC)LoadMethod(PREFIX "glQueryCounter");
    this->_GetQueryObjecti64v = (PFNGLGETQUERYOBJECTI64VPROC)LoadMethod(PREFIX "glGetQueryObjecti64v");
    this->_GetQueryObjectui64v = (PFNGLGETQUERYOBJECTUI64VPROC)LoadMethod(PREFIX "glGetQueryObjectui64v");
    this->_VertexAttribDivisor = (PFNGLVERTEXATTRIBDIVISORPROC)LoadMethod(PREFIX "glVertexAttribDivisor");
    this->_VertexAttribP1ui = (PFNGLVERTEXATTRIBP1UIPROC)LoadMethod(PREFIX "glVertexAttribP1ui");
    this->_VertexAttribP1uiv = (PFNGLVERTEXATTRIBP1UIVPROC)LoadMethod(PREFIX "glVertexAttribP1uiv");
    this->_VertexAttribP2ui = (PFNGLVERTEXATTRIBP2UIPROC)LoadMethod(PREFIX "glVertexAttribP2ui");
    this->_VertexAttribP2uiv = (PFNGLVERTEXATTRIBP2UIVPROC)LoadMethod(PREFIX "glVertexAttribP2uiv");
    this->_VertexAttribP3ui = (PFNGLVERTEXATTRIBP3UIPROC)LoadMethod(PREFIX "glVertexAttribP3ui");
    this->_VertexAttribP3uiv = (PFNGLVERTEXATTRIBP3UIVPROC)LoadMethod(PREFIX "glVertexAttribP3uiv");
    this->_VertexAttribP4ui = (PFNGLVERTEXATTRIBP4UIPROC)LoadMethod(PREFIX "glVertexAttribP4ui");
    this->_VertexAttribP4uiv = (PFNGLVERTEXATTRIBP4UIVPROC)LoadMethod(PREFIX "glVertexAttribP4uiv");
    this->_MinSampleShading = (PFNGLMINSAMPLESHADINGPROC)LoadMethod(PREFIX "glMinSampleShading");
    this->_BlendEquationi = (PFNGLBLENDEQUATIONIPROC)LoadMethod(PREFIX "glBlendEquationi");
    this->_BlendEquationSeparatei = (PFNGLBLENDEQUATIONSEPARATEIPROC)LoadMethod(PREFIX "glBlendEquationSeparatei");
    this->_BlendFunci = (PFNGLBLENDFUNCIPROC)LoadMethod(PREFIX "glBlendFunci");
    this->_BlendFuncSeparatei = (PFNGLBLENDFUNCSEPARATEIPROC)LoadMethod(PREFIX "glBlendFuncSeparatei");
    this->_DrawArraysIndirect = (PFNGLDRAWARRAYSINDIRECTPROC)LoadMethod(PREFIX "glDrawArraysIndirect");
    this->_DrawElementsIndirect = (PFNGLDRAWELEMENTSINDIRECTPROC)LoadMethod(PREFIX "glDrawElementsIndirect");
    this->_Uniform1d = (PFNGLUNIFORM1DPROC)LoadMethod(PREFIX "glUniform1d");
    this->_Uniform2d = (PFNGLUNIFORM2DPROC)LoadMethod(PREFIX "glUniform2d");
    this->_Uniform3d = (PFNGLUNIFORM3DPROC)LoadMethod(PREFIX "glUniform3d");
    this->_Uniform4d = (PFNGLUNIFORM4DPROC)LoadMethod(PREFIX "glUniform4d");
    this->_Uniform1dv = (PFNGLUNIFORM1DVPROC)LoadMethod(PREFIX "glUniform1dv");
    this->_Uniform2dv = (PFNGLUNIFORM2DVPROC)LoadMethod(PREFIX "glUniform2dv");
    this->_Uniform3dv = (PFNGLUNIFORM3DVPROC)LoadMethod(PREFIX "glUniform3dv");
    this->_Uniform4dv = (PFNGLUNIFORM4DVPROC)LoadMethod(PREFIX "glUniform4dv");
    this->_UniformMatrix2dv = (PFNGLUNIFORMMATRIX2DVPROC)LoadMethod(PREFIX "glUniformMatrix2dv");
    this->_UniformMatrix3dv = (PFNGLUNIFORMMATRIX3DVPROC)LoadMethod(PREFIX "glUniformMatrix3dv");
    this->_UniformMatrix4dv = (PFNGLUNIFORMMATRIX4DVPROC)LoadMethod(PREFIX "glUniformMatrix4dv");
    this->_UniformMatrix2x3dv = (PFNGLUNIFORMMATRIX2X3DVPROC)LoadMethod(PREFIX "glUniformMatrix2x3dv");
    this->_UniformMatrix2x4dv = (PFNGLUNIFORMMATRIX2X4DVPROC)LoadMethod(PREFIX "glUniformMatrix2x4dv");
    this->_UniformMatrix3x2dv = (PFNGLUNIFORMMATRIX3X2DVPROC)LoadMethod(PREFIX "glUniformMatrix3x2dv");
    this->_UniformMatrix3x4dv = (PFNGLUNIFORMMATRIX3X4DVPROC)LoadMethod(PREFIX "glUniformMatrix3x4dv");
    this->_UniformMatrix4x2dv = (PFNGLUNIFORMMATRIX4X2DVPROC)LoadMethod(PREFIX "glUniformMatrix4x2dv");
    this->_UniformMatrix4x3dv = (PFNGLUNIFORMMATRIX4X3DVPROC)LoadMethod(PREFIX "glUniformMatrix4x3dv");
    this->_GetUniformdv = (PFNGLGETUNIFORMDVPROC)LoadMethod(PREFIX "glGetUniformdv");
    this->_GetSubroutineUniformLocation = (PFNGLGETSUBROUTINEUNIFORMLOCATIONPROC)LoadMethod(PREFIX "glGetSubroutineUniformLocation");
    this->_GetSubroutineIndex = (PFNGLGETSUBROUTINEINDEXPROC)LoadMethod(PREFIX "glGetSubroutineIndex");
    this->_GetActiveSubroutineUniformiv = (PFNGLGETACTIVESUBROUTINEUNIFORMIVPROC)LoadMethod(PREFIX "glGetActiveSubroutineUniformiv");
    this->_GetActiveSubroutineUniformName = (PFNGLGETACTIVESUBROUTINEUNIFORMNAMEPROC)LoadMethod(PREFIX "glGetActiveSubroutineUniformName");
    this->_GetActiveSubroutineName = (PFNGLGETACTIVESUBROUTINENAMEPROC)LoadMethod(PREFIX "glGetActiveSubroutineName");
    this->_UniformSubroutinesuiv = (PFNGLUNIFORMSUBROUTINESUIVPROC)LoadMethod(PREFIX "glUniformSubroutinesuiv");
    this->_GetUniformSubroutineuiv = (PFNGLGETUNIFORMSUBROUTINEUIVPROC)LoadMethod(PREFIX "glGetUniformSubroutineuiv");
    this->_GetProgramStageiv = (PFNGLGETPROGRAMSTAGEIVPROC)LoadMethod(PREFIX "glGetProgramStageiv");
    this->_PatchParameteri = (PFNGLPATCHPARAMETERIPROC)LoadMethod(PREFIX "glPatchParameteri");
    this->_PatchParameterfv = (PFNGLPATCHPARAMETERFVPROC)LoadMethod(PREFIX "glPatchParameterfv");
    this->_BindTransformFeedback = (PFNGLBINDTRANSFORMFEEDBACKPROC)LoadMethod(PREFIX "glBindTransformFeedback");
    this->_DeleteTransformFeedbacks = (PFNGLDELETETRANSFORMFEEDBACKSPROC)LoadMethod(PREFIX "glDeleteTransformFeedbacks");
    this->_GenTransformFeedbacks = (PFNGLGENTRANSFORMFEEDBACKSPROC)LoadMethod(PREFIX "glGenTransformFeedbacks");
    this->_IsTransformFeedback = (PFNGLISTRANSFORMFEEDBACKPROC)LoadMethod(PREFIX "glIsTransformFeedback");
    this->_PauseTransformFeedback = (PFNGLPAUSETRANSFORMFEEDBACKPROC)LoadMethod(PREFIX "glPauseTransformFeedback");
    this->_ResumeTransformFeedback = (PFNGLRESUMETRANSFORMFEEDBACKPROC)LoadMethod(PREFIX "glResumeTransformFeedback");
    this->_DrawTransformFeedback = (PFNGLDRAWTRANSFORMFEEDBACKPROC)LoadMethod(PREFIX "glDrawTransformFeedback");
    this->_DrawTransformFeedbackStream = (PFNGLDRAWTRANSFORMFEEDBACKSTREAMPROC)LoadMethod(PREFIX "glDrawTransformFeedbackStream");
    this->_BeginQueryIndexed = (PFNGLBEGINQUERYINDEXEDPROC)LoadMethod(PREFIX "glBeginQueryIndexed");
    this->_EndQueryIndexed = (PFNGLENDQUERYINDEXEDPROC)LoadMethod(PREFIX "glEndQueryIndexed");
    this->_GetQueryIndexediv = (PFNGLGETQUERYINDEXEDIVPROC)LoadMethod(PREFIX "glGetQueryIndexediv");
    this->_ReleaseShaderCompiler = (PFNGLRELEASESHADERCOMPILERPROC)LoadMethod(PREFIX "glReleaseShaderCompiler");
    this->_ShaderBinary = (PFNGLSHADERBINARYPROC)LoadMethod(PREFIX "glShaderBinary");
    this->_GetShaderPrecisionFormat = (PFNGLGETSHADERPRECISIONFORMATPROC)LoadMethod(PREFIX "glGetShaderPrecisionFormat");
    this->_DepthRangef = (PFNGLDEPTHRANGEFPROC)LoadMethod(PREFIX "glDepthRangef");
    this->_ClearDepthf = (PFNGLCLEARDEPTHFPROC)LoadMethod(PREFIX "glClearDepthf");
    this->_GetProgramBinary = (PFNGLGETPROGRAMBINARYPROC)LoadMethod(PREFIX "glGetProgramBinary");
    this->_ProgramBinary = (PFNGLPROGRAMBINARYPROC)LoadMethod(PREFIX "glProgramBinary");
    this->_ProgramParameteri = (PFNGLPROGRAMPARAMETERIPROC)LoadMethod(PREFIX "glProgramParameteri");
    this->_UseProgramStages = (PFNGLUSEPROGRAMSTAGESPROC)LoadMethod(PREFIX "glUseProgramStages");
    this->_ActiveShaderProgram = (PFNGLACTIVESHADERPROGRAMPROC)LoadMethod(PREFIX "glActiveShaderProgram");
    this->_CreateShaderProgramv = (PFNGLCREATESHADERPROGRAMVPROC)LoadMethod(PREFIX "glCreateShaderProgramv");
    this->_BindProgramPipeline = (PFNGLBINDPROGRAMPIPELINEPROC)LoadMethod(PREFIX "glBindProgramPipeline");
    this->_DeleteProgramPipelines = (PFNGLDELETEPROGRAMPIPELINESPROC)LoadMethod(PREFIX "glDeleteProgramPipelines");
    this->_GenProgramPipelines = (PFNGLGENPROGRAMPIPELINESPROC)LoadMethod(PREFIX "glGenProgramPipelines");
    this->_IsProgramPipeline = (PFNGLISPROGRAMPIPELINEPROC)LoadMethod(PREFIX "glIsProgramPipeline");
    this->_GetProgramPipelineiv = (PFNGLGETPROGRAMPIPELINEIVPROC)LoadMethod(PREFIX "glGetProgramPipelineiv");
    this->_ProgramUniform1i = (PFNGLPROGRAMUNIFORM1IPROC)LoadMethod(PREFIX "glProgramUniform1i");
    this->_ProgramUniform1iv = (PFNGLPROGRAMUNIFORM1IVPROC)LoadMethod(PREFIX "glProgramUniform1iv");
    this->_ProgramUniform1f = (PFNGLPROGRAMUNIFORM1FPROC)LoadMethod(PREFIX "glProgramUniform1f");
    this->_ProgramUniform1fv = (PFNGLPROGRAMUNIFORM1FVPROC)LoadMethod(PREFIX "glProgramUniform1fv");
    this->_ProgramUniform1d = (PFNGLPROGRAMUNIFORM1DPROC)LoadMethod(PREFIX "glProgramUniform1d");
    this->_ProgramUniform1dv = (PFNGLPROGRAMUNIFORM1DVPROC)LoadMethod(PREFIX "glProgramUniform1dv");
    this->_ProgramUniform1ui = (PFNGLPROGRAMUNIFORM1UIPROC)LoadMethod(PREFIX "glProgramUniform1ui");
    this->_ProgramUniform1uiv = (PFNGLPROGRAMUNIFORM1UIVPROC)LoadMethod(PREFIX "glProgramUniform1uiv");
    this->_ProgramUniform2i = (PFNGLPROGRAMUNIFORM2IPROC)LoadMethod(PREFIX "glProgramUniform2i");
    this->_ProgramUniform2iv = (PFNGLPROGRAMUNIFORM2IVPROC)LoadMethod(PREFIX "glProgramUniform2iv");
    this->_ProgramUniform2f = (PFNGLPROGRAMUNIFORM2FPROC)LoadMethod(PREFIX "glProgramUniform2f");
    this->_ProgramUniform2fv = (PFNGLPROGRAMUNIFORM2FVPROC)LoadMethod(PREFIX "glProgramUniform2fv");
    this->_ProgramUniform2d = (PFNGLPROGRAMUNIFORM2DPROC)LoadMethod(PREFIX "glProgramUniform2d");
    this->_ProgramUniform2dv = (PFNGLPROGRAMUNIFORM2DVPROC)LoadMethod(PREFIX "glProgramUniform2dv");
    this->_ProgramUniform2ui = (PFNGLPROGRAMUNIFORM2UIPROC)LoadMethod(PREFIX "glProgramUniform2ui");
    this->_ProgramUniform2uiv = (PFNGLPROGRAMUNIFORM2UIVPROC)LoadMethod(PREFIX "glProgramUniform2uiv");
    this->_ProgramUniform3i = (PFNGLPROGRAMUNIFORM3IPROC)LoadMethod(PREFIX "glProgramUniform3i");
    this->_ProgramUniform3iv = (PFNGLPROGRAMUNIFORM3IVPROC)LoadMethod(PREFIX "glProgramUniform3iv");
    this->_ProgramUniform3f = (PFNGLPROGRAMUNIFORM3FPROC)LoadMethod(PREFIX "glProgramUniform3f");
    this->_ProgramUniform3fv = (PFNGLPROGRAMUNIFORM3FVPROC)LoadMethod(PREFIX "glProgramUniform3fv");
    this->_ProgramUniform3d = (PFNGLPROGRAMUNIFORM3DPROC)LoadMethod(PREFIX "glProgramUniform3d");
    this->_ProgramUniform3dv = (PFNGLPROGRAMUNIFORM3DVPROC)LoadMethod(PREFIX "glProgramUniform3dv");
    this->_ProgramUniform3ui = (PFNGLPROGRAMUNIFORM3UIPROC)LoadMethod(PREFIX "glProgramUniform3ui");
    this->_ProgramUniform3uiv = (PFNGLPROGRAMUNIFORM3UIVPROC)LoadMethod(PREFIX "glProgramUniform3uiv");
    this->_ProgramUniform4i = (PFNGLPROGRAMUNIFORM4IPROC)LoadMethod(PREFIX "glProgramUniform4i");
    this->_ProgramUniform4iv = (PFNGLPROGRAMUNIFORM4IVPROC)LoadMethod(PREFIX "glProgramUniform4iv");
    this->_ProgramUniform4f = (PFNGLPROGRAMUNIFORM4FPROC)LoadMethod(PREFIX "glProgramUniform4f");
    this->_ProgramUniform4fv = (PFNGLPROGRAMUNIFORM4FVPROC)LoadMethod(PREFIX "glProgramUniform4fv");
    this->_ProgramUniform4d = (PFNGLPROGRAMUNIFORM4DPROC)LoadMethod(PREFIX "glProgramUniform4d");
    this->_ProgramUniform4dv = (PFNGLPROGRAMUNIFORM4DVPROC)LoadMethod(PREFIX "glProgramUniform4dv");
    this->_ProgramUniform4ui = (PFNGLPROGRAMUNIFORM4UIPROC)LoadMethod(PREFIX "glProgramUniform4ui");
    this->_ProgramUniform4uiv = (PFNGLPROGRAMUNIFORM4UIVPROC)LoadMethod(PREFIX "glProgramUniform4uiv");
    this->_ProgramUniformMatrix2fv = (PFNGLPROGRAMUNIFORMMATRIX2FVPROC)LoadMethod(PREFIX "glProgramUniformMatrix2fv");
    this->_ProgramUniformMatrix3fv = (PFNGLPROGRAMUNIFORMMATRIX3FVPROC)LoadMethod(PREFIX "glProgramUniformMatrix3fv");
    this->_ProgramUniformMatrix4fv = (PFNGLPROGRAMUNIFORMMATRIX4FVPROC)LoadMethod(PREFIX "glProgramUniformMatrix4fv");
    this->_ProgramUniformMatrix2dv = (PFNGLPROGRAMUNIFORMMATRIX2DVPROC)LoadMethod(PREFIX "glProgramUniformMatrix2dv");
    this->_ProgramUniformMatrix3dv = (PFNGLPROGRAMUNIFORMMATRIX3DVPROC)LoadMethod(PREFIX "glProgramUniformMatrix3dv");
    this->_ProgramUniformMatrix4dv = (PFNGLPROGRAMUNIFORMMATRIX4DVPROC)LoadMethod(PREFIX "glProgramUniformMatrix4dv");
    this->_ProgramUniformMatrix2x3fv = (PFNGLPROGRAMUNIFORMMATRIX2X3FVPROC)LoadMethod(PREFIX "glProgramUniformMatrix2x3fv");
    this->_ProgramUniformMatrix3x2fv = (PFNGLPROGRAMUNIFORMMATRIX3X2FVPROC)LoadMethod(PREFIX "glProgramUniformMatrix3x2fv");
    this->_ProgramUniformMatrix2x4fv = (PFNGLPROGRAMUNIFORMMATRIX2X4FVPROC)LoadMethod(PREFIX "glProgramUniformMatrix2x4fv");
    this->_ProgramUniformMatrix4x2fv = (PFNGLPROGRAMUNIFORMMATRIX4X2FVPROC)LoadMethod(PREFIX "glProgramUniformMatrix4x2fv");
    this->_ProgramUniformMatrix3x4fv = (PFNGLPROGRAMUNIFORMMATRIX3X4FVPROC)LoadMethod(PREFIX "glProgramUniformMatrix3x4fv");
    this->_ProgramUniformMatrix4x3fv = (PFNGLPROGRAMUNIFORMMATRIX4X3FVPROC)LoadMethod(PREFIX "glProgramUniformMatrix4x3fv");
    this->_ProgramUniformMatrix2x3dv = (PFNGLPROGRAMUNIFORMMATRIX2X3DVPROC)LoadMethod(PREFIX "glProgramUniformMatrix2x3dv");
    this->_ProgramUniformMatrix3x2dv = (PFNGLPROGRAMUNIFORMMATRIX3X2DVPROC)LoadMethod(PREFIX "glProgramUniformMatrix3x2dv");
    this->_ProgramUniformMatrix2x4dv = (PFNGLPROGRAMUNIFORMMATRIX2X4DVPROC)LoadMethod(PREFIX "glProgramUniformMatrix2x4dv");
    this->_ProgramUniformMatrix4x2dv = (PFNGLPROGRAMUNIFORMMATRIX4X2DVPROC)LoadMethod(PREFIX "glProgramUniformMatrix4x2dv");
    this->_ProgramUniformMatrix3x4dv = (PFNGLPROGRAMUNIFORMMATRIX3X4DVPROC)LoadMethod(PREFIX "glProgramUniformMatrix3x4dv");
    this->_ProgramUniformMatrix4x3dv = (PFNGLPROGRAMUNIFORMMATRIX4X3DVPROC)LoadMethod(PREFIX "glProgramUniformMatrix4x3dv");
    this->_ValidateProgramPipeline = (PFNGLVALIDATEPROGRAMPIPELINEPROC)LoadMethod(PREFIX "glValidateProgramPipeline");
    this->_GetProgramPipelineInfoLog = (PFNGLGETPROGRAMPIPELINEINFOLOGPROC)LoadMethod(PREFIX "glGetProgramPipelineInfoLog");
    this->_VertexAttribL1d = (PFNGLVERTEXATTRIBL1DPROC)LoadMethod(PREFIX "glVertexAttribL1d");
    this->_VertexAttribL2d = (PFNGLVERTEXATTRIBL2DPROC)LoadMethod(PREFIX "glVertexAttribL2d");
    this->_VertexAttribL3d = (PFNGLVERTEXATTRIBL3DPROC)LoadMethod(PREFIX "glVertexAttribL3d");
    this->_VertexAttribL4d = (PFNGLVERTEXATTRIBL4DPROC)LoadMethod(PREFIX "glVertexAttribL4d");
    this->_VertexAttribL1dv = (PFNGLVERTEXATTRIBL1DVPROC)LoadMethod(PREFIX "glVertexAttribL1dv");
    this->_VertexAttribL2dv = (PFNGLVERTEXATTRIBL2DVPROC)LoadMethod(PREFIX "glVertexAttribL2dv");
    this->_VertexAttribL3dv = (PFNGLVERTEXATTRIBL3DVPROC)LoadMethod(PREFIX "glVertexAttribL3dv");
    this->_VertexAttribL4dv = (PFNGLVERTEXATTRIBL4DVPROC)LoadMethod(PREFIX "glVertexAttribL4dv");
    this->_VertexAttribLPointer = (PFNGLVERTEXATTRIBLPOINTERPROC)LoadMethod(PREFIX "glVertexAttribLPointer");
    this->_GetVertexAttribLdv = (PFNGLGETVERTEXATTRIBLDVPROC)LoadMethod(PREFIX "glGetVertexAttribLdv");
    this->_ViewportArrayv = (PFNGLVIEWPORTARRAYVPROC)LoadMethod(PREFIX "glViewportArrayv");
    this->_ViewportIndexedf = (PFNGLVIEWPORTINDEXEDFPROC)LoadMethod(PREFIX "glViewportIndexedf");
    this->_ViewportIndexedfv = (PFNGLVIEWPORTINDEXEDFVPROC)LoadMethod(PREFIX "glViewportIndexedfv");
    this->_ScissorArrayv = (PFNGLSCISSORARRAYVPROC)LoadMethod(PREFIX "glScissorArrayv");
    this->_ScissorIndexed = (PFNGLSCISSORINDEXEDPROC)LoadMethod(PREFIX "glScissorIndexed");
    this->_ScissorIndexedv = (PFNGLSCISSORINDEXEDVPROC)LoadMethod(PREFIX "glScissorIndexedv");
    this->_DepthRangeArrayv = (PFNGLDEPTHRANGEARRAYVPROC)LoadMethod(PREFIX "glDepthRangeArrayv");
    this->_DepthRangeIndexed = (PFNGLDEPTHRANGEINDEXEDPROC)LoadMethod(PREFIX "glDepthRangeIndexed");
    this->_GetFloati_v = (PFNGLGETFLOATI_VPROC)LoadMethod(PREFIX "glGetFloati_v");
    this->_GetDoublei_v = (PFNGLGETDOUBLEI_VPROC)LoadMethod(PREFIX "glGetDoublei_v");
    this->_DrawArraysInstancedBaseInstance = (PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEPROC)LoadMethod(PREFIX "glDrawArraysInstancedBaseInstance");
    this->_DrawElementsInstancedBaseInstance = (PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCEPROC)LoadMethod(PREFIX "glDrawElementsInstancedBaseInstance");
    this->_DrawElementsInstancedBaseVertexBaseInstance = (PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEPROC)LoadMethod(PREFIX "glDrawElementsInstancedBaseVertexBaseInstance");
    this->_GetInternalformativ = (PFNGLGETINTERNALFORMATIVPROC)LoadMethod(PREFIX "glGetInternalformativ");
    this->_GetActiveAtomicCounterBufferiv = (PFNGLGETACTIVEATOMICCOUNTERBUFFERIVPROC)LoadMethod(PREFIX "glGetActiveAtomicCounterBufferiv");
    this->_BindImageTexture = (PFNGLBINDIMAGETEXTUREPROC)LoadMethod(PREFIX "glBindImageTexture");
    this->_MemoryBarrier = (PFNGLMEMORYBARRIERPROC)LoadMethod(PREFIX "glMemoryBarrier");
    this->_TexStorage1D = (PFNGLTEXSTORAGE1DPROC)LoadMethod(PREFIX "glTexStorage1D");
    this->_TexStorage2D = (PFNGLTEXSTORAGE2DPROC)LoadMethod(PREFIX "glTexStorage2D");
    this->_TexStorage3D = (PFNGLTEXSTORAGE3DPROC)LoadMethod(PREFIX "glTexStorage3D");
    this->_DrawTransformFeedbackInstanced = (PFNGLDRAWTRANSFORMFEEDBACKINSTANCEDPROC)LoadMethod(PREFIX "glDrawTransformFeedbackInstanced");
    this->_DrawTransformFeedbackStreamInstanced = (PFNGLDRAWTRANSFORMFEEDBACKSTREAMINSTANCEDPROC)LoadMethod(PREFIX "glDrawTransformFeedbackStreamInstanced");
    this->_ClearBufferData = (PFNGLCLEARBUFFERDATAPROC)LoadMethod(PREFIX "glClearBufferData");
    this->_ClearBufferSubData = (PFNGLCLEARBUFFERSUBDATAPROC)LoadMethod(PREFIX "glClearBufferSubData");
    this->_DispatchCompute = (PFNGLDISPATCHCOMPUTEPROC)LoadMethod(PREFIX "glDispatchCompute");
    this->_DispatchComputeIndirect = (PFNGLDISPATCHCOMPUTEINDIRECTPROC)LoadMethod(PREFIX "glDispatchComputeIndirect");
    this->_CopyImageSubData = (PFNGLCOPYIMAGESUBDATAPROC)LoadMethod(PREFIX "glCopyImageSubData");
    this->_FramebufferParameteri = (PFNGLFRAMEBUFFERPARAMETERIPROC)LoadMethod(PREFIX "glFramebufferParameteri");
    this->_GetFramebufferParameteriv = (PFNGLGETFRAMEBUFFERPARAMETERIVPROC)LoadMethod(PREFIX "glGetFramebufferParameteriv");
    this->_GetInternalformati64v = (PFNGLGETINTERNALFORMATI64VPROC)LoadMethod(PREFIX "glGetInternalformati64v");
    this->_InvalidateTexSubImage = (PFNGLINVALIDATETEXSUBIMAGEPROC)LoadMethod(PREFIX "glInvalidateTexSubImage");
    this->_InvalidateTexImage = (PFNGLINVALIDATETEXIMAGEPROC)LoadMethod(PREFIX "glInvalidateTexImage");
    this->_InvalidateBufferSubData = (PFNGLINVALIDATEBUFFERSUBDATAPROC)LoadMethod(PREFIX "glInvalidateBufferSubData");
    this->_InvalidateBufferData = (PFNGLINVALIDATEBUFFERDATAPROC)LoadMethod(PREFIX "glInvalidateBufferData");
    this->_InvalidateFramebuffer = (PFNGLINVALIDATEFRAMEBUFFERPROC)LoadMethod(PREFIX "glInvalidateFramebuffer");
    this->_InvalidateSubFramebuffer = (PFNGLINVALIDATESUBFRAMEBUFFERPROC)LoadMethod(PREFIX "glInvalidateSubFramebuffer");
    this->_MultiDrawArraysIndirect = (PFNGLMULTIDRAWARRAYSINDIRECTPROC)LoadMethod(PREFIX "glMultiDrawArraysIndirect");
    this->_MultiDrawElementsIndirect = (PFNGLMULTIDRAWELEMENTSINDIRECTPROC)LoadMethod(PREFIX "glMultiDrawElementsIndirect");
    this->_GetProgramInterfaceiv = (PFNGLGETPROGRAMINTERFACEIVPROC)LoadMethod(PREFIX "glGetProgramInterfaceiv");
    this->_GetProgramResourceIndex = (PFNGLGETPROGRAMRESOURCEINDEXPROC)LoadMethod(PREFIX "glGetProgramResourceIndex");
    this->_GetProgramResourceName = (PFNGLGETPROGRAMRESOURCENAMEPROC)LoadMethod(PREFIX "glGetProgramResourceName");
    this->_GetProgramResourceiv = (PFNGLGETPROGRAMRESOURCEIVPROC)LoadMethod(PREFIX "glGetProgramResourceiv");
    this->_GetProgramResourceLocation = (PFNGLGETPROGRAMRESOURCELOCATIONPROC)LoadMethod(PREFIX "glGetProgramResourceLocation");
    this->_GetProgramResourceLocationIndex = (PFNGLGETPROGRAMRESOURCELOCATIONINDEXPROC)LoadMethod(PREFIX "glGetProgramResourceLocationIndex");
    this->_ShaderStorageBlockBinding = (PFNGLSHADERSTORAGEBLOCKBINDINGPROC)LoadMethod(PREFIX "glShaderStorageBlockBinding");
    this->_TexBufferRange = (PFNGLTEXBUFFERRANGEPROC)LoadMethod(PREFIX "glTexBufferRange");
    this->_TexStorage2DMultisample = (PFNGLTEXSTORAGE2DMULTISAMPLEPROC)LoadMethod(PREFIX "glTexStorage2DMultisample");
    this->_TexStorage3DMultisample = (PFNGLTEXSTORAGE3DMULTISAMPLEPROC)LoadMethod(PREFIX "glTexStorage3DMultisample");
    this->_TextureView = (PFNGLTEXTUREVIEWPROC)LoadMethod(PREFIX "glTextureView");
    this->_BindVertexBuffer = (PFNGLBINDVERTEXBUFFERPROC)LoadMethod(PREFIX "glBindVertexBuffer");
    this->_VertexAttribFormat = (PFNGLVERTEXATTRIBFORMATPROC)LoadMethod(PREFIX "glVertexAttribFormat");
    this->_VertexAttribIFormat = (PFNGLVERTEXATTRIBIFORMATPROC)LoadMethod(PREFIX "glVertexAttribIFormat");
    this->_VertexAttribLFormat = (PFNGLVERTEXATTRIBLFORMATPROC)LoadMethod(PREFIX "glVertexAttribLFormat");
    this->_VertexAttribBinding = (PFNGLVERTEXATTRIBBINDINGPROC)LoadMethod(PREFIX "glVertexAttribBinding");
    this->_VertexBindingDivisor = (PFNGLVERTEXBINDINGDIVISORPROC)LoadMethod(PREFIX "glVertexBindingDivisor");
    this->_DebugMessageControl = (PFNGLDEBUGMESSAGECONTROLPROC)LoadMethod(PREFIX "glDebugMessageControl");
    this->_DebugMessageInsert = (PFNGLDEBUGMESSAGEINSERTPROC)LoadMethod(PREFIX "glDebugMessageInsert");
    this->_DebugMessageCallback = (PFNGLDEBUGMESSAGECALLBACKPROC)LoadMethod(PREFIX "glDebugMessageCallback");
    this->_GetDebugMessageLog = (PFNGLGETDEBUGMESSAGELOGPROC)LoadMethod(PREFIX "glGetDebugMessageLog");
    this->_PushDebugGroup = (PFNGLPUSHDEBUGGROUPPROC)LoadMethod(PREFIX "glPushDebugGroup");
    this->_PopDebugGroup = (PFNGLPOPDEBUGGROUPPROC)LoadMethod(PREFIX "glPopDebugGroup");
    this->_ObjectLabel = (PFNGLOBJECTLABELPROC)LoadMethod(PREFIX "glObjectLabel");
    this->_GetObjectLabel = (PFNGLGETOBJECTLABELPROC)LoadMethod(PREFIX "glGetObjectLabel");
    this->_ObjectPtrLabel = (PFNGLOBJECTPTRLABELPROC)LoadMethod(PREFIX "glObjectPtrLabel");
    this->_GetObjectPtrLabel = (PFNGLGETOBJECTPTRLABELPROC)LoadMethod(PREFIX "glGetObjectPtrLabel");
    this->_BufferStorage = (PFNGLBUFFERSTORAGEPROC)LoadMethod(PREFIX "glBufferStorage");
    this->_ClearTexImage = (PFNGLCLEARTEXIMAGEPROC)LoadMethod(PREFIX "glClearTexImage");
    this->_ClearTexSubImage = (PFNGLCLEARTEXSUBIMAGEPROC)LoadMethod(PREFIX "glClearTexSubImage");
    this->_BindBuffersBase = (PFNGLBINDBUFFERSBASEPROC)LoadMethod(PREFIX "glBindBuffersBase");
    this->_BindBuffersRange = (PFNGLBINDBUFFERSRANGEPROC)LoadMethod(PREFIX "glBindBuffersRange");
    this->_BindTextures = (PFNGLBINDTEXTURESPROC)LoadMethod(PREFIX "glBindTextures");
    this->_BindSamplers = (PFNGLBINDSAMPLERSPROC)LoadMethod(PREFIX "glBindSamplers");
    this->_BindImageTextures = (PFNGLBINDIMAGETEXTURESPROC)LoadMethod(PREFIX "glBindImageTextures");
    this->_BindVertexBuffers = (PFNGLBINDVERTEXBUFFERSPROC)LoadMethod(PREFIX "glBindVertexBuffers");
    this->_ClipControl = (PFNGLCLIPCONTROLPROC)LoadMethod(PREFIX "glClipControl");
    this->_CreateTransformFeedbacks = (PFNGLCREATETRANSFORMFEEDBACKSPROC)LoadMethod(PREFIX "glCreateTransformFeedbacks");
    this->_TransformFeedbackBufferBase = (PFNGLTRANSFORMFEEDBACKBUFFERBASEPROC)LoadMethod(PREFIX "glTransformFeedbackBufferBase");
    this->_TransformFeedbackBufferRange = (PFNGLTRANSFORMFEEDBACKBUFFERRANGEPROC)LoadMethod(PREFIX "glTransformFeedbackBufferRange");
    this->_GetTransformFeedbackiv = (PFNGLGETTRANSFORMFEEDBACKIVPROC)LoadMethod(PREFIX "glGetTransformFeedbackiv");
    this->_GetTransformFeedbacki_v = (PFNGLGETTRANSFORMFEEDBACKI_VPROC)LoadMethod(PREFIX "glGetTransformFeedbacki_v");
    this->_GetTransformFeedbacki64_v = (PFNGLGETTRANSFORMFEEDBACKI64_VPROC)LoadMethod(PREFIX "glGetTransformFeedbacki64_v");
    this->_CreateBuffers = (PFNGLCREATEBUFFERSPROC)LoadMethod(PREFIX "glCreateBuffers");
    this->_NamedBufferStorage = (PFNGLNAMEDBUFFERSTORAGEPROC)LoadMethod(PREFIX "glNamedBufferStorage");
    this->_NamedBufferData = (PFNGLNAMEDBUFFERDATAPROC)LoadMethod(PREFIX "glNamedBufferData");
    this->_NamedBufferSubData = (PFNGLNAMEDBUFFERSUBDATAPROC)LoadMethod(PREFIX "glNamedBufferSubData");
    this->_CopyNamedBufferSubData = (PFNGLCOPYNAMEDBUFFERSUBDATAPROC)LoadMethod(PREFIX "glCopyNamedBufferSubData");
    this->_ClearNamedBufferData = (PFNGLCLEARNAMEDBUFFERDATAPROC)LoadMethod(PREFIX "glClearNamedBufferData");
    this->_ClearNamedBufferSubData = (PFNGLCLEARNAMEDBUFFERSUBDATAPROC)LoadMethod(PREFIX "glClearNamedBufferSubData");
    this->_MapNamedBuffer = (PFNGLMAPNAMEDBUFFERPROC)LoadMethod(PREFIX "glMapNamedBuffer");
    this->_MapNamedBufferRange = (PFNGLMAPNAMEDBUFFERRANGEPROC)LoadMethod(PREFIX "glMapNamedBufferRange");
    this->_UnmapNamedBuffer = (PFNGLUNMAPNAMEDBUFFERPROC)LoadMethod(PREFIX "glUnmapNamedBuffer");
    this->_FlushMappedNamedBufferRange = (PFNGLFLUSHMAPPEDNAMEDBUFFERRANGEPROC)LoadMethod(PREFIX "glFlushMappedNamedBufferRange");
    this->_GetNamedBufferParameteriv = (PFNGLGETNAMEDBUFFERPARAMETERIVPROC)LoadMethod(PREFIX "glGetNamedBufferParameteriv");
    this->_GetNamedBufferParameteri64v = (PFNGLGETNAMEDBUFFERPARAMETERI64VPROC)LoadMethod(PREFIX "glGetNamedBufferParameteri64v");
    this->_GetNamedBufferPointerv = (PFNGLGETNAMEDBUFFERPOINTERVPROC)LoadMethod(PREFIX "glGetNamedBufferPointerv");
    this->_GetNamedBufferSubData = (PFNGLGETNAMEDBUFFERSUBDATAPROC)LoadMethod(PREFIX "glGetNamedBufferSubData");
    this->_CreateFramebuffers = (PFNGLCREATEFRAMEBUFFERSPROC)LoadMethod(PREFIX "glCreateFramebuffers");
    this->_NamedFramebufferRenderbuffer = (PFNGLNAMEDFRAMEBUFFERRENDERBUFFERPROC)LoadMethod(PREFIX "glNamedFramebufferRenderbuffer");
    this->_NamedFramebufferParameteri = (PFNGLNAMEDFRAMEBUFFERPARAMETERIPROC)LoadMethod(PREFIX "glNamedFramebufferParameteri");
    this->_NamedFramebufferTexture = (PFNGLNAMEDFRAMEBUFFERTEXTUREPROC)LoadMethod(PREFIX "glNamedFramebufferTexture");
    this->_NamedFramebufferTextureLayer = (PFNGLNAMEDFRAMEBUFFERTEXTURELAYERPROC)LoadMethod(PREFIX "glNamedFramebufferTextureLayer");
    this->_NamedFramebufferDrawBuffer = (PFNGLNAMEDFRAMEBUFFERDRAWBUFFERPROC)LoadMethod(PREFIX "glNamedFramebufferDrawBuffer");
    this->_NamedFramebufferDrawBuffers = (PFNGLNAMEDFRAMEBUFFERDRAWBUFFERSPROC)LoadMethod(PREFIX "glNamedFramebufferDrawBuffers");
    this->_NamedFramebufferReadBuffer = (PFNGLNAMEDFRAMEBUFFERREADBUFFERPROC)LoadMethod(PREFIX "glNamedFramebufferReadBuffer");
    this->_InvalidateNamedFramebufferData = (PFNGLINVALIDATENAMEDFRAMEBUFFERDATAPROC)LoadMethod(PREFIX "glInvalidateNamedFramebufferData");
    this->_InvalidateNamedFramebufferSubData = (PFNGLINVALIDATENAMEDFRAMEBUFFERSUBDATAPROC)LoadMethod(PREFIX "glInvalidateNamedFramebufferSubData");
    this->_ClearNamedFramebufferiv = (PFNGLCLEARNAMEDFRAMEBUFFERIVPROC)LoadMethod(PREFIX "glClearNamedFramebufferiv");
    this->_ClearNamedFramebufferuiv = (PFNGLCLEARNAMEDFRAMEBUFFERUIVPROC)LoadMethod(PREFIX "glClearNamedFramebufferuiv");
    this->_ClearNamedFramebufferfv = (PFNGLCLEARNAMEDFRAMEBUFFERFVPROC)LoadMethod(PREFIX "glClearNamedFramebufferfv");
    this->_ClearNamedFramebufferfi = (PFNGLCLEARNAMEDFRAMEBUFFERFIPROC)LoadMethod(PREFIX "glClearNamedFramebufferfi");
    this->_BlitNamedFramebuffer = (PFNGLBLITNAMEDFRAMEBUFFERPROC)LoadMethod(PREFIX "glBlitNamedFramebuffer");
    this->_CheckNamedFramebufferStatus = (PFNGLCHECKNAMEDFRAMEBUFFERSTATUSPROC)LoadMethod(PREFIX "glCheckNamedFramebufferStatus");
    this->_GetNamedFramebufferParameteriv = (PFNGLGETNAMEDFRAMEBUFFERPARAMETERIVPROC)LoadMethod(PREFIX "glGetNamedFramebufferParameteriv");
    this->_GetNamedFramebufferAttachmentParameteriv = (PFNGLGETNAMEDFRAMEBUFFERATTACHMENTPARAMETERIVPROC)LoadMethod(PREFIX "glGetNamedFramebufferAttachmentParameteriv");
    this->_CreateRenderbuffers = (PFNGLCREATERENDERBUFFERSPROC)LoadMethod(PREFIX "glCreateRenderbuffers");
    this->_NamedRenderbufferStorage = (PFNGLNAMEDRENDERBUFFERSTORAGEPROC)LoadMethod(PREFIX "glNamedRenderbufferStorage");
    this->_NamedRenderbufferStorageMultisample = (PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLEPROC)LoadMethod(PREFIX "glNamedRenderbufferStorageMultisample");
    this->_GetNamedRenderbufferParameteriv = (PFNGLGETNAMEDRENDERBUFFERPARAMETERIVPROC)LoadMethod(PREFIX "glGetNamedRenderbufferParameteriv");
    this->_CreateTextures = (PFNGLCREATETEXTURESPROC)LoadMethod(PREFIX "glCreateTextures");
    this->_TextureBuffer = (PFNGLTEXTUREBUFFERPROC)LoadMethod(PREFIX "glTextureBuffer");
    this->_TextureBufferRange = (PFNGLTEXTUREBUFFERRANGEPROC)LoadMethod(PREFIX "glTextureBufferRange");
    this->_TextureStorage1D = (PFNGLTEXTURESTORAGE1DPROC)LoadMethod(PREFIX "glTextureStorage1D");
    this->_TextureStorage2D = (PFNGLTEXTURESTORAGE2DPROC)LoadMethod(PREFIX "glTextureStorage2D");
    this->_TextureStorage3D = (PFNGLTEXTURESTORAGE3DPROC)LoadMethod(PREFIX "glTextureStorage3D");
    this->_TextureStorage2DMultisample = (PFNGLTEXTURESTORAGE2DMULTISAMPLEPROC)LoadMethod(PREFIX "glTextureStorage2DMultisample");
    this->_TextureStorage3DMultisample = (PFNGLTEXTURESTORAGE3DMULTISAMPLEPROC)LoadMethod(PREFIX "glTextureStorage3DMultisample");
    this->_TextureSubImage1D = (PFNGLTEXTURESUBIMAGE1DPROC)LoadMethod(PREFIX "glTextureSubImage1D");
    this->_TextureSubImage2D = (PFNGLTEXTURESUBIMAGE2DPROC)LoadMethod(PREFIX "glTextureSubImage2D");
    this->_TextureSubImage3D = (PFNGLTEXTURESUBIMAGE3DPROC)LoadMethod(PREFIX "glTextureSubImage3D");
    this->_CompressedTextureSubImage1D = (PFNGLCOMPRESSEDTEXTURESUBIMAGE1DPROC)LoadMethod(PREFIX "glCompressedTextureSubImage1D");
    this->_CompressedTextureSubImage2D = (PFNGLCOMPRESSEDTEXTURESUBIMAGE2DPROC)LoadMethod(PREFIX "glCompressedTextureSubImage2D");
    this->_CompressedTextureSubImage3D = (PFNGLCOMPRESSEDTEXTURESUBIMAGE3DPROC)LoadMethod(PREFIX "glCompressedTextureSubImage3D");
    this->_CopyTextureSubImage1D = (PFNGLCOPYTEXTURESUBIMAGE1DPROC)LoadMethod(PREFIX "glCopyTextureSubImage1D");
    this->_CopyTextureSubImage2D = (PFNGLCOPYTEXTURESUBIMAGE2DPROC)LoadMethod(PREFIX "glCopyTextureSubImage2D");
    this->_CopyTextureSubImage3D = (PFNGLCOPYTEXTURESUBIMAGE3DPROC)LoadMethod(PREFIX "glCopyTextureSubImage3D");
    this->_TextureParameterf = (PFNGLTEXTUREPARAMETERFPROC)LoadMethod(PREFIX "glTextureParameterf");
    this->_TextureParameterfv = (PFNGLTEXTUREPARAMETERFVPROC)LoadMethod(PREFIX "glTextureParameterfv");
    this->_TextureParameteri = (PFNGLTEXTUREPARAMETERIPROC)LoadMethod(PREFIX "glTextureParameteri");
    this->_TextureParameterIiv = (PFNGLTEXTUREPARAMETERIIVPROC)LoadMethod(PREFIX "glTextureParameterIiv");
    this->_TextureParameterIuiv = (PFNGLTEXTUREPARAMETERIUIVPROC)LoadMethod(PREFIX "glTextureParameterIuiv");
    this->_TextureParameteriv = (PFNGLTEXTUREPARAMETERIVPROC)LoadMethod(PREFIX "glTextureParameteriv");
    this->_GenerateTextureMipmap = (PFNGLGENERATETEXTUREMIPMAPPROC)LoadMethod(PREFIX "glGenerateTextureMipmap");
    this->_BindTextureUnit = (PFNGLBINDTEXTUREUNITPROC)LoadMethod(PREFIX "glBindTextureUnit");
    this->_GetTextureImage = (PFNGLGETTEXTUREIMAGEPROC)LoadMethod(PREFIX "glGetTextureImage");
    this->_GetCompressedTextureImage = (PFNGLGETCOMPRESSEDTEXTUREIMAGEPROC)LoadMethod(PREFIX "glGetCompressedTextureImage");
    this->_GetTextureLevelParameterfv = (PFNGLGETTEXTURELEVELPARAMETERFVPROC)LoadMethod(PREFIX "glGetTextureLevelParameterfv");
    this->_GetTextureLevelParameteriv = (PFNGLGETTEXTURELEVELPARAMETERIVPROC)LoadMethod(PREFIX "glGetTextureLevelParameteriv");
    this->_GetTextureParameterfv = (PFNGLGETTEXTUREPARAMETERFVPROC)LoadMethod(PREFIX "glGetTextureParameterfv");
    this->_GetTextureParameterIiv = (PFNGLGETTEXTUREPARAMETERIIVPROC)LoadMethod(PREFIX "glGetTextureParameterIiv");
    this->_GetTextureParameterIuiv = (PFNGLGETTEXTUREPARAMETERIUIVPROC)LoadMethod(PREFIX "glGetTextureParameterIuiv");
    this->_GetTextureParameteriv = (PFNGLGETTEXTUREPARAMETERIVPROC)LoadMethod(PREFIX "glGetTextureParameteriv");
    this->_CreateVertexArrays = (PFNGLCREATEVERTEXARRAYSPROC)LoadMethod(PREFIX "glCreateVertexArrays");
    this->_DisableVertexArrayAttrib = (PFNGLDISABLEVERTEXARRAYATTRIBPROC)LoadMethod(PREFIX "glDisableVertexArrayAttrib");
    this->_EnableVertexArrayAttrib = (PFNGLENABLEVERTEXARRAYATTRIBPROC)LoadMethod(PREFIX "glEnableVertexArrayAttrib");
    this->_VertexArrayElementBuffer = (PFNGLVERTEXARRAYELEMENTBUFFERPROC)LoadMethod(PREFIX "glVertexArrayElementBuffer");
    this->_VertexArrayVertexBuffer = (PFNGLVERTEXARRAYVERTEXBUFFERPROC)LoadMethod(PREFIX "glVertexArrayVertexBuffer");
    this->_VertexArrayVertexBuffers = (PFNGLVERTEXARRAYVERTEXBUFFERSPROC)LoadMethod(PREFIX "glVertexArrayVertexBuffers");
    this->_VertexArrayAttribBinding = (PFNGLVERTEXARRAYATTRIBBINDINGPROC)LoadMethod(PREFIX "glVertexArrayAttribBinding");
    this->_VertexArrayAttribFormat = (PFNGLVERTEXARRAYATTRIBFORMATPROC)LoadMethod(PREFIX "glVertexArrayAttribFormat");
    this->_VertexArrayAttribIFormat = (PFNGLVERTEXARRAYATTRIBIFORMATPROC)LoadMethod(PREFIX "glVertexArrayAttribIFormat");
    this->_VertexArrayAttribLFormat = (PFNGLVERTEXARRAYATTRIBLFORMATPROC)LoadMethod(PREFIX "glVertexArrayAttribLFormat");
    this->_VertexArrayBindingDivisor = (PFNGLVERTEXARRAYBINDINGDIVISORPROC)LoadMethod(PREFIX "glVertexArrayBindingDivisor");
    this->_GetVertexArrayiv = (PFNGLGETVERTEXARRAYIVPROC)LoadMethod(PREFIX "glGetVertexArrayiv");
    this->_GetVertexArrayIndexediv = (PFNGLGETVERTEXARRAYINDEXEDIVPROC)LoadMethod(PREFIX "glGetVertexArrayIndexediv");
    this->_GetVertexArrayIndexed64iv = (PFNGLGETVERTEXARRAYINDEXED64IVPROC)LoadMethod(PREFIX "glGetVertexArrayIndexed64iv");
    this->_CreateSamplers = (PFNGLCREATESAMPLERSPROC)LoadMethod(PREFIX "glCreateSamplers");
    this->_CreateProgramPipelines = (PFNGLCREATEPROGRAMPIPELINESPROC)LoadMethod(PREFIX "glCreateProgramPipelines");
    this->_CreateQueries = (PFNGLCREATEQUERIESPROC)LoadMethod(PREFIX "glCreateQueries");
    this->_GetQueryBufferObjecti64v = (PFNGLGETQUERYBUFFEROBJECTI64VPROC)LoadMethod(PREFIX "glGetQueryBufferObjecti64v");
    this->_GetQueryBufferObjectiv = (PFNGLGETQUERYBUFFEROBJECTIVPROC)LoadMethod(PREFIX "glGetQueryBufferObjectiv");
    this->_GetQueryBufferObjectui64v = (PFNGLGETQUERYBUFFEROBJECTUI64VPROC)LoadMethod(PREFIX "glGetQueryBufferObjectui64v");
    this->_GetQueryBufferObjectuiv = (PFNGLGETQUERYBUFFEROBJECTUIVPROC)LoadMethod(PREFIX "glGetQueryBufferObjectuiv");
    this->_MemoryBarrierByRegion = (PFNGLMEMORYBARRIERBYREGIONPROC)LoadMethod(PREFIX "glMemoryBarrierByRegion");
    this->_GetTextureSubImage = (PFNGLGETTEXTURESUBIMAGEPROC)LoadMethod(PREFIX "glGetTextureSubImage");
    this->_GetCompressedTextureSubImage = (PFNGLGETCOMPRESSEDTEXTURESUBIMAGEPROC)LoadMethod(PREFIX "glGetCompressedTextureSubImage");
    this->_GetGraphicsResetStatus = (PFNGLGETGRAPHICSRESETSTATUSPROC)LoadMethod(PREFIX "glGetGraphicsResetStatus");
    this->_GetnCompressedTexImage = (PFNGLGETNCOMPRESSEDTEXIMAGEPROC)LoadMethod(PREFIX "glGetnCompressedTexImage");
    this->_GetnTexImage = (PFNGLGETNTEXIMAGEPROC)LoadMethod(PREFIX "glGetnTexImage");
    this->_GetnUniformdv = (PFNGLGETNUNIFORMDVPROC)LoadMethod(PREFIX "glGetnUniformdv");
    this->_GetnUniformfv = (PFNGLGETNUNIFORMFVPROC)LoadMethod(PREFIX "glGetnUniformfv");
    this->_GetnUniformiv = (PFNGLGETNUNIFORMIVPROC)LoadMethod(PREFIX "glGetnUniformiv");
    this->_GetnUniformuiv = (PFNGLGETNUNIFORMUIVPROC)LoadMethod(PREFIX "glGetnUniformuiv");
    this->_ReadnPixels = (PFNGLREADNPIXELSPROC)LoadMethod(PREFIX "glReadnPixels");
    this->_TextureBarrier = (PFNGLTEXTUREBARRIERPROC)LoadMethod(PREFIX "glTextureBarrier");
    this->_SpecializeShader = (PFNGLSPECIALIZESHADERPROC)LoadMethod(PREFIX "glSpecializeShader");
    this->_MultiDrawArraysIndirectCount = (PFNGLMULTIDRAWARRAYSINDIRECTCOUNTPROC)LoadMethod(PREFIX "glMultiDrawArraysIndirectCount");
    this->_MultiDrawElementsIndirectCount = (PFNGLMULTIDRAWELEMENTSINDIRECTCOUNTPROC)LoadMethod(PREFIX "glMultiDrawElementsIndirectCount");
    this->_PolygonOffsetClamp = (PFNGLPOLYGONOFFSETCLAMPPROC)LoadMethod(PREFIX "glPolygonOffsetClamp");

    this->has.CullFace = (bool)this->_CullFace;
    this->has.FrontFace = (bool)this->_FrontFace;
    this->has.Hint = (bool)this->_Hint;
    this->has.LineWidth = (bool)this->_LineWidth;
    this->has.PointSize = (bool)this->_PointSize;
    this->has.PolygonMode = (bool)this->_PolygonMode;
    this->has.Scissor = (bool)this->_Scissor;
    this->has.TexParameterf = (bool)this->_TexParameterf;
    this->has.TexParameterfv = (bool)this->_TexParameterfv;
    this->has.TexParameteri = (bool)this->_TexParameteri;
    this->has.TexParameteriv = (bool)this->_TexParameteriv;
    this->has.TexImage1D = (bool)this->_TexImage1D;
    this->has.TexImage2D = (bool)this->_TexImage2D;
    this->has.DrawBuffer = (bool)this->_DrawBuffer;
    this->has.Clear = (bool)this->_Clear;
    this->has.ClearColor = (bool)this->_ClearColor;
    this->has.ClearStencil = (bool)this->_ClearStencil;
    this->has.ClearDepth = (bool)this->_ClearDepth;
    this->has.StencilMask = (bool)this->_StencilMask;
    this->has.ColorMask = (bool)this->_ColorMask;
    this->has.DepthMask = (bool)this->_DepthMask;
    this->has.Disable = (bool)this->_Disable;
    this->has.Enable = (bool)this->_Enable;
    this->has.Finish = (bool)this->_Finish;
    this->has.Flush = (bool)this->_Flush;
    this->has.BlendFunc = (bool)this->_BlendFunc;
    this->has.LogicOp = (bool)this->_LogicOp;
    this->has.StencilFunc = (bool)this->_StencilFunc;
    this->has.StencilOp = (bool)this->_StencilOp;
    this->has.DepthFunc = (bool)this->_DepthFunc;
    this->has.PixelStoref = (bool)this->_PixelStoref;
    this->has.PixelStorei = (bool)this->_PixelStorei;
    this->has.ReadBuffer = (bool)this->_ReadBuffer;
    this->has.ReadPixels = (bool)this->_ReadPixels;
    this->has.GetBooleanv = (bool)this->_GetBooleanv;
    this->has.GetDoublev = (bool)this->_GetDoublev;
    this->has.GetError = (bool)this->_GetError;
    this->has.GetFloatv = (bool)this->_GetFloatv;
    this->has.GetIntegerv = (bool)this->_GetIntegerv;
    this->has.GetString = (bool)this->_GetString;
    this->has.GetTexImage = (bool)this->_GetTexImage;
    this->has.GetTexParameterfv = (bool)this->_GetTexParameterfv;
    this->has.GetTexParameteriv = (bool)this->_GetTexParameteriv;
    this->has.GetTexLevelParameterfv = (bool)this->_GetTexLevelParameterfv;
    this->has.GetTexLevelParameteriv = (bool)this->_GetTexLevelParameteriv;
    this->has.IsEnabled = (bool)this->_IsEnabled;
    this->has.DepthRange = (bool)this->_DepthRange;
    this->has.Viewport = (bool)this->_Viewport;
    this->has.DrawArrays = (bool)this->_DrawArrays;
    this->has.DrawElements = (bool)this->_DrawElements;
    this->has.GetPointerv = (bool)this->_GetPointerv;
    this->has.PolygonOffset = (bool)this->_PolygonOffset;
    this->has.CopyTexImage1D = (bool)this->_CopyTexImage1D;
    this->has.CopyTexImage2D = (bool)this->_CopyTexImage2D;
    this->has.CopyTexSubImage1D = (bool)this->_CopyTexSubImage1D;
    this->has.CopyTexSubImage2D = (bool)this->_CopyTexSubImage2D;
    this->has.TexSubImage1D = (bool)this->_TexSubImage1D;
    this->has.TexSubImage2D = (bool)this->_TexSubImage2D;
    this->has.BindTexture = (bool)this->_BindTexture;
    this->has.DeleteTextures = (bool)this->_DeleteTextures;
    this->has.GenTextures = (bool)this->_GenTextures;
    this->has.IsTexture = (bool)this->_IsTexture;
    this->has.DrawRangeElements = (bool)this->_DrawRangeElements;
    this->has.TexImage3D = (bool)this->_TexImage3D;
    this->has.TexSubImage3D = (bool)this->_TexSubImage3D;
    this->has.CopyTexSubImage3D = (bool)this->_CopyTexSubImage3D;
    this->has.ActiveTexture = (bool)this->_ActiveTexture;
    this->has.SampleCoverage = (bool)this->_SampleCoverage;
    this->has.CompressedTexImage3D = (bool)this->_CompressedTexImage3D;
    this->has.CompressedTexImage2D = (bool)this->_CompressedTexImage2D;
    this->has.CompressedTexImage1D = (bool)this->_CompressedTexImage1D;
    this->has.CompressedTexSubImage3D = (bool)this->_CompressedTexSubImage3D;
    this->has.CompressedTexSubImage2D = (bool)this->_CompressedTexSubImage2D;
    this->has.CompressedTexSubImage1D = (bool)this->_CompressedTexSubImage1D;
    this->has.GetCompressedTexImage = (bool)this->_GetCompressedTexImage;
    this->has.BlendFuncSeparate = (bool)this->_BlendFuncSeparate;
    this->has.MultiDrawArrays = (bool)this->_MultiDrawArrays;
    this->has.MultiDrawElements = (bool)this->_MultiDrawElements;
    this->has.PointParameterf = (bool)this->_PointParameterf;
    this->has.PointParameterfv = (bool)this->_PointParameterfv;
    this->has.PointParameteri = (bool)this->_PointParameteri;
    this->has.PointParameteriv = (bool)this->_PointParameteriv;
    this->has.BlendColor = (bool)this->_BlendColor;
    this->has.BlendEquation = (bool)this->_BlendEquation;
    this->has.GenQueries = (bool)this->_GenQueries;
    this->has.DeleteQueries = (bool)this->_DeleteQueries;
    this->has.IsQuery = (bool)this->_IsQuery;
    this->has.BeginQuery = (bool)this->_BeginQuery;
    this->has.EndQuery = (bool)this->_EndQuery;
    this->has.GetQueryiv = (bool)this->_GetQueryiv;
    this->has.GetQueryObjectiv = (bool)this->_GetQueryObjectiv;
    this->has.GetQueryObjectuiv = (bool)this->_GetQueryObjectuiv;
    this->has.BindBuffer = (bool)this->_BindBuffer;
    this->has.DeleteBuffers = (bool)this->_DeleteBuffers;
    this->has.GenBuffers = (bool)this->_GenBuffers;
    this->has.IsBuffer = (bool)this->_IsBuffer;
    this->has.BufferData = (bool)this->_BufferData;
    this->has.BufferSubData = (bool)this->_BufferSubData;
    this->has.GetBufferSubData = (bool)this->_GetBufferSubData;
    this->has.MapBuffer = (bool)this->_MapBuffer;
    this->has.UnmapBuffer = (bool)this->_UnmapBuffer;
    this->has.GetBufferParameteriv = (bool)this->_GetBufferParameteriv;
    this->has.GetBufferPointerv = (bool)this->_GetBufferPointerv;
    this->has.BlendEquationSeparate = (bool)this->_BlendEquationSeparate;
    this->has.DrawBuffers = (bool)this->_DrawBuffers;
    this->has.StencilOpSeparate = (bool)this->_StencilOpSeparate;
    this->has.StencilFuncSeparate = (bool)this->_StencilFuncSeparate;
    this->has.StencilMaskSeparate = (bool)this->_StencilMaskSeparate;
    this->has.AttachShader = (bool)this->_AttachShader;
    this->has.BindAttribLocation = (bool)this->_BindAttribLocation;
    this->has.CompileShader = (bool)this->_CompileShader;
    this->has.CreateProgram = (bool)this->_CreateProgram;
    this->has.CreateShader = (bool)this->_CreateShader;
    this->has.DeleteProgram = (bool)this->_DeleteProgram;
    this->has.DeleteShader = (bool)this->_DeleteShader;
    this->has.DetachShader = (bool)this->_DetachShader;
    this->has.DisableVertexAttribArray = (bool)this->_DisableVertexAttribArray;
    this->has.EnableVertexAttribArray = (bool)this->_EnableVertexAttribArray;
    this->has.GetActiveAttrib = (bool)this->_GetActiveAttrib;
    this->has.GetActiveUniform = (bool)this->_GetActiveUniform;
    this->has.GetAttachedShaders = (bool)this->_GetAttachedShaders;
    this->has.GetAttribLocation = (bool)this->_GetAttribLocation;
    this->has.GetProgramiv = (bool)this->_GetProgramiv;
    this->has.GetProgramInfoLog = (bool)this->_GetProgramInfoLog;
    this->has.GetShaderiv = (bool)this->_GetShaderiv;
    this->has.GetShaderInfoLog = (bool)this->_GetShaderInfoLog;
    this->has.GetShaderSource = (bool)this->_GetShaderSource;
    this->has.GetUniformLocation = (bool)this->_GetUniformLocation;
    this->has.GetUniformfv = (bool)this->_GetUniformfv;
    this->has.GetUniformiv = (bool)this->_GetUniformiv;
    this->has.GetVertexAttribdv = (bool)this->_GetVertexAttribdv;
    this->has.GetVertexAttribfv = (bool)this->_GetVertexAttribfv;
    this->has.GetVertexAttribiv = (bool)this->_GetVertexAttribiv;
    this->has.GetVertexAttribPointerv = (bool)this->_GetVertexAttribPointerv;
    this->has.IsProgram = (bool)this->_IsProgram;
    this->has.IsShader = (bool)this->_IsShader;
    this->has.LinkProgram = (bool)this->_LinkProgram;
    this->has.ShaderSource = (bool)this->_ShaderSource;
    this->has.UseProgram = (bool)this->_UseProgram;
    this->has.Uniform1f = (bool)this->_Uniform1f;
    this->has.Uniform2f = (bool)this->_Uniform2f;
    this->has.Uniform3f = (bool)this->_Uniform3f;
    this->has.Uniform4f = (bool)this->_Uniform4f;
    this->has.Uniform1i = (bool)this->_Uniform1i;
    this->has.Uniform2i = (bool)this->_Uniform2i;
    this->has.Uniform3i = (bool)this->_Uniform3i;
    this->has.Uniform4i = (bool)this->_Uniform4i;
    this->has.Uniform1fv = (bool)this->_Uniform1fv;
    this->has.Uniform2fv = (bool)this->_Uniform2fv;
    this->has.Uniform3fv = (bool)this->_Uniform3fv;
    this->has.Uniform4fv = (bool)this->_Uniform4fv;
    this->has.Uniform1iv = (bool)this->_Uniform1iv;
    this->has.Uniform2iv = (bool)this->_Uniform2iv;
    this->has.Uniform3iv = (bool)this->_Uniform3iv;
    this->has.Uniform4iv = (bool)this->_Uniform4iv;
    this->has.UniformMatrix2fv = (bool)this->_UniformMatrix2fv;
    this->has.UniformMatrix3fv = (bool)this->_UniformMatrix3fv;
    this->has.UniformMatrix4fv = (bool)this->_UniformMatrix4fv;
    this->has.ValidateProgram = (bool)this->_ValidateProgram;
    this->has.VertexAttrib1d = (bool)this->_VertexAttrib1d;
    this->has.VertexAttrib1dv = (bool)this->_VertexAttrib1dv;
    this->has.VertexAttrib1f = (bool)this->_VertexAttrib1f;
    this->has.VertexAttrib1fv = (bool)this->_VertexAttrib1fv;
    this->has.VertexAttrib1s = (bool)this->_VertexAttrib1s;
    this->has.VertexAttrib1sv = (bool)this->_VertexAttrib1sv;
    this->has.VertexAttrib2d = (bool)this->_VertexAttrib2d;
    this->has.VertexAttrib2dv = (bool)this->_VertexAttrib2dv;
    this->has.VertexAttrib2f = (bool)this->_VertexAttrib2f;
    this->has.VertexAttrib2fv = (bool)this->_VertexAttrib2fv;
    this->has.VertexAttrib2s = (bool)this->_VertexAttrib2s;
    this->has.VertexAttrib2sv = (bool)this->_VertexAttrib2sv;
    this->has.VertexAttrib3d = (bool)this->_VertexAttrib3d;
    this->has.VertexAttrib3dv = (bool)this->_VertexAttrib3dv;
    this->has.VertexAttrib3f = (bool)this->_VertexAttrib3f;
    this->has.VertexAttrib3fv = (bool)this->_VertexAttrib3fv;
    this->has.VertexAttrib3s = (bool)this->_VertexAttrib3s;
    this->has.VertexAttrib3sv = (bool)this->_VertexAttrib3sv;
    this->has.VertexAttrib4Nbv = (bool)this->_VertexAttrib4Nbv;
    this->has.VertexAttrib4Niv = (bool)this->_VertexAttrib4Niv;
    this->has.VertexAttrib4Nsv = (bool)this->_VertexAttrib4Nsv;
    this->has.VertexAttrib4Nub = (bool)this->_VertexAttrib4Nub;
    this->has.VertexAttrib4Nubv = (bool)this->_VertexAttrib4Nubv;
    this->has.VertexAttrib4Nuiv = (bool)this->_VertexAttrib4Nuiv;
    this->has.VertexAttrib4Nusv = (bool)this->_VertexAttrib4Nusv;
    this->has.VertexAttrib4bv = (bool)this->_VertexAttrib4bv;
    this->has.VertexAttrib4d = (bool)this->_VertexAttrib4d;
    this->has.VertexAttrib4dv = (bool)this->_VertexAttrib4dv;
    this->has.VertexAttrib4f = (bool)this->_VertexAttrib4f;
    this->has.VertexAttrib4fv = (bool)this->_VertexAttrib4fv;
    this->has.VertexAttrib4iv = (bool)this->_VertexAttrib4iv;
    this->has.VertexAttrib4s = (bool)this->_VertexAttrib4s;
    this->has.VertexAttrib4sv = (bool)this->_VertexAttrib4sv;
    this->has.VertexAttrib4ubv = (bool)this->_VertexAttrib4ubv;
    this->has.VertexAttrib4uiv = (bool)this->_VertexAttrib4uiv;
    this->has.VertexAttrib4usv = (bool)this->_VertexAttrib4usv;
    this->has.VertexAttribPointer = (bool)this->_VertexAttribPointer;
    this->has.UniformMatrix2x3fv = (bool)this->_UniformMatrix2x3fv;
    this->has.UniformMatrix3x2fv = (bool)this->_UniformMatrix3x2fv;
    this->has.UniformMatrix2x4fv = (bool)this->_UniformMatrix2x4fv;
    this->has.UniformMatrix4x2fv = (bool)this->_UniformMatrix4x2fv;
    this->has.UniformMatrix3x4fv = (bool)this->_UniformMatrix3x4fv;
    this->has.UniformMatrix4x3fv = (bool)this->_UniformMatrix4x3fv;
    this->has.ColorMaski = (bool)this->_ColorMaski;
    this->has.GetBooleani_v = (bool)this->_GetBooleani_v;
    this->has.GetIntegeri_v = (bool)this->_GetIntegeri_v;
    this->has.Enablei = (bool)this->_Enablei;
    this->has.Disablei = (bool)this->_Disablei;
    this->has.IsEnabledi = (bool)this->_IsEnabledi;
    this->has.BeginTransformFeedback = (bool)this->_BeginTransformFeedback;
    this->has.EndTransformFeedback = (bool)this->_EndTransformFeedback;
    this->has.BindBufferRange = (bool)this->_BindBufferRange;
    this->has.BindBufferBase = (bool)this->_BindBufferBase;
    this->has.TransformFeedbackVaryings = (bool)this->_TransformFeedbackVaryings;
    this->has.GetTransformFeedbackVarying = (bool)this->_GetTransformFeedbackVarying;
    this->has.ClampColor = (bool)this->_ClampColor;
    this->has.BeginConditionalRender = (bool)this->_BeginConditionalRender;
    this->has.EndConditionalRender = (bool)this->_EndConditionalRender;
    this->has.VertexAttribIPointer = (bool)this->_VertexAttribIPointer;
    this->has.GetVertexAttribIiv = (bool)this->_GetVertexAttribIiv;
    this->has.GetVertexAttribIuiv = (bool)this->_GetVertexAttribIuiv;
    this->has.VertexAttribI1i = (bool)this->_VertexAttribI1i;
    this->has.VertexAttribI2i = (bool)this->_VertexAttribI2i;
    this->has.VertexAttribI3i = (bool)this->_VertexAttribI3i;
    this->has.VertexAttribI4i = (bool)this->_VertexAttribI4i;
    this->has.VertexAttribI1ui = (bool)this->_VertexAttribI1ui;
    this->has.VertexAttribI2ui = (bool)this->_VertexAttribI2ui;
    this->has.VertexAttribI3ui = (bool)this->_VertexAttribI3ui;
    this->has.VertexAttribI4ui = (bool)this->_VertexAttribI4ui;
    this->has.VertexAttribI1iv = (bool)this->_VertexAttribI1iv;
    this->has.VertexAttribI2iv = (bool)this->_VertexAttribI2iv;
    this->has.VertexAttribI3iv = (bool)this->_VertexAttribI3iv;
    this->has.VertexAttribI4iv = (bool)this->_VertexAttribI4iv;
    this->has.VertexAttribI1uiv = (bool)this->_VertexAttribI1uiv;
    this->has.VertexAttribI2uiv = (bool)this->_VertexAttribI2uiv;
    this->has.VertexAttribI3uiv = (bool)this->_VertexAttribI3uiv;
    this->has.VertexAttribI4uiv = (bool)this->_VertexAttribI4uiv;
    this->has.VertexAttribI4bv = (bool)this->_VertexAttribI4bv;
    this->has.VertexAttribI4sv = (bool)this->_VertexAttribI4sv;
    this->has.VertexAttribI4ubv = (bool)this->_VertexAttribI4ubv;
    this->has.VertexAttribI4usv = (bool)this->_VertexAttribI4usv;
    this->has.GetUniformuiv = (bool)this->_GetUniformuiv;
    this->has.BindFragDataLocation = (bool)this->_BindFragDataLocation;
    this->has.GetFragDataLocation = (bool)this->_GetFragDataLocation;
    this->has.Uniform1ui = (bool)this->_Uniform1ui;
    this->has.Uniform2ui = (bool)this->_Uniform2ui;
    this->has.Uniform3ui = (bool)this->_Uniform3ui;
    this->has.Uniform4ui = (bool)this->_Uniform4ui;
    this->has.Uniform1uiv = (bool)this->_Uniform1uiv;
    this->has.Uniform2uiv = (bool)this->_Uniform2uiv;
    this->has.Uniform3uiv = (bool)this->_Uniform3uiv;
    this->has.Uniform4uiv = (bool)this->_Uniform4uiv;
    this->has.TexParameterIiv = (bool)this->_TexParameterIiv;
    this->has.TexParameterIuiv = (bool)this->_TexParameterIuiv;
    this->has.GetTexParameterIiv = (bool)this->_GetTexParameterIiv;
    this->has.GetTexParameterIuiv = (bool)this->_GetTexParameterIuiv;
    this->has.ClearBufferiv = (bool)this->_ClearBufferiv;
    this->has.ClearBufferuiv = (bool)this->_ClearBufferuiv;
    this->has.ClearBufferfv = (bool)this->_ClearBufferfv;
    this->has.ClearBufferfi = (bool)this->_ClearBufferfi;
    this->has.GetStringi = (bool)this->_GetStringi;
    this->has.IsRenderbuffer = (bool)this->_IsRenderbuffer;
    this->has.BindRenderbuffer = (bool)this->_BindRenderbuffer;
    this->has.DeleteRenderbuffers = (bool)this->_DeleteRenderbuffers;
    this->has.GenRenderbuffers = (bool)this->_GenRenderbuffers;
    this->has.RenderbufferStorage = (bool)this->_RenderbufferStorage;
    this->has.GetRenderbufferParameteriv = (bool)this->_GetRenderbufferParameteriv;
    this->has.IsFramebuffer = (bool)this->_IsFramebuffer;
    this->has.BindFramebuffer = (bool)this->_BindFramebuffer;
    this->has.DeleteFramebuffers = (bool)this->_DeleteFramebuffers;
    this->has.GenFramebuffers = (bool)this->_GenFramebuffers;
    this->has.CheckFramebufferStatus = (bool)this->_CheckFramebufferStatus;
    this->has.FramebufferTexture1D = (bool)this->_FramebufferTexture1D;
    this->has.FramebufferTexture2D = (bool)this->_FramebufferTexture2D;
    this->has.FramebufferTexture3D = (bool)this->_FramebufferTexture3D;
    this->has.FramebufferRenderbuffer = (bool)this->_FramebufferRenderbuffer;
    this->has.GetFramebufferAttachmentParameteriv = (bool)this->_GetFramebufferAttachmentParameteriv;
    this->has.GenerateMipmap = (bool)this->_GenerateMipmap;
    this->has.BlitFramebuffer = (bool)this->_BlitFramebuffer;
    this->has.RenderbufferStorageMultisample = (bool)this->_RenderbufferStorageMultisample;
    this->has.FramebufferTextureLayer = (bool)this->_FramebufferTextureLayer;
    this->has.MapBufferRange = (bool)this->_MapBufferRange;
    this->has.FlushMappedBufferRange = (bool)this->_FlushMappedBufferRange;
    this->has.BindVertexArray = (bool)this->_BindVertexArray;
    this->has.DeleteVertexArrays = (bool)this->_DeleteVertexArrays;
    this->has.GenVertexArrays = (bool)this->_GenVertexArrays;
    this->has.IsVertexArray = (bool)this->_IsVertexArray;
    this->has.DrawArraysInstanced = (bool)this->_DrawArraysInstanced;
    this->has.DrawElementsInstanced = (bool)this->_DrawElementsInstanced;
    this->has.TexBuffer = (bool)this->_TexBuffer;
    this->has.PrimitiveRestartIndex = (bool)this->_PrimitiveRestartIndex;
    this->has.CopyBufferSubData = (bool)this->_CopyBufferSubData;
    this->has.GetUniformIndices = (bool)this->_GetUniformIndices;
    this->has.GetActiveUniformsiv = (bool)this->_GetActiveUniformsiv;
    this->has.GetActiveUniformName = (bool)this->_GetActiveUniformName;
    this->has.GetUniformBlockIndex = (bool)this->_GetUniformBlockIndex;
    this->has.GetActiveUniformBlockiv = (bool)this->_GetActiveUniformBlockiv;
    this->has.GetActiveUniformBlockName = (bool)this->_GetActiveUniformBlockName;
    this->has.UniformBlockBinding = (bool)this->_UniformBlockBinding;
    this->has.DrawElementsBaseVertex = (bool)this->_DrawElementsBaseVertex;
    this->has.DrawRangeElementsBaseVertex = (bool)this->_DrawRangeElementsBaseVertex;
    this->has.DrawElementsInstancedBaseVertex = (bool)this->_DrawElementsInstancedBaseVertex;
    this->has.MultiDrawElementsBaseVertex = (bool)this->_MultiDrawElementsBaseVertex;
    this->has.ProvokingVertex = (bool)this->_ProvokingVertex;
    this->has.FenceSync = (bool)this->_FenceSync;
    this->has.IsSync = (bool)this->_IsSync;
    this->has.DeleteSync = (bool)this->_DeleteSync;
    this->has.ClientWaitSync = (bool)this->_ClientWaitSync;
    this->has.WaitSync = (bool)this->_WaitSync;
    this->has.GetInteger64v = (bool)this->_GetInteger64v;
    this->has.GetSynciv = (bool)this->_GetSynciv;
    this->has.GetInteger64i_v = (bool)this->_GetInteger64i_v;
    this->has.GetBufferParameteri64v = (bool)this->_GetBufferParameteri64v;
    this->has.FramebufferTexture = (bool)this->_FramebufferTexture;
    this->has.TexImage2DMultisample = (bool)this->_TexImage2DMultisample;
    this->has.TexImage3DMultisample = (bool)this->_TexImage3DMultisample;
    this->has.GetMultisamplefv = (bool)this->_GetMultisamplefv;
    this->has.SampleMaski = (bool)this->_SampleMaski;
    this->has.BindFragDataLocationIndexed = (bool)this->_BindFragDataLocationIndexed;
    this->has.GetFragDataIndex = (bool)this->_GetFragDataIndex;
    this->has.GenSamplers = (bool)this->_GenSamplers;
    this->has.DeleteSamplers = (bool)this->_DeleteSamplers;
    this->has.IsSampler = (bool)this->_IsSampler;
    this->has.BindSampler = (bool)this->_BindSampler;
    this->has.SamplerParameteri = (bool)this->_SamplerParameteri;
    this->has.SamplerParameteriv = (bool)this->_SamplerParameteriv;
    this->has.SamplerParameterf = (bool)this->_SamplerParameterf;
    this->has.SamplerParameterfv = (bool)this->_SamplerParameterfv;
    this->has.SamplerParameterIiv = (bool)this->_SamplerParameterIiv;
    this->has.SamplerParameterIuiv = (bool)this->_SamplerParameterIuiv;
    this->has.GetSamplerParameteriv = (bool)this->_GetSamplerParameteriv;
    this->has.GetSamplerParameterIiv = (bool)this->_GetSamplerParameterIiv;
    this->has.GetSamplerParameterfv = (bool)this->_GetSamplerParameterfv;
    this->has.GetSamplerParameterIuiv = (bool)this->_GetSamplerParameterIuiv;
    this->has.QueryCounter = (bool)this->_QueryCounter;
    this->has.GetQueryObjecti64v = (bool)this->_GetQueryObjecti64v;
    this->has.GetQueryObjectui64v = (bool)this->_GetQueryObjectui64v;
    this->has.VertexAttribDivisor = (bool)this->_VertexAttribDivisor;
    this->has.VertexAttribP1ui = (bool)this->_VertexAttribP1ui;
    this->has.VertexAttribP1uiv = (bool)this->_VertexAttribP1uiv;
    this->has.VertexAttribP2ui = (bool)this->_VertexAttribP2ui;
    this->has.VertexAttribP2uiv = (bool)this->_VertexAttribP2uiv;
    this->has.VertexAttribP3ui = (bool)this->_VertexAttribP3ui;
    this->has.VertexAttribP3uiv = (bool)this->_VertexAttribP3uiv;
    this->has.VertexAttribP4ui = (bool)this->_VertexAttribP4ui;
    this->has.VertexAttribP4uiv = (bool)this->_VertexAttribP4uiv;
    this->has.MinSampleShading = (bool)this->_MinSampleShading;
    this->has.BlendEquationi = (bool)this->_BlendEquationi;
    this->has.BlendEquationSeparatei = (bool)this->_BlendEquationSeparatei;
    this->has.BlendFunci = (bool)this->_BlendFunci;
    this->has.BlendFuncSeparatei = (bool)this->_BlendFuncSeparatei;
    this->has.DrawArraysIndirect = (bool)this->_DrawArraysIndirect;
    this->has.DrawElementsIndirect = (bool)this->_DrawElementsIndirect;
    this->has.Uniform1d = (bool)this->_Uniform1d;
    this->has.Uniform2d = (bool)this->_Uniform2d;
    this->has.Uniform3d = (bool)this->_Uniform3d;
    this->has.Uniform4d = (bool)this->_Uniform4d;
    this->has.Uniform1dv = (bool)this->_Uniform1dv;
    this->has.Uniform2dv = (bool)this->_Uniform2dv;
    this->has.Uniform3dv = (bool)this->_Uniform3dv;
    this->has.Uniform4dv = (bool)this->_Uniform4dv;
    this->has.UniformMatrix2dv = (bool)this->_UniformMatrix2dv;
    this->has.UniformMatrix3dv = (bool)this->_UniformMatrix3dv;
    this->has.UniformMatrix4dv = (bool)this->_UniformMatrix4dv;
    this->has.UniformMatrix2x3dv = (bool)this->_UniformMatrix2x3dv;
    this->has.UniformMatrix2x4dv = (bool)this->_UniformMatrix2x4dv;
    this->has.UniformMatrix3x2dv = (bool)this->_UniformMatrix3x2dv;
    this->has.UniformMatrix3x4dv = (bool)this->_UniformMatrix3x4dv;
    this->has.UniformMatrix4x2dv = (bool)this->_UniformMatrix4x2dv;
    this->has.UniformMatrix4x3dv = (bool)this->_UniformMatrix4x3dv;
    this->has.GetUniformdv = (bool)this->_GetUniformdv;
    this->has.GetSubroutineUniformLocation = (bool)this->_GetSubroutineUniformLocation;
    this->has.GetSubroutineIndex = (bool)this->_GetSubroutineIndex;
    this->has.GetActiveSubroutineUniformiv = (bool)this->_GetActiveSubroutineUniformiv;
    this->has.GetActiveSubroutineUniformName = (bool)this->_GetActiveSubroutineUniformName;
    this->has.GetActiveSubroutineName = (bool)this->_GetActiveSubroutineName;
    this->has.UniformSubroutinesuiv = (bool)this->_UniformSubroutinesuiv;
    this->has.GetUniformSubroutineuiv = (bool)this->_GetUniformSubroutineuiv;
    this->has.GetProgramStageiv = (bool)this->_GetProgramStageiv;
    this->has.PatchParameteri = (bool)this->_PatchParameteri;
    this->has.PatchParameterfv = (bool)this->_PatchParameterfv;
    this->has.BindTransformFeedback = (bool)this->_BindTransformFeedback;
    this->has.DeleteTransformFeedbacks = (bool)this->_DeleteTransformFeedbacks;
    this->has.GenTransformFeedbacks = (bool)this->_GenTransformFeedbacks;
    this->has.IsTransformFeedback = (bool)this->_IsTransformFeedback;
    this->has.PauseTransformFeedback = (bool)this->_PauseTransformFeedback;
    this->has.ResumeTransformFeedback = (bool)this->_ResumeTransformFeedback;
    this->has.DrawTransformFeedback = (bool)this->_DrawTransformFeedback;
    this->has.DrawTransformFeedbackStream = (bool)this->_DrawTransformFeedbackStream;
    this->has.BeginQueryIndexed = (bool)this->_BeginQueryIndexed;
    this->has.EndQueryIndexed = (bool)this->_EndQueryIndexed;
    this->has.GetQueryIndexediv = (bool)this->_GetQueryIndexediv;
    this->has.ReleaseShaderCompiler = (bool)this->_ReleaseShaderCompiler;
    this->has.ShaderBinary = (bool)this->_ShaderBinary;
    this->has.GetShaderPrecisionFormat = (bool)this->_GetShaderPrecisionFormat;
    this->has.DepthRangef = (bool)this->_DepthRangef;
    this->has.ClearDepthf = (bool)this->_ClearDepthf;
    this->has.GetProgramBinary = (bool)this->_GetProgramBinary;
    this->has.ProgramBinary = (bool)this->_ProgramBinary;
    this->has.ProgramParameteri = (bool)this->_ProgramParameteri;
    this->has.UseProgramStages = (bool)this->_UseProgramStages;
    this->has.ActiveShaderProgram = (bool)this->_ActiveShaderProgram;
    this->has.CreateShaderProgramv = (bool)this->_CreateShaderProgramv;
    this->has.BindProgramPipeline = (bool)this->_BindProgramPipeline;
    this->has.DeleteProgramPipelines = (bool)this->_DeleteProgramPipelines;
    this->has.GenProgramPipelines = (bool)this->_GenProgramPipelines;
    this->has.IsProgramPipeline = (bool)this->_IsProgramPipeline;
    this->has.GetProgramPipelineiv = (bool)this->_GetProgramPipelineiv;
    this->has.ProgramUniform1i = (bool)this->_ProgramUniform1i;
    this->has.ProgramUniform1iv = (bool)this->_ProgramUniform1iv;
    this->has.ProgramUniform1f = (bool)this->_ProgramUniform1f;
    this->has.ProgramUniform1fv = (bool)this->_ProgramUniform1fv;
    this->has.ProgramUniform1d = (bool)this->_ProgramUniform1d;
    this->has.ProgramUniform1dv = (bool)this->_ProgramUniform1dv;
    this->has.ProgramUniform1ui = (bool)this->_ProgramUniform1ui;
    this->has.ProgramUniform1uiv = (bool)this->_ProgramUniform1uiv;
    this->has.ProgramUniform2i = (bool)this->_ProgramUniform2i;
    this->has.ProgramUniform2iv = (bool)this->_ProgramUniform2iv;
    this->has.ProgramUniform2f = (bool)this->_ProgramUniform2f;
    this->has.ProgramUniform2fv = (bool)this->_ProgramUniform2fv;
    this->has.ProgramUniform2d = (bool)this->_ProgramUniform2d;
    this->has.ProgramUniform2dv = (bool)this->_ProgramUniform2dv;
    this->has.ProgramUniform2ui = (bool)this->_ProgramUniform2ui;
    this->has.ProgramUniform2uiv = (bool)this->_ProgramUniform2uiv;
    this->has.ProgramUniform3i = (bool)this->_ProgramUniform3i;
    this->has.ProgramUniform3iv = (bool)this->_ProgramUniform3iv;
    this->has.ProgramUniform3f = (bool)this->_ProgramUniform3f;
    this->has.ProgramUniform3fv = (bool)this->_ProgramUniform3fv;
    this->has.ProgramUniform3d = (bool)this->_ProgramUniform3d;
    this->has.ProgramUniform3dv = (bool)this->_ProgramUniform3dv;
    this->has.ProgramUniform3ui = (bool)this->_ProgramUniform3ui;
    this->has.ProgramUniform3uiv = (bool)this->_ProgramUniform3uiv;
    this->has.ProgramUniform4i = (bool)this->_ProgramUniform4i;
    this->has.ProgramUniform4iv = (bool)this->_ProgramUniform4iv;
    this->has.ProgramUniform4f = (bool)this->_ProgramUniform4f;
    this->has.ProgramUniform4fv = (bool)this->_ProgramUniform4fv;
    this->has.ProgramUniform4d = (bool)this->_ProgramUniform4d;
    this->has.ProgramUniform4dv = (bool)this->_ProgramUniform4dv;
    this->has.ProgramUniform4ui = (bool)this->_ProgramUniform4ui;
    this->has.ProgramUniform4uiv = (bool)this->_ProgramUniform4uiv;
    this->has.ProgramUniformMatrix2fv = (bool)this->_ProgramUniformMatrix2fv;
    this->has.ProgramUniformMatrix3fv = (bool)this->_ProgramUniformMatrix3fv;
    this->has.ProgramUniformMatrix4fv = (bool)this->_ProgramUniformMatrix4fv;
    this->has.ProgramUniformMatrix2dv = (bool)this->_ProgramUniformMatrix2dv;
    this->has.ProgramUniformMatrix3dv = (bool)this->_ProgramUniformMatrix3dv;
    this->has.ProgramUniformMatrix4dv = (bool)this->_ProgramUniformMatrix4dv;
    this->has.ProgramUniformMatrix2x3fv = (bool)this->_ProgramUniformMatrix2x3fv;
    this->has.ProgramUniformMatrix3x2fv = (bool)this->_ProgramUniformMatrix3x2fv;
    this->has.ProgramUniformMatrix2x4fv = (bool)this->_ProgramUniformMatrix2x4fv;
    this->has.ProgramUniformMatrix4x2fv = (bool)this->_ProgramUniformMatrix4x2fv;
    this->has.ProgramUniformMatrix3x4fv = (bool)this->_ProgramUniformMatrix3x4fv;
    this->has.ProgramUniformMatrix4x3fv = (bool)this->_ProgramUniformMatrix4x3fv;
    this->has.ProgramUniformMatrix2x3dv = (bool)this->_ProgramUniformMatrix2x3dv;
    this->has.ProgramUniformMatrix3x2dv = (bool)this->_ProgramUniformMatrix3x2dv;
    this->has.ProgramUniformMatrix2x4dv = (bool)this->_ProgramUniformMatrix2x4dv;
    this->has.ProgramUniformMatrix4x2dv = (bool)this->_ProgramUniformMatrix4x2dv;
    this->has.ProgramUniformMatrix3x4dv = (bool)this->_ProgramUniformMatrix3x4dv;
    this->has.ProgramUniformMatrix4x3dv = (bool)this->_ProgramUniformMatrix4x3dv;
    this->has.ValidateProgramPipeline = (bool)this->_ValidateProgramPipeline;
    this->has.GetProgramPipelineInfoLog = (bool)this->_GetProgramPipelineInfoLog;
    this->has.VertexAttribL1d = (bool)this->_VertexAttribL1d;
    this->has.VertexAttribL2d = (bool)this->_VertexAttribL2d;
    this->has.VertexAttribL3d = (bool)this->_VertexAttribL3d;
    this->has.VertexAttribL4d = (bool)this->_VertexAttribL4d;
    this->has.VertexAttribL1dv = (bool)this->_VertexAttribL1dv;
    this->has.VertexAttribL2dv = (bool)this->_VertexAttribL2dv;
    this->has.VertexAttribL3dv = (bool)this->_VertexAttribL3dv;
    this->has.VertexAttribL4dv = (bool)this->_VertexAttribL4dv;
    this->has.VertexAttribLPointer = (bool)this->_VertexAttribLPointer;
    this->has.GetVertexAttribLdv = (bool)this->_GetVertexAttribLdv;
    this->has.ViewportArrayv = (bool)this->_ViewportArrayv;
    this->has.ViewportIndexedf = (bool)this->_ViewportIndexedf;
    this->has.ViewportIndexedfv = (bool)this->_ViewportIndexedfv;
    this->has.ScissorArrayv = (bool)this->_ScissorArrayv;
    this->has.ScissorIndexed = (bool)this->_ScissorIndexed;
    this->has.ScissorIndexedv = (bool)this->_ScissorIndexedv;
    this->has.DepthRangeArrayv = (bool)this->_DepthRangeArrayv;
    this->has.DepthRangeIndexed = (bool)this->_DepthRangeIndexed;
    this->has.GetFloati_v = (bool)this->_GetFloati_v;
    this->has.GetDoublei_v = (bool)this->_GetDoublei_v;
    this->has.DrawArraysInstancedBaseInstance = (bool)this->_DrawArraysInstancedBaseInstance;
    this->has.DrawElementsInstancedBaseInstance = (bool)this->_DrawElementsInstancedBaseInstance;
    this->has.DrawElementsInstancedBaseVertexBaseInstance = (bool)this->_DrawElementsInstancedBaseVertexBaseInstance;
    this->has.GetInternalformativ = (bool)this->_GetInternalformativ;
    this->has.GetActiveAtomicCounterBufferiv = (bool)this->_GetActiveAtomicCounterBufferiv;
    this->has.BindImageTexture = (bool)this->_BindImageTexture;
    this->has.MemoryBarrier = (bool)this->_MemoryBarrier;
    this->has.TexStorage1D = (bool)this->_TexStorage1D;
    this->has.TexStorage2D = (bool)this->_TexStorage2D;
    this->has.TexStorage3D = (bool)this->_TexStorage3D;
    this->has.DrawTransformFeedbackInstanced = (bool)this->_DrawTransformFeedbackInstanced;
    this->has.DrawTransformFeedbackStreamInstanced = (bool)this->_DrawTransformFeedbackStreamInstanced;
    this->has.ClearBufferData = (bool)this->_ClearBufferData;
    this->has.ClearBufferSubData = (bool)this->_ClearBufferSubData;
    this->has.DispatchCompute = (bool)this->_DispatchCompute;
    this->has.DispatchComputeIndirect = (bool)this->_DispatchComputeIndirect;
    this->has.CopyImageSubData = (bool)this->_CopyImageSubData;
    this->has.FramebufferParameteri = (bool)this->_FramebufferParameteri;
    this->has.GetFramebufferParameteriv = (bool)this->_GetFramebufferParameteriv;
    this->has.GetInternalformati64v = (bool)this->_GetInternalformati64v;
    this->has.InvalidateTexSubImage = (bool)this->_InvalidateTexSubImage;
    this->has.InvalidateTexImage = (bool)this->_InvalidateTexImage;
    this->has.InvalidateBufferSubData = (bool)this->_InvalidateBufferSubData;
    this->has.InvalidateBufferData = (bool)this->_InvalidateBufferData;
    this->has.InvalidateFramebuffer = (bool)this->_InvalidateFramebuffer;
    this->has.InvalidateSubFramebuffer = (bool)this->_InvalidateSubFramebuffer;
    this->has.MultiDrawArraysIndirect = (bool)this->_MultiDrawArraysIndirect;
    this->has.MultiDrawElementsIndirect = (bool)this->_MultiDrawElementsIndirect;
    this->has.GetProgramInterfaceiv = (bool)this->_GetProgramInterfaceiv;
    this->has.GetProgramResourceIndex = (bool)this->_GetProgramResourceIndex;
    this->has.GetProgramResourceName = (bool)this->_GetProgramResourceName;
    this->has.GetProgramResourceiv = (bool)this->_GetProgramResourceiv;
    this->has.GetProgramResourceLocation = (bool)this->_GetProgramResourceLocation;
    this->has.GetProgramResourceLocationIndex = (bool)this->_GetProgramResourceLocationIndex;
    this->has.ShaderStorageBlockBinding = (bool)this->_ShaderStorageBlockBinding;
    this->has.TexBufferRange = (bool)this->_TexBufferRange;
    this->has.TexStorage2DMultisample = (bool)this->_TexStorage2DMultisample;
    this->has.TexStorage3DMultisample = (bool)this->_TexStorage3DMultisample;
    this->has.TextureView = (bool)this->_TextureView;
    this->has.BindVertexBuffer = (bool)this->_BindVertexBuffer;
    this->has.VertexAttribFormat = (bool)this->_VertexAttribFormat;
    this->has.VertexAttribIFormat = (bool)this->_VertexAttribIFormat;
    this->has.VertexAttribLFormat = (bool)this->_VertexAttribLFormat;
    this->has.VertexAttribBinding = (bool)this->_VertexAttribBinding;
    this->has.VertexBindingDivisor = (bool)this->_VertexBindingDivisor;
    this->has.DebugMessageControl = (bool)this->_DebugMessageControl;
    this->has.DebugMessageInsert = (bool)this->_DebugMessageInsert;
    this->has.DebugMessageCallback = (bool)this->_DebugMessageCallback;
    this->has.GetDebugMessageLog = (bool)this->_GetDebugMessageLog;
    this->has.PushDebugGroup = (bool)this->_PushDebugGroup;
    this->has.PopDebugGroup = (bool)this->_PopDebugGroup;
    this->has.ObjectLabel = (bool)this->_ObjectLabel;
    this->has.GetObjectLabel = (bool)this->_GetObjectLabel;
    this->has.ObjectPtrLabel = (bool)this->_ObjectPtrLabel;
    this->has.GetObjectPtrLabel = (bool)this->_GetObjectPtrLabel;
    this->has.BufferStorage = (bool)this->_BufferStorage;
    this->has.ClearTexImage = (bool)this->_ClearTexImage;
    this->has.ClearTexSubImage = (bool)this->_ClearTexSubImage;
    this->has.BindBuffersBase = (bool)this->_BindBuffersBase;
    this->has.BindBuffersRange = (bool)this->_BindBuffersRange;
    this->has.BindTextures = (bool)this->_BindTextures;
    this->has.BindSamplers = (bool)this->_BindSamplers;
    this->has.BindImageTextures = (bool)this->_BindImageTextures;
    this->has.BindVertexBuffers = (bool)this->_BindVertexBuffers;
    this->has.ClipControl = (bool)this->_ClipControl;
    this->has.CreateTransformFeedbacks = (bool)this->_CreateTransformFeedbacks;
    this->has.TransformFeedbackBufferBase = (bool)this->_TransformFeedbackBufferBase;
    this->has.TransformFeedbackBufferRange = (bool)this->_TransformFeedbackBufferRange;
    this->has.GetTransformFeedbackiv = (bool)this->_GetTransformFeedbackiv;
    this->has.GetTransformFeedbacki_v = (bool)this->_GetTransformFeedbacki_v;
    this->has.GetTransformFeedbacki64_v = (bool)this->_GetTransformFeedbacki64_v;
    this->has.CreateBuffers = (bool)this->_CreateBuffers;
    this->has.NamedBufferStorage = (bool)this->_NamedBufferStorage;
    this->has.NamedBufferData = (bool)this->_NamedBufferData;
    this->has.NamedBufferSubData = (bool)this->_NamedBufferSubData;
    this->has.CopyNamedBufferSubData = (bool)this->_CopyNamedBufferSubData;
    this->has.ClearNamedBufferData = (bool)this->_ClearNamedBufferData;
    this->has.ClearNamedBufferSubData = (bool)this->_ClearNamedBufferSubData;
    this->has.MapNamedBuffer = (bool)this->_MapNamedBuffer;
    this->has.MapNamedBufferRange = (bool)this->_MapNamedBufferRange;
    this->has.UnmapNamedBuffer = (bool)this->_UnmapNamedBuffer;
    this->has.FlushMappedNamedBufferRange = (bool)this->_FlushMappedNamedBufferRange;
    this->has.GetNamedBufferParameteriv = (bool)this->_GetNamedBufferParameteriv;
    this->has.GetNamedBufferParameteri64v = (bool)this->_GetNamedBufferParameteri64v;
    this->has.GetNamedBufferPointerv = (bool)this->_GetNamedBufferPointerv;
    this->has.GetNamedBufferSubData = (bool)this->_GetNamedBufferSubData;
    this->has.CreateFramebuffers = (bool)this->_CreateFramebuffers;
    this->has.NamedFramebufferRenderbuffer = (bool)this->_NamedFramebufferRenderbuffer;
    this->has.NamedFramebufferParameteri = (bool)this->_NamedFramebufferParameteri;
    this->has.NamedFramebufferTexture = (bool)this->_NamedFramebufferTexture;
    this->has.NamedFramebufferTextureLayer = (bool)this->_NamedFramebufferTextureLayer;
    this->has.NamedFramebufferDrawBuffer = (bool)this->_NamedFramebufferDrawBuffer;
    this->has.NamedFramebufferDrawBuffers = (bool)this->_NamedFramebufferDrawBuffers;
    this->has.NamedFramebufferReadBuffer = (bool)this->_NamedFramebufferReadBuffer;
    this->has.InvalidateNamedFramebufferData = (bool)this->_InvalidateNamedFramebufferData;
    this->has.InvalidateNamedFramebufferSubData = (bool)this->_InvalidateNamedFramebufferSubData;
    this->has.ClearNamedFramebufferiv = (bool)this->_ClearNamedFramebufferiv;
    this->has.ClearNamedFramebufferuiv = (bool)this->_ClearNamedFramebufferuiv;
    this->has.ClearNamedFramebufferfv = (bool)this->_ClearNamedFramebufferfv;
    this->has.ClearNamedFramebufferfi = (bool)this->_ClearNamedFramebufferfi;
    this->has.BlitNamedFramebuffer = (bool)this->_BlitNamedFramebuffer;
    this->has.CheckNamedFramebufferStatus = (bool)this->_CheckNamedFramebufferStatus;
    this->has.GetNamedFramebufferParameteriv = (bool)this->_GetNamedFramebufferParameteriv;
    this->has.GetNamedFramebufferAttachmentParameteriv = (bool)this->_GetNamedFramebufferAttachmentParameteriv;
    this->has.CreateRenderbuffers = (bool)this->_CreateRenderbuffers;
    this->has.NamedRenderbufferStorage = (bool)this->_NamedRenderbufferStorage;
    this->has.NamedRenderbufferStorageMultisample = (bool)this->_NamedRenderbufferStorageMultisample;
    this->has.GetNamedRenderbufferParameteriv = (bool)this->_GetNamedRenderbufferParameteriv;
    this->has.CreateTextures = (bool)this->_CreateTextures;
    this->has.TextureBuffer = (bool)this->_TextureBuffer;
    this->has.TextureBufferRange = (bool)this->_TextureBufferRange;
    this->has.TextureStorage1D = (bool)this->_TextureStorage1D;
    this->has.TextureStorage2D = (bool)this->_TextureStorage2D;
    this->has.TextureStorage3D = (bool)this->_TextureStorage3D;
    this->has.TextureStorage2DMultisample = (bool)this->_TextureStorage2DMultisample;
    this->has.TextureStorage3DMultisample = (bool)this->_TextureStorage3DMultisample;
    this->has.TextureSubImage1D = (bool)this->_TextureSubImage1D;
    this->has.TextureSubImage2D = (bool)this->_TextureSubImage2D;
    this->has.TextureSubImage3D = (bool)this->_TextureSubImage3D;
    this->has.CompressedTextureSubImage1D = (bool)this->_CompressedTextureSubImage1D;
    this->has.CompressedTextureSubImage2D = (bool)this->_CompressedTextureSubImage2D;
    this->has.CompressedTextureSubImage3D = (bool)this->_CompressedTextureSubImage3D;
    this->has.CopyTextureSubImage1D = (bool)this->_CopyTextureSubImage1D;
    this->has.CopyTextureSubImage2D = (bool)this->_CopyTextureSubImage2D;
    this->has.CopyTextureSubImage3D = (bool)this->_CopyTextureSubImage3D;
    this->has.TextureParameterf = (bool)this->_TextureParameterf;
    this->has.TextureParameterfv = (bool)this->_TextureParameterfv;
    this->has.TextureParameteri = (bool)this->_TextureParameteri;
    this->has.TextureParameterIiv = (bool)this->_TextureParameterIiv;
    this->has.TextureParameterIuiv = (bool)this->_TextureParameterIuiv;
    this->has.TextureParameteriv = (bool)this->_TextureParameteriv;
    this->has.GenerateTextureMipmap = (bool)this->_GenerateTextureMipmap;
    this->has.BindTextureUnit = (bool)this->_BindTextureUnit;
    this->has.GetTextureImage = (bool)this->_GetTextureImage;
    this->has.GetCompressedTextureImage = (bool)this->_GetCompressedTextureImage;
    this->has.GetTextureLevelParameterfv = (bool)this->_GetTextureLevelParameterfv;
    this->has.GetTextureLevelParameteriv = (bool)this->_GetTextureLevelParameteriv;
    this->has.GetTextureParameterfv = (bool)this->_GetTextureParameterfv;
    this->has.GetTextureParameterIiv = (bool)this->_GetTextureParameterIiv;
    this->has.GetTextureParameterIuiv = (bool)this->_GetTextureParameterIuiv;
    this->has.GetTextureParameteriv = (bool)this->_GetTextureParameteriv;
    this->has.CreateVertexArrays = (bool)this->_CreateVertexArrays;
    this->has.DisableVertexArrayAttrib = (bool)this->_DisableVertexArrayAttrib;
    this->has.EnableVertexArrayAttrib = (bool)this->_EnableVertexArrayAttrib;
    this->has.VertexArrayElementBuffer = (bool)this->_VertexArrayElementBuffer;
    this->has.VertexArrayVertexBuffer = (bool)this->_VertexArrayVertexBuffer;
    this->has.VertexArrayVertexBuffers = (bool)this->_VertexArrayVertexBuffers;
    this->has.VertexArrayAttribBinding = (bool)this->_VertexArrayAttribBinding;
    this->has.VertexArrayAttribFormat = (bool)this->_VertexArrayAttribFormat;
    this->has.VertexArrayAttribIFormat = (bool)this->_VertexArrayAttribIFormat;
    this->has.VertexArrayAttribLFormat = (bool)this->_VertexArrayAttribLFormat;
    this->has.VertexArrayBindingDivisor = (bool)this->_VertexArrayBindingDivisor;
    this->has.GetVertexArrayiv = (bool)this->_GetVertexArrayiv;
    this->has.GetVertexArrayIndexediv = (bool)this->_GetVertexArrayIndexediv;
    this->has.GetVertexArrayIndexed64iv = (bool)this->_GetVertexArrayIndexed64iv;
    this->has.CreateSamplers = (bool)this->_CreateSamplers;
    this->has.CreateProgramPipelines = (bool)this->_CreateProgramPipelines;
    this->has.CreateQueries = (bool)this->_CreateQueries;
    this->has.GetQueryBufferObjecti64v = (bool)this->_GetQueryBufferObjecti64v;
    this->has.GetQueryBufferObjectiv = (bool)this->_GetQueryBufferObjectiv;
    this->has.GetQueryBufferObjectui64v = (bool)this->_GetQueryBufferObjectui64v;
    this->has.GetQueryBufferObjectuiv = (bool)this->_GetQueryBufferObjectuiv;
    this->has.MemoryBarrierByRegion = (bool)this->_MemoryBarrierByRegion;
    this->has.GetTextureSubImage = (bool)this->_GetTextureSubImage;
    this->has.GetCompressedTextureSubImage = (bool)this->_GetCompressedTextureSubImage;
    this->has.GetGraphicsResetStatus = (bool)this->_GetGraphicsResetStatus;
    this->has.GetnCompressedTexImage = (bool)this->_GetnCompressedTexImage;
    this->has.GetnTexImage = (bool)this->_GetnTexImage;
    this->has.GetnUniformdv = (bool)this->_GetnUniformdv;
    this->has.GetnUniformfv = (bool)this->_GetnUniformfv;
    this->has.GetnUniformiv = (bool)this->_GetnUniformiv;
    this->has.GetnUniformuiv = (bool)this->_GetnUniformuiv;
    this->has.ReadnPixels = (bool)this->_ReadnPixels;
    this->has.TextureBarrier = (bool)this->_TextureBarrier;
    this->has.SpecializeShader = (bool)this->_SpecializeShader;
    this->has.MultiDrawArraysIndirectCount = (bool)this->_MultiDrawArraysIndirectCount;
    this->has.MultiDrawElementsIndirectCount = (bool)this->_MultiDrawElementsIndirectCount;
    this->has.PolygonOffsetClamp = (bool)this->_PolygonOffsetClamp;

    return true;
}
