#pragma once

#include <ArcEngine.h>

namespace ArcEngine
{
	class SettingsPanel
	{
	public:
		void OnImGuiRender();

	private:
		float m_Time = 0.0f;

		float m_FpsValues[50];
		std::vector<float> m_FrameTimes;
	};
}

