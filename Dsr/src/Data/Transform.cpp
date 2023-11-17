#include "dsrpch.h"
#include "Transform.h"

namespace dsr
{
	namespace data
	{
		DirectX::XMMATRIX Transform::CalculateModelMatrix() const
		{
			using namespace DirectX;

			XMMATRIX matTranslate = XMMatrixTranslationFromVector(Position);
			XMMATRIX matRotate = XMMatrixRotationRollPitchYawFromVector(Rotation);
			XMMATRIX matScale = XMMatrixScaling(XMVectorGetX(Scale), XMVectorGetY(Scale), XMVectorGetZ(Scale));

			return matTranslate * matScale * matRotate;
		}

		Transform::Transform()
		{
			Position = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
			Scale = DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
			Rotation = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		}
	}
}
