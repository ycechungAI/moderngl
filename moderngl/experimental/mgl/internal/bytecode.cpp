#include "bytecode.hpp"

#include <memory.h>

namespace MGLBytecode {

bool error;
char * buffer;
char * ptr;
GLMethods rec;
GLMethods gl;

template <typename T>
inline void write(const T & value) {
    *(T *)ptr = value;
    ptr += sizeof(T);
}

template <>
inline void write(const unsigned char & value) {
    *(unsigned *)ptr = value;
    ptr += 4;
}

template <typename T>
inline void write(const T * array, int count) {
    memcpy(ptr, array, sizeof(T) * count);
    ptr += sizeof(T) * count;
}

template <typename T>
inline T read() {
    ptr += sizeof(T);
    return *(T *)(ptr - sizeof(T));
}

template <>
inline unsigned char read() {
    ptr += 4;
    return *(unsigned char *)(ptr - 4);
}

template <typename T>
inline T * read(int count) {
    ptr += sizeof(T) * count;
    return (T *)(ptr - sizeof(T) * count);
}

void GLAPI rec_glActiveTexture(GLenum texture) {
    write(OP_glActiveTexture);
    write(texture);
}

void GLAPI rec_glBeginConditionalRender(GLuint id, GLenum mode) {
    write(OP_glBeginConditionalRender);
    write(mode);
    write(id);
}

void GLAPI rec_glBeginQuery(GLenum target, GLuint id) {
    write(OP_glBeginQuery);
    write(id);
    write(target);
}

void GLAPI rec_glBeginTransformFeedback(GLenum primitiveMode) {
    write(OP_glBeginTransformFeedback);
    write(primitiveMode);
}

void GLAPI rec_glBindBuffer(GLenum target, GLuint buffer) {
    write(OP_glBindBuffer);
    write(buffer);
    write(target);
}

void GLAPI rec_glBindBufferBase(GLenum target, GLuint index, GLuint buffer) {
    write(OP_glBindBufferBase);
    write(buffer);
    write(index);
    write(target);
}

void GLAPI rec_glBindBufferRange(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size) {
    write(OP_glBindBufferRange);
    write(size);
    write(offset);
    write(buffer);
    write(index);
    write(target);
}

void GLAPI rec_glBindFramebuffer(GLenum target, GLuint framebuffer) {
    write(OP_glBindFramebuffer);
    write(framebuffer);
    write(target);
}

void GLAPI rec_glBindSampler(GLuint unit, GLuint sampler) {
    write(OP_glBindSampler);
    write(sampler);
    write(unit);
}

void GLAPI rec_glBindTexture(GLenum target, GLuint texture) {
    write(OP_glBindTexture);
    write(texture);
    write(target);
}

void GLAPI rec_glBindVertexArray(GLuint array) {
    write(OP_glBindVertexArray);
    write(array);
}

void GLAPI rec_glBlendFunc(GLenum sfactor, GLenum dfactor) {
    write(OP_glBlendFunc);
    write(dfactor);
    write(sfactor);
}

void GLAPI rec_glBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const void * data) {
    int data_size = (int)((size + 3) & ~0x3);
    write(OP_glBufferSubData);
    write(data_size);
    write((unsigned *)data, data_size >> 2);
    write(size);
    write(offset);
    write(target);
}

void GLAPI rec_glClearBufferData(GLenum target, GLenum internalformat, GLenum format, GLenum type, const void * data) {
    write(OP_glClearBufferData);
    write((unsigned *)data, 1);
    write(type);
    write(format);
    write(internalformat);
    write(target);
}

void GLAPI rec_glClearBufferfv(GLenum buffer, GLint drawbuffer, const GLfloat * value) {
    write(OP_glClearBufferfv);
    write(value, 4);
    write(drawbuffer);
    write(buffer);
}

void GLAPI rec_glClearBufferiv(GLenum buffer, GLint drawbuffer, const GLint * value) {
    write(OP_glClearBufferiv);
    write(value, 4);
    write(drawbuffer);
    write(buffer);
}

void GLAPI rec_glClearBufferuiv(GLenum buffer, GLint drawbuffer, const GLuint * value) {
    write(OP_glClearBufferuiv);
    write(value, 4);
    write(drawbuffer);
    write(buffer);
}

void GLAPI rec_glColorMaski(GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a) {
    write(OP_glColorMaski);
    write(a);
    write(b);
    write(g);
    write(r);
    write(index);
}

void GLAPI rec_glCopyBufferSubData(GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size) {
    write(OP_glCopyBufferSubData);
    write(size);
    write(writeOffset);
    write(readOffset);
    write(writeTarget);
    write(readTarget);
}

void GLAPI rec_glDepthMask(GLboolean flag) {
    write(OP_glDepthMask);
    write(flag);
}

void GLAPI rec_glDisable(GLenum cap) {
    write(OP_glDisable);
    write(cap);
}

void GLAPI rec_glDispatchCompute(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z) {
    write(OP_glDispatchCompute);
    write(num_groups_z);
    write(num_groups_y);
    write(num_groups_x);
}

void GLAPI rec_glDrawArraysInstanced(GLenum mode, GLint first, GLsizei count, GLsizei instancecount) {
    write(OP_glDrawArraysInstanced);
    write(instancecount);
    write(count);
    write(first);
    write(mode);
}

void GLAPI rec_glDrawElementsInstanced(GLenum mode, GLsizei count, GLenum type, const void * indices, GLsizei instancecount) {
    write(OP_glDrawElementsInstanced);
    write(instancecount);
    write(indices);
    write(type);
    write(count);
    write(mode);
}

void GLAPI rec_glEnable(GLenum cap) {
    write(OP_glEnable);
    write(cap);
}

