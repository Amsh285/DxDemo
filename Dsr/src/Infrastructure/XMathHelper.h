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

	DirectX::XMVECTOR ScreenToWorld(
		const int32_t& mouseX, const int32_t& mouseY,
		const int32_t& clientWidth, const int32_t& clientHeight,
		const DirectX::XMMATRIX& projectionMatrix,
		const DirectX::XMMATRIX& viewMatrix
	);
}
