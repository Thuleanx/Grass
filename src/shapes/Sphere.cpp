#include "Sphere.h"

#define f(i,a,b) for (int i = a; i < b; i++)
#define fr(i,a,b) for (int i = b-1; i >= a; i--)

void Sphere::updateParams(int param1, int param2) {
    m_vertexData = std::vector<float>();
    m_param1 = std::max(param1, 2);
    m_param2 = std::max(param2, 3);
    setVertexData();
}

void Sphere::makeTile(glm::vec3 topLeft,
                      glm::vec3 topRight,
                      glm::vec3 bottomLeft,
                      glm::vec3 bottomRight) {
    // Task 5: Implement the makeTile() function for a Sphere
    // Note: this function is very similar to the makeTile() function for Cube,
    //       but the normals are calculated in a different way!

	insertVec3(m_vertexData, bottomRight); 
	insertVec3(m_vertexData, normalize(bottomRight)); 
	insertVec3(m_vertexData, topRight); 
	insertVec3(m_vertexData, normalize(topRight)); 
	insertVec3(m_vertexData, topLeft); 
	insertVec3(m_vertexData, normalize(topLeft)); 
	insertVec3(m_vertexData, bottomRight); 
	insertVec3(m_vertexData, normalize(bottomRight)); 
	insertVec3(m_vertexData, topLeft); 
	insertVec3(m_vertexData, normalize(topLeft)); 
	insertVec3(m_vertexData, bottomLeft); 
	insertVec3(m_vertexData, normalize(bottomLeft));
}

void Sphere::makeWedge(float currentTheta, float nextTheta) {
    // Task 6: create a single wedge of the sphere using the
    //         makeTile() function you implemented in Task 5
    // Note: think about how param 1 comes into play here!

    float theta[2] = {currentTheta, nextTheta};

	f(i,0,m_param1) {
        float phi[2] = {
            float(i)/m_param1 * float(M_PI),
            float(i+1)/m_param1 * float(M_PI)
		};

        glm::vec3 verts[4];

		int p = 0;
        f(yi,0,2) f(xi,0,2)
			verts[p++] = glm::vec3(
                m_radius * sin(phi[yi]) * sin(theta[xi]),
                m_radius * cos(phi[yi]),
                m_radius * sin(phi[yi]) * cos(theta[xi])
			);

		makeTile(verts[0], verts[1], verts[2], verts[3]);
	}
}

void Sphere::makeSphere() {
    // Task 7: create a full sphere using the makeWedge() function you
    //         implemented in Task 6
    // Note: think about how param 2 comes into play here!

    float thetaStep = glm::radians(360.f / m_param2);

    f(i,0,m_param2)  {
		float currentTheta = i * thetaStep;
		float nextTheta = (i+1) * thetaStep;
		makeWedge(currentTheta, nextTheta);
	}
}

void Sphere::setVertexData() {
    // Uncomment these lines to make a wedge for Task 6, then comment them out for Task 7:

    /* float thetaStep = glm::radians(360.f / m_param2); */
    /* float currentTheta = 0 * thetaStep; */
    /* float nextTheta = 1 * thetaStep; */
    /* makeWedge(currentTheta, nextTheta); */

    // Uncomment these lines to make sphere for Task 7:

    makeSphere();
}

// Inserts a glm::vec3 into a vector of floats.
// This will come in handy if you want to take advantage of vectors to build your shape!
void Sphere::insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}
