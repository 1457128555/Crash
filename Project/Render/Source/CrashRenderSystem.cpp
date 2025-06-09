#include "CrashPCH.h"
#include "CrashRenderSystem.h"
#include "CrashRenderCommand.h"
#include "CrashShaderProgram.h"
#include "CrashRenderBuffer.h"
#include "CrashTexture.h"
#include "CrashFrameBuffer.h"

namespace Crash
{
    RenderSystem::RenderSystem(void* procAddress, bool asyncRender)
        : mProcAddress(procAddress)
        , mAsyncRender(asyncRender)
    {
        
    }
    
    RenderSystem::~RenderSystem()
    {

    }

    void RenderSystem::init()
    {
        if(mInit) return;
        mInit = true;

        if(false == RenderCommand::Initlize(mProcAddress))
        {
            LogManager::Instance()->log("RenderCommand initialization failed");
            assert(false && "RenderCommand initialization failed");
        }
    }

    void RenderSystem::uninit()
    {
        if(!mInit) return;
        mInit = false;

        RenderCommand::Shutdown();
    }

    void RenderSystem::clear(RenderProtocol::ClearFlag flag)
    {
        auto command = [_flag = flag]{ RenderCommand::Clear(_flag); };
        if(mAsyncRender)
            mCommandQueue[0].push_back(command);
        else
            command();
    }

    void RenderSystem::frameBegin()
    {
        auto command = []{};
        if(mAsyncRender)
            mCommandQueue[0].push_back(command);
        else
            command();
    }

    void RenderSystem::frameEnd()
    {
        auto command = []{};
        if(mAsyncRender)
            mCommandQueue[0].push_back(command);
        else
            command();
    }

    void RenderSystem::setDepthEnable(bool enable)
    {
        auto command = [_enable = enable]{ RenderCommand::SetDepthEnable(_enable); };
        if(mAsyncRender)
            mCommandQueue[0].push_back(command);
        else
            command();
    }

    void RenderSystem::setDepthFunc(RenderProtocol::CompareFunc func)
    {
        auto command = [_func = func]{ RenderCommand::SetDetphFunc(_func); };
        if(mAsyncRender)
            mCommandQueue[0].push_back(command);
        else
            command();
    }

    void RenderSystem::setClearDepth(float depth)
    {
        auto command = [_depth = depth]{ RenderCommand::SetClearDepth(_depth); };
        if(mAsyncRender)
            mCommandQueue[0].push_back(command);
        else
            command();
    }

    void RenderSystem::setStencilEnable(bool enable)
    {
        auto command = [_enable = enable]{ RenderCommand::SetStencilEnable(_enable); };
        if(mAsyncRender)
            mCommandQueue[0].push_back(command);
        else
            command();
    }

    void RenderSystem::setStencilFunc(RenderProtocol::CompareFunc func, int ref, unsigned int mask)
    {
        auto command = [_func = func, _ref = ref, _mask = mask] {
            RenderCommand::SetStencilFunc(_func, _ref, _mask);
        };
        if(mAsyncRender)
            mCommandQueue[0].push_back(command);
        else
            command();
    }

    void RenderSystem::setStencilOp(RenderProtocol::OperateFunc sfail, RenderProtocol::OperateFunc dpfail, RenderProtocol::OperateFunc dppass)
    {
        auto command = [_sfail = sfail, _dpfail = dpfail, _dppass = dppass] {
            RenderCommand::SetStencilOp(_sfail, _dpfail, _dppass);
        };
        if(mAsyncRender)
            mCommandQueue[0].push_back(command);
        else
            command();
    }

    void RenderSystem::setStencilMask(unsigned int mask)
    {
        auto command = [_mask = mask] {
            RenderCommand::SetStencilMask(_mask); 
        };
        if(mAsyncRender)
            mCommandQueue[0].push_back(command);
        else
            command();
    }

    void RenderSystem::setClearColor(const glm::vec4& color)
    {
        auto command = [_color = color] {
            RenderCommand::SetClearColor(_color);
        };
        if(mAsyncRender)
            mCommandQueue[0].push_back(command);
        else
            command();
    }

    void RenderSystem::setCullFaceEnable(bool enable)
    {
        auto command = [_enable = enable] {
            RenderCommand::SetCullFaceEnable(_enable);
        };
        if(mAsyncRender)
            mCommandQueue[0].push_back(command);
        else
            command();
    }

    void RenderSystem::setViewport(const glm::ivec4& viewport)
    {
        auto command = [_viewport = viewport] {
            RenderCommand::SetViewport(_viewport);
        };
        if(mAsyncRender)
            mCommandQueue[0].push_back(command);
        else
            command();
    }

