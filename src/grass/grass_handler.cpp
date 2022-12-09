#include "grass_handler.h"
#include "t_utils/Blit.h"

#define f(i,a,b) for (int i = a; i < b; i++)
using namespace glm;
using namespace std;

void GrassHandler::initShaders() {
	shader_default.initializeProgram();
	shader_default.attachShader(GL_VERTEX_SHADER, ":/resources/shaders/grass.vert");
	shader_default.attachShader(GL_FRAGMENT_SHADER, ":/resources/shaders/grass.frag");
	shader_default.finalizeProgram();

	shader_compute_grass.initializeProgram();
	shader_compute_grass.attachShader(GL_COMPUTE_SHADER, ":/resources/shaders/grassGenerate.compute");
	shader_compute_grass.finalizeProgram();
}

void GrassHandler::initVAOVBO() {
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vertexDataBuffer);
}

void GrassHandler::generateGrass() {
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vertexDataBuffer);

	shader_compute_grass.useProgram();

	int numBlades = numGrassBlades();
	cout << numBlades << endl;
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, vertexDataBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(GLfloat) * vertexOutputSizeBytes * trianglesPerBlade * 3 * 
		numBlades, nullptr, GL_STATIC_DRAW);

	shader_compute_grass.setInt("bladeCntX", bladeCntX);
	shader_compute_grass.setInt("bladeCntZ", bladeCntZ);
	shader_compute_grass.setFloat("bladeWidth", bladeWidth);
	shader_compute_grass.setFloat("bladeHeight", bladeHeight);
	shader_compute_grass.setVec2("density", vec2(density, density));

	glDispatchCompute(2*bladeCntX+1, 1, 2*bladeCntZ+1);
	glMemoryBarrier(GL_ALL_BARRIER_BITS);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);

	shader_compute_grass.detach();

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, reinterpret_cast<void*>(0));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, reinterpret_cast<void*>(4 * sizeof(GLfloat)));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER,0);
}

void GrassHandler::awake(RenderData &renderData) {
    Blit::initialize();

	int work_grp_cnt[3];
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &work_grp_cnt[0]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &work_grp_cnt[1]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &work_grp_cnt[2]);

	printf("max global (total) work group counts x:%i y:%i z:%i\n",
		   work_grp_cnt[0], work_grp_cnt[1], work_grp_cnt[2]);

	int work_grp_size[3];
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &work_grp_size[0]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &work_grp_size[1]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &work_grp_size[2]);

	printf("max local (in one shader) work group sizes x:%i y:%i z:%i\n",
		   work_grp_size[0], work_grp_size[1], work_grp_size[2]);

	int work_grp_inv;
	glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &work_grp_inv);
	printf("max local work group invocations %i\n", work_grp_inv);

	initShaders();
	initVAOVBO();
	generateGrass();
	loadGrassData(shader_default);
	loadLightData(shader_default, renderData);
}

void GrassHandler::onResize(int screen_width, int screen_height, 
	int fbo_width, int fbo_height) {

	this->screen_width = screen_width;
	this->screen_height = screen_height;
	this->fbo_width = fbo_width;
	this->fbo_height = fbo_height;
}

GLenum errorCheck()
{
	GLenum code;
	const GLubyte *string;
	code = glGetError();
	if (code != GL_NO_ERROR)
	{
		string = gluErrorString(code);
		fprintf(stderr, "OpenGL error: %s\n", string);
	}
	return code;
}

void GrassHandler::update(Camera &camera) {
	glViewport(0, 0, screen_width, screen_height);
	fbo_main.use();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shader_default.useProgram();
	loadCameraData(shader_default, camera);

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, trianglesPerBlade * numGrassBlades());
	glBindVertexArray(0);
	errorCheck();

	shader_default.detach();
	fbo_main.detach();
}

void GrassHandler::onDestroy() {
	Blit::destroy();
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vertexDataBuffer);
	shader_default.destroy();
	shader_compute_grass.destroy();
}
