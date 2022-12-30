#include "grass_handler.h"
#include "t_utils/ErrorHandler.h"
#define f(i,a,b) for (int i = a; i < b; i++)
using namespace glm;
using namespace std;

void GrassHandler::initVAOVBO() {
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vertexDataBuffer);
}

void GrassHandler::destroyVAOVBO() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vertexDataBuffer);
}

void GrassHandler::generateGrass() {
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vertexDataBuffer);

	shader_compute_grass.useProgram();

	settings.bladeSegments = settings.tempBladeSegments;
	settings.bladeCnt = settings.bladeCntTmp;
	int numBlades = numGrassBlades();
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, vertexDataBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(GLfloat) * vertexOutputSizeBytes * trianglesPerBlade() * 3 * 
		numBlades, nullptr, GL_STATIC_DRAW);

	shader_compute_grass.setInt("bladeCntX", settings.bladeCntTmp);
	shader_compute_grass.setInt("bladeCntZ", settings.bladeCntTmp);
	shader_compute_grass.setFloat("bladeWidth", settings.bladeWidth);
	shader_compute_grass.setFloat("bladeHeight", settings.bladeHeight * settings.bladeHeightScale);

	shader_compute_grass.setFloat("bladePosVariance", settings.bladePosVariance);
	shader_compute_grass.setFloat("bladeWidthVariance", settings.bladeWidthVariance);
	shader_compute_grass.setFloat("bladeHeightVariance", settings.bladeHeightVariance);

	shader_compute_grass.setFloat("bladeHeightNoiseScale", settings.bladeHeightNoiseScale);

	shader_compute_grass.setFloat("bladeBendMax", settings.bladeBendMax);

	shader_compute_grass.setInt("bladeSegments", settings.bladeSegments);
	shader_compute_grass.setFloat("bladeCurvature", settings.bladeCurvature);

	shader_compute_grass.setVec2("density", vec2(settings.density, settings.density));

	shader_compute_grass.setInt("applyBendVariance", settings.applyBendVariance);
	shader_compute_grass.setInt("applyPosWidthVariance", settings.applyPosWidthVariance);
	shader_compute_grass.setInt("applyHeightVariance", settings.applyHeightVariance);

	// shader_compute_grass.setFloat("hillHeightMax", settings.hillHeightMax);
	// shader_compute_grass.setFloat("hillHeightNoiseScale", settings.hillHeightNoiseScale);

	glDispatchCompute(
		ceil((2*settings.bladeCntTmp+1) / workGroupSz.x), 
		ceil(1/workGroupSz.y), 
		ceil((2*settings.bladeCntTmp+1) / workGroupSz.z));
	glMemoryBarrier(GL_ALL_BARRIER_BITS);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);

	shader_compute_grass.detach();

	// posWS
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 
		sizeof(GLfloat) * vertexOutputSizeBytes, reinterpret_cast<void*>(0));

	// normWS
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 
		sizeof(GLfloat) * vertexOutputSizeBytes, reinterpret_cast<void*>(4 * sizeof(GLfloat)));

	// uv
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 
		sizeof(GLfloat) * vertexOutputSizeBytes, reinterpret_cast<void*>(8 * sizeof(GLfloat)));

	// planeUV
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 
		sizeof(GLfloat) * vertexOutputSizeBytes, reinterpret_cast<void*>(10 * sizeof(GLfloat)));

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 
		sizeof(GLfloat) * vertexOutputSizeBytes, reinterpret_cast<void*>(3 * sizeof(GLfloat)));

	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, 
		sizeof(GLfloat) * vertexOutputSizeBytes, reinterpret_cast<void*>(7 * sizeof(GLfloat)));

	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, 
		sizeof(GLfloat) * vertexOutputSizeBytes, reinterpret_cast<void*>(12 * sizeof(GLfloat)));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER,0);

}

void GrassHandler::generateWindTexture() {
	shader_default.useProgram();
	ErrorHandler::errorCheck("-- on use program");
	shader_default.setVec4("windNoise_TexelSize", vec4(float(1)/WIND_NOISE_TEXTURE_SZ, float(1)/WIND_NOISE_TEXTURE_SZ,
		WIND_NOISE_TEXTURE_SZ, WIND_NOISE_TEXTURE_SZ));
	shader_default.setInt("windNoise", 0);
	ErrorHandler::errorCheck("-- on set ints");

	shader_compute_windNoise.useProgram();

	glGenTextures(1, &wind_noiseTexture);
	glBindTexture(GL_TEXTURE_2D, wind_noiseTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, WIND_NOISE_TEXTURE_SZ, WIND_NOISE_TEXTURE_SZ, 
		0, GL_RGBA, GL_FLOAT, nullptr);
	glBindImageTexture(0, wind_noiseTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

	ErrorHandler::errorCheck("-- on gen texture");

	glDispatchCompute(
		ceil(WIND_NOISE_TEXTURE_SZ/workGroupSz.x), 
		ceil(WIND_NOISE_TEXTURE_SZ/workGroupSz.x), 1);
	glMemoryBarrier(GL_ALL_BARRIER_BITS);

	ErrorHandler::errorCheck("-- on dispatch");

	glBindImageTexture(0, 0, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
	shader_compute_windNoise.detach();
}

void GrassHandler::destroyWindTexture() {
	glDeleteTextures(1, &wind_noiseTexture);
}