#pragma once

#include "scenenode.h"
#include "scenecamera.h"
#include <QColor>

class BaseScene : public SceneNode
{
    Q_OBJECT
    Q_DISABLE_COPY(BaseScene)
public:
    BaseScene(QSize viewportSize = QSize(0, 0));
    ~BaseScene();

    QColor clearColor();
    void setClearColor(const QColor &clearColor);

    void setViewport(QSize viewport);
    void advance(int64_t msec) override;
    void render(QPainter &painter) override;
    void onPush();
    void onPop();

    SceneCamera &camera();
    SceneCamera const& camera() const;

private:
    SceneCamera m_camera;
    QColor m_clearColor;
};
