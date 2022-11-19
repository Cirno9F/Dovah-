#pragma once

#include "Dovah/Core.h"
#include "Dovah/Core/Timestep.h"
#include "Dovah/Events/Event.h"

namespace Dovah
{
	class DOVAH_API Layer
	{
	public:
		Layer(const std::string & name = "layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};
};

