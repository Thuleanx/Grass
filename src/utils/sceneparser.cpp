#include "sceneparser.h"
#include "scenefilereader.h"
#include "glm/gtx/transform.hpp"

#include <QPainter>
#include <chrono>
#include <memory>
#include <iostream>

using namespace std;
#define f(i,a,b) for (int i = a; i < b; i++)
#define fr(i,a,b) for (int i = (b)-1; i >= a; i--)
#define sz(x) ((x).size())

glm::mat4 transform(glm::mat4 m, SceneTransformation* t) {
    if (t->type == TransformationType::TRANSFORMATION_TRANSLATE) return m * glm::translate(t->translate);
    if (t->type == TransformationType::TRANSFORMATION_SCALE) return m * glm::scale(t->scale);
    if (t->type == TransformationType::TRANSFORMATION_ROTATE) return m * glm::rotate(glm::mat4(1), t->angle, t->rotate);
    if (t->type == TransformationType::TRANSFORMATION_MATRIX) return m * t->matrix;
	return glm::mat4(1);
}

void trav(SceneNode *node, RenderData &renderData, glm::mat4 ctm) {
	if (!node) return;
    f(i, 0, sz(node->transformations))	ctm = transform(ctm, node->transformations[i]);
	f(i,0,sz(node->primitives)) {
		renderData.shapes.push_back(
            RenderShapeData{*(node->primitives[i]), ctm}
        );
	}
	f(i,0,sz(node->children))
		trav(node->children[i], renderData, ctm);
}

bool SceneParser::parse(std::string filepath, RenderData &renderData) {
    ScenefileReader fileReader = ScenefileReader(filepath);
    bool success = fileReader.readXML();
    if (!success) {
        return false;
    }

    renderData.globalData = fileReader.getGlobalData();
    renderData.cameraData = fileReader.getCameraData();
    renderData.lights = fileReader.getLights();
    renderData.shapes.clear();

    SceneNode* root = fileReader.getRootNode();

	// Load shapes
    trav(root, renderData, glm::mat4(1));

    return true;
}

