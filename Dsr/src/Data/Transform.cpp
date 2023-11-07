#include "dsrpch.h"
#include "Transform.h"

namespace dsr
{
	namespace data
	{
		Transform::Transform()
			: m_position(0.0f, 0.0f, 0.0f), m_scale(1.0f, 1.0f, 1.0f), m_rotation(0.0f, 0.0f, 0.0f)
		{
		}
	}
}
