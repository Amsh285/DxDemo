#include "dsrpch.h"
#include "TransformComponent.h"

namespace dsr
{
	namespace ecs
	{
		void TransformComponent::SetPosition(const DirectX::XMVECTOR& vec)
		{
			m_position = vec;
		}

		void TransformComponent::SetScale(const DirectX::XMVECTOR& scale)
		{
			m_scale = scale;
		}

		DirectX::XMMATRIX TransformComponent::GetModelMatrix() const
		{
			using namespace DirectX;

			XMMATRIX matTranslate = XMMatrixTranslationFromVector(m_position);
			XMMATRIX matRotate = XMMatrixRotationQuaternion(m_rotation);
			XMMATRIX matScale = XMMatrixScaling(XMVectorGetX(m_scale), XMVectorGetY(m_scale), XMVectorGetZ(m_scale));

			return matTranslate * matScale * matRotate;
		}

		RenderTransform TransformComponent::GetRenderTransform() const
		{
			using namespace DirectX;

			XMMATRIX model = GetModelMatrix();
			XMVECTOR determinant = XMMatrixDeterminant(model);

			// http://www.lighthouse3d.com/tutorials/glsl-12-tutorial/the-normal-matrix/
			DirectX::XMMATRIX normal = XMMatrixTranspose(XMMatrixInverse(&determinant, model));

			RenderTransform transform;
			transform.Model = model;
			transform.Normal = normal;
			return transform;
		}

		TransformComponent::TransformComponent()
		{
			m_position = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
			m_scale = DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
			m_rotation = DirectX::XMQuaternionIdentity();
		}
	}
}