void GLAPI rec_glEndConditionalRender() {
    write(OP_glEndConditionalRender);
}

void GLAPI rec_glEndQuery(GLenum target) {
    write(OP_glEndQuery);
    write(target);
}

void GLAPI rec_glEndTransformFeedback() {
    write(OP_glEndTransformFeedback);
}

void GLAPI rec_glFlush() {
    write(OP_glFlush);
}

void GLAPI rec_glPixelStorei(GLenum pname, GLint param) {
    write(OP_glPixelStorei);
    write(param);
    write(pname);
}

void GLAPI rec_glSamplerParameteri(GLuint sampler, GLenum pname, GLint param) {
    write(OP_glSamplerParameteri);
    write(param);
    write(pname);
    write(sampler);
}

void GLAPI rec_glScissor(GLint x, GLint y, GLsizei width, GLsizei height) {
    write(OP_glScissor);
    write(height);
    write(width);
    write(y);
    write(x);
}

void GLAPI rec_glTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void * pixels) {
    error = true;
/*
    write(OP_glTexSubImage2D);
    // TODO: is_null + size
    write(pixels);
    write(type);
    write(format);
    write(height);
    write(width);
    write(yoffset);
    write(xoffset);
    write(level);
    write(target);
*/
}

void GLAPI rec_glTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void * pixels) {
    error = true;
/*
    write(OP_glTexSubImage3D);
    // TODO: is_null + size
    write(pixels);
    write(type);
    write(format);
    write(depth);
    write(height);
    write(width);
    write(zoffset);
    write(yoffset);
    write(xoffset);
    write(level);
    write(target);
*/
}

void GLAPI rec_glUseProgram(GLuint program) {
    write(OP_glUseProgram);
    write(program);
}

void GLAPI rec_glViewport(GLint x, GLint y, GLsizei width, GLsizei height) {
    error = true;
/*
    write(OP_glViewport);
    write(height);
    write(width);
    write(y);
    write(x);
*/
}

void GLAPI rec_glUniform1dv(GLint location, GLsizei count, const GLdouble * value) {
    int value_size = (int)(count * 1);
    write(OP_glUniform);
    write(OP_glUniform1dv);
    write(value_size);
    write(value, value_size);
    write(count);
    write(location);
}

void GLAPI rec_glUniform1fv(GLint location, GLsizei count, const GLfloat * value) {
    int value_size = (int)(count * 1);
    write(OP_glUniform);
    write(OP_glUniform1fv);
    write(value_size);
    write(value, value_size);
    write(count);
    write(location);
}

void GLAPI rec_glUniform1iv(GLint location, GLsizei count, const GLint * value) {
    int value_size = (int)(count * 1);
    write(OP_glUniform);
    write(OP_glUniform1iv);
    write(value_size);
    write(value, value_size);
    write(count);
    write(location);
}

void GLAPI rec_glUniform1uiv(GLint location, GLsizei count, const GLuint * value) {
    int value_size = (int)(count * 1);
    write(OP_glUniform);
    write(OP_glUniform1uiv);
    write(value_size);
    write(value, value_size);
    write(count);
    write(location);
}

void GLAPI rec_glUniform2dv(GLint location, GLsizei count, const GLdouble * value) {
    int value_size = (int)(count * 2);
    write(OP_glUniform);
    write(OP_glUniform2dv);
    write(value_size);
    write(value, value_size);
    write(count);
    write(location);
}

void GLAPI rec_glUniform2fv(GLint location, GLsizei count, const GLfloat * value) {
    int value_size = (int)(count * 2);
    write(OP_glUniform);
    write(OP_glUniform2fv);
    write(value_size);
    write(value, value_size);
    write(count);
    write(location);
}

void GLAPI rec_glUniform2iv(GLint location, GLsizei count, const GLint * value) {
    int value_size = (int)(count * 2);
    write(OP_glUniform);
    write(OP_glUniform2iv);
    write(value_size);
    write(value, value_size);
    write(count);
    write(location);
}

void GLAPI rec_glUniform2uiv(GLint location, GLsizei count, const GLuint * value) {
    int value_size = (int)(count * 2);
    write(OP_glUniform);
    write(OP_glUniform2uiv);
    write(value_size);
    write(value, value_size);
    write(count);
    write(location);
}

void GLAPI rec_glUniform3dv(GLint location, GLsizei count, const GLdouble * value) {
    int value_size = (int)(count * 3);
    write(OP_glUniform);
    write(OP_glUniform3dv);
    write(value_size);
    write(value, value_size);
    write(count);
    write(location);
}

void GLAPI rec_glUniform3fv(GLint location, GLsizei count, const GLfloat * value) {
    int value_size = (int)(count * 3);
    write(OP_glUniform);
    write(OP_glUniform3fv);
    write(value_size);
    write(value, value_size);
    write(count);
    write(location);
}

void GLAPI rec_glUniform3iv(GLint location, GLsizei count, const GLint * value) {
    int value_size = (int)(count * 3);
    write(OP_glUniform);
    write(OP_glUniform3iv);
    write(value_size);
    write(value, value_size);
    write(count);
    write(location);
}

void GLAPI rec_glUniform3uiv(GLint location, GLsizei count, const GLuint * value) {
    int value_size = (int)(count * 3);
    write(OP_glUniform);
    write(OP_glUniform3uiv);
    write(value_size);
    write(value, value_size);
    write(count);
    write(location);
}

void GLAPI rec_glUniform4dv(GLint location, GLsizei count, const GLdouble * value) {
    int value_size = (int)(count * 4);
    write(OP_glUniform);
    write(OP_glUniform4dv);
    write(value_size);
    write(value, value_size);
    write(count);
    write(location);
}

