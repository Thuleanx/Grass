#include "Triangle.h"
using namespace std;

#define f(i,a,b) for (int i = a; i < b; i++)

void Triangle::updateParams(int param1, int param2) {
    m_vertexData = std::vector<float>();
    setVertexData();
}

void Triangle::setVertexData() {
    // Task 1: update m_vertexData with the vertices and normals
    //         needed to tesselate a triangle
    // Note: you may find the insertVec3 function (in the OpenGLShape class
    //       useful in adding your points into m_vertexData

	float x[] = {-0.5, 0, -0.5, 0, 0.5, 0};
	float y[] = {0.5, 0, -0.5, 0, -0.5, 0};
	float z[] = {0, 1, 0, 1, 0, 1};
	f(i,0,6) insertVec3(m_vertexData, glm::vec3(x[i],y[i],z[i]));
}

// Inserts a glm::vec3 into a vector of floats.
// This will come in handy if you want to take advantage of vectors to build your shape!
void Triangle::insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}
