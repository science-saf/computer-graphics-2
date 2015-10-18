#include "basescene.h"
#include <QColor>
#include <qopengl.h>
#include "glhelper.h"

static float Z_NEAR = 0.02;
static float Z_FAR = 50.0;

BaseScene::BaseScene(QSize viewportSize)
    : SceneNode()
    , m_camera(viewportSize)
    , m_clearColor(Qt::white)
{
}

BaseScene::~BaseScene()
{
}

QColor BaseScene::clearColor()
{
    return m_clearColor;
}

void BaseScene::setClearColor(QColor const& clearColor)
{
    m_clearColor = clearColor;
}

void BaseScene::setViewport(QSize viewport)
{
    glViewport(0, 0, viewport.width(), viewport.height());

    QMatrix4x4 proj;
    proj.perspective(90, float(viewport.width()) / viewport.height(), Z_NEAR, Z_FAR);
    GLHelper::setProjectionMatrix(proj);

    m_camera.setViewport(viewport);
    GLHelper::dumpIfError();
}

void BaseScene::advance(int64_t msec)
{
    (void)msec;
}

void BaseScene::render(QPainter &painter)
{
    (void)painter;
    glClearColor(m_clearColor.redF(), m_clearColor.greenF(), m_clearColor.blueF(), m_clearColor.alphaF());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glLineWidth(2);
    glEnable(GL_LINE_SMOOTH);

    GLHelper::dumpIfError();
}

void BaseScene::onPush()
{
    visit([](SceneNode &node) {
        node.onExitScene();
    });
}

void BaseScene::onPop()
{
    visit([](SceneNode &node) {
        node.onExitScene();
    });
}

SceneCamera &BaseScene::camera()
{
    return m_camera;
}

const SceneCamera &BaseScene::camera() const
{
    return m_camera;
}

