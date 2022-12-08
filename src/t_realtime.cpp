#include "realtime.h"
#include "settings.h"
#include "utils/sceneparser.h"
#include <iostream>

#include "shapes/Cone.h"
#include "shapes/Cube.h"
#include "shapes/Cylinder.h"
#include "shapes/Sphere.h"
#include "shapes/Triangle.h"

#define f(i,a,b) for (int i = a; i < b; i++)
#define mu make_unique
using namespace std;
using namespace glm;

void Realtime::t_calculateVAOVBO() {
	// glBindVertexArray(vao);
	// glBindBuffer(GL_ARRAY_BUFFER,vbo);

	// // mapKey>>3 is the distance tesselation
	// float LODeffect = LODlookup(mapKey>>3);
	// float multi = LODeffect * numberOfObjeffect;

	// shape_ptr->updateParams(settings.shapeParameter1 * multi, settings.shapeParameter2 * multi);

	// vector<float> data = shape_ptr->generateShape();
	// vector<float> dataOnlyPos;
	// f(i,0,data.size()) {
	// 	if (!((i/3) % 2)) dataOnlyPos.push_back(data[i]);
	// 	if (i%6==5) dataOnlyPos.push_back(0);
	// }
	// int triangleCnt = shape_ptr->shapeCount();

	// // we replace this stage by a compute shader 
	// shader_compute_grass.useProgram();

	// glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, vbo_in);
	// glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(GLfloat) * dataOnlyPos.size(), dataOnlyPos.data(), GL_STREAM_READ);

	// glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, vbo);
	// glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(GLfloat) * dataOnlyPos.size() * 18, nullptr, GL_STATIC_DRAW);

	// shader_compute_grass.setFloat("GrassHeight", 0.2f);
	// shader_compute_grass.setFloat("GrassBaseDistance", 0.05f);
	// shader_compute_grass.setInt("numTriangles", triangleCnt);

	// glDispatchCompute(triangleCnt, 1, 1);
	// glMemoryBarrier(GL_ALL_BARRIER_BITS);

	// glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
	// glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);

	// shader_compute_grass.detach();

	// glEnableVertexAttribArray(0);
	// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, reinterpret_cast<void*>(0));

	// glEnableVertexAttribArray(1);
	// glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, reinterpret_cast<void*>(4 * sizeof(GLfloat)));

	// glBindVertexArray(0);
	// glBindBuffer(GL_ARRAY_BUFFER,0);
}

void Realtime::t_loadScene() {
	// glClearColor(0,0,1,1);
    if (settings.sceneFilePath.length() > 0) {
		bool success = SceneParser::parse(settings.sceneFilePath, t_renderData);
		if (!success)
			std::cerr << "Error loading scene: \"" << settings.sceneFilePath << "\"" << std::endl;
		else {
			t_camera = Camera(size().width(), size().height(), t_renderData.cameraData);
			t_calculateVAOVBO();
			t_recalculateVAO = false;
		}
	}
}
