#pragma once

#include "Arc/Renderer/RenderCommand.h"

namespace ArcEngine
{
	class EditorCamera;
	class Camera;
	class Shader;
	class Texture2D;
	class TextureCube;

	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();
		
		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(EditorCamera& camera);
		static void BeginScene(Camera& camera, glm::mat4 transform);
		static void EndScene();

		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));
		static void DrawCube(const Ref<Shader>& shader, const glm::mat4& transform = glm::mat4(1.0f));
		static void DrawSkybox(Ref<TextureCube>& textureCube, EditorCamera& camera);
		static void DrawSkybox(Ref<TextureCube>& textureCube, Camera& camera, glm::mat4& transform);
		
		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;

			Ref<VertexArray> CubeVertexArray;
			
			Ref<Texture2D> whiteTexture;
			Ref<Shader> SkyboxShader;
		};

		static SceneData* s_SceneData;
	};
}
