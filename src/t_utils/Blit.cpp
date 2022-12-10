#include "Blit.h"

std::vector<GLfloat> Blit::fullscreen_quad_data =
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
GLuint Blit::vao;
GLuint Blit::vbo;

void Blit::initialize() {
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, fullscreen_quad_data.size() * sizeof(GLfloat), fullscreen_quad_data.data(), 
		GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, reinterpret_cast<void*>(0));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, reinterpret_cast<void*>(3 * sizeof(GLfloat)));

	glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Blit::blit(const Framebuffer &fbo) {
	fbo.use();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	fbo.detach();
}

void Blit::destroy() {
	glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}
