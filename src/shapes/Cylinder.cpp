#include "Cylinder.h"
#include "glm/gtc/constants.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/compatibility.hpp"
#include "glm/gtc/matrix_transform.hpp"
#define f(i,a,b) for (int i = a; i < b; i++)
using namespace glm;

void Cylinder::updateParams(int param1, int param2) {
    m_vertexData = std::vector<float>();
    m_param1 = std::max(param1, 1);
    m_param2 = std::max(param2, 3);
    setVertexData();
}

// borrowed from cube
void Cylinder::t_makeTile(glm::vec3 topLeft,
                    glm::vec3 topRight,
                    glm::vec3 bottomLeft,
                    glm::vec3 bottomRight, bool normalFace = true) {
    // Task 2: create a tile (i.e. 2 triangles) based on 4 given points.

	glm::vec3 normal = normalize(cross(topRight-topLeft, topRight-bottomRight));

	auto calcNormal = [&](vec3 pos) {
		if (normalFace) return normal;
		pos.y = 0;
		return normalize(pos);
	};

	insertVec3(m_vertexData, bottomRight); insertVec3(m_vertexData, calcNormal(bottomRight));
	insertVec3(m_vertexData, topRight); insertVec3(m_vertexData, calcNormal(topRight));
	insertVec3(m_vertexData, topLeft); insertVec3(m_vertexData, calcNormal(topLeft));

	insertVec3(m_vertexData, bottomRight); insertVec3(m_vertexData, calcNormal(bottomRight));
	insertVec3(m_vertexData, topLeft); insertVec3(m_vertexData, calcNormal(topLeft));
	insertVec3(m_vertexData, bottomLeft); insertVec3(m_vertexData, calcNormal(bottomLeft));
}

void Cylinder::t_makeStrip(glm::vec3 topLeft,
                    glm::vec3 topRight,
                    glm::vec3 bottomLeft,
                    glm::vec3 bottomRight, bool isTriangle) {

	glm::vec3 normal = normalize(cross(topRight-bottomRight, bottomLeft-bottomRight));
	
	f(i,0,m_param1) {
		vec3 bL = lerp(bottomLeft, topLeft, float(i)/m_param1);
		vec3 bR = lerp(bottomRight, topRight, float(i)/m_param1);
		vec3 tL = lerp(bottomLeft, topLeft, float(i+1)/m_param1);
		vec3 tR = lerp(bottomRight, topRight, float(i+1)/m_param1);

		if (isTriangle && i == m_param1 - 1) {
			insertVec3(m_vertexData, bL); insertVec3(m_vertexData, normal);
			insertVec3(m_vertexData, bR); insertVec3(m_vertexData, normal);
			insertVec3(m_vertexData, tL); insertVec3(m_vertexData, normal);
		} else {
			t_makeTile(tL, tR, bL, bR, isTriangle); 
		}
	}
}

void Cylinder::setVertexData() {
    // TODO for Project 5: Lights, Camera
	// make side

	vec3 topCenter = vec3(0, .5, 0);
	vec3 bottomCenter = vec3(0, -.5, 0);

	f(i,0,m_param2) {
        float theta[2] = {
            float(i)/m_param2 * 2*float(M_PI),
            float(i+1)/m_param2 * 2*float(M_PI)
		};

        vec3 bottomLeft = vec3(sin(theta[0]), -1, cos(theta[0])) / 2.0f;
        vec3 bottomRight = vec3(sin(theta[1]), -1, cos(theta[1])) / 2.0f;
		vec3 topLeft = bottomLeft + vec3(0,1,0);
		vec3 topRight = bottomRight + vec3(0,1,0);

		t_makeStrip(topLeft, topRight, bottomLeft, bottomRight, false); 
		t_makeStrip(topCenter, topCenter, topLeft, topRight, true);
        t_makeStrip(bottomCenter, bottomCenter, bottomRight, bottomLeft, true);
	}
}

// Inserts a glm::vec3 into a vector of floats.
// This will come in handy if you want to take advantage of vectors to build your shape!
void Cylinder::insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}
