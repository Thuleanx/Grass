#pragma once

#include "iShape.h"
#include <vector>
#include <glm/glm.hpp>

class Sphere : public iShape
{
public:
    void updateParams(int param1, int param2);
private:
    void insertVec3(std::vector<float> &data, glm::vec3 v);
    void setVertexData();
    void makeTile(glm::vec3 topLeft,
                  glm::vec3 topRight,
                  glm::vec3 bottomLeft,
                  glm::vec3 bottomRight);
    void makeWedge(float currTheta, float nextTheta);
    void makeSphere();
    float m_radius = 0.5;
    int m_param1;
    int m_param2;
};
