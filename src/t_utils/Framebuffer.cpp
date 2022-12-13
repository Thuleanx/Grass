#include "Framebuffer.h"
#include "t_utils/ErrorHandler.h"
using namespace std;

void Framebuffer::initialize() {
	glGenFramebuffers(1, &fbo);
}

void Framebuffer::attachTexture(const GLuint &texture, GLenum attachmentPoint, bool is2D) {
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	if (is2D) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentPoint, GL_TEXTURE_2D, texture, 0);
		attachmentPoints.push_back(attachmentPoint);
	} else	glFramebufferTexture(GL_FRAMEBUFFER, attachmentPoint, texture, 0);
}

void Framebuffer::attachRenderBuffer(const GLuint &renderBuffer, GLenum attachmentPoint) {
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachmentPoint, GL_RENDERBUFFER, renderBuffer);
}

void Framebuffer::finalize() {
	glDrawBuffers(attachmentPoints.size(), attachmentPoints.data());
}

void Framebuffer::use() const { glBindFramebuffer(GL_FRAMEBUFFER, fbo); }
void Framebuffer::detach() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

void Framebuffer::checkStatus() {
	GLenum err = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	assert(err == GL_FRAMEBUFFER_COMPLETE);
}

void Framebuffer::destroy() {
	glDeleteFramebuffers(1, &fbo);
	attachmentPoints.clear();
}

void Framebuffer::createTexture(GLuint &texture, GLenum format, GLenum internalFormat, 
    GLenum dataType, int width, int height, GLenum filter, GLenum wrapMode, const void* data) {
	glGenTextures(1, &texture);
	ErrorHandler::errorCheck("-- on create texture 1");
	glBindTexture(GL_TEXTURE_2D, texture);
	ErrorHandler::errorCheck("-- on create texture 2");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	ErrorHandler::errorCheck("-- on create texture 3");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
	ErrorHandler::errorCheck("-- on create texture 4");

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 
		0, internalFormat, dataType, data);
	ErrorHandler::errorCheck("-- on create texture 5");
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Framebuffer::createRenderBuffer(GLuint &renderBuffer, GLenum internalFormat, int width, int height) {
	glGenRenderbuffers(1, &renderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}
