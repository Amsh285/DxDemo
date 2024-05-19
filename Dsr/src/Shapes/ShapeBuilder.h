#pragma once

namespace dsr
{
	namespace shapes
	{
		std::vector<float> Line(
			const DirectX::XMFLOAT3& from,
			const DirectX::XMFLOAT3& to,
			const DirectX::XMFLOAT4& color
		);
	}
}
