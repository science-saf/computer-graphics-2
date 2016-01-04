#include "window3d.h"
#include <QResizeEvent>
#include <QPainter>
#include <QGuiApplication>
#define _USE_MATH_DEFINES
#include <math.h>

Window3D::Window3D(QWindow *parent)
    : QWindow(parent)
{
    setSurfaceType(QWindow::OpenGLSurface);
    setFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
	m_radius = sqrt(m_lookFrom.x() * m_lookFrom.x() + m_lookFrom.y() * m_lookFrom.y());
	m_theta = acos((float) m_lookFrom.z() / m_radius);
	m_phi = atan((float)m_lookFrom.y() / m_lookFrom.x());
}

void Window3D::setFixedSize(QSize size)
{
    setMinimumSize(size);
    setMaximumSize(size);
}

void Window3D::pushScene(std::shared_ptr<BaseScene> scene)
{
    m_sceneStack.push_back(scene);
    scene->onPush();
}

void Window3D::popScene()
{
    if (!m_sceneStack.empty())
    {
        m_sceneStack.back()->onPop();
        m_sceneStack.pop_back();
    }
}

bool Window3D::event(QEvent *event)
{
    switch (event->type())
    {
    case QEvent::UpdateRequest:
        m_updatePending = false;
        render();
        return true;
    case QEvent::Close:
        if (m_canRender)
        {
            stopRendering();
        }
        return QWindow::event(event);
    default:
        return QWindow::event(event);
    }
}

void Window3D::exposeEvent(QExposeEvent *event)
{
    QWindow::exposeEvent(event);
    if (isExposed())
        render();
}

void Window3D::resizeEvent(QResizeEvent *event)
{
    QWindow::resizeEvent(event);
    if (!m_canRender)
    {
        initRendering();
    }
}

void Window3D::showEvent(QShowEvent *event)
{
    QWindow::showEvent(event);
}

void Window3D::deferRender()
{
    if (!m_updatePending) {
        m_updatePending = true;
        QGuiApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
    }
}

void Window3D::render()
{
    if (!m_canRender)
    {
        return;
    }

    m_pContext->makeCurrent(this);
    if (!m_sceneStack.empty())
    {
        updateScene(*m_sceneStack.back());
    }
    else
    {
        glClearColor(1, 1, 1, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

    m_pContext->swapBuffers(this);
    deferRender();
}

void Window3D::stopRendering()
{
    while (!m_sceneStack.empty())
    {
        popScene();
    }
    m_canRender = false;
}

void Window3D::initRendering()
{
    if (!m_pContext) {
        m_pContext = new QOpenGLContext(this);
        m_pContext->setFormat(requestedFormat());
        m_pContext->create();
    }
    m_canRender = true;
    m_updateTime.start();
}

void Window3D::updateScene(BaseScene &scene)
{
    scene.setViewport(size());

    int msec = m_updateTime.elapsed();
    m_updateTime.restart();
    scene.visit([&](SceneNode & node) {
        node.advance(msec);
    });
    scene.camera().advance(msec);

    QOpenGLPaintDevice device(size());
    QPainter painter(&device);
	scene.camera().lookAt(m_lookFrom, QVector3D(0, 0, 0), QVector3D(0, 0, 1));
    scene.camera().loadMatrix();
    scene.render(painter);
    scene.visit([&](SceneNode & node) {
        node.render(painter);
    });
}

void Window3D::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
		m_isLMBPressed = true;
		m_oldX = event->x();
		m_oldY = event->y();
	}
}

void Window3D::mouseReleaseEvent(QMouseEvent *event)
{
	if(event->button() == Qt::LeftButton) {
		m_isLMBPressed = false;
	}
}

void Window3D::mouseMoveEvent(QMouseEvent *event)
{
	if (m_isLMBPressed) {
		calculateLookFrom(event->x(), event->y());
	}
}

void Window3D::wheelEvent(QWheelEvent *event)
{
	const int DELTA_R_PER_STEP = 2;
	const int ONE_STEP = 120;
	const int MIN_RADIUS = 3;
	const int MAX_RADIUS = 30;

	int angleDelta = event->angleDelta().y();
	if (angleDelta != 0)
	{
		m_radius -= (float)angleDelta / ONE_STEP * DELTA_R_PER_STEP;
		if (m_radius < MIN_RADIUS)
		{
			m_radius = MIN_RADIUS;
		}
		else if (m_radius > MAX_RADIUS)
		{
			m_radius = MAX_RADIUS;
		}
		calculateLookFrom(m_oldX, m_oldY);
	}
}

void Window3D::calculateLookFrom(int x, int y)
{
	const float MIN_THETA = M_PI * 0.01;
	const float MAX_THETA = M_PI - M_PI * 0.01;
	const float RAD_TO_PX_COEFF = 0.005;

	m_phi -= (x - m_oldX) * RAD_TO_PX_COEFF;
	m_theta -= (y - m_oldY) * RAD_TO_PX_COEFF;
	if (m_theta < MIN_THETA)
	{
		m_theta = MIN_THETA;
	}
	if (m_theta > MAX_THETA)
	{
		m_theta = MAX_THETA;
	}
	float newX = m_radius * sin(m_theta) * cos(m_phi);
	float newY = m_radius * sin(m_theta) * sin(m_phi);
	float newZ = m_radius * cos(m_theta);

	m_lookFrom = QVector3D(newX, newY, newZ);
	m_oldX = x;
	m_oldY = y;
}