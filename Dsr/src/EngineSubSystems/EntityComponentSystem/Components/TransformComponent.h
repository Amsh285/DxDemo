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

			RenderTransform()
				: Model(DirectX::XMMatrixIdentity()), Normal(DirectX::XMMatrixIdentity())
			{
			}
		};

		//Todo: The Component should only store and provide the model matrix.
		//Getters and Setters can maybe stay as helpers to modify the model matrix.

		class TransformComponent : public Component
		{
		public:
			DirectX::XMVECTOR GetPosition() const { return m_position; }
			void SetPosition(const DirectX::XMVECTOR& vec);

			DirectX::XMVECTOR GetScale() const { return m_scale; }
			void SetScale(const DirectX::XMVECTOR& scale);

			DirectX::XMVECTOR GetRotation() const { return m_rotation; }
			void SetRotation(const DirectX::XMVECTOR& quaternion) { m_rotation = quaternion; }

			DirectX::XMMATRIX GetModelMatrix() const;
			RenderTransform GetRenderTransform() const;

			TransformComponent();
		private:
			DirectX::XMVECTOR m_position;
			DirectX::XMVECTOR m_scale;
			DirectX::XMVECTOR m_rotation;
		};
	}
}
