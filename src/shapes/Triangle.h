#pragma once

#include "iShape.h"
#include <vector>
#include <glm/glm.hpp>

class Triangle : public iShape
{
public:
    void updateParams(int param1, int param2);

private:
    void insertVec3(std::vector<float> &data, glm::vec3 v);
    void setVertexData();
};
