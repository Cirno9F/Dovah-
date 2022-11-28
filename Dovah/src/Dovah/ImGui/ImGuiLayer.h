#pragma once

#include "Dovah/Core/Layer.h"
#include "Dovah/Events/MouseEvent.h"
#include "Dovah/Events/ApplicationEvent.h"
#include "Dovah/Events/KeyEvent.h"

namespace Dovah
{
	class DOVAH_API ImGuiLayer : public  Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
	 	virtual void OnImGuiRender() override;
		
		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};
}