#include "dsrpch.h"
#include "System.h"

namespace dsr
{
	namespace ecs
	{
		System::System(const std::type_index& type)
			: System(type, 0)
		{
		}

		System::System(const std::type_index& type, const int32_t& sortOrder)
			: m_type(type), m_sortOrder(sortOrder)
		{
		}
	}
}
