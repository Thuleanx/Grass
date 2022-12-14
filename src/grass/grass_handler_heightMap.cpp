#include "grass_handler.h"
#include "t_utils/ErrorHandler.h"
#include <QCoreApplication>
#include <QWindow>

#define f(i,a,b) for (int i = a; i < b; i++)
using namespace glm;
using namespace std;

void GrassHandler::initHeightMap() {
	QImage heightImage(QString(":/resources/textures/heightmap1.png"));
	heightImage = heightImage.convertToFormat(QImage::Format_RGBA8888).mirrored();

	Framebuffer::createTexture(
        heightMap, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE,
        heightImage.width(), heightImage.height(), GL_LINEAR, GL_REPEAT, heightImage.bits()
	);
}

void GrassHandler::destroyHeightMap() {
	glDeleteTextures(1, &heightMap);
}
