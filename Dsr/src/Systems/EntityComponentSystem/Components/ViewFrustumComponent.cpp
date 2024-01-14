#include "dsrpch.h"
#include "ViewFrustumComponent.h"

namespace dsr
{
	namespace ecs
	{
		ViewFrustumComponent::ViewFrustumComponent()
			: m_fieldOfViewY(45.0f), m_nearZ(0.1f), m_farZ(1000.0f)
		{
		}
	}
}
