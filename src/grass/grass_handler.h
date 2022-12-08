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
		void awake();
		void initialize();
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

		void loadScene(std::string fileName);
		static void loadLightData(const ShaderProgram &shader, RenderData &renderData);
		static void loadCameraData(const ShaderProgram &shader, Camera &camera);
};
