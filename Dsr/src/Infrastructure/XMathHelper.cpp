#include "dsrpch.h"
#include "XMathHelper.h"

namespace dsr
{
	float Vector3Determinant(const DirectX::XMVECTOR& v0, const DirectX::XMVECTOR& v1, const DirectX::XMVECTOR& v2)
	{
		using namespace DirectX;

		XMMATRIX m;
		m.r[0] = v0;
		m.r[1] = v1;
		m.r[2] = v2;

		return XMVectorGetX(XMMatrixDeterminant(m));
	}

	RaycastPlaneHit Vector3PlaneIntersection(const DirectX::XMVECTOR& rayOrigin, const DirectX::XMVECTOR& rayDirection, const DirectX::XMVECTOR& planeNormal, const float& planeDistance)
	{
		using namespace DirectX;

		float denom = XMVectorGetX(XMVector3Dot(rayDirection, planeNormal));

		if (denom == 0.0f)
			return RaycastPlaneHit(true, std::numeric_limits<float>::infinity());

		float num = XMVectorGetX(XMVector3Dot(rayOrigin, planeNormal)) + planeDistance;

		return RaycastPlaneHit(false, -(num / denom));
	}

	RaycastPlaneHit Vector3PlaneIntersection(const DirectX::XMVECTOR& rayOrigin, const DirectX::XMVECTOR& rayDirection, const DirectX::XMVECTOR& planeNormal, const DirectX::XMVECTOR& planeOrigin)
	{
		using namespace DirectX;

		float denom = XMVectorGetX(XMVector3Dot(planeNormal, rayDirection));

		if (denom == 0.0f)
			return RaycastPlaneHit(true, std::numeric_limits<float>::infinity());

		float num = XMVectorGetX(XMVector3Dot(planeNormal, XMVectorSubtract(planeOrigin, rayOrigin)));

		return RaycastPlaneHit(false, num / denom);
	}
}