    ShaderProgram* RenderSystem::createShaderProgram(const std::string& name, const std::string& vs, const std::string& fs)
    {
        ShaderProgram* program = new ShaderProgram(name, vs, fs);

        auto command = [_program = program] {
            _program->createHandle();
        };
        if(mAsyncRender)
            mCommandQueue[0].push_back(command);
        else
            command();

        return program;
    }

    void RenderSystem::destroyShaderProgram(ShaderProgram* program)
    {
        auto command = [_program = program] {
            delete _program;
        };
        if(mAsyncRender)
            mCommandQueue[0].push_back(command);
        else
            command();
    }

    void RenderSystem::bindShaderProgram(ShaderProgram* program)
    {
        auto command = [_program = program] {
            _program->bind();
        };
        if(mAsyncRender)
            mCommandQueue[0].push_back(command);
        else
            command();
    }

    void RenderSystem::unbindShaderProgram()
    {
        auto command = [] {
            RenderCommand::UnbindShaderProgram();
        };
        if(mAsyncRender)
            mCommandQueue[0].push_back(command);
        else
            command();
    }

    void RenderSystem::setUniform1i(const ShaderProgram* program, const std::string& name, int value)
    {
        auto command = [_program = program, _name = name, _value = value] {
            _program->setUniform1i(_name, _value);
        };
        if(mAsyncRender)
            mCommandQueue[0].push_back(command);
        else
            command();
    }

    void RenderSystem::setUniform4f(const ShaderProgram* program, const std::string& name, const glm::vec4& value)
    {
        auto command = [_program = program, _name = name, _value = value] {
            _program->setUniform4f(_name, _value);
        };
        if(mAsyncRender)
            mCommandQueue[0].push_back(command);
        else
            command();
    }

    void RenderSystem::setUniformMatrix4fv(const ShaderProgram* program, const std::string& name, const glm::mat4& value)
    {
        auto command = [_program = program, _name = name, _value = value] {
            _program->setUniformMatrix4fv(_name, _value);
        };
        if(mAsyncRender)
            mCommandQueue[0].push_back(command);
        else
            command();
    }

    FrameBuffer* RenderSystem::createFrameBuffer(const std::string& name, unsigned int width, unsigned int height, bool useRBO)
    {
        FrameBuffer* framebuffer = new FrameBuffer(name, width, height, useRBO);
        auto command = [_framebuffer = framebuffer] {
            _framebuffer->createHandle();
        };

        if(mAsyncRender)
            mCommandQueue[0].push_back(command);
        else
            command();

        return framebuffer;
    }

    void RenderSystem::destroyFrameBuffer(FrameBuffer* framebuffer)
    {
        auto command = [_framebuffer = framebuffer] {
            delete _framebuffer;
        };
        if(mAsyncRender)
            mCommandQueue[0].push_back(command);
        else
            command();
    }

    void RenderSystem::bindFrameBuffer(FrameBuffer* framebuffer)
    {
        auto command = [_framebuffer = framebuffer] {
            _framebuffer->bind();
        };
        if(mAsyncRender)
            mCommandQueue[0].push_back(command);
        else
            command();
    }

    void RenderSystem::unbindFrameBuffer()
    {
        auto command = [] {
            RenderCommand::UnbindFrameBuffer();
        };
        if(mAsyncRender)
            mCommandQueue[0].push_back(command);
        else
            command();
    }

    const std::shared_ptr<Texture>& RenderSystem::getFrameBufferColorAttachment(FrameBuffer* framebuffer)const
    {
        return framebuffer->getColorAttachment();
    }

    const std::shared_ptr<Texture>& RenderSystem::getFrameBufferDepthStencilAttachment(FrameBuffer* framebuffer)const
    {
        return framebuffer->getDepthStencilAttachment();
    }

    VertexArrayObject* RenderSystem::createVertexArray()
    {
        VertexArrayObject* vao = new VertexArrayObject();

        auto command = [_vao = vao] {
            _vao->createHandle();
        };
        if(mAsyncRender)
            mCommandQueue[0].push_back(command);
        else
            command();

        return vao;
    }

    void RenderSystem::destroyVertexArray(VertexArrayObject* vao)
    {
        auto command = [_vao = vao] {
            delete _vao;
        };
        if(mAsyncRender)
            mCommandQueue[0].push_back(command);
        else
            command();
    }

    void RenderSystem::bindVertexArray(VertexArrayObject* vao)
    {
        auto command = [_vao = vao] {
            _vao->bind();
        };
        if(mAsyncRender)
            mCommandQueue[0].push_back(command);
        else
            command();
    }

