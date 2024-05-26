#pragma once

#include "EngineSubSystems/EntityComponentSystem/Component.h"

namespace dsr
{
	namespace ecs
	{
		class NameComponent : public Component
		{
		public:
			std::string GetName() const { return m_name; }
			void SetName(const std::string& name) { m_name = name; }

			NameComponent();
			NameComponent(const std::string& name);
		private:
			std::string m_name;
		};
	}
}
