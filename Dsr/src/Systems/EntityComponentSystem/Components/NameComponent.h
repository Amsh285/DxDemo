#pragma once

#include "Systems/EntityComponentSystem/Component.h"

namespace dsr
{
	namespace ecs
	{
		class NameComponent : Component
		{
		public:
			std::string GetName() const { return m_name; }
			void SetName(const std::string& name) { m_name = name; }

			NameComponent();
			explicit NameComponent(const std::string& name);
		private:
			std::string m_name;
		};
	}
}
