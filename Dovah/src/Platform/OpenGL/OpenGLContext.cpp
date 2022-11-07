#include "dovahpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Dovah
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		:m_WindowHandle(windowHandle)
	{
		DOVAH_CORE_ASSERT(windowHandle, "Window Handle is null!");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		DOVAH_CORE_ASSERT(status, "Failed to initialize Glad!");

		DOVAH_CORE_INFO("OpenGL Info:");
		DOVAH_CORE_INFO("	Vendor: {0}", glGetString(GL_VENDOR));
		DOVAH_CORE_INFO("	Renderer: {0}", glGetString(GL_RENDERER));
		DOVAH_CORE_INFO("	Version: {0}", glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}