#pragma once

#include "iShape.h"
#include <vector>
#include <glm/glm.hpp>

class Cylinder : public iShape
{
public:
    void updateParams(int param1, int param2);
private:
    void insertVec3(std::vector<float> &data, glm::vec3 v);
	void t_makeTile(glm::vec3 topLeft,
                    glm::vec3 topRight,
                    glm::vec3 bottomLeft,
                    glm::vec3 bottomRight, bool normalFace);
	void t_makeStrip(glm::vec3 topLeft,
                    glm::vec3 topRight,
                    glm::vec3 bottomLeft,
                    glm::vec3 bottomRight, bool isTriangle);
    void setVertexData();
    int m_param1;
    int m_param2;
    float m_radius = 0.5;
};
