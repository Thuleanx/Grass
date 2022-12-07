#include "realtime.h"

#include <QCoreApplication>
#include <QMouseEvent>
#include <QKeyEvent>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include "t_utils/Blit.h"

#define f(i,a,b) for (int i = a; i < b; i++)
using namespace glm;
using namespace std;

// ================== Project 5: Lights, Camera

Realtime::Realtime(QWidget *parent)
    : QOpenGLWidget(parent)
{
    m_prev_mouse_pos = glm::vec2(size().width()/2, size().height()/2);
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);

    m_keyMap[Qt::Key_W]       = false;
    m_keyMap[Qt::Key_A]       = false;
    m_keyMap[Qt::Key_S]       = false;
    m_keyMap[Qt::Key_D]       = false;
    m_keyMap[Qt::Key_Control] = false;
    m_keyMap[Qt::Key_Space]   = false;

    // If you must use this function, do not edit anything above this
}

void Realtime::finish() {
    killTimer(m_timer);
    this->makeCurrent();

    // Students: anything requiring OpenGL calls when the program exits should be done here
	a_deleteOpenGLVars();

    this->doneCurrent();
}

void Realtime::initializeGL() {
    m_devicePixelRatio = this->devicePixelRatio();

    m_timer = startTimer(1000/60);
    m_elapsedTimer.start();

    // Initializing GL.
    // GLEW (GL Extension Wrangler) provides access to OpenGL functions.
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Error while initializing GL: " << glewGetErrorString(err) << std::endl;
    }
    std::cout << "Initialized GL: Version " << glewGetString(GLEW_VERSION) << std::endl;

    // Allows OpenGL to draw objects appropriately on top of one another
    glEnable(GL_DEPTH_TEST);
   // Tells OpenGL to only draw the front face
    glEnable(GL_CULL_FACE);
    // Tells OpenGL how big the screen is
    glViewport(0, 0, size().width() * m_devicePixelRatio, size().height() * m_devicePixelRatio);

	m_fbo_height = size().height();
	m_fbo_width = size().width();


	a_pipelineInit();
    Blit::initialize();
}

void Realtime::paintGL() {
    // Students: anything requiring OpenGL calls every frame should be done here
    if (t_reloadscene) t_loadScene();
	t_reloadscene = false;
    if (t_recalculateVAO) t_calculateVAOVBO();
	t_recalculateVAO = false;

    a_pipelineRun();
}

void Realtime::resizeGL(int w, int h) {
    // Tells OpenGL how big the screen is
    glViewport(0, 0, size().width() * m_devicePixelRatio, size().height() * m_devicePixelRatio);

    // Students: anything requiring OpenGL calls when the program starts should be done here
	t_camera.setSize(w,h);

	m_fbo_height = size().height();
	m_fbo_width = size().width();

	a_deleteFBOandTextures();
	a_initFBOs();
}

void Realtime::sceneChanged() {
	t_reloadscene = true;
    update(); // asks for a PaintGL() call to occur
}

void Realtime::settingsChanged() {
	t_recalculateVAO = true;
	t_camera.recalculateProj();

    update(); // asks for a PaintGL() call to occur
}

// ================== Project 6: Action!

void Realtime::keyPressEvent(QKeyEvent *event) {
    m_keyMap[Qt::Key(event->key())] = true;
}

void Realtime::keyReleaseEvent(QKeyEvent *event) {
    m_keyMap[Qt::Key(event->key())] = false;
}

void Realtime::mousePressEvent(QMouseEvent *event) {
    if (event->buttons().testFlag(Qt::LeftButton)) {
        m_mouseDown = true;
        m_prev_mouse_pos = glm::vec2(event->position().x(), event->position().y());
    }
}

void Realtime::mouseReleaseEvent(QMouseEvent *event) {
    if (!event->buttons().testFlag(Qt::LeftButton)) {
        m_mouseDown = false;
    }
}

void Realtime::mouseMoveEvent(QMouseEvent *event) {
    if (m_mouseDown) {
        int posX = event->position().x();
        int posY = event->position().y();
        int deltaX = posX - m_prev_mouse_pos.x;
        int deltaY = posY - m_prev_mouse_pos.y;
        m_prev_mouse_pos = glm::vec2(posX, posY);

		float speedX = 2.0f/1000;
		float speedY = 3.0f/1000;

        // Use deltaX and deltaY here to rotate
		if (m_mouseDown) {
			t_camera.rotate(-vec2(speedX * deltaX, speedY * deltaY));
		}

        update(); // asks for a PaintGL() call to occur
    }
}

void Realtime::timerEvent(QTimerEvent *event) {
    int elapsedms   = m_elapsedTimer.elapsed();
    float deltaTime = elapsedms * 0.001f;
	float moveSpeed = 5;
    m_elapsedTimer.restart();

    // Use deltaTime and m_keyMap here to move around
    t_camera.translate(vec3(
		m_keyMap[Qt::Key_D] - m_keyMap[Qt::Key_A], 
		m_keyMap[Qt::Key_W] - m_keyMap[Qt::Key_S],
        m_keyMap[Qt::Key_Space] - m_keyMap[Qt::Key_Control]
	) * deltaTime * moveSpeed);

    update(); // asks for a PaintGL() call to occur
}
