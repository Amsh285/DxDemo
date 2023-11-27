#include "dsrpch.h"
#include "Camera.h"

namespace dsr
{
	namespace camerasystem
	{
		DirectX::XMMATRIX Camera::GetProjectionMatrix() const
		{
			return DirectX::XMMatrixPerspectiveFovLH(
				FovAngleY,
				AspectRatio,
				NearZ,
				FarZ
			);
		}

		DirectX::XMMATRIX Camera::GetViewMatrix() const
		{
			using namespace DirectX;

			XMVECTOR eyePosition = Transform.Position;
			XMVECTOR upVector = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

			XMVECTOR quaternion = XMQuaternionRotationRollPitchYaw(
				XMConvertToRadians(XMVectorGetX(Transform.Rotation)),
				XMConvertToRadians(XMVectorGetY(Transform.Rotation)),
				XMConvertToRadians(XMVectorGetZ(Transform.Rotation)));

			XMVECTOR forward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
			forward = XMVector3Rotate(forward, quaternion);

			XMVECTOR focusPoint = XMVectorAdd(eyePosition, forward);

			return XMMatrixLookAtLH(eyePosition, focusPoint, upVector);
		}

		Camera::Camera()
			: FovAngleY(DirectX::XMConvertToRadians(45.0f)),
			AspectRatio(0), NearZ(0.1f), FarZ(1000.0f)
		{
		}
	}
}
