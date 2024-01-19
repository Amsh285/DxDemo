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

			TagComponent(const std::string& tag);
		private:
			std::string m_tag;
		};
	}
}
