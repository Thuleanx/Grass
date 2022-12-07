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

	// custom
	int shapeBucketCnt = 6;
    std::string t_fileLoaded;
    RenderData t_renderData;
    Camera t_camera;
    std::unordered_map<int, std::unique_ptr<iShape>> t_shapeMap;
	std::unordered_map<int, GLuint> t_vao;
	std::unordered_map<int, GLuint> t_vbo;

	bool t_recalculateVAO = false;
	bool t_reloadscene = false;
	bool a_resized = false;

	int t_getKey(glm::vec4 camPos, RenderShapeData &shape) const;
    void t_loadScene();
	void t_calculateLOD();
	void t_generateShapeMap();
	void t_calculateVAOVBO();

	GLuint t_fbo_main = 5; // this is the screen fbo

	GLuint a_shader_default;
	GLuint a_shader_postprocessing;
	GLuint a_shader_ambientCombine;
	GLuint a_shader_blur;
	GLuint a_shader_ambientOcclusion;

	GLuint a_fbo_default;
	GLuint a_fbo_default_diffuseSpecular;
	GLuint a_fbo_default_ambientColor;
	GLuint a_fbo_default_normalVS;
	GLuint a_fbo_default_posVS;
	GLuint a_fbo_default_depth;

	GLuint a_fbo_AO_noiseTexture;
	GLuint a_fbo_AO;
	GLuint a_fbo_AO_value;

	GLuint a_fbo_blur;
	GLuint a_fbo_blur_value;

	GLuint a_fbo_normalOutput;
	GLuint a_fbo_normalOutput_color;

	GLuint a_blitVAO;
	GLuint a_blitVBO;

	void a_createTexture(GLuint &texture, 
		GLenum format, GLenum internalFormat, GLenum dataType,
        GLenum filter = GL_LINEAR, GLenum wrapMode = GL_REPEAT
	); 
	void a_deleteOpenGLVars();
	void a_deleteFBOandTextures();

	void a_drawScene(const GLuint &shader);
	void a_loadSSAOData(const GLuint &shader);
	void a_loadSceneGlobalData(const GLuint &shader);
	void a_loadCameraData(const GLuint &shader);
	void a_loadLightData(const GLuint &shader);
	void a_loadShape(const GLuint &shader, int index);

	void a_initBlit();
    void a_blit(GLuint fbo);
	void a_initSSAOKernel();
	void a_initFBOs();
	void a_pipelineInit();
	void a_pipelineRun();
};
