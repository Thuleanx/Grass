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

vector<float> LODLU;

float tesselationMultiplier(int sceneObjectCount) {
	if (!settings.extraCredit1) return 1;
	// so multiplier goes like this: from 10^x to 10^{x+1}-1, the tesselation count for each parameter is scaled by a factor of 1/(x+1), and ceiled up
	return 1/floor(1 + log(sceneObjectCount)/log(10));
}

void Realtime::t_calculateLOD() {
	LODLU = vector<float>(shapeBucketCnt);
	f(i,0,shapeBucketCnt) LODLU[i] = pow(0.5f, i);
}

float LODlookup(int distanceTesselation) {
	// divide near->far plane into shapeBucketCnt sections by sqrt distances
	return LODLU[distanceTesselation];
}

int LODindex(vec4 camPos, vec4 shapePos, int shapeBucketCnt) {
	vec3 displacement = camPos - shapePos;
	float dist = sqrt(dot(displacement, displacement));

	// dont tesselate
	if (!settings.extraCredit2) return 0;

	f(i,0,shapeBucketCnt-1) 
        if (dist <= (settings.farPlane - settings.nearPlane)*float(i+1)/shapeBucketCnt + settings.nearPlane)
			return i;
	return shapeBucketCnt-1;
}

int Realtime::t_getKey(vec4 camPos, RenderShapeData &shape) const {
    int distanceTesselation = LODindex(camPos, shape.ctm * vec4(0,0,0,1), shapeBucketCnt);
	int mapKey = (distanceTesselation << 3) | int(shape.primitive.type);
	return mapKey;
}

unique_ptr<iShape> getShapeFromPrimitive(PrimitiveType type) {
    if (type == PrimitiveType::PRIMITIVE_CONE) return mu<Cone>(Cone());
    if (type == PrimitiveType::PRIMITIVE_CUBE) return mu<Cube>(Cube());
    if (type == PrimitiveType::PRIMITIVE_CYLINDER) return mu<Cylinder>(Cylinder());
    if (type == PrimitiveType::PRIMITIVE_SPHERE) return mu<Sphere>(Sphere());
    return mu<Triangle>(Triangle());
}

void Realtime::t_generateShapeMap() {
	t_shapeMap.clear();
	for (const RenderShapeData& shape : t_renderData.shapes) {
        if (!t_shapeMap.count(int(shape.primitive.type))) {
			f(i,0,shapeBucketCnt) {
                int mapKey = (i << 3) | int(shape.primitive.type);
				t_shapeMap[mapKey] = getShapeFromPrimitive(shape.primitive.type); // we gotta be careful of references suddenly getting dropped 
			}
		}
	}
}

void Realtime::t_calculateVAOVBO() {
	float numberOfObjeffect = tesselationMultiplier(t_renderData.shapes.size());
    for (const auto &[mapKey, shape_ptr] : t_shapeMap) {
		// vbo and vao calculations here
		if (!t_vao.count(mapKey)) glGenVertexArrays(1, &t_vao[mapKey]);
		if (!t_vbo.count(mapKey)) glGenBuffers(1, &t_vbo[mapKey]);
		GLuint vao = t_vao[mapKey];
		GLuint vbo = t_vbo[mapKey];

		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER,vbo);

		// mapKey>>3 is the distance tesselation
		float LODeffect = LODlookup(mapKey>>3);
		float multi = LODeffect * numberOfObjeffect;

		shape_ptr->updateParams(settings.shapeParameter1 * multi, settings.shapeParameter2 * multi);

		vector<float> data = shape_ptr->generateShape();
		glBufferData(GL_ARRAY_BUFFER,data.size() * sizeof(GLfloat),data.data(), GL_STATIC_DRAW);


		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, reinterpret_cast<void*>(0));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, reinterpret_cast<void*>(3 * sizeof(GLfloat)));

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER,0);
	}
}

void Realtime::t_loadScene() {
	// glClearColor(0,0,1,1);
    if (settings.sceneFilePath.length() > 0) {
		bool success = SceneParser::parse(settings.sceneFilePath, t_renderData);
		if (!success)
			std::cerr << "Error loading scene: \"" << settings.sceneFilePath << "\"" << std::endl;
		else {
			t_calculateLOD();
			t_camera = Camera(size().width(), size().height(), t_renderData.cameraData);
			t_generateShapeMap();
			t_calculateVAOVBO();
			t_recalculateVAO = false;
		}
	}
}
