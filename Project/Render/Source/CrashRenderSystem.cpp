#include "CrashPCH.h"
#include "CrashRenderSystem.h"
#include "CrashRenderCommand.h"
#include "CrashShaderProgram.h"
#include "CrashRenderBuffer.h"
#include "CrashTexture.h"

namespace Crash
{
    namespace
    {
        enum class _CommandType : unsigned int
        {
            FrameBegin = 0,
            FrameEnd,
            Clear,
            SetClearColor,
            SetViewport, 
            SetCullFaceEnable,

            SetDepthEnable,
            SetClearDepth,
            SetDetphFunc,

            SetStencilEnable,
            SetStencilFunc,
            SetStencilOp,
            SetStencilMask,

            CreateShaderProgram,
            DestroyShaderProgram,
            BindShaderProgram,
            UnbindShaderProgram,
            SetUniform1i,
            SetUniform4f,
            SetUniformMatrix4fv,

            CreateVertexArray,
            DestroyVertexArray,
            BindVertexArray,
            UnbindVertexArray,
            AddVBToVertexArray,
            AddIBToVertexArray,

            CreateVertexBuffer,
            DestroyVertexBuffer,
            SetVertexBufferData,

            CreateIndexBuffer,
            DestroyIndexBuffer,
            SetIndexBufferData,

            CreateTexture,
            DestroyTexture,
            BindTexture,
            UnBindTexture,
            SetTextureData,
            SetTextureWarpMode,
            SetTextureFilterMode,
            ActivateTextureUnit,

            DrawArray,
            DrawElements,
        };

        template<typename T>
        struct SetUniformInfo
        {
            const ShaderProgram*    program;
            std::string             name;
            T                       value;
        };

        struct AddVBToVAOInfo
        {
            VertexArrayObject*  vao;
            VertexBuffer*       buffer;
            unsigned int        index;
            unsigned int        size;
            unsigned int        stride;
            const void*         pointer;
        };

        struct AddIBToVAOInfo
        {
            VertexArrayObject* vao;
            IndexBuffer*       buffer;
        };

        template<typename T>
        struct SetBufferDataInfo
        {
            T*                  buffer;
            void*               data;
            unsigned int        size;   
        };

        struct DrawArrayInfo
        {
            RenderProtocol::DrawMode        mode;
            unsigned int                    first;
            unsigned int                    count;
        };

        struct DrawElementsInfo
        {
            RenderProtocol::DrawMode        mode;
            unsigned int                    count;
            RenderProtocol::DrawElementType type;
            const void*                     indices;
        };

        struct SetTexDataInfo
        {
            Texture*                        texture;
            int                             level;
            RenderProtocol::TexFormat       internalFormat;
            int                             width;
            int                             height;
            RenderProtocol::TexFormat       format;
            RenderProtocol::TexDataType     dataType;
            void*                           data;
            bool                            generateMipmap;
        };

        struct SetTexWarpModeInfo
        {
            Texture*                        texture;
            RenderProtocol::TexWrap         wrap;
            RenderProtocol::TexSurround     surround;
        };

        struct SetTexFilterModeInfo
        {
            Texture*                        texture;
            RenderProtocol::TexFilter       filter;
            RenderProtocol::TexFilterType   filterType;
        };
    }

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

    void RenderSystem::frameBegin()
    {
        if(mAsyncRender)
            mCommandQueue[0].push_back({ (unsigned int)_CommandType::FrameBegin, nullptr });
    }

    void RenderSystem::frameEnd()
    {
        if(mAsyncRender)
            mCommandQueue[0].push_back({ (unsigned int)_CommandType::FrameEnd, nullptr });
    }

    void RenderSystem::clear(RenderProtocol::ClearFlag flag)
    {
        if(mAsyncRender)
            mCommandQueue[0].push_back({ (unsigned int)_CommandType::Clear, (void*)(new RenderProtocol::ClearFlag(flag)) });
        else
            RenderCommand::Clear(flag);
    }

