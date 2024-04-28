#pragma once

#include "EngineSubSystems/EntityComponentSystem/Component.h"

namespace dsr
{
	namespace ecs
	{
		/*This should be adressed later on.
		NameComponent and TagComponent are probably not needed
		I can store them either directly in the entity or in the store
		This accesspattern is not effective to just get a name or a tag*/

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
