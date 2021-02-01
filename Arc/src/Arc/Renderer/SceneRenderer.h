#pragma once

#include "Arc/Core/Base.h"

#include "Arc/Renderer/Shader.h"
#include "Arc/Renderer/Buffer.h"

#include <glm/glm.hpp>

namespace ArcEngine
{
	class Camera;
	class TextureCube;
	class Entity;
	class EditorCamera;
	class MaterialInstance;
	class Mesh;
	class Scene;

	class SceneRenderer
	{
	public:
		static void Init();
		static void Shutdown();
		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(const EditorCamera& camera, std::vector<Entity>& lights);
		static void BeginScene(const Camera& camera, const glm::mat4& cameraTransform, glm::vec3& cameraPosition, std::vector<Entity>& lights);
		static void EndScene();

		static void SubmitMesh(const Ref<Mesh>& mesh, const glm::mat4& transform, Ref<MaterialInstance> overrideMaterial = nullptr);

	private:
		static void SetupLights(std::vector<Entity>& lights);
	
	private:
		static Ref<UniformBuffer> m_UBOCamera;
		static Ref<UniformBuffer> m_UBOLights;
		static Ref<Shader> m_Shader;
	};
}