    void RenderSystem::setDepthEnable(bool enable)
    {
        if(mAsyncRender)
            mCommandQueue[0].push_back({ (unsigned int)_CommandType::SetDepthEnable, (void*)(new bool(enable)) });
        else
            RenderCommand::SetDepthEnable(enable);
    }

    void RenderSystem::setDepthFunc(RenderProtocol::CompareFunc func)
    {
        if(mAsyncRender)
            mCommandQueue[0].push_back({ (unsigned int)_CommandType::SetDetphFunc, (void*)(new RenderProtocol::CompareFunc(func)) });
        else
            RenderCommand::SetDetphFunc(func);
    }

    void RenderSystem::setClearDepth(float depth)
    {
        if(mAsyncRender)
            mCommandQueue[0].push_back({ (unsigned int)_CommandType::SetClearDepth, (void*)(new float(depth)) });
        else
            RenderCommand::SetClearDepth(depth);
    }

    void RenderSystem::setStencilEnable(bool enable)
    {
        if(mAsyncRender)
            mCommandQueue[0].push_back({ (unsigned int)_CommandType::SetStencilEnable, (void*)(new bool(enable)) });
        else
            RenderCommand::SetStencilEnable(enable);
    }

    void RenderSystem::setStencilFunc(RenderProtocol::CompareFunc func, int ref, unsigned int mask)
    {
        if(mAsyncRender)
        {
            auto* info = new std::tuple<RenderProtocol::CompareFunc, int, unsigned int>(func, ref, mask);
            mCommandQueue[0].push_back({ (unsigned int)_CommandType::SetStencilFunc, info });
        }
        else
            RenderCommand::SetStencilFunc(func, ref, mask);
    }

    void RenderSystem::setStencilOp(RenderProtocol::OperateFunc sfail, RenderProtocol::OperateFunc dpfail, RenderProtocol::OperateFunc dppass)
    {
        if(mAsyncRender)
        {
            auto* info = new std::tuple<RenderProtocol::OperateFunc, RenderProtocol::OperateFunc, RenderProtocol::OperateFunc>(sfail, dpfail, dppass);
            mCommandQueue[0].push_back({ (unsigned int)_CommandType::SetStencilOp, info });
        }
        else
            RenderCommand::SetStencilOp(sfail, dpfail, dppass);
    }

    void RenderSystem::setStencilMask(unsigned int mask)
    {
        if(mAsyncRender)
            mCommandQueue[0].push_back({ (unsigned int)_CommandType::SetStencilMask, (void*)(new unsigned int(mask)) });
        else
            RenderCommand::SetStencilMask(mask);
    }

    void RenderSystem::setClearColor(const glm::vec4& color)
    {
        if(mAsyncRender)
            mCommandQueue[0].push_back({ (unsigned int)_CommandType::SetClearColor, new glm::vec4(color) });
        else
            RenderCommand::SetClearColor(color);
    }

    void RenderSystem::setCullFaceEnable(bool enable)
    {
        if(mAsyncRender)
            mCommandQueue[0].push_back({ (unsigned int)_CommandType::SetCullFaceEnable, (void*)(new bool(enable)) });
        else
            RenderCommand::SetCullFaceEnable(enable);
    }

    void RenderSystem::setViewport(const glm::ivec4& viewport)
    {
        if(mAsyncRender)
            mCommandQueue[0].push_back({ (unsigned int)_CommandType::SetViewport, new glm::ivec4(viewport) });
        else
            RenderCommand::SetViewport(viewport);
    }

    ShaderProgram* RenderSystem::createShaderProgram(const std::string& name, const std::string& vs, const std::string& fs)
    {
        ShaderProgram* program = new ShaderProgram(name, vs, fs);
        if(mAsyncRender)
            mCommandQueue[0].push_back({ (unsigned int)_CommandType::CreateShaderProgram, program });
        else
            program->createHandle();
        return program;
    }

    void RenderSystem::destroyShaderProgram(ShaderProgram* program)
    {
        if(mAsyncRender)
            mCommandQueue[0].push_back({ (unsigned int)_CommandType::Clear, (void*)program });
        else
            delete program;
    }

    void RenderSystem::bindShaderProgram(ShaderProgram* program)
    {
        if(mAsyncRender)
            mCommandQueue[0].push_back({ (unsigned int)_CommandType::BindShaderProgram, program });
        else
            program->bind();
    }

    void RenderSystem::unbindShaderProgram()
    {
        if(mAsyncRender)
            mCommandQueue[0].push_back({ (unsigned int)_CommandType::UnbindShaderProgram, nullptr });
        else
            RenderCommand::UnbindShaderProgram();
    }

    void RenderSystem::setUniform1i(const ShaderProgram* program, const std::string& name, int value)
    {
        if(mAsyncRender)
        {
            SetUniformInfo<int>* info = new SetUniformInfo<int>();
            info->program = program;
            info->name    = name;
            info->value   = value;
            mCommandQueue[0].push_back({ (unsigned int)_CommandType::SetUniform1i, info });
        }
        else
        {
           program->setUniform1i(name, value);
        }
    }

    void RenderSystem::setUniform4f(const ShaderProgram* program, const std::string& name, const glm::vec4& value)
    {
        if(mAsyncRender)
        {
            SetUniformInfo<glm::vec4>* info = new SetUniformInfo<glm::vec4>();
            info->program = program;
            info->name    = name;
            info->value   = value;
            mCommandQueue[0].push_back({ (unsigned int)_CommandType::SetUniform4f, info });
        }
        else
        {
           program->setUniform4f(name, value);
        }
    }

    void RenderSystem::setUniformMatrix4fv(const ShaderProgram* program, const std::string& name, const glm::mat4& value)
    {
        if(mAsyncRender)
        {
            SetUniformInfo<glm::mat4>* info = new SetUniformInfo<glm::mat4>();
            info->program = program;
            info->name    = name;
            info->value   = value;
            mCommandQueue[0].push_back({ (unsigned int)_CommandType::SetUniformMatrix4fv, info });
        }
        else
        {
           program->setUniformMatrix4fv(name, value);
        }
    }

    VertexArrayObject* RenderSystem::createVertexArray()
    {
        VertexArrayObject* vao = new VertexArrayObject();
        if(mAsyncRender)
            mCommandQueue[0].push_back({ (unsigned int)_CommandType::CreateVertexArray, vao });
        else
            vao->createHandle();
        return vao;
    }

    void RenderSystem::destroyVertexArray(VertexArrayObject* vao)
    {
        if(mAsyncRender)
            mCommandQueue[0].push_back({ (unsigned int)_CommandType::DestroyVertexArray, vao });
        else
            delete vao;
    }

    void RenderSystem::bindVertexArray(VertexArrayObject* vao)
    {
        if(mAsyncRender)
            mCommandQueue[0].push_back({ (unsigned int)_CommandType::BindVertexArray, vao });
        else
            vao->bind();
    }

    void RenderSystem::unbindVertexArray()
    {
        if(mAsyncRender)
            mCommandQueue[0].push_back({ (unsigned int)_CommandType::UnbindVertexArray, nullptr });
        else
            RenderCommand::UnbindVertexArray();
    }

    void RenderSystem::addBufferToVertexArray(VertexArrayObject* vao, VertexBuffer* buffer, unsigned int index, unsigned int size, unsigned int stride, const void* pointer)
    {
        if(mAsyncRender)
        {
            AddVBToVAOInfo* info = new AddVBToVAOInfo();
            info->vao       = vao;
            info->buffer    = buffer;
            info->index     = index;
            info->size      = size;
            info->stride    = stride;
            info->pointer   = pointer;
            mCommandQueue[0].push_back({ (unsigned int)_CommandType::AddVBToVertexArray, info });
        }
        else
        {
            vao->addBuffer(buffer, index, size, stride, pointer);
        }
    }

    void RenderSystem::addBufferToVertexArray(VertexArrayObject* vao, IndexBuffer* buffer)
    {
        if(mAsyncRender)
        {
            AddIBToVAOInfo* info = new AddIBToVAOInfo();
            info->vao       = vao;
            info->buffer    = buffer;
            mCommandQueue[0].push_back({ (unsigned int)_CommandType::AddIBToVertexArray, info });
        }
        else
        {
            vao->addBuffer(buffer);
        }
    }

    VertexBuffer* RenderSystem::createBuffer()
    {
        VertexBuffer* buffer = new VertexBuffer(RenderProtocol::BufferUsage::StaticDraw);
        if(mAsyncRender)
            mCommandQueue[0].push_back({ (unsigned int)_CommandType::CreateVertexBuffer, buffer });
        else
            buffer->createHandle();
        return buffer;
    }

    void RenderSystem::destroyBuffer(VertexBuffer* buffer)
    {
        if(mAsyncRender)
            mCommandQueue[0].push_back({ (unsigned int)_CommandType::DestroyVertexBuffer, buffer });
        else
            delete buffer;
    }

    void RenderSystem::setBufferData(VertexBuffer* buffer, const void* data, unsigned int size)
    {
        if(mAsyncRender)
        {
            void* safeData = std::malloc(size);
            std::memcpy(safeData, data, size);

            SetBufferDataInfo<VertexBuffer>* info = new SetBufferDataInfo<VertexBuffer>();
            info->buffer = buffer;
            info->data   = safeData;
            info->size   = size;
            mCommandQueue[0].push_back({ (unsigned int)_CommandType::SetVertexBufferData, info });
        }
        else
        {
            buffer->setBufferData(data, size);
        }
    }

    IndexBuffer* RenderSystem::createIndexBuffer()
    {
        IndexBuffer* buffer = new IndexBuffer(RenderProtocol::BufferUsage::StaticDraw);
        if(mAsyncRender)
            mCommandQueue[0].push_back({ (unsigned int)_CommandType::CreateIndexBuffer, buffer });
        else
            buffer->createHandle();
        return buffer;
    }

    void RenderSystem::destroyIndexBuffer(IndexBuffer* buffer)
    {
        if(mAsyncRender)
            mCommandQueue[0].push_back({ (unsigned int)_CommandType::DestroyIndexBuffer, buffer });
        else
            delete buffer;
    }

    void RenderSystem::setIndexBufferData(IndexBuffer* buffer, const void* data, unsigned int size)
    {
        if(mAsyncRender)
        {
            void* safeData = std::malloc(size);
            std::memcpy(safeData, data, size);

            SetBufferDataInfo<IndexBuffer>* info = new SetBufferDataInfo<IndexBuffer>();
            info->buffer = buffer;
            info->data   = safeData;
            info->size   = size;
            mCommandQueue[0].push_back({ (unsigned int)_CommandType::SetIndexBufferData, info });
        }
        else
        {
            buffer->setBufferData(data, size);
        }
    }

    Texture* RenderSystem::createTexture(const std::string& name,RenderProtocol::TexType type)
    {
        Texture* texture = new Texture(name, type);
        if(mAsyncRender)
            mCommandQueue[0].push_back({ (unsigned int)_CommandType::CreateTexture, texture });
        else
            texture->createHandle();
        return texture;
    }

    void RenderSystem::destroyTexture(Texture* texture)
    {
        if(mAsyncRender)
            mCommandQueue[0].push_back({ (unsigned int)_CommandType::DestroyTexture, texture });
        else
            delete texture;
    }

    void RenderSystem::bindTexture(Texture* texture)
    {
        if(mAsyncRender)
            mCommandQueue[0].push_back({ (unsigned int)_CommandType::BindTexture, texture });
        else
            texture->bind();
    }

    void RenderSystem::unbindTexture(Texture* texture)
    {
        if(mAsyncRender)
            mCommandQueue[0].push_back({ (unsigned int)_CommandType::UnBindTexture, texture });
        else
            texture->unbind();
    }

    void RenderSystem::setTextureData(Texture* texture, int level, RenderProtocol::TexFormat internalFormat, int width, int height, 
        RenderProtocol::TexFormat format, RenderProtocol::TexDataType dataType, const void* data, bool generateMipmap)
    {
        if(mAsyncRender)
        {
            SetTexDataInfo* info = new SetTexDataInfo();
            info->texture         = texture;
            info->level           = level;
            info->internalFormat  = internalFormat;
            info->width           = width;
            info->height          = height;
            info->format          = format;

            const int byteSize    = width * height * RenderProtocol::GetTexFormatByteSize(format);
            info->data            = malloc(byteSize);
            memcpy(info->data, data, byteSize);

            info->dataType        = dataType;
            info->generateMipmap = generateMipmap;
            
            mCommandQueue[0].push_back({ (unsigned int)_CommandType::SetTextureData, info });
        }
        else
            texture->setTextureData(level, internalFormat, width, height, format, dataType, data, generateMipmap);
    }

    void RenderSystem::setTextureWarpMode(Texture* texture, RenderProtocol::TexWrap wrap, RenderProtocol::TexSurround surround)
    {
        if(mAsyncRender)
        {
            SetTexWarpModeInfo* info = new SetTexWarpModeInfo();
            info->texture   = texture;
            info->wrap      = wrap;
            info->surround  = surround;
            mCommandQueue[0].push_back({ (unsigned int)_CommandType::SetTextureWarpMode, info });
        }
        else
        {
            texture->setTextureWarpMode(wrap, surround);
        }
    }

    void RenderSystem::setTextureFilterMode(Texture* texture, RenderProtocol::TexFilter filter, RenderProtocol::TexFilterType filterType)
    {
        if(mAsyncRender)
        {
            SetTexFilterModeInfo* info = new SetTexFilterModeInfo();
            info->texture   = texture;
            info->filter    = filter;
            info->filterType= filterType;
            mCommandQueue[0].push_back({ (unsigned int)_CommandType::SetTextureFilterMode, info });
        }
        else
        {
            texture->setTextureFilterMode(filter, filterType);
        }
    }

    void RenderSystem::activateTextureUnit(unsigned int unit)
    {
        if(mAsyncRender)
            mCommandQueue[0].push_back({ (unsigned int)_CommandType::ActivateTextureUnit, (void*)(new unsigned int(unit)) });
        else
            RenderCommand::ActivateTextureUnit(unit);
    }

    void RenderSystem::drawArray(RenderProtocol::DrawMode mode, unsigned int first, unsigned int count)
    {
        if(mAsyncRender)
        {
            DrawArrayInfo* info = new DrawArrayInfo();
            info->mode   = mode;
            info->first  = first;
            info->count  = count;
            mCommandQueue[0].push_back({ (unsigned int)_CommandType::DrawArray, info });
        }
        else
        {
            RenderCommand::DrawArray(mode, first, count);
        }
    }

    void RenderSystem::drawElements(RenderProtocol::DrawMode mode, unsigned int count, RenderProtocol::DrawElementType type, const void* indices)
    {
        if(mAsyncRender)
        {
            DrawElementsInfo* info = new DrawElementsInfo();
            info->mode   = mode;
            info->count  = count;
            info->type   = type;
            info->indices = indices;
            mCommandQueue[0].push_back({ (unsigned int)_CommandType::DrawElements, info });
        }
        else
        {
            RenderCommand::DrawElements(mode, count, type, indices);
        }
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

        for(auto &&it : mCommandQueue[2])
        {
            _CommandType    cmdType = (_CommandType)it.first;
            void*           cmdData = it.second;

            switch (cmdType)
            {
                case _CommandType::CreateTexture:
                {
                    Texture* texture = (Texture*)cmdData;
                    texture->createHandle();
                }
                break;
                case _CommandType::DestroyTexture:
                {
                    Texture* texture = (Texture*)cmdData;
                    delete texture;
                }
                break;
                case _CommandType::BindTexture:
                {
                    Texture* texture = (Texture*)cmdData;
                    texture->bind();
                }
                break;
                case _CommandType::UnBindTexture:
                {
                    Texture* texture = (Texture*)cmdData;
                    texture->unbind();
                }
                break;
                case _CommandType::SetTextureData:
                {
                    SetTexDataInfo* info = (SetTexDataInfo*)cmdData;
                    info->texture->setTextureData(info->level, info->internalFormat, info->width, info->height, 
                        info->format, info->dataType, info->data, info->generateMipmap);
                    std::free(info->data);
                    delete info;
                }
                break;
                case _CommandType::SetTextureWarpMode:
                {
                    SetTexWarpModeInfo* info = (SetTexWarpModeInfo*)cmdData;
                    info->texture->setTextureWarpMode(info->wrap, info->surround);
                    delete info;
                }
                break;
                case _CommandType::SetTextureFilterMode:
                {
                    SetTexFilterModeInfo* info = (SetTexFilterModeInfo*)cmdData;
                    info->texture->setTextureFilterMode(info->filter, info->filterType);
                    delete info;
                }
                break;
                case _CommandType::ActivateTextureUnit:
                {
                    unsigned int* unit = (unsigned int*)cmdData;
                    RenderCommand::ActivateTextureUnit(*unit);
                    delete unit;
                }
                break;
                case _CommandType::DrawArray:
                {
                    DrawArrayInfo* info = (DrawArrayInfo*)cmdData;
                    RenderCommand::DrawArray(info->mode, info->first, info->count);
                    delete info;
                }
                break;
                case _CommandType::DrawElements:
                {
                    DrawElementsInfo* info = (DrawElementsInfo*)cmdData;
                    RenderCommand::DrawElements(info->mode, info->count, info->type, info->indices);
                    std::free((void*)info->indices);
                    delete info;
                }
                break;
                case _CommandType::CreateVertexArray:
                {
                    VertexArrayObject* vao = (VertexArrayObject*)cmdData;
                    vao->createHandle();
                }
                break;
                case _CommandType::DestroyVertexArray:
                {
                    VertexArrayObject* vao = (VertexArrayObject*)cmdData;
                    delete vao;
                }   
                break;
                case _CommandType::BindVertexArray:
                {
                    VertexArrayObject* vao = (VertexArrayObject*)cmdData;
                    vao->bind();
                }
                break;
                case _CommandType::UnbindVertexArray:
                {
                    RenderCommand::UnbindVertexArray();
                }
                break;
                case _CommandType::AddVBToVertexArray:
                {
                    AddVBToVAOInfo* info = (AddVBToVAOInfo*)cmdData;
                    info->vao->addBuffer(info->buffer, info->index, info->size, info->stride, info->pointer);
                    delete info;
                }
                break;
                case _CommandType::AddIBToVertexArray:
                {
                    AddIBToVAOInfo* info = (AddIBToVAOInfo*)cmdData;
                    info->vao->addBuffer(info->buffer);
                    delete info;
                }
                break;

                case _CommandType::CreateVertexBuffer:
                {
                    VertexBuffer* buffer = (VertexBuffer*)cmdData;
                    buffer->createHandle();
                }
                break;
                case _CommandType::DestroyVertexBuffer:
                {
                    VertexBuffer* buffer = (VertexBuffer*)cmdData;
                    delete buffer;
                }
                break;
                case _CommandType::SetVertexBufferData:
                {
                    SetBufferDataInfo<VertexBuffer>* info = (SetBufferDataInfo<VertexBuffer>*)cmdData;
                    info->buffer->setBufferData(info->data, info->size);
                    std::free(info->data);
                    delete info;
                }
                break;
                case _CommandType::CreateIndexBuffer:
                {
                    IndexBuffer* buffer = (IndexBuffer*)cmdData;
                    buffer->createHandle();
                }
                break;
                case _CommandType::DestroyIndexBuffer:
                {
                    IndexBuffer* buffer = (IndexBuffer*)cmdData;
                    delete buffer;
                }
                break;
                case _CommandType::SetIndexBufferData:
                {
                    SetBufferDataInfo<IndexBuffer>* info = (SetBufferDataInfo<IndexBuffer>*)cmdData;
                    info->buffer->setBufferData(info->data, info->size);
                    std::free(info->data);
                    delete info;
                }
                break;
                case _CommandType::CreateShaderProgram:
                {
                    ShaderProgram* program = (ShaderProgram*)cmdData;
                    program->createHandle();
                }
                break;
                case _CommandType::DestroyShaderProgram:
                {
                    ShaderProgram* program = (ShaderProgram*)cmdData;
                    delete program;
                }
                break;
                case _CommandType::BindShaderProgram:
                {
                    ShaderProgram* program = (ShaderProgram*)cmdData;
                    program->bind();
                }
                break;
                case _CommandType::UnbindShaderProgram:
                {
                    RenderCommand::UnbindShaderProgram();
                }
                break;
                case _CommandType::SetUniform1i:
                {
                    SetUniformInfo<int>* info = (SetUniformInfo<int>*)cmdData;
                    info->program->setUniform1i(info->name, info->value);
                    delete info;
                }
                break;
                case _CommandType::SetUniform4f:
                {
                    SetUniformInfo<glm::vec4>* info = (SetUniformInfo<glm::vec4>*)cmdData;
                    info->program->setUniform4f(info->name, info->value);
                    delete info;
                }
                break;
                case _CommandType::SetUniformMatrix4fv:
                {
                    SetUniformInfo<glm::mat4>* info = (SetUniformInfo<glm::mat4>*)cmdData;
                    info->program->setUniformMatrix4fv(info->name, info->value);
                    delete info;
                }
                break;
                case _CommandType::FrameBegin:
                {

                }
                break;
                case _CommandType::FrameEnd:
                {

                } 
                break;
                case _CommandType::Clear:
                {
                    RenderCommand::Clear(*(RenderProtocol::ClearFlag*)cmdData);
                    delete (RenderProtocol::ClearFlag*)cmdData;
                }
                break;
                case _CommandType::SetClearColor:
                {
                    glm::vec4* color = (glm::vec4*)cmdData;
                    RenderCommand::SetClearColor(*color);
                    delete color;
                }
                break;
                case _CommandType::SetViewport:
                {
                    glm::ivec4* viewport = (glm::ivec4*)cmdData;
                    RenderCommand::SetViewport(*viewport);
                    delete viewport;
                }
                break;
                case _CommandType::SetDepthEnable:
                {
                    bool* enable = (bool*)cmdData;
                    RenderCommand::SetDepthEnable(*enable);
                    delete enable;
                }
                break;
                case _CommandType::SetDetphFunc:
                {
                    RenderProtocol::CompareFunc* func = (RenderProtocol::CompareFunc*)cmdData;
                    RenderCommand::SetDetphFunc(*func);
                    delete func;
                }
                break;
                case _CommandType::SetClearDepth:
                {
                    float* depth = (float*)cmdData;
                    RenderCommand::SetClearDepth(*depth);
                    delete depth;
                }
                break;
                case _CommandType::SetStencilEnable:
                {
                    bool* enable = (bool*)cmdData;
                    RenderCommand::SetStencilEnable(*enable);
                    delete enable;
                }
                break;
                case _CommandType::SetStencilFunc:
                {
                    auto* info = (std::tuple<RenderProtocol::CompareFunc, int, unsigned int>*)cmdData;
                    RenderCommand::SetStencilFunc(std::get<0>(*info), std::get<1>(*info), std::get<2>(*info));
                    delete info;
                }
                break;
                case _CommandType::SetStencilOp:
                {
                    auto* info = (std::tuple<RenderProtocol::OperateFunc, RenderProtocol::OperateFunc, RenderProtocol::OperateFunc>*)cmdData;
                    RenderCommand::SetStencilOp(std::get<0>(*info), std::get<1>(*info), std::get<2>(*info));
                    delete info;
                }
                break;
                case _CommandType::SetStencilMask:
                {
                    unsigned int* mask = (unsigned int*)cmdData;
                    RenderCommand::SetStencilMask(*mask);
                    delete mask;
                }
                break;
                case _CommandType::SetCullFaceEnable:
                {
                    bool* enable = (bool*)cmdData;
                    RenderCommand::SetCullFaceEnable(*enable);
                    delete enable;
                }
                break;
                default:
                {
                    LogManager::Instance()->log("RenderSystem::executeCommand unknown command");
                    assert(false && "RenderSystem::executeCommand unknown command");
                }
            }
        }

         mCommandQueue[2].clear();
        return true;
    }
}
