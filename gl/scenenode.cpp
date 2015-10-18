#include "scenenode.h"
#include "basescene.h"
#include <QChildEvent>

static void visitRecursive(const SceneNode::VisitorFunc &visitor, const QObjectList &objects)
{
    for (QObject *object : objects)
    {
        if (auto node = qobject_cast<SceneNode *>(object))
        {
            visitor(*node);
            visitRecursive(visitor, node->children());
        }
    }
}

SceneNode::SceneNode(SceneNode *parent)
    : QObject(parent)
{
}

SceneNode::~SceneNode()
{
}

bool SceneNode::isOnScene() const
{
    return m_isOnScene;
}

void SceneNode::childEvent(QChildEvent *event)
{
    if (m_isOnScene)
    {
        if (auto node = qobject_cast<SceneNode *>(event->child()))
        {
            switch (event->type())
            {
            case QEvent::ChildAdded:
                node->visit([](SceneNode &visitedNode) {
                    visitedNode.onExitScene();
                });
                break;
            case QEvent::ChildRemoved:
                node->visit([](SceneNode &visitedNode) {
                    visitedNode.onExitScene();
                });
                break;
            default:
                break;
            }
        }
    }
}

void SceneNode::onEnterScene()
{
    m_isOnScene = true;
}

void SceneNode::onExitScene()
{
    m_isOnScene = false;
}

void SceneNode::visit(const VisitorFunc &visitor)
{
    visitRecursive(visitor, children());
}
