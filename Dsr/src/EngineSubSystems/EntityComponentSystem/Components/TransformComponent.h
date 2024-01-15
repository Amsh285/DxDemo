#pragma once

#include "EngineSubSystems/EntityComponentSystem/Component.h"

namespace dsr
{
	namespace ecs
	{
		struct RenderTransform
		{
			DirectX::XMMATRIX Model;
			DirectX::XMMATRIX Normal;
		};

		class TransformComponent : public Component
		{
		public:
			DirectX::XMFLOAT3 GetPositionVec3() const;
			DirectX::XMVECTOR GetPosition() const { return m_position; }
			void SetPosition(const DirectX::XMFLOAT3& position);

			DirectX::XMFLOAT3 GetScaleVec3() const;
			DirectX::XMVECTOR GetScale() const { return m_scale; }
			void SetScale(const DirectX::XMFLOAT3& scale);

			DirectX::XMVECTOR GetRotation() const { return m_rotation; }
			void SetRotation(const DirectX::XMVECTOR& quaternion) { m_rotation = quaternion; }

			RenderTransform GetRenderTransform() const;

			TransformComponent();
		private:
			DirectX::XMMATRIX CalculateModelMatrix() const;

			DirectX::XMVECTOR m_position;
			DirectX::XMVECTOR m_scale;
			DirectX::XMVECTOR m_rotation;
		};
	}
}
