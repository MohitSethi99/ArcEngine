#include "arcpch.h"
#include "EditorCamera.h"

#include "Arc/Core/Input.h"
#include "Arc/Core/KeyCodes.h"
#include "Arc/Core/MouseCodes.h"

#include <glfw/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace ArcEngine
{
	EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip)
		: m_FOV(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip), Camera(glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip))
	{
		UpdateView();
	}

	void EditorCamera::UpdateProjection()
	{
		m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
		m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
	}

	void EditorCamera::UpdateView()
	{
		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Forward, m_Up);
	}
	
	void EditorCamera::OnUpdate(Timestep ts)
	{
		// WASD Movement
		m_MovementSpeed = (Input::IsKeyPressed(Key::LeftShift)) ? (m_NormalSpeed * 2.0f) : m_NormalSpeed;

	    if (Input::IsMouseButtonPressed(Mouse::ButtonRight))
	    {
	        if (m_ProjectionType == ProjectionType::Perspective)
	        {
	            if (Input::IsKeyPressed(Key::W))
	                ProcessKeyboard(CameraMovement::FORWARD, ts);
	            if (Input::IsKeyPressed(Key::A))
	                ProcessKeyboard(CameraMovement::LEFT, ts);
	            if (Input::IsKeyPressed(Key::S))
	                ProcessKeyboard(CameraMovement::BACKWARD, ts);
	            if (Input::IsKeyPressed(Key::D))
	                ProcessKeyboard(CameraMovement::RIGHT, ts);
	        }
	    }


		// Look Around and Pan
		static bool constrainPitch = true;
		static bool firstMouse = true;
		static float lastX, lastY;

		if (firstMouse)
		{
			lastX = Input::GetMouseX();
			lastY = Input::GetMouseY();
			firstMouse = false;
		}

		float xOffset = Input::GetMouseX() - lastX;
		float yOffset = Input::GetMouseY() - lastY;

		lastX = Input::GetMouseX();
		lastY = Input::GetMouseY();

		if (Input::IsMouseButtonPressed(Mouse::ButtonMiddle) || m_ProjectionType == ProjectionType::Orthographic && Input::IsMouseButtonPressed(Mouse::ButtonRight))
		{
			xOffset *= 0.01f;
			yOffset *= 0.01f;

			m_Position += -(m_Right * xOffset) + m_Up * yOffset;
		}
		else if (Input::IsMouseButtonPressed(Mouse::ButtonRight))
		{
			xOffset *= m_MouseSensitivity;
			yOffset *= m_MouseSensitivity;

			m_Yaw += xOffset;
			m_Pitch -= yOffset;

			// Make sure that when pitch is out of bounds, screen doesn't get flipped
			if (constrainPitch)
			{
				if (m_Pitch > 89.0f)
					m_Pitch = 89.0f;
				if (m_Pitch < -89.0f)
					m_Pitch = -89.0f;
			}

			UpdateCameraVectors();
			UpdateView();
		}

		
	    UpdateCameraVectors();
		UpdateView();
	}

	void EditorCamera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(ARC_BIND_EVENT_FN(EditorCamera::OnMouseScrolled));
	}

	void EditorCamera::ProcessKeyboard(CameraMovement direction, float ts)
	{
		const float velocity = m_MovementSpeed * ts;
		
	    if (direction == CameraMovement::FORWARD)
	        m_Position += m_Forward * velocity;
	    else if (direction == CameraMovement::BACKWARD)
	        m_Position -= m_Forward * velocity;

	    if (direction == CameraMovement::RIGHT)
	        m_Position += m_Right * velocity;
		else if (direction == CameraMovement::LEFT)
	        m_Position -= m_Right * velocity;
	}

	bool EditorCamera::OnMouseScrolled(MouseScrolledEvent& e)
	{
		if (Input::IsMouseButtonPressed(Mouse::ButtonRight))
		{
			m_NormalSpeed = std::clamp(m_NormalSpeed += e.GetYOffset(), 0.1f, 50.0f);
		}
		else
		{
			if (m_ProjectionType == ProjectionType::Perspective)
			{
				if (m_FOV >= 4.0f && m_FOV <= 120.0f)
					m_FOV -= e.GetYOffset();
				if (m_FOV <= 4.0f)
					m_FOV = 4.0f;
				if (m_FOV >= 120.0f)
					m_FOV = 120.0f;
			}
			// else
			// {
			// 	// invert since we want to zoom out when scrolling down
			// 	// which is done by increasing the size of the ortho viewport
			// 	activeCamera->size = std::clamp(activeCamera->size -= e.GetYOffset(), 0.1f, 50.0f);
			// }
		}

		UpdateProjection();

		return false;
	}

	void EditorCamera::UpdateCameraVectors()
	{
		glm::vec3 offset;
		offset.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		offset.y = sin(glm::radians(m_Pitch));
		offset.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		
		m_Forward = glm::normalize(offset);
		m_Right = glm::normalize(glm::cross(m_Forward, m_WorldUp));
		m_Up = glm::normalize(glm::cross(m_Right, m_Forward));
	}

}