void GLAPI rec_glUniform4fv(GLint location, GLsizei count, const GLfloat * value) {
    int value_size = (int)(count * 4);
    write(OP_glUniform);
    write(OP_glUniform4fv);
    write(value_size);
    write(value, value_size);
    write(count);
    write(location);
}

void GLAPI rec_glUniform4iv(GLint location, GLsizei count, const GLint * value) {
    int value_size = (int)(count * 4);
    write(OP_glUniform);
    write(OP_glUniform4iv);
    write(value_size);
    write(value, value_size);
    write(count);
    write(location);
}

void GLAPI rec_glUniform4uiv(GLint location, GLsizei count, const GLuint * value) {
    int value_size = (int)(count * 4);
    write(OP_glUniform);
    write(OP_glUniform4uiv);
    write(value_size);
    write(value, value_size);
    write(count);
    write(location);
}

void GLAPI rec_glUniformBlockBinding(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding) {
    write(OP_glUniform);
    write(OP_glUniformBlockBinding);
    write(uniformBlockBinding);
    write(uniformBlockIndex);
    write(program);
}

void GLAPI rec_glUniformMatrix2dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value) {
    int value_size = (int)(count * 4);
    write(OP_glUniform);
    write(OP_glUniformMatrix2dv);
    write(value_size);
    write(value, value_size);
    write(transpose);
    write(count);
    write(location);
}

void GLAPI rec_glUniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value) {
    int value_size = (int)(count * 4);
    write(OP_glUniform);
    write(OP_glUniformMatrix2fv);
    write(value_size);
    write(value, value_size);
    write(transpose);
    write(count);
    write(location);
}

void GLAPI rec_glUniformMatrix2x3dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value) {
    int value_size = (int)(count * 6);
    write(OP_glUniform);
    write(OP_glUniformMatrix2x3dv);
    write(value_size);
    write(value, value_size);
    write(transpose);
    write(count);
    write(location);
}

void GLAPI rec_glUniformMatrix2x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value) {
    int value_size = (int)(count * 6);
    write(OP_glUniform);
    write(OP_glUniformMatrix2x3fv);
    write(value_size);
    write(value, value_size);
    write(transpose);
    write(count);
    write(location);
}

void GLAPI rec_glUniformMatrix2x4dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value) {
    int value_size = (int)(count * 8);
    write(OP_glUniform);
    write(OP_glUniformMatrix2x4dv);
    write(value_size);
    write(value, value_size);
    write(transpose);
    write(count);
    write(location);
}

void GLAPI rec_glUniformMatrix2x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value) {
    int value_size = (int)(count * 8);
    write(OP_glUniform);
    write(OP_glUniformMatrix2x4fv);
    write(value_size);
    write(value, value_size);
    write(transpose);
    write(count);
    write(location);
}

void GLAPI rec_glUniformMatrix3dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value) {
    int value_size = (int)(count * 9);
    write(OP_glUniform);
    write(OP_glUniformMatrix3dv);
    write(value_size);
    write(value, value_size);
    write(transpose);
    write(count);
    write(location);
}

void GLAPI rec_glUniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value) {
    int value_size = (int)(count * 9);
    write(OP_glUniform);
    write(OP_glUniformMatrix3fv);
    write(value_size);
    write(value, value_size);
    write(transpose);
    write(count);
    write(location);
}

void GLAPI rec_glUniformMatrix3x2dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value) {
    int value_size = (int)(count * 6);
    write(OP_glUniform);
    write(OP_glUniformMatrix3x2dv);
    write(value_size);
    write(value, value_size);
    write(transpose);
    write(count);
    write(location);
}

void GLAPI rec_glUniformMatrix3x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value) {
    int value_size = (int)(count * 6);
    write(OP_glUniform);
    write(OP_glUniformMatrix3x2fv);
    write(value_size);
    write(value, value_size);
    write(transpose);
    write(count);
    write(location);
}

void GLAPI rec_glUniformMatrix3x4dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value) {
    int value_size = (int)(count * 12);
    write(OP_glUniform);
    write(OP_glUniformMatrix3x4dv);
    write(value_size);
    write(value, value_size);
    write(transpose);
    write(count);
    write(location);
}

void GLAPI rec_glUniformMatrix3x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value) {
    int value_size = (int)(count * 12);
    write(OP_glUniform);
    write(OP_glUniformMatrix3x4fv);
    write(value_size);
    write(value, value_size);
    write(transpose);
    write(count);
    write(location);
}

void GLAPI rec_glUniformMatrix4dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value) {
    int value_size = (int)(count * 16);
    write(OP_glUniform);
    write(OP_glUniformMatrix4dv);
    write(value_size);
    write(value, value_size);
    write(transpose);
    write(count);
    write(location);
}

void GLAPI rec_glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value) {
    int value_size = (int)(count * 16);
    write(OP_glUniform);
    write(OP_glUniformMatrix4fv);
    write(value_size);
    write(value, value_size);
    write(transpose);
    write(count);
    write(location);
}

void GLAPI rec_glUniformMatrix4x2dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value) {
    int value_size = (int)(count * 8);
    write(OP_glUniform);
    write(OP_glUniformMatrix4x2dv);
    write(value_size);
    write(value, value_size);
    write(transpose);
    write(count);
    write(location);
}

void GLAPI rec_glUniformMatrix4x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value) {
    int value_size = (int)(count * 8);
    write(OP_glUniform);
    write(OP_glUniformMatrix4x2fv);
    write(value_size);
    write(value, value_size);
    write(transpose);
    write(count);
    write(location);
}

