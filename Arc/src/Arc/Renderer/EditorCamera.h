#pragma once

#include "Arc/Renderer/Camera.h"

#include "Arc/Core/Timestep.h"
#include "Arc/Events/Event.h"
#include "Arc/Events/MouseEvent.h"

#include <glm/glm.hpp>

namespace ArcEngine
{
	class EditorCamera : public Camera
	{
		enum class CameraMovement
	    {
	        FORWARD,
	        BACKWARD,
	        LEFT,
	        RIGHT
	    };
	
	public:
		enum class ProjectionType { Perspective = 0, Orthographic = 1 };
		
		EditorCamera() = default;
		EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		inline void SetViewportSize(float width, float height) { m_ViewportWidth = width; m_ViewportHeight = height; UpdateProjection(); }

		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		glm::mat4 GetViewProjection() const { return m_Projection * m_ViewMatrix; }

		const glm::vec3& GetPosition() const { return m_Position; }

		float GetPitch() const { return m_Pitch; }
		float GetYaw() const { return m_Yaw; }
	private:
		void UpdateProjection();
		void UpdateView();
		void ProcessKeyboard(CameraMovement direction, float ts);
		bool OnMouseScrolled(MouseScrolledEvent& e);
		void UpdateCameraVectors();
	
	private:
		float m_FOV = 60.0f, m_AspectRatio = 1.778f, m_NearClip = 0.1f, m_FarClip = 1000.0f;

		ProjectionType m_ProjectionType = ProjectionType::Perspective;

		glm::mat4 m_ViewMatrix;
		glm::vec3 m_Position = { 0.0f, 0.0f, 10.0f };

		float m_NormalSpeed = 10.0f;
	    float m_MovementSpeed = m_NormalSpeed;
		float m_MouseSensitivity = 0.1f;
		
		glm::vec2 m_InitialMousePosition = { 0.0f, 0.0f };

		float m_Pitch = 0.0f, m_Yaw = -90.0f;

		glm::vec3 m_Right = glm::vec3(1, 0, 0);
		glm::vec3 m_Up = glm::vec3(0, 1, 0);
		glm::vec3 m_Forward = glm::vec3(0, 0, -1);
		glm::vec3 m_WorldUp = glm::vec3(0, 1, 0);

		float m_ViewportWidth = 1280, m_ViewportHeight = 720;
	};

}

