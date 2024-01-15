#include "dsrpch.h"
#include "NameComponent.h"

namespace dsr
{
	namespace ecs
	{
		NameComponent::NameComponent()
			: NameComponent("")
		{
		}

		NameComponent::NameComponent(const std::string& name)
			: m_name(name)
		{
		}
	}
}
