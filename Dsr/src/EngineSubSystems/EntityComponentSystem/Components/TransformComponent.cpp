#include "dsrpch.h"
#include "TransformComponent.h"

namespace dsr
{
	namespace ecs
	{
		DirectX::XMFLOAT3 TransformComponent::GetPositionVec3() const
		{
			using namespace DirectX;
			return XMFLOAT3(XMVectorGetX(m_position), XMVectorGetY(m_position), XMVectorGetZ(m_position));
		}

		void TransformComponent::SetPositionVec3(const DirectX::XMFLOAT3& position)
		{
			m_position = DirectX::XMVectorSet(position.x, position.y, position.z, 1.0f);
		}

		void TransformComponent::SetPosition(const DirectX::XMVECTOR& vec)
		{
			m_position = vec;
		}

		DirectX::XMFLOAT3 TransformComponent::GetScaleVec3() const
		{
			using namespace DirectX;
			return XMFLOAT3(XMVectorGetX(m_scale), XMVectorGetY(m_scale), XMVectorGetZ(m_scale));
		}

		void TransformComponent::SetScale(const DirectX::XMFLOAT3& scale)
		{
			m_scale = DirectX::XMVectorSet(scale.x, scale.y, scale.z, 0.0f);
		}

		RenderTransform TransformComponent::GetRenderTransform() const
		{
			using namespace DirectX;

			XMMATRIX model = CalculateModelMatrix();
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

		DirectX::XMMATRIX TransformComponent::CalculateModelMatrix() const
		{
			using namespace DirectX;

			XMMATRIX matTranslate = XMMatrixTranslationFromVector(m_position);
			XMMATRIX matRotate = XMMatrixRotationQuaternion(m_rotation);
			XMMATRIX matScale = XMMatrixScaling(XMVectorGetX(m_scale), XMVectorGetY(m_scale), XMVectorGetZ(m_scale));

			return matTranslate * matScale * matRotate;
		}
	}
}
