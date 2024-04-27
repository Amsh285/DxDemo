#pragma once

#include "EngineSubSystems/EntityComponentSystem/Component.h"

namespace dsr
{
	namespace tests
	{
		class TestTagComponent : public dsr::ecs::Component
		{
		public:
			std::string GetName() const { return m_name; }
			void SetName(const std::string& name) { m_name = name; }

			TestTagComponent(const std::string& name);
		private:
			std::string m_name;
		};
	}
}
