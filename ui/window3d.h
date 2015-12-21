#pragma once

#include <QWindow>
#include <QOpenGLPaintDevice>
#include <QTime>
#include <memory>
#include "../gl/basescene.h"

class Window3D : public QWindow
{
    Q_OBJECT
    Q_DISABLE_COPY(Window3D)

public:
    explicit Window3D(QWindow *parent = 0);
    void setFixedSize(QSize size);
    void pushScene(std::shared_ptr<BaseScene> scene);
    void popScene();

    bool event(QEvent *) override;

protected:
    void exposeEvent(QExposeEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;

private slots:
    void deferRender();
    void render();
    void stopRendering();
    void initRendering();
    void updateScene(BaseScene &scene);

private:
	void calculateLookFrom(int x, int y);
	QVector3D m_lookFrom = QVector3D(6, 3, 2);
    QTime m_updateTime;
    bool m_canRender = false;
    bool m_updatePending = false;
	bool m_isLMBPressed = false;
	int m_radius = 10;
    std::vector<std::shared_ptr<BaseScene>> m_sceneStack;
	QOpenGLContext *m_pContext = nullptr;
};
