#pragma once

#include "Infrastructure/DsrTypes.h"
#include "EngineSubSystems/EntityComponentSystem/Component.h"

namespace dsr
{
	namespace ecs
	{
		class ViewFrustumComponent : public Component
		{
		public:
			Degreef GetFieldOfViewY() const { return m_fieldOfViewY; }
			void SetFieldOfViewY(const Degreef& value) { m_fieldOfViewY = value; }

			float GetNearZ() const { return m_nearZ; }
			void SetNearZ(const float& value) { m_nearZ = value; }

			float GetFarZ() const { return m_farZ; }
			void SetFarZ(const float& value) { m_farZ = value; }

			ViewFrustumComponent();
		private:
			Degreef m_fieldOfViewY;
			float m_nearZ;
			float m_farZ;
		};
	}
}
