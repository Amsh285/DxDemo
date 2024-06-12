#pragma once

namespace dsr
{
	struct RaycastPlaneHit
	{
		bool IsOrthogonal;
		float Distance;

		RaycastPlaneHit()
			: IsOrthogonal(false), Distance(0.0f)
		{
		}

		RaycastPlaneHit(const bool& isOrthogonal, const float& distance)
			: IsOrthogonal(isOrthogonal), Distance(distance)
		{
		}
	};

	DirectX::XMVECTOR Barycenter(
		const DirectX::XMVECTOR& a,
		const DirectX::XMVECTOR& b,
		const DirectX::XMVECTOR& c
	);

	float Vector3Determinant(
		const DirectX::XMVECTOR& v0,
		const DirectX::XMVECTOR& v1,
		const DirectX::XMVECTOR& v2
	);

	RaycastPlaneHit Vector3PlaneIntersection(
		const DirectX::XMVECTOR& rayOrigin,
		const DirectX::XMVECTOR& rayDirection,
		const DirectX::XMVECTOR& planeNormal,
		const float& planeDistance);

	RaycastPlaneHit Vector3PlaneIntersection(
		const DirectX::XMVECTOR& rayOrigin,
		const DirectX::XMVECTOR& rayDirection,
		const DirectX::XMVECTOR& planeNormal,
		const DirectX::XMVECTOR& planeOrigin
	);

	bool Intersects(
		const DirectX::XMVECTOR& value,
		const DirectX::XMVECTOR& planeNormal,
		const DirectX::XMVECTOR& planeOrigin,
		const float& epsilon = 1e-6f
	);

	std::pair<uint32_t, uint32_t> FindClosestVertices(
		const DirectX::XMVECTOR& v0, const uint32_t& idx0,
		const DirectX::XMVECTOR& v1, const uint32_t& idx1,
		const DirectX::XMVECTOR& v2, const uint32_t& idx2,
		const DirectX::XMVECTOR& v3, const uint32_t& idx3,
		const DirectX::XMVECTOR& v4, const uint32_t& idx4,
		const DirectX::XMVECTOR& v5, const uint32_t& idx5
	);

	DirectX::XMVECTOR ScreenToWorld(
		const int32_t& mouseX, const int32_t& mouseY,
		const int32_t& clientWidth, const int32_t& clientHeight,
		const DirectX::XMMATRIX& projectionMatrix,
		const DirectX::XMMATRIX& viewMatrix
	);
}
