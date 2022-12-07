#include "realtime.h"
#include <iostream>
#include "utils/scenedata.h"
#include "utils/shaderloader.h"
#include "settings.h"
#include <glm/gtc/type_ptr.hpp>
#include "t_utils/Blit.h"
#include "t_utils/Framebuffer.h"

#define f(i, a, b) for (int i = a; i < b; i++)
using namespace glm;
using namespace std;

void Realtime::a_initFBOs()
{
}

void Realtime::a_pipelineInit()
{
	a_initFBOs();

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

	// load shaders in
	a_shader_default = ShaderLoader::createShaderProgram(":/resources/shaders/default.vert",
														 ":/resources/shaders/default.frag");
}

void Realtime::a_pipelineRun()
{
	glViewport(0, 0, size().width() * m_devicePixelRatio, size().height() * m_devicePixelRatio);

drawToDefault:
{
	fbo_main.use();
	a_drawScene(a_shader_default);
}

unbinding:
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
}
