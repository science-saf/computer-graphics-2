#pragma once
#include <QGuiApplication>
#include "ui/window3d.h"

class GameApplication : public QGuiApplication
{
    Q_OBJECT
public:
    GameApplication(int argc, char *argv[]);

    int enterGameLoop();

private slots:
    void loadScene();

private:
    Window3D m_window;
};
