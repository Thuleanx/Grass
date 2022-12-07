#include "Cone.h"
#include "glm/gtc/constants.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/perpendicular.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/compatibility.hpp"
#define f(i,a,b) for (int i = a; i < b; i++)
using namespace glm;

void Cone::updateParams(int param1, int param2) {
    m_vertexData = std::vector<float>();
    m_param1 = std::max(param1, 1);
    m_param2 = std::max(param2, 3);
    setVertexData();
}

// borrowed from cube
void Cone::t_makeTile(glm::vec3 topLeft,
                    glm::vec3 topRight,
                    glm::vec3 bottomLeft,
                    glm::vec3 bottomRight, 
					vec3 normalL,
					vec3 normalR) {
    // Task 2: create a tile (i.e. 2 triangles) based on 4 given points.

	insertVec3(m_vertexData, bottomRight); insertVec3(m_vertexData, normalR);
	insertVec3(m_vertexData, topRight); insertVec3(m_vertexData, normalR);
	insertVec3(m_vertexData, topLeft); insertVec3(m_vertexData, normalL);

	insertVec3(m_vertexData, bottomRight); insertVec3(m_vertexData, normalR);
	insertVec3(m_vertexData, topLeft); insertVec3(m_vertexData, normalL);
	insertVec3(m_vertexData, bottomLeft); insertVec3(m_vertexData, normalL);
}

void Cone::t_makeStrip(glm::vec3 top,
                    glm::vec3 bottomLeft,
                    glm::vec3 bottomRight, bool isBottom) {

	glm::vec3 normalL = normalize(cross(top-bottomRight, bottomLeft-bottomRight));
	glm::vec3 normalR = normalL;
	if (!isBottom) {
		vec3 bottomCenter = vec3(0, -.5, 0);

		vec3 dL = bottomLeft - bottomCenter;
		vec3 dR = bottomRight - bottomCenter;
        vec3 up = vec3(0,1,0);
		vec3 pL = cross(up, dL);
		vec3 pR = cross(up, dR);

		normalL = -normalize(cross(top - bottomLeft, pL));
		normalR = -normalize(cross(top - bottomRight, pR));
	}
	
	f(i,0,m_param1) {
		vec3 bL = lerp(bottomLeft, top, float(i)/m_param1);
		vec3 bR = lerp(bottomRight, top, float(i)/m_param1);
		vec3 tL = lerp(bottomLeft, top, float(i+1)/m_param1);
		vec3 tR = lerp(bottomRight, top, float(i+1)/m_param1);

		if (i == m_param1 - 1) {
			insertVec3(m_vertexData, bL); insertVec3(m_vertexData, normalL);
			insertVec3(m_vertexData, bR); insertVec3(m_vertexData, normalR);
			vec3 normalTop = normalL;
			if (!isBottom) normalTop = normalize(cross(bottomRight - bottomLeft, top-bottomLeft));
			insertVec3(m_vertexData, tL); insertVec3(m_vertexData, normalTop);
		} else 
			t_makeTile(tL, tR, bL, bR, normalL, normalR); 
	}
}

void Cone::setVertexData() {
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

		t_makeStrip(topCenter, bottomLeft, bottomRight, false); 
		t_makeStrip(bottomCenter, bottomRight, bottomLeft, true);
	}
}

// Inserts a glm::vec3 into a vector of floats.
// This will come in handy if you want to take advantage of vectors to build your shape!
void Cone::insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}
