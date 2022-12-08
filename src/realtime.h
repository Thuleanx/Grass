#pragma once

// Defined before including GLEW to suppress deprecation messages on macOS
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif
#include <GL/glew.h>
#include <glm/glm.hpp>

#include <unordered_map>
#include <QElapsedTimer>
#include <QOpenGLWidget>
#include <QTime>
#include <QTimer>

#include <vector>
#include "utils/sceneparser.h"
#include "shapes/iShape.h"
#include "camera/camera.h"
#include "t_utils/Framebuffer.h"
#include "t_utils/ShaderProgram.h"

class Realtime : public QOpenGLWidget
{
public:
    Realtime(QWidget *parent = nullptr);
    void finish();                                      // Called on program exit
    void sceneChanged();
    void settingsChanged();

public slots:
    void tick(QTimerEvent* event);                      // Called once per tick of m_timer

protected:
    void initializeGL() override;                       // Called once at the start of the program
    void paintGL() override;                            // Called whenever the OpenGL context changes or by an update() request
    void resizeGL(int width, int height) override;      // Called when window size changes

private:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void timerEvent(QTimerEvent *event) override;

    // Tick Related Variables
    int m_timer;                                        // Stores timer which attempts to run ~60 times per second
    QElapsedTimer m_elapsedTimer;                       // Stores timer which keeps track of actual time between frames

    // Input Related Variables
    bool m_mouseDown = false;                           // Stores state of left mouse button
    glm::vec2 m_prev_mouse_pos;                         // Stores mouse position
    std::unordered_map<Qt::Key, bool> m_keyMap;         // Stores whether keys are pressed or not

    // Device Correction Variables
    int m_devicePixelRatio;
	int m_fbo_width;
	int m_fbo_height;

    std::string t_fileLoaded;
    RenderData t_renderData;
    Camera t_camera;

	bool t_recalculateVAO = false;
	bool t_reloadscene = false;

    void t_loadScene();
	void t_calculateVAOVBO();

	// void a_deleteOpenGLVars();
	// void a_deleteFBOandTextures();

	// void a_loadSceneGlobalData(const ShaderProgram &shader);
	// void a_loadCameraData(const ShaderProgram &shader);
	// void a_loadLightData(const ShaderProgram &shader);

	// void a_pipelineRun();
};
