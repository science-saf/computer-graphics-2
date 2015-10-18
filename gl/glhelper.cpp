#include "glhelper.h"
#include <qopengl.h>
#include <QDebug>

namespace {
void setOpenGLMatrix(const float *matrix, GLint mode)
{
    GLint oldMode = 0;
    glGetIntegerv(GL_MATRIX_MODE, &oldMode);
    if (oldMode != mode)
    {
        glMatrixMode(mode);
        glLoadMatrixf(matrix);
        glMatrixMode(oldMode);
    }
    else
    {
        glLoadMatrixf(matrix);
    }
}
}

void GLHelper::setModelViewMatrix(const QMatrix4x4 &matrix)
{
    setOpenGLMatrix(matrix.constData(), GL_MODELVIEW);
}

void GLHelper::setProjectionMatrix(const QMatrix4x4 &matrix)
{
    setOpenGLMatrix(matrix.constData(), GL_PROJECTION);
}

void GLHelper::dumpIfError()
{
    GLenum error = glGetError();
    if (error == GL_NO_ERROR)
        return;

    switch (error)
    {
        case GL_INVALID_ENUM:
            qDebug() << "OpenGL ERROR: Invalid enum value passed";
            break;

        case GL_INVALID_VALUE:
            qDebug() << "OpenGL ERROR: Invalid value passed";
            break;

        case GL_INVALID_OPERATION:
            qDebug() << "OpenGL ERROR: Operation cannot be called in such place";
            break;

        case GL_STACK_OVERFLOW:
            qDebug() << "OpenGL ERROR: Stack overflow";
            break;

        case GL_STACK_UNDERFLOW:
            qDebug() << "OpenGL ERROR: Stack underflow";
            break;

        default:
            qDebug() << "OpenGL ERROR: Reason unknown";
            break;
    }
}

