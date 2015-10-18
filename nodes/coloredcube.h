#pragma once

#include "../gl/scenenode.h"

class ColoredCube : public SceneNode
{
public:
    ColoredCube(SceneNode *parent);

    void advance(int64_t msec) override;
    void render(QPainter &painter) override;
};
