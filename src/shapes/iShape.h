#pragma once

#include <vector>
#include <glm/glm.hpp>

class iShape
{
public:
    virtual void updateParams(int param1, int param2) = 0;
    std::vector<float> generateShape() { return m_vertexData; }
	int size() { return m_vertexData.size(); }
	int shapeCount() { return m_vertexData.size() / 6; }

protected:
    std::vector<float> m_vertexData;
};
