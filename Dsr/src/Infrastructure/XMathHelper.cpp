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
		m.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

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

	DirectX::XMVECTOR ScreenToWorld(
		const int32_t& mouseX, const int32_t& mouseY, const int32_t& clientWidth, const int32_t& clientHeight,
		const DirectX::XMMATRIX& projectionMatrix,
		const DirectX::XMMATRIX& viewMatrix)
	{
		using namespace DirectX;

		XMVECTOR projectionDeterminant = XMMatrixDeterminant(projectionMatrix);
		XMMATRIX inverseProjectionMatrix = XMMatrixInverse(&projectionDeterminant, projectionMatrix);

		XMVECTOR viewDeterminant = XMMatrixDeterminant(viewMatrix);
		XMMATRIX inverseViewMatrix = XMMatrixInverse(&viewDeterminant, viewMatrix);

		float resx = 1.0f / clientWidth;
		float resy = 1.0f / clientHeight;

		float x = resx * mouseX;
		float y = resy * mouseY;

		XMVECTOR mClipSpace = XMVectorSet(2.0f * x - 1.0f, 1.0f - 2.0f * y, 1.0f, 1.0f);
		XMVECTOR mWorldSpace = XMVector4Transform(mClipSpace, inverseProjectionMatrix);

		// memo use this vector and apply inverse of model matrix for each geometry to test
		// this will translate mWorldspace to the local space of the geometry
		// you can calculate intersection there and dont net to transform geomtry to worldspace
		mWorldSpace = XMVector4Transform(mClipSpace, inverseViewMatrix);
		return mWorldSpace;
	}
}
