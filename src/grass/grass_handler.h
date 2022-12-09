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
#include "settings.h"

class GrassHandler {
	public: 
		void awake(RenderData &renderData);
		void onResize(int screen_width, int screen_height, 
			int fbo_width, int fbo_height);
		void onSettingsChanged();
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

		float bladeWidth = 0.05f;
		float bladeHeight = 1.0f;
		const int vertexOutputSizeBytes = 12;
		const int trianglesPerBlade = 1;
        const glm::vec3 workGroupSz = glm::vec3(8,1,8);

        int numGrassBlades() { return (settings.bladeCnt*2+1) * (settings.bladeCnt*2+1);}
		void generateGrass();
		void initShaders();
		void initVAOVBO();
		void destroyVAOVBO();

		void loadGrassData(const ShaderProgram &shader);
		static void loadLightData(const ShaderProgram &shader, RenderData &renderData);
		static void loadCameraData(const ShaderProgram &shader, Camera &camera);
};
