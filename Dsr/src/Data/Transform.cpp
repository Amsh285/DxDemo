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

			XMVECTOR quaternion = XMQuaternionRotationRollPitchYaw(
				XMConvertToRadians(XMVectorGetX(Rotation)),
				XMConvertToRadians(XMVectorGetY(Rotation)),
				XMConvertToRadians(XMVectorGetZ(Rotation)));
			XMMATRIX matRotate = XMMatrixRotationQuaternion(quaternion);
			XMMATRIX matScale = XMMatrixScaling(XMVectorGetX(Scale), XMVectorGetY(Scale), XMVectorGetZ(Scale));

			//Counterclockwise
			//XMMatrixTransformation(,)
			//XMMatrixDecompose

			return matTranslate * matScale * matRotate;
		}

		DirectX::XMMATRIX Transform::CalculateNormalMatrix() const
		{
			using namespace DirectX;

			XMMATRIX model = CalculateModelMatrix();
			XMVECTOR determinant = XMMatrixDeterminant(model);

			// http://www.lighthouse3d.com/tutorials/glsl-12-tutorial/the-normal-matrix/
			return XMMatrixTranspose(XMMatrixInverse(&determinant, model));
		}

		Transform::Transform()
		{
			Position = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
			Scale = DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
			Rotation = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		}
	}
}