    void RenderSystem::unbindVertexArray()
    {
        auto command = [] {
            RenderCommand::UnbindVertexArray();
        };
        if(mAsyncRender)
            mCommandQueue[0].push_back(command);
        else
            command();
    }

    void RenderSystem::addBufferToVertexArray(VertexArrayObject* vao, VertexBuffer* buffer, unsigned int index, unsigned int size, unsigned int stride, const void* pointer)
    {
        auto command = [_vao = vao, _buffer = buffer, _index = index, _size = size, _stride = stride, _pointer = pointer] {
            _vao->addBuffer(_buffer, _index, _size, _stride, _pointer);
        };
        if(mAsyncRender)
            mCommandQueue[0].push_back(command);
        else
            command();
    }

    void RenderSystem::addBufferToVertexArray(VertexArrayObject* vao, IndexBuffer* buffer)
    {
        auto command = [_vao = vao, _buffer = buffer] {
            _vao->addBuffer(_buffer);
        };
        if(mAsyncRender)
            mCommandQueue[0].push_back(command);
        else
            command();
    }

    VertexBuffer* RenderSystem::createBuffer()
    {
        VertexBuffer* buffer = new VertexBuffer(RenderProtocol::BufferUsage::StaticDraw);

        auto command = [_buffer = buffer] {
            _buffer->createHandle();
        };
        if(mAsyncRender)
            mCommandQueue[0].push_back(command);
        else
            command();

        return buffer;
    }

    void RenderSystem::destroyBuffer(VertexBuffer* buffer)
    {
        auto command = [_buffer = buffer] {
            delete _buffer;
        };
        if(mAsyncRender)
            mCommandQueue[0].push_back(command);
        else
            command();
    }

    void RenderSystem::setBufferData(VertexBuffer* buffer, const void* data, unsigned int size)
    {
        void* safeData = std::malloc(size);
        std::memcpy(safeData, data, size);

        auto command = [_buffer = buffer, _data = safeData, _size = size] {
            _buffer->setBufferData(_data, _size);
            std::free(_data); 
        };  
        if(mAsyncRender)
            mCommandQueue[0].push_back(command);
        else
            command();
    }

    IndexBuffer* RenderSystem::createIndexBuffer()
    {
        IndexBuffer* buffer = new IndexBuffer(RenderProtocol::BufferUsage::StaticDraw);
        auto command = [_buffer = buffer] {
            _buffer->createHandle();
        };
        if(mAsyncRender)
            mCommandQueue[0].push_back(command);
        else
            command();
        return buffer;
    }

    void RenderSystem::destroyIndexBuffer(IndexBuffer* buffer)
    {
        auto command = [_buffer = buffer] {
            delete _buffer;
        };
        if(mAsyncRender)
            mCommandQueue[0].push_back(command);
        else
            command();
    }

    void RenderSystem::setIndexBufferData(IndexBuffer* buffer, const void* data, unsigned int size)
    {
        void* safeData = std::malloc(size);
        std::memcpy(safeData, data, size);
        auto command = [_buffer = buffer, _data = safeData, _size = size] {
            _buffer->setBufferData(_data, _size);
            std::free(_data); 
        };
        if(mAsyncRender)
            mCommandQueue[0].push_back(command);
        else
            command();
    }

    Texture* RenderSystem::createTexture(const std::string& name,RenderProtocol::TexType type)
    {
        Texture* texture = new Texture(name, type);
        auto command = [_texture = texture] {
            _texture->createHandle();
        };
        if(mAsyncRender)
            mCommandQueue[0].push_back(command);
        else
            command();
        return texture;
    }

    void RenderSystem::destroyTexture(Texture* texture)
    {
        auto command = [_texture = texture] {
            delete _texture;
        };
        if(mAsyncRender)
            mCommandQueue[0].push_back(command);
        else
            command();
    }

    void RenderSystem::bindTexture(Texture* texture)
    {
        auto command = [_texture = texture] {
            _texture->bind();
        };
        if(mAsyncRender)
            mCommandQueue[0].push_back(command);
        else
            command();
    }

    void RenderSystem::unbindTexture(Texture* texture)
    {
        auto command = [_texture = texture] {
            _texture->unbind();
        };
        if(mAsyncRender)
            mCommandQueue[0].push_back(command);
        else
            command();
    }