void GLAPI rec_glUniformMatrix4x3dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value) {
    int value_size = (int)(count * 12);
    write(OP_glUniform);
    write(OP_glUniformMatrix4x3dv);
    write(value_size);
    write(value, value_size);
    write(transpose);
    write(count);
    write(location);
}

void GLAPI rec_glUniformMatrix4x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value) {
    int value_size = (int)(count * 12);
    write(OP_glUniform);
    write(OP_glUniformMatrix4x3fv);
    write(value_size);
    write(value, value_size);
    write(transpose);
    write(count);
    write(location);
}

void GLAPI rec_glGetQueryObjectiv(GLuint id, GLenum pname, GLint * params) {
}

void GLAPI rec_glAttachShader(GLuint program, GLuint shader) {
    error = true;
}

void GLAPI rec_glBindRenderbuffer(GLenum target, GLuint renderbuffer) {
    error = true;
}

void GLAPI rec_glBufferData(GLenum target, GLsizeiptr size, const void * data, GLenum usage) {
    error = true;
}

void GLAPI rec_glBufferStorage(GLenum target, GLsizeiptr size, const void * data, GLbitfield flags) {
    error = true;
}

GLenum GLAPI rec_glCheckFramebufferStatus(GLenum target) {
    error = true;
    return 0;
}

void GLAPI rec_glCompileShader(GLuint shader) {
    error = true;
}

GLuint GLAPI rec_glCreateShader(GLenum type) {
    error = true;
    return 0;
}

void GLAPI rec_glDeleteBuffers(GLsizei n, const GLuint * buffers) {
    error = true;
}

void GLAPI rec_glDeleteFramebuffers(GLsizei n, const GLuint * framebuffers) {
    error = true;
}

void GLAPI rec_glDeleteProgram(GLuint program) {
    error = true;
}

void GLAPI rec_glDeleteQueries(GLsizei n, const GLuint * ids) {
    error = true;
}

void GLAPI rec_glDeleteRenderbuffers(GLsizei n, const GLuint * renderbuffers) {
    error = true;
}

void GLAPI rec_glDeleteSamplers(GLsizei count, const GLuint * samplers) {
    error = true;
}

void GLAPI rec_glDeleteShader(GLuint shader) {
    error = true;
}

void GLAPI rec_glDeleteTextures(GLsizei n, const GLuint * textures) {
    error = true;
}

void GLAPI rec_glDeleteVertexArrays(GLsizei n, const GLuint * arrays) {
    error = true;
}

void GLAPI rec_glEnableVertexAttribArray(GLuint index) {
    error = true;
}

void GLAPI rec_glFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer) {
    error = true;
}

void GLAPI rec_glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level) {
    error = true;
}

void GLAPI rec_glGenBuffers(GLsizei n, GLuint * buffers) {
    error = true;
}

void GLAPI rec_glGenFramebuffers(GLsizei n, GLuint * framebuffers) {
    error = true;
}

void GLAPI rec_glGenQueries(GLsizei n, GLuint * ids) {
    error = true;
}

void GLAPI rec_glGenRenderbuffers(GLsizei n, GLuint * renderbuffers) {
    error = true;
}

void GLAPI rec_glGenSamplers(GLsizei count, GLuint * samplers) {
    error = true;
}

void GLAPI rec_glGenTextures(GLsizei n, GLuint * textures) {
    error = true;
}

void GLAPI rec_glGenVertexArrays(GLsizei n, GLuint * arrays) {
    error = true;
}

void GLAPI rec_glGetActiveAttrib(GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLint * size, GLenum * type, GLchar * name) {
    error = true;
}

void GLAPI rec_glGetActiveUniform(GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLint * size, GLenum * type, GLchar * name) {
    error = true;
}

void GLAPI rec_glGetActiveUniformBlockName(GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei * length, GLchar * uniformBlockName) {
    error = true;
}

void GLAPI rec_glGetActiveUniformBlockiv(GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint * params) {
    error = true;
}

GLint GLAPI rec_glGetAttribLocation(GLuint program, const GLchar * name) {
    error = true;
    return -1;
}

void GLAPI rec_glGetFloatv(GLenum pname, GLfloat * data) {
    error = true;
}

void GLAPI rec_glGetInteger64v(GLenum pname, GLint64 * data) {
    error = true;
}

void GLAPI rec_glGetIntegeri_v(GLenum target, GLuint index, GLint * data) {
    error = true;
}

void GLAPI rec_glGetIntegerv(GLenum pname, GLint * data) {
    error = true;
}

void GLAPI rec_glGetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei * length, GLchar * infoLog) {
    error = true;
}

void GLAPI rec_glGetProgramiv(GLuint program, GLenum pname, GLint * params) {
    error = true;
}

void GLAPI rec_glGetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei * length, GLchar * infoLog) {
    error = true;
}

void GLAPI rec_glGetShaderiv(GLuint shader, GLenum pname, GLint * params) {
    error = true;
}

const GLubyte * GLAPI rec_glGetString(GLenum name) {
    error = true;
    return 0;
}

const GLubyte * GLAPI rec_glGetStringi(GLenum name, GLuint index) {
    error = true;
    return 0;
}

GLuint GLAPI rec_glGetUniformBlockIndex(GLuint program, const GLchar * uniformBlockName) {
    error = true;
    return 0xffffffff;
}

GLint GLAPI rec_glGetUniformLocation(GLuint program, const GLchar * name) {
    error = true;
    return 0;
}

void GLAPI rec_glGetUniformdv(GLuint program, GLint location, GLdouble * params) {
    error = true;
}

void GLAPI rec_glGetUniformfv(GLuint program, GLint location, GLfloat * params) {
    error = true;
}

void GLAPI rec_glGetUniformiv(GLuint program, GLint location, GLint * params) {
    error = true;
}

