#pragma once

namespace dsr
{
	struct Vertex3FP2FTx3FN
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT2 texCoords;
		DirectX::XMFLOAT3 Normal;
	};

	struct Vertex3FP4FC
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT4 Color;

		Vertex3FP4FC()
			: Position(0.0f, 0.0f, 0.0f), Color(1.0f, 1.0f, 1.0f, 1.0f)
		{
		}

		Vertex3FP4FC(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT4& color)
			: Position(position), Color(color)
		{
		}
	};
}
