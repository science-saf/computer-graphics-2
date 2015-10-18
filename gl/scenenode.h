#pragma once

#include <QObject>
#include <memory>
#include <stdint.h>

class QPainter;
class SceneNode;
class BaseScene;

// Abstract class: animate() and render() are not implemented.
// Nodes has children. To add children, use setParent() method in children node.
class SceneNode
        : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(SceneNode)
public:
    typedef std::function<void(SceneNode & node)> VisitorFunc;

    explicit SceneNode(SceneNode *parent);
    ~SceneNode();

    /// \param msec - miliseconds since last animate call
    /// or since window started rendering
    virtual void advance(int64_t msec) = 0;
    virtual void render(QPainter &painter) = 0;

    bool isOnScene() const;
    virtual void onEnterScene();
    virtual void onExitScene();
    void visit(VisitorFunc const& visitor);

protected:
    SceneNode() = default;
    // QObject interface
    void childEvent(QChildEvent *event) override;

private:
    bool m_isOnScene = false;
};