void GLAPI rec_glGetUniformuiv(GLuint program, GLint location, GLuint * params) {
    error = true;
}

void GLAPI rec_glLinkProgram(GLuint program) {
    error = true;
}

void * GLAPI rec_glMapBuffer(GLenum target, GLenum access) {
    error = true;
    return 0;
}

void * GLAPI rec_glMapBufferRange(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access) {
    error = true;
    return 0;
}

void GLAPI rec_glPrimitiveRestartIndex(GLuint index) {
    error = true;
}

void GLAPI rec_glReadBuffer(GLenum src) {
    error = true;
}

void GLAPI rec_glReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void * pixels) {
    error = true;
}

void GLAPI rec_glRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height) {
    error = true;
}

void GLAPI rec_glRenderbufferStorageMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height) {
    error = true;
}

void GLAPI rec_glShaderSource(GLuint shader, GLsizei count, const GLchar * const * string, const GLint * length) {
    error = true;
}

void GLAPI rec_glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void * pixels) {
    error = true;
}

void GLAPI rec_glTexImage2DMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations) {
    error = true;
}

void GLAPI rec_glTexImage3D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void * pixels) {
    error = true;
}

void GLAPI rec_glTexStorage2D(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height) {
    error = true;
}

void GLAPI rec_glTexStorage3D(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth) {
    error = true;
}

void GLAPI rec_glTransformFeedbackVaryings(GLuint program, GLsizei count, const GLchar * const * varyings, GLenum bufferMode) {
    error = true;
}

GLboolean GLAPI rec_glUnmapBuffer(GLenum target) {
    error = true;
    return 0;
}

void GLAPI rec_glVertexAttribDivisor(GLuint index, GLuint divisor) {
    error = true;
}

void GLAPI rec_glVertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const void * pointer) {
    error = true;
}

void GLAPI rec_glVertexAttribLPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const void * pointer) {
    error = true;
}

void GLAPI rec_glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void * pointer) {
    error = true;
}

