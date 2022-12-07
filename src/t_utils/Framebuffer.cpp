#include "Framebuffer.h"
using namespace std;

void Framebuffer::initialize() {
	glGenFramebuffers(1, &fbo);
}

void Framebuffer::attachTexture(const GLenum &texture, GLenum attachmentPoint, bool is2D) {
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	if (is2D) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentPoint, GL_TEXTURE_2D, texture, 0);
		attachmentPoints.push_back(attachmentPoint);
	} else	glFramebufferTexture(GL_FRAMEBUFFER, attachmentPoint, texture, 0);
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
}

void Framebuffer::createTexture(GLuint &texture, GLenum format, GLenum internalFormat, 
    GLenum dataType, int width, int height, GLenum filter, GLenum wrapMode) {
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
	glViewport(0,0, width, height); // not needed

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 
		0, internalFormat, dataType, nullptr);
	glBindTexture(GL_TEXTURE_2D, 0);
}