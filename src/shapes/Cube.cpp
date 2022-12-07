#include "Cube.h"
#define f(i,a,b) for (int i = a; i < b; i++)

void Cube::updateParams(int param1, int param2) {
    m_vertexData = std::vector<float>();
    m_param1 = std::max(param1,1);
    setVertexData();
}

void Cube::makeTile(glm::vec3 topLeft,
                    glm::vec3 topRight,
                    glm::vec3 bottomLeft,
                    glm::vec3 bottomRight) {
    // Task 2: create a tile (i.e. 2 triangles) based on 4 given points.

	glm::vec3 normal = normalize(cross(topRight-topLeft, topRight-bottomRight));

	insertVec3(m_vertexData, bottomRight); insertVec3(m_vertexData, normal);
	insertVec3(m_vertexData, topRight); insertVec3(m_vertexData, normal);
	insertVec3(m_vertexData, topLeft); insertVec3(m_vertexData, normal);

	insertVec3(m_vertexData, bottomRight); insertVec3(m_vertexData, normal);
	insertVec3(m_vertexData, topLeft); insertVec3(m_vertexData, normal);
	insertVec3(m_vertexData, bottomLeft); insertVec3(m_vertexData, normal);
}

void Cube::makeFace(glm::vec3 topLeft,
                    glm::vec3 topRight,
                    glm::vec3 bottomLeft,
                    glm::vec3 bottomRight) {
    // Task 3: create a single side of the cube out of the 4
    //         given points and makeTile()
    // Note: think about how param 1 affects the number of triangles on
    //       the face of the cube

	glm::vec3 tx = (topRight - topLeft) / float(m_param1);
	glm::vec3 ty = (topLeft - bottomLeft) / float(m_param1);

    f(i,0,m_param1) {
    	f(j,0,m_param1) {
			glm::vec3 verts[4];
			f(dx,0,2) f(dy,0,2) 
				verts[dy<<1|dx] = tx * float(i+dx) + ty * float(j+dy) + bottomLeft;
			makeTile(verts[2], verts[3], verts[0], verts[1]);
		}
	}


}

void Cube::setVertexData() {
    // Uncomment these lines for Task 2, then comment them out for Task 3:

    /* makeTile(glm::vec3(-0.5f,  0.5f, 0.5f), */
    /*           glm::vec3( 0.5f,  0.5f, 0.5f), */
    /*           glm::vec3(-0.5f, -0.5f, 0.5f), */
    /*           glm::vec3( 0.5f, -0.5f, 0.5f)); */

    // Uncomment these lines for Task 3:

    /* makeFace(glm::vec3(-0.5f,  0.5f, 0.5f), */
    /*           glm::vec3( 0.5f,  0.5f, 0.5f), */
    /*           glm::vec3(-0.5f, -0.5f, 0.5f), */
    /*           glm::vec3( 0.5f, -0.5f, 0.5f)); */

    // Task 4: Use the makeFace() function to make all 6 sides of the cube

    int di[] = {-1,1,-1,1};
    int dj[] = {1,1,-1,-1};

	f(d,0,3) {
		f(s,-1,2) if (s) {
			glm::vec3 verts[4];
			f(k,0,4) {
				verts[k][d] = s*0.5f;
				verts[k][(d + s + 3) % 3] = di[k]*0.5f;
				verts[k][(d - s + 3) % 3] = dj[k]*0.5f;
			}
			makeFace(verts[0], verts[1], verts[2], verts[3]);
		}
	}
}

// Inserts a glm::vec3 into a vector of floats.
// This will come in handy if you want to take advantage of vectors to build your shape!
void Cube::insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}