void evaluate(int size) {
    char * end = ptr + size;
    while (ptr < end) {
        switch (read<Opcode>()) {
            case OP_glActiveTexture: gl.ActiveTexture(read<GLenum>()); break;
            case OP_glBeginConditionalRender: gl.BeginConditionalRender(read<GLuint>(), read<GLenum>()); break;
            case OP_glBeginQuery: gl.BeginQuery(read<GLenum>(), read<GLuint>()); break;
            case OP_glBeginTransformFeedback: gl.BeginTransformFeedback(read<GLenum>()); break;
            case OP_glBindBuffer: gl.BindBuffer(read<GLenum>(), read<GLuint>()); break;
            case OP_glBindBufferBase: gl.BindBufferBase(read<GLenum>(), read<GLuint>(), read<GLuint>()); break;
            case OP_glBindBufferRange: gl.BindBufferRange(read<GLenum>(), read<GLuint>(), read<GLuint>(), read<GLintptr>(), read<GLsizeiptr>()); break;
            case OP_glBindFramebuffer: gl.BindFramebuffer(read<GLenum>(), read<GLuint>()); break;
            case OP_glBindSampler: gl.BindSampler(read<GLuint>(), read<GLuint>()); break;
            case OP_glBindTexture: gl.BindTexture(read<GLenum>(), read<GLuint>()); break;
            case OP_glBindVertexArray: gl.BindVertexArray(read<GLuint>()); break;
            case OP_glBlendFunc: gl.BlendFunc(read<GLenum>(), read<GLenum>()); break;
            case OP_glBufferSubData: gl.BufferSubData(read<GLenum>(), read<GLintptr>(), read<GLsizeiptr>(), read<char>(read<unsigned>())); break;
            case OP_glClearBufferData: gl.ClearBufferData(read<GLenum>(), read<GLenum>(), read<GLenum>(), read<GLenum>(), read<unsigned>(1)); break;
            case OP_glClearBufferfv: gl.ClearBufferfv(read<GLenum>(), read<GLint>(), read<GLfloat>(4)); break;
            case OP_glClearBufferiv: gl.ClearBufferiv(read<GLenum>(), read<GLint>(), read<GLint>(4)); break;
            case OP_glClearBufferuiv: gl.ClearBufferuiv(read<GLenum>(), read<GLint>(), read<GLuint>(4)); break;
            case OP_glColorMaski: gl.ColorMaski(read<GLuint>(), read<GLboolean>(), read<GLboolean>(), read<GLboolean>(), read<GLboolean>()); break;
            case OP_glCopyBufferSubData: gl.CopyBufferSubData(read<GLenum>(), read<GLenum>(), read<GLintptr>(), read<GLintptr>(), read<GLsizeiptr>()); break;
            case OP_glDepthMask: gl.DepthMask(read<GLboolean>()); break;
            case OP_glDisable: gl.Disable(read<GLenum>()); break;
            case OP_glDispatchCompute: gl.DispatchCompute(read<GLuint>(), read<GLuint>(), read<GLuint>()); break;
            case OP_glDrawArraysInstanced: gl.DrawArraysInstanced(read<GLenum>(), read<GLint>(), read<GLsizei>(), read<GLsizei>()); break;
            case OP_glDrawElementsInstanced: gl.DrawElementsInstanced(read<GLenum>(), read<GLsizei>(), read<GLenum>(), read<void *>(), read<GLsizei>()); break;
            case OP_glEnable: gl.Enable(read<GLenum>()); break;
            case OP_glEndConditionalRender: gl.EndConditionalRender(); break;
            case OP_glEndQuery: gl.EndQuery(read<GLenum>()); break;
            case OP_glEndTransformFeedback: gl.EndTransformFeedback(); break;
            case OP_glFlush: gl.Flush(); break;
            case OP_glPixelStorei: gl.PixelStorei(read<GLenum>(), read<GLint>()); break;
            case OP_glSamplerParameteri: gl.SamplerParameteri(read<GLuint>(), read<GLenum>(), read<GLint>()); break;
            case OP_glScissor: gl.Scissor(read<GLint>(), read<GLint>(), read<GLsizei>(), read<GLsizei>()); break;
            case OP_glTexSubImage2D: gl.TexSubImage2D(read<GLenum>(), read<GLint>(), read<GLint>(), read<GLint>(), read<GLsizei>(), read<GLsizei>(), read<GLenum>(), read<GLenum>(), read<int>() ? read<char>(read<unsigned>()) : 0); break;
            case OP_glTexSubImage3D: gl.TexSubImage3D(read<GLenum>(), read<GLint>(), read<GLint>(), read<GLint>(), read<GLint>(), read<GLsizei>(), read<GLsizei>(), read<GLsizei>(), read<GLenum>(), read<GLenum>(), read<int>() ? read<char>(read<unsigned>()) : 0); break;
            case OP_glUseProgram: gl.UseProgram(read<GLuint>()); break;
            case OP_glViewport: gl.Viewport(read<GLint>(), read<GLint>(), read<GLsizei>(), read<GLsizei>()); break;

            case OP_glUniform: {
                switch (read<UniformOpcode>()) {
                    case OP_glUniform1dv: gl.Uniform1dv(read<GLint>(), read<GLsizei>(), read<GLdouble>(read<int>())); break;
                    case OP_glUniform1fv: gl.Uniform1fv(read<GLint>(), read<GLsizei>(), read<GLfloat>(read<int>())); break;
                    case OP_glUniform1iv: gl.Uniform1iv(read<GLint>(), read<GLsizei>(), read<GLint>(read<int>())); break;
                    case OP_glUniform1uiv: gl.Uniform1uiv(read<GLint>(), read<GLsizei>(), read<GLuint>(read<int>())); break;
                    case OP_glUniform2dv: gl.Uniform2dv(read<GLint>(), read<GLsizei>(), read<GLdouble>(read<int>())); break;
                    case OP_glUniform2fv: gl.Uniform2fv(read<GLint>(), read<GLsizei>(), read<GLfloat>(read<int>())); break;
                    case OP_glUniform2iv: gl.Uniform2iv(read<GLint>(), read<GLsizei>(), read<GLint>(read<int>())); break;
                    case OP_glUniform2uiv: gl.Uniform2uiv(read<GLint>(), read<GLsizei>(), read<GLuint>(read<int>())); break;
                    case OP_glUniform3dv: gl.Uniform3dv(read<GLint>(), read<GLsizei>(), read<GLdouble>(read<int>())); break;
                    case OP_glUniform3fv: gl.Uniform3fv(read<GLint>(), read<GLsizei>(), read<GLfloat>(read<int>())); break;
                    case OP_glUniform3iv: gl.Uniform3iv(read<GLint>(), read<GLsizei>(), read<GLint>(read<int>())); break;
                    case OP_glUniform3uiv: gl.Uniform3uiv(read<GLint>(), read<GLsizei>(), read<GLuint>(read<int>())); break;
                    case OP_glUniform4dv: gl.Uniform4dv(read<GLint>(), read<GLsizei>(), read<GLdouble>(read<int>())); break;
                    case OP_glUniform4fv: gl.Uniform4fv(read<GLint>(), read<GLsizei>(), read<GLfloat>(read<int>())); break;
                    case OP_glUniform4iv: gl.Uniform4iv(read<GLint>(), read<GLsizei>(), read<GLint>(read<int>())); break;
                    case OP_glUniform4uiv: gl.Uniform4uiv(read<GLint>(), read<GLsizei>(), read<GLuint>(read<int>())); break;
                    case OP_glUniformBlockBinding: gl.UniformBlockBinding(read<GLuint>(), read<GLuint>(), read<GLuint>()); break;
                    case OP_glUniformMatrix2dv: gl.UniformMatrix2dv(read<GLint>(), read<GLsizei>(), read<GLboolean>(), read<GLdouble>(read<int>())); break;
                    case OP_glUniformMatrix2fv: gl.UniformMatrix2fv(read<GLint>(), read<GLsizei>(), read<GLboolean>(), read<GLfloat>(read<int>())); break;
                    case OP_glUniformMatrix2x3dv: gl.UniformMatrix2x3dv(read<GLint>(), read<GLsizei>(), read<GLboolean>(), read<GLdouble>(read<int>())); break;
                    case OP_glUniformMatrix2x3fv: gl.UniformMatrix2x3fv(read<GLint>(), read<GLsizei>(), read<GLboolean>(), read<GLfloat>(read<int>())); break;
                    case OP_glUniformMatrix2x4dv: gl.UniformMatrix2x4dv(read<GLint>(), read<GLsizei>(), read<GLboolean>(), read<GLdouble>(read<int>())); break;
                    case OP_glUniformMatrix2x4fv: gl.UniformMatrix2x4fv(read<GLint>(), read<GLsizei>(), read<GLboolean>(), read<GLfloat>(read<int>())); break;
                    case OP_glUniformMatrix3dv: gl.UniformMatrix3dv(read<GLint>(), read<GLsizei>(), read<GLboolean>(), read<GLdouble>(read<int>())); break;
                    case OP_glUniformMatrix3fv: gl.UniformMatrix3fv(read<GLint>(), read<GLsizei>(), read<GLboolean>(), read<GLfloat>(read<int>())); break;
                    case OP_glUniformMatrix3x2dv: gl.UniformMatrix3x2dv(read<GLint>(), read<GLsizei>(), read<GLboolean>(), read<GLdouble>(read<int>())); break;
                    case OP_glUniformMatrix3x2fv: gl.UniformMatrix3x2fv(read<GLint>(), read<GLsizei>(), read<GLboolean>(), read<GLfloat>(read<int>())); break;
                    case OP_glUniformMatrix3x4dv: gl.UniformMatrix3x4dv(read<GLint>(), read<GLsizei>(), read<GLboolean>(), read<GLdouble>(read<int>())); break;
                    case OP_glUniformMatrix3x4fv: gl.UniformMatrix3x4fv(read<GLint>(), read<GLsizei>(), read<GLboolean>(), read<GLfloat>(read<int>())); break;
                    case OP_glUniformMatrix4dv: gl.UniformMatrix4dv(read<GLint>(), read<GLsizei>(), read<GLboolean>(), read<GLdouble>(read<int>())); break;
                    case OP_glUniformMatrix4fv: gl.UniformMatrix4fv(read<GLint>(), read<GLsizei>(), read<GLboolean>(), read<GLfloat>(read<int>())); break;
                    case OP_glUniformMatrix4x2dv: gl.UniformMatrix4x2dv(read<GLint>(), read<GLsizei>(), read<GLboolean>(), read<GLdouble>(read<int>())); break;
                    case OP_glUniformMatrix4x2fv: gl.UniformMatrix4x2fv(read<GLint>(), read<GLsizei>(), read<GLboolean>(), read<GLfloat>(read<int>())); break;
                    case OP_glUniformMatrix4x3dv: gl.UniformMatrix4x3dv(read<GLint>(), read<GLsizei>(), read<GLboolean>(), read<GLdouble>(read<int>())); break;
                    case OP_glUniformMatrix4x3fv: gl.UniformMatrix4x3fv(read<GLint>(), read<GLsizei>(), read<GLboolean>(), read<GLfloat>(read<int>())); break;
                }
                break;
            }
        }
    }

    ptr = buffer;
}

void init() {
    rec.ActiveTexture = rec_glActiveTexture;
    rec.BeginConditionalRender = rec_glBeginConditionalRender;
    rec.BeginQuery = rec_glBeginQuery;
    rec.BeginTransformFeedback = rec_glBeginTransformFeedback;
    rec.BindBuffer = rec_glBindBuffer;
    rec.BindBufferBase = rec_glBindBufferBase;
    rec.BindBufferRange = rec_glBindBufferRange;
    rec.BindFramebuffer = rec_glBindFramebuffer;
    rec.BindSampler = rec_glBindSampler;
    rec.BindTexture = rec_glBindTexture;
    rec.BindVertexArray = rec_glBindVertexArray;
    rec.BlendFunc = rec_glBlendFunc;
    rec.BufferSubData = rec_glBufferSubData;
    rec.ClearBufferData = rec_glClearBufferData;
    rec.ClearBufferfv = rec_glClearBufferfv;
    rec.ClearBufferiv = rec_glClearBufferiv;
    rec.ClearBufferuiv = rec_glClearBufferuiv;
    rec.ColorMaski = rec_glColorMaski;
    rec.CopyBufferSubData = rec_glCopyBufferSubData;
    rec.DepthMask = rec_glDepthMask;
    rec.Disable = rec_glDisable;
    rec.DispatchCompute = rec_glDispatchCompute;
    rec.DrawArraysInstanced = rec_glDrawArraysInstanced;
    rec.DrawElementsInstanced = rec_glDrawElementsInstanced;
    rec.Enable = rec_glEnable;
    rec.EnableVertexAttribArray = rec_glEnableVertexAttribArray;
    rec.EndConditionalRender = rec_glEndConditionalRender;
    rec.EndQuery = rec_glEndQuery;
    rec.EndTransformFeedback = rec_glEndTransformFeedback;
    rec.Flush = rec_glFlush;
    rec.PixelStorei = rec_glPixelStorei;
    rec.SamplerParameteri = rec_glSamplerParameteri;
    rec.Scissor = rec_glScissor;
    rec.TexSubImage2D = rec_glTexSubImage2D;
    rec.TexSubImage3D = rec_glTexSubImage3D;
    rec.Uniform1dv = rec_glUniform1dv;
    rec.Uniform1fv = rec_glUniform1fv;
    rec.Uniform1iv = rec_glUniform1iv;
    rec.Uniform1uiv = rec_glUniform1uiv;
    rec.Uniform2dv = rec_glUniform2dv;
    rec.Uniform2fv = rec_glUniform2fv;
    rec.Uniform2iv = rec_glUniform2iv;
    rec.Uniform2uiv = rec_glUniform2uiv;
    rec.Uniform3dv = rec_glUniform3dv;
    rec.Uniform3fv = rec_glUniform3fv;
    rec.Uniform3iv = rec_glUniform3iv;
    rec.Uniform3uiv = rec_glUniform3uiv;
    rec.Uniform4dv = rec_glUniform4dv;
    rec.Uniform4fv = rec_glUniform4fv;
    rec.Uniform4iv = rec_glUniform4iv;
    rec.Uniform4uiv = rec_glUniform4uiv;
    rec.UniformBlockBinding = rec_glUniformBlockBinding;
    rec.UniformMatrix2dv = rec_glUniformMatrix2dv;
    rec.UniformMatrix2fv = rec_glUniformMatrix2fv;
    rec.UniformMatrix2x3dv = rec_glUniformMatrix2x3dv;
    rec.UniformMatrix2x3fv = rec_glUniformMatrix2x3fv;
    rec.UniformMatrix2x4dv = rec_glUniformMatrix2x4dv;
    rec.UniformMatrix2x4fv = rec_glUniformMatrix2x4fv;
    rec.UniformMatrix3dv = rec_glUniformMatrix3dv;
    rec.UniformMatrix3fv = rec_glUniformMatrix3fv;
    rec.UniformMatrix3x2dv = rec_glUniformMatrix3x2dv;
    rec.UniformMatrix3x2fv = rec_glUniformMatrix3x2fv;
    rec.UniformMatrix3x4dv = rec_glUniformMatrix3x4dv;
    rec.UniformMatrix3x4fv = rec_glUniformMatrix3x4fv;
    rec.UniformMatrix4dv = rec_glUniformMatrix4dv;
    rec.UniformMatrix4fv = rec_glUniformMatrix4fv;
    rec.UniformMatrix4x2dv = rec_glUniformMatrix4x2dv;
    rec.UniformMatrix4x2fv = rec_glUniformMatrix4x2fv;
    rec.UniformMatrix4x3dv = rec_glUniformMatrix4x3dv;
    rec.UniformMatrix4x3fv = rec_glUniformMatrix4x3fv;
    rec.UseProgram = rec_glUseProgram;
    rec.Viewport = rec_glViewport;

    rec.GetQueryObjectiv = rec_glGetQueryObjectiv;

    rec.AttachShader = rec_glAttachShader;
    rec.BindRenderbuffer = rec_glBindRenderbuffer;
    rec.BufferData = rec_glBufferData;
    rec.BufferStorage = rec_glBufferStorage;
    rec.CheckFramebufferStatus = rec_glCheckFramebufferStatus;
    rec.CompileShader = rec_glCompileShader;
    rec.CreateShader = rec_glCreateShader;
    rec.DeleteBuffers = rec_glDeleteBuffers;
    rec.DeleteFramebuffers = rec_glDeleteFramebuffers;
    rec.DeleteProgram = rec_glDeleteProgram;
    rec.DeleteQueries = rec_glDeleteQueries;
    rec.DeleteRenderbuffers = rec_glDeleteRenderbuffers;
    rec.DeleteSamplers = rec_glDeleteSamplers;
    rec.DeleteShader = rec_glDeleteShader;
    rec.DeleteTextures = rec_glDeleteTextures;
    rec.DeleteVertexArrays = rec_glDeleteVertexArrays;
    rec.FramebufferRenderbuffer = rec_glFramebufferRenderbuffer;
    rec.FramebufferTexture2D = rec_glFramebufferTexture2D;
    rec.GenBuffers = rec_glGenBuffers;
    rec.GenFramebuffers = rec_glGenFramebuffers;
    rec.GenQueries = rec_glGenQueries;
    rec.GenRenderbuffers = rec_glGenRenderbuffers;
    rec.GenSamplers = rec_glGenSamplers;
    rec.GenTextures = rec_glGenTextures;
    rec.GenVertexArrays = rec_glGenVertexArrays;
    rec.GetActiveAttrib = rec_glGetActiveAttrib;
    rec.GetActiveUniform = rec_glGetActiveUniform;
    rec.GetActiveUniformBlockName = rec_glGetActiveUniformBlockName;
    rec.GetActiveUniformBlockiv = rec_glGetActiveUniformBlockiv;
    rec.GetAttribLocation = rec_glGetAttribLocation;
    rec.GetFloatv = rec_glGetFloatv;
    rec.GetInteger64v = rec_glGetInteger64v;
    rec.GetIntegeri_v = rec_glGetIntegeri_v;
    rec.GetIntegerv = rec_glGetIntegerv;
    rec.GetProgramInfoLog = rec_glGetProgramInfoLog;
    rec.GetProgramiv = rec_glGetProgramiv;
    rec.GetShaderInfoLog = rec_glGetShaderInfoLog;
    rec.GetShaderiv = rec_glGetShaderiv;
    rec.GetString = rec_glGetString;
    rec.GetStringi = rec_glGetStringi;
    rec.GetUniformBlockIndex = rec_glGetUniformBlockIndex;
    rec.GetUniformLocation = rec_glGetUniformLocation;
    rec.GetUniformdv = rec_glGetUniformdv;
    rec.GetUniformfv = rec_glGetUniformfv;
    rec.GetUniformiv = rec_glGetUniformiv;
    rec.GetUniformuiv = rec_glGetUniformuiv;
    rec.LinkProgram = rec_glLinkProgram;
    rec.MapBuffer = rec_glMapBuffer;
    rec.MapBufferRange = rec_glMapBufferRange;
    rec.PrimitiveRestartIndex = rec_glPrimitiveRestartIndex;
    rec.ReadBuffer = rec_glReadBuffer;
    rec.ReadPixels = rec_glReadPixels;
    rec.RenderbufferStorage = rec_glRenderbufferStorage;
    rec.RenderbufferStorageMultisample = rec_glRenderbufferStorageMultisample;
    rec.ShaderSource = rec_glShaderSource;
    rec.TexImage2D = rec_glTexImage2D;
    rec.TexImage2DMultisample = rec_glTexImage2DMultisample;
    rec.TexImage3D = rec_glTexImage3D;
    rec.TexStorage2D = rec_glTexStorage2D;
    rec.TexStorage3D = rec_glTexStorage3D;
    rec.TransformFeedbackVaryings = rec_glTransformFeedbackVaryings;
    rec.UnmapBuffer = rec_glUnmapBuffer;
    rec.VertexAttribDivisor = rec_glVertexAttribDivisor;
    rec.VertexAttribIPointer = rec_glVertexAttribIPointer;
    rec.VertexAttribLPointer = rec_glVertexAttribLPointer;
    rec.VertexAttribPointer = rec_glVertexAttribPointer;
}

}
