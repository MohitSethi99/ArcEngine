#pragma once
#include "Arc/Renderer/RendererAPI.h"

namespace ArcEngine
{
	class RenderCommand
	{
	public:
		inline static void Init()
		{
			s_RendererAPI->Init();
		}
		
		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_RendererAPI->SetViewport(x, y, width, height);
		}
		
		inline static void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI->SetClearColor(color);
		}
		
		inline static void Clear()
		{
			s_RendererAPI->Clear();
		}
		
		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0)
		{
			s_RendererAPI->DrawIndexed(vertexArray, indexCount);
		}

		inline static void DrawArray(uint32_t first, uint32_t count)
		{
			s_RendererAPI->DrawArray(first, count);
		}
		
		inline static void SetDepthMask(bool flag)
		{
			s_RendererAPI->SetDepthMask(flag);
		}

		inline static void SetDepthTest(bool flag)
		{
			s_RendererAPI->SetDepthTest(flag);
		}
	private:
		static Scope<RendererAPI> s_RendererAPI;
	};
}
