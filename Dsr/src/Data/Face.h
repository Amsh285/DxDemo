#pragma once

namespace dsr
{
	namespace data
	{
		struct Face
		{
			DirectX::XMFLOAT3 Centroid;
			DirectX::XMFLOAT3 Normal;

			Face();
			Face(
				const DirectX::XMFLOAT3& centroid,
				const DirectX::XMFLOAT3& normal
			);
		};
	}
}
