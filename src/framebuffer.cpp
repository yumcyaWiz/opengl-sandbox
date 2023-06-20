#include "framebuffer.hpp"

using namespace ogls;

FrameBuffer::FrameBuffer(const std::vector<GLenum>& attachments)
    : attachments(attachments)
{
    glCreateFramebuffers(1, &framebuffer);
    glNamedFramebufferDrawBuffers(framebuffer, this->attachments.size(),
                                  this->attachments.data());

    spdlog::debug("[FrameBuffer] create framebuffer {:x}", framebuffer);
}

FrameBuffer::FrameBuffer(FrameBuffer&& other)
    : framebuffer(other.framebuffer), attachments(other.attachments)
{
    other.framebuffer = 0;
}

FrameBuffer::~FrameBuffer() { release(); }

FrameBuffer& FrameBuffer::operator=(FrameBuffer&& other)
{
    if (this != &other) {
        release();

        framebuffer = other.framebuffer;
        attachments = std::move(other.attachments);

        other.framebuffer = 0;
    }

    return *this;
}

void FrameBuffer::release()
{
    if (framebuffer) {
        spdlog::debug("[FrameBuffer] release framebuffer {:x}", framebuffer);

        glDeleteFramebuffers(1, &framebuffer);
        this->framebuffer = 0;
    }
}

void FrameBuffer::setDrawBuffer(GLenum buf) const
{
    glNamedFramebufferDrawBuffer(framebuffer, buf);
}

void FrameBuffer::setReadBuffer(GLenum buf) const
{
    glNamedFramebufferReadBuffer(framebuffer, buf);
}

void FrameBuffer::bindTexture(const Texture& texture,
                              std::size_t attachment_index) const
{
    glNamedFramebufferTexture(framebuffer, attachments.at(attachment_index),
                              texture.getTextureName(), 0);
}

void FrameBuffer::activate() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
}

void FrameBuffer::deactivate() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }