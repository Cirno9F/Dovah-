#include "dovahpch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Dovah
{
	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			DOVAH_CORE_ASSERT(false, "RenderAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLTexture2D>(path);
		}

		DOVAH_CORE_ASSERT(false, "Unknown RenderAPI");
		return nullptr;
	}
}