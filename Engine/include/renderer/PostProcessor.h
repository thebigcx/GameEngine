#pragma once

#include <renderer/Framebuffer.h>
#include <renderer/MeshFactory.h>

namespace Engine
{

class PostProcessor
{
public:
    PostProcessor();
    ~PostProcessor();

    void onWindowResize(uint32_t width, uint32_t height);

    Reference<Framebuffer> finishHdr(const Reference<Framebuffer>& framebuffer);
    Reference<Framebuffer> finishHdrAndBloom(const Reference<Framebuffer>& framebuffer);

    static PostProcessor* getInstance()
    {
        static PostProcessor bloom;
        return &bloom;
    }

private:
    Reference<Framebuffer> m_hdrBuffer;
    Reference<Mesh> m_framebufferMesh;

    Reference<Framebuffer> m_pingPongBuffers[2];
};

}