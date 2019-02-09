#pragma once

#include "opengl/opengl.hpp"
#include "opengl/gl_methods.hpp"

namespace MGLBytecode {

enum Opcode {
    OP_glActiveTexture,
    OP_glBeginConditionalRender,
    OP_glBeginQuery,
    OP_glBeginTransformFeedback,
    OP_glBindBuffer,
    OP_glBindBufferBase,
    OP_glBindBufferRange,
    OP_glBindFramebuffer,
    OP_glBindSampler,
    OP_glBindTexture,
    OP_glBindVertexArray,
    OP_glBlendFunc,
    OP_glBufferSubData,
    OP_glClearBufferData,
    OP_glClearBufferfv,
    OP_glClearBufferiv,
    OP_glClearBufferuiv,
    OP_glColorMaski,
    OP_glCopyBufferSubData,
    OP_glDepthMask,
    OP_glDisable,
    OP_glDispatchCompute,
    OP_glDrawArraysInstanced,
    OP_glDrawElementsInstanced,
    OP_glEnable,
    OP_glEndConditionalRender,
    OP_glEndQuery,
    OP_glEndTransformFeedback,
    OP_glFlush,
    OP_glPixelStorei,
    OP_glSamplerParameteri,
    OP_glScissor,
    OP_glTexSubImage2D,
    OP_glTexSubImage3D,
    OP_glUseProgram,
    OP_glViewport,
    OP_glUniform,
};

enum UniformOpcode {
    OP_glUniform1dv,
    OP_glUniform1fv,
    OP_glUniform1iv,
    OP_glUniform1uiv,
    OP_glUniform2dv,
    OP_glUniform2fv,
    OP_glUniform2iv,
    OP_glUniform2uiv,
    OP_glUniform3dv,
    OP_glUniform3fv,
    OP_glUniform3iv,
    OP_glUniform3uiv,
    OP_glUniform4dv,
    OP_glUniform4fv,
    OP_glUniform4iv,
    OP_glUniform4uiv,
    OP_glUniformBlockBinding,
    OP_glUniformMatrix2dv,
    OP_glUniformMatrix2fv,
    OP_glUniformMatrix2x3dv,
    OP_glUniformMatrix2x3fv,
    OP_glUniformMatrix2x4dv,
    OP_glUniformMatrix2x4fv,
    OP_glUniformMatrix3dv,
    OP_glUniformMatrix3fv,
    OP_glUniformMatrix3x2dv,
    OP_glUniformMatrix3x2fv,
    OP_glUniformMatrix3x4dv,
    OP_glUniformMatrix3x4fv,
    OP_glUniformMatrix4dv,
    OP_glUniformMatrix4fv,
    OP_glUniformMatrix4x2dv,
    OP_glUniformMatrix4x2fv,
    OP_glUniformMatrix4x3dv,
    OP_glUniformMatrix4x3fv,
};

extern bool error;
extern char * buffer;
extern char * ptr;
extern GLMethods rec;
extern GLMethods gl;

void evaluate(int size);
void init();

}
