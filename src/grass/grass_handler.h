#pragma once

// Defined before including GLEW to suppress deprecation messages on macOS
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif
#include <GL/glew.h>
#include <iostream>
#include <vector>
#include "utils/sceneparser.h"
#include "t_utils/Framebuffer.h"
#include "t_utils/ShaderProgram.h"
#include "camera/camera.h"

class GrassHandler {
	public: 
		void awake(RenderData &renderData);
		void onResize(int screen_width, int screen_height, 
			int fbo_width, int fbo_height);
		void update(Camera &camera);
		void onDestroy();
	private:
		Framebuffer fbo_main = Framebuffer(1);
		ShaderProgram shader_default;
		ShaderProgram shader_compute_grass;
		int screen_width, screen_height;
		int fbo_width, fbo_height;

		GLuint vao;
		GLuint vertexDataBuffer;

        int bladeCntX = 6, bladeCntZ = 6;
        float density = 1;
		float bladeWidth = 0.05f;
		float bladeHeight = 1.0f;
		int vertexOutputSizeBytes = 8;
		const int trianglesPerBlade = 1;

        int numGrassBlades() { return (bladeCntX*2+1) * (bladeCntZ*2+1);}
		void generateGrass();
		void initShaders();
		void initVAOVBO();

		void loadScene(std::string fileName);
		void loadGrassData(const ShaderProgram &shader);
		static void loadLightData(const ShaderProgram &shader, RenderData &renderData);
		static void loadCameraData(const ShaderProgram &shader, Camera &camera);
};
