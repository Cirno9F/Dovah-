#include "dovahpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Dovah
{
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}