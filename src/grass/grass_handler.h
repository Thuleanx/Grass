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
#include "player/player.h"
#include "player/player_group.h"
#include "camera/camera.h"
#include "settings.h"

class GrassHandler {
	public: 
		PlayerGroup players;
		void awake(RenderData &renderData);
		void onResize(int screen_width, int screen_height, 
			int fbo_width, int fbo_height);
		void onSettingsChanged();
		void onForceGrassRedraw();
		void update(Camera &camera);
		void onDestroy();

		GLuint getHeightMap() const { return heightMap; }
	private:
		Framebuffer fbo_main = Framebuffer(2);
		Framebuffer fbo_raw;
		ShaderProgram shader_default;
		ShaderProgram shader_postprocessing;
		ShaderProgram shader_compute_grass;
		ShaderProgram shader_compute_windNoise;
		int screen_width, screen_height;
		int fbo_width = 100, fbo_height = 100;

		GLuint vao;
		GLuint vertexDataBuffer;

		GLuint default_screen;
		GLuint default_screen_depth;
		GLuint wind_noiseTexture;

		GLuint heightMap;

		const int vertexOutputSizeBytes = 16;
        const glm::vec3 workGroupSz = glm::vec3(8,1,8);
		const int WIND_NOISE_TEXTURE_SZ = 800;

		int trianglesPerBlade() { return settings.bladeSegments * 2 + 1; }
        int numGrassBlades() { return (settings.bladeCnt*2+1) * (settings.bladeCnt*2+1);}

		void generateGrass();
		void generateWindTexture();
		void destroyWindTexture();

		void initShaders();
		void onResizeShaders();
		void destroyShaders();

		void initVAOVBO();
		void destroyVAOVBO();

		void initFramebuffers();
		void destroyFramebuffers();

		void initHeightMap();
		void destroyHeightMap();

		void loadGrassData(const ShaderProgram &shader);
		static void loadLightData(const ShaderProgram &shader, RenderData &renderData);
		static void loadCameraData(const ShaderProgram &shader, Camera &camera);
};
