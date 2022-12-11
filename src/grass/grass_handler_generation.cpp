#include "grass_handler.h"
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

	int numBlades = numGrassBlades();
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, vertexDataBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(GLfloat) * vertexOutputSizeBytes * trianglesPerBlade * 3 * 
		numBlades, nullptr, GL_STATIC_DRAW);

	shader_compute_grass.setInt("bladeCntX", settings.bladeCnt);
	shader_compute_grass.setInt("bladeCntZ", settings.bladeCnt);
	shader_compute_grass.setFloat("bladeWidth", settings.bladeWidth);
	shader_compute_grass.setFloat("bladeHeight", settings.bladeHeight * settings.bladeHeightScale);

	shader_compute_grass.setFloat("bladePosVariance", settings.bladePosVariance);
	shader_compute_grass.setFloat("bladeWidthVariance", settings.bladeWidthVariance);
	shader_compute_grass.setFloat("bladeHeightVariance", settings.bladeHeightVariance);

	shader_compute_grass.setFloat("bladeHeightNoiseScale", settings.bladeHeightNoiseScale);

	shader_compute_grass.setFloat("bladeBendMax", settings.bladeBendMax);

	shader_compute_grass.setVec2("density", vec2(settings.density, settings.density));

	glDispatchCompute(
		ceil((2*settings.bladeCnt+1) / workGroupSz.x), 
		ceil(1/workGroupSz.y), 
		ceil((2*settings.bladeCnt+1) / workGroupSz.z));
	glMemoryBarrier(GL_ALL_BARRIER_BITS);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);

	shader_compute_grass.detach();

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 
		sizeof(GLfloat) * vertexOutputSizeBytes, reinterpret_cast<void*>(0));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 
		sizeof(GLfloat) * vertexOutputSizeBytes, reinterpret_cast<void*>(4 * sizeof(GLfloat)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 
		sizeof(GLfloat) * vertexOutputSizeBytes, reinterpret_cast<void*>(8 * sizeof(GLfloat)));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER,0);
}