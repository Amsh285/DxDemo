#include "dsrpch.h"
#include "XMathHelper.h"

namespace dsr
{
	DirectX::XMVECTOR Barycenter(const DirectX::XMVECTOR& a, const DirectX::XMVECTOR& b, const DirectX::XMVECTOR& c)
	{
		using namespace DirectX;

		const float scale = 1.0f / 3.0f;

		return XMVectorAdd(
			XMVectorScale(a, scale),
			XMVectorAdd(XMVectorScale(b, scale), XMVectorScale(c, scale))
		);
	}

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

	bool Intersects(const DirectX::XMVECTOR& value, const DirectX::XMVECTOR& planeNormal, const DirectX::XMVECTOR& planeOrigin, const float& epsilon)
	{
		using namespace DirectX;

		XMVECTOR hitTest = XMVector3Normalize(XMVectorSubtract(value, planeOrigin));
		return std::abs(XMVectorGetX(XMVector3Dot(hitTest, planeNormal))) < epsilon;
	}

	std::pair<uint32_t, uint32_t> FindClosestVertices(
		const DirectX::XMVECTOR& v0, const uint32_t& idx0,
		const DirectX::XMVECTOR& v1, const uint32_t& idx1,
		const DirectX::XMVECTOR& v2, const uint32_t& idx2,
		const DirectX::XMVECTOR& v3, const uint32_t& idx3,
		const DirectX::XMVECTOR& v4, const uint32_t& idx4,
		const DirectX::XMVECTOR& v5, const uint32_t& idx5
	)
	{
		using namespace DirectX;

		auto computeSquaredDistance = [](const XMVECTOR& a, const XMVECTOR& b) -> float
		{
			XMVECTOR diff = XMVectorSubtract(a, b);
			return XMVectorGetX(XMVector3LengthSq(diff));
		};

		std::tuple<uint32_t, uint32_t, float> distanceIndexMap[9];
		distanceIndexMap[0] = std::make_tuple(idx0, idx3, computeSquaredDistance(v0, v3));
		distanceIndexMap[1] = std::make_tuple(idx0, idx4, computeSquaredDistance(v0, v4));
		distanceIndexMap[2] = std::make_tuple(idx0, idx5, computeSquaredDistance(v0, v5));

		distanceIndexMap[3] = std::make_tuple(idx1, idx3, computeSquaredDistance(v1, v3));
		distanceIndexMap[4] = std::make_tuple(idx1, idx4, computeSquaredDistance(v1, v4));
		distanceIndexMap[5] = std::make_tuple(idx1, idx5, computeSquaredDistance(v1, v5));

		distanceIndexMap[6] = std::make_tuple(idx2, idx3, computeSquaredDistance(v2, v3));
		distanceIndexMap[7] = std::make_tuple(idx2, idx4, computeSquaredDistance(v2, v4));
		distanceIndexMap[8] = std::make_tuple(idx2, idx5, computeSquaredDistance(v2, v5));

		std::sort(
			distanceIndexMap,
			distanceIndexMap + 8,
			[](std::tuple<uint32_t, uint32_t, float> left, std::tuple<uint32_t, uint32_t, float> right)
			{ return std::get<2>(left) < std::get<2>(right); }
		);

		return std::make_pair(std::get<0>(distanceIndexMap[0]), std::get<1>(distanceIndexMap[0]));
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

		XMVECTOR clipSpace = XMVectorSet(2.0f * x - 1.0f, 1.0f - 2.0f * y, 1.0f, 0.0f);
		XMVECTOR viewSpace = XMVector3Transform(clipSpace, inverseProjectionMatrix);

		return XMVector3Transform(viewSpace, inverseViewMatrix);
	}
}
