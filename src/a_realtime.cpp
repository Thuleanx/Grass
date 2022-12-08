#include "realtime.h"
#include <iostream>
#include "utils/scenedata.h"
#include "settings.h"
#include <glm/gtc/type_ptr.hpp>
#include "t_utils/Blit.h"

#define f(i,a,b) for (int i = a; i < b; i++)
using namespace glm;
using namespace std;

// void Realtime::a_deleteFBOandTextures() {
//     glBindTexture(GL_TEXTURE_2D, 0);
// 	glBindFramebuffer(GL_FRAMEBUFFER, 0);
// }

// void Realtime::a_deleteOpenGLVars() {
//     Blit::destroy();
//     a_deleteFBOandTextures();
// 	shader_default.destroy();

// 	t_shapeMap.clear(); // effectively drops references to unique ptr 
// }


// void Realtime::a_drawScene(const ShaderProgram &shader) {
// 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
// 	shader.useProgram();
//     a_loadSceneGlobalData(shader);
//     a_loadCameraData(shader);
//     a_loadLightData(shader);

// 	unordered_map<int, vector<int>> buckets;
// 	int n = t_renderData.shapes.size();
// 	f(i,0,n) {
//         int key = t_getKey(t_camera.getPosition(), t_renderData.shapes[i]);
// 		if (!buckets.count(key)) buckets[key] = vector<int>();
// 		buckets[key].push_back(i);
// 	}

//     for (const auto &[mapKey, shape_ptr] : t_shapeMap) {
// 		if (buckets.count(mapKey)) {
//             GLuint vao = t_vao[mapKey];
// 			glBindVertexArray(vao);

// 			for (int& index : buckets[mapKey]) {
//                 a_loadShape(shader,index);
// 				glDrawArrays(GL_TRIANGLES, 0, shape_ptr->shapeCount() * 9);
// 			}
// 		}
// 	}

// 	glBindVertexArray(0);
// 	shader.detach();
// }
