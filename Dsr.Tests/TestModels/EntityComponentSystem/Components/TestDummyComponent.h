#pragma once

#include "EngineSubSystems/EntityComponentSystem/Component.h"

namespace dsr
{
	namespace tests
	{
		class TestDummyComponent : public dsr::ecs::Component
		{
		public:
			std::string GetDummy() const { return m_dummy; }
			void SetDummy(const std::string& dummy) { m_dummy = dummy; }

			TestDummyComponent();
		private:
			std::string m_dummy;
		};
	}
}
