#include "realtime.h"
#include <iostream>
#include "utils/scenedata.h"
#include "settings.h"
#include <glm/gtc/type_ptr.hpp>
#include "t_utils/Blit.h"
#include "t_utils/Framebuffer.h"

#define f(i, a, b) for (int i = a; i < b; i++)
using namespace glm;
using namespace std;

// void Realtime::a_initFBOs()
// {
// }

// void Realtime::a_pipelineRun()
// {
// 	glViewport(0, 0, size().width() * m_devicePixelRatio, size().height() * m_devicePixelRatio);

// drawToDefault:
// {
// 	fbo_main.use();
//     a_drawScene(shader_default);
// }

// unbinding:
// {
// 	glActiveTexture(GL_TEXTURE0);
// 	glBindTexture(GL_TEXTURE_2D, 0);
// 	glActiveTexture(GL_TEXTURE1);
// 	glBindTexture(GL_TEXTURE_2D, 0);
// 	glActiveTexture(GL_TEXTURE2);
// 	glBindTexture(GL_TEXTURE_2D, 0);
// 	glActiveTexture(GL_TEXTURE3);
// 	glBindTexture(GL_TEXTURE_2D, 0);
// 	glUseProgram(0);
// 	glBindFramebuffer(GL_FRAMEBUFFER, 0);
// }
// }
