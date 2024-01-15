#pragma once

#include "EngineSubSystems/EntityComponentSystem/Component.h"

namespace dsr
{
	namespace ecs
	{
		class TagComponent : public Component
		{
		public:
			std::string GetTag() const { return m_tag; }
			void SetTag(const std::string& tag) { m_tag = tag; }
		private:
			std::string m_tag;
		};
	}
}
