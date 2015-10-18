#pragma once

#include <QMatrix4x4>

class GLHelper
{
public:
    static void setModelViewMatrix(QMatrix4x4 const& matrix);
    static void setProjectionMatrix(QMatrix4x4 const& matrix);
    static void dumpIfError();

private:
    GLHelper() = delete;
    ~GLHelper() = delete;
};
