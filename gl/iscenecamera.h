#include <QVector3D>

class SceneCamera : public QObject
{
public:
    virtual ~ISceneCamera() = default;

    /// Sets current OpenGL model-view matrix
    virtual void loadMatrix() const = 0;

    virtual void loadLookAtRH(vec3d const& eye,
                              vec3d const& at,
                              vec3d const& up) = 0;
    virtual void setViewport(QSize size) = 0;
    virtual void movedMouse(int dx, int dy) = 0;

    /// Returns eye position
    virtual vec3d eye() const = 0;
};
