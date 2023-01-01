#include "dovahpch.h"
#include "Renderer2D.h"

#include "glm/gtc/matrix_transform.hpp"

#include "RenderCommand.h"
#include "VertexArray.h"
#include "Shader.h"

namespace Dovah
{
	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> FlatColorShader;
 	};
	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		s_Data = new Renderer2DStorage();

		//square
		s_Data->QuadVertexArray = VertexArray::Create();

		float squareVertices[4 * 7] = {
			-0.5f,-0.5f,0.0f, 0.2f,0.3f,0.8f,1.0f,
			 0.5f,-0.5f,0.0f, 0.2f,0.3f,0.8f,1.0f,
			 0.5f, 0.5f,0.0f, 0.2f,0.3f,0.8f,1.0f,
			-0.5f, 0.5f,0.0f, 0.2f,0.3f,0.8f,1.0f,
		};

		Ref<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		BufferLayout squareLayout = {
			{ShaderDataType::Float3, "a_Position" },
			{ShaderDataType::Float4, "a_Color" },
		};
		squareVB->SetLayout(squareLayout);
		s_Data->QuadVertexArray->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 0, 2, 3 };
		Ref<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		s_Data->QuadVertexArray->SetIndexBuffer(squareIB);

		s_Data->FlatColorShader = Shader::Create("assets/shaders/FlatColor.glsl");	//square
	}

	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_Data->FlatColorShader->Bind();
		s_Data->FlatColorShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad(glm::vec3{ position.x,position.y,0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		s_Data->FlatColorShader->Bind();
		s_Data->FlatColorShader->SetFloat4("u_Color", color);

		s_Data->FlatColorShader->SetMat4("u_Transform",
			glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f)));
		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

}
