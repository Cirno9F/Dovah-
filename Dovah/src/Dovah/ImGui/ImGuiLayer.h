#pragma once

#include "Dovah/Core.h"
#include "Dovah/Layer.h"

namespace Dovah
{
	class DOVAH_API ImGuiLayer : public  Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach();
		void OnDetach();
		void OnUpdate();
		void OnEvent(Event& event);
	private:
		float m_Time = 0.0f;
	};
}