    void RenderSystem::setCubeMapTextureData(Texture* texture, RenderProtocol::TexFormat internalFormat, int width, int height, 
            RenderProtocol::TexFormat format, RenderProtocol::TexDataType dataType, std::array<const void*, 6> data, bool generateMipmap)
    {
        std::array<void*, 6> safeData;
        const int byteSize = width * height * RenderProtocol::GetTexFormatByteSize(format);
        for(int i = 0; i < 6; ++i)
        {
            safeData[i] = malloc(byteSize);
            memcpy(safeData[i], data.at(i), byteSize);
        }

        auto command = [_texture = texture, _internalFormat = internalFormat, _width = width, _height = height, 
            _format = format, _dataType = dataType, _data = safeData, _generateMipmap = generateMipmap] {

            std::array<const void*, 6> _data_const = { _data[0], _data[1], _data[2], _data[3], _data[4], _data[5] };
            _texture->setCubeMapTextureData(0, _internalFormat, _width, _height, _format, _dataType, _data_const, _generateMipmap);
            
            for(auto& faceData : _data)
                free(faceData); // Free each face's data after use
        };
        if(mAsyncRender)
            mCommandQueue[0].push_back(command);
        else
            command();
    }

    void RenderSystem::setTextureData(Texture* texture, int level, RenderProtocol::TexFormat internalFormat, int width, int height, 
        RenderProtocol::TexFormat format, RenderProtocol::TexDataType dataType, const void* data, bool generateMipmap)
    {
        const int byteSize = width * height * RenderProtocol::GetTexFormatByteSize(format);
        void* safeData = malloc(byteSize);
        memcpy(safeData, data, byteSize);
        auto command = [_texture = texture, _level = level, _internalFormat = internalFormat, _width = width, 
            _height = height, _format = format, _dataType = dataType, _data = safeData, _generateMipmap = generateMipmap] {
            _texture->setTextureData(_level, _internalFormat, _width, _height, _format, _dataType, _data, _generateMipmap);
            std::free(_data); // Free the data after use
        };
        if(mAsyncRender)
            mCommandQueue[0].push_back(command);
        else
            command();
    }

    void RenderSystem::setTextureWarpMode(Texture* texture, RenderProtocol::TexWrap wrap, RenderProtocol::TexSurround surround)
    {
        auto command = [_texture = texture, _wrap = wrap, _surround = surround] {
            _texture->setTextureWarpMode(_wrap, _surround);
        };
        if(mAsyncRender)
            mCommandQueue[0].push_back(command);
        else
            command();
    }

    void RenderSystem::setTextureFilterMode(Texture* texture, RenderProtocol::TexFilter filter, RenderProtocol::TexFilterType filterType)
    {
        auto command = [_texture = texture, _filter = filter, _filterType = filterType] {
            _texture->setTextureFilterMode(_filter, _filterType);
        };
        if(mAsyncRender)
            mCommandQueue[0].push_back(command);
        else
            command();
    }

    void RenderSystem::activateTextureUnit(unsigned int unit)
    {
        auto command = [_unit = unit] {
            RenderCommand::ActivateTextureUnit(_unit);
        };
        if(mAsyncRender)
            mCommandQueue[0].push_back(command);
        else
            command();
    }

    void RenderSystem::drawArray(RenderProtocol::DrawMode mode, unsigned int first, unsigned int count)
    {
        auto command = [_mode = mode, _first = first, _count = count] {
            RenderCommand::DrawArray(_mode, _first, _count);
        };
        if(mAsyncRender)
            mCommandQueue[0].push_back(command);
        else
            command();
    }

    void RenderSystem::drawElements(RenderProtocol::DrawMode mode, unsigned int count, RenderProtocol::DrawElementType type, const void* indices)
    {
        auto command = [_mode = mode, _count = count, _type = type, _indices = indices] {
            RenderCommand::DrawElements(_mode, _count, _type, _indices);
        };
        if(mAsyncRender)
            mCommandQueue[0].push_back(command);
        else
            command();
    }

    void RenderSystem::submitCommand()
    {
        while (true)
        {
            std::lock_guard<std::mutex> lock(mQueueMutex);
            if(mCommandQueue[1].empty())
            {
                std::swap(mCommandQueue[0], mCommandQueue[1]);
                break;
            }
        }
        mCommandQueue[0].clear();
    }

    bool RenderSystem::executeCommand()
    {
        {
            std::lock_guard<std::mutex> lock(mQueueMutex);
            if(mCommandQueue[1].empty())
                return false;

            std::swap(mCommandQueue[1], mCommandQueue[2]); 
        }

        for(auto &&command : mCommandQueue[2])
            command();

        mCommandQueue[2].clear();
        return true;
    }
}
