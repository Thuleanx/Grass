#include "realtime.h"
#include <iostream>
#include "utils/scenedata.h"
#include "settings.h"
#include <glm/gtc/type_ptr.hpp>
#include <random>
#include <chrono>
#include <time.h>
#include <iomanip>


#define f(i,a,b) for (int i = a; i < b; i++)
using namespace glm;
using namespace std;

std::vector<GLfloat> fullscreen_quad_data =
{ 
	//     POSITIONS    //
	//     UV 			//
	-1,  1, 0,
	0, 1,
	-1, -1, 0,
    0, 0,
	1, -1, 0,
    1, 0,
	1,  1, 0,
	1, 1,
	-1,  1, 0,
	0, 1,
    1, -1, 0,
	1, 0
};

void Realtime::a_initBlit() {

	glGenVertexArrays(1, &a_blitVAO);
	glGenBuffers(1, &a_blitVBO);

	glBindVertexArray(a_blitVAO);
	glBindBuffer(GL_ARRAY_BUFFER, a_blitVBO);

	glBufferData(GL_ARRAY_BUFFER, fullscreen_quad_data.size() * sizeof(GLfloat), fullscreen_quad_data.data(), 
		GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, reinterpret_cast<void*>(0));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, reinterpret_cast<void*>(3 * sizeof(GLfloat)));

	glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Realtime::a_blit(GLuint fbo) {
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(a_blitVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
