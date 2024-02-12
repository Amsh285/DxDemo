#pragma once

namespace dsr
{
	namespace data
	{
		struct Vertex3FP2FTx3FN
		{
			DirectX::XMFLOAT3 Position;
			DirectX::XMFLOAT2 texCoords;
			DirectX::XMFLOAT3 Normal;

			Vertex3FP2FTx3FN()
				: Position(0.0f, 0.0f, 0.0f), texCoords(0.0f, 0.0f), Normal(0.0f, 0.0f, 0.0f)
			{
			}

			Vertex3FP2FTx3FN(
				const DirectX::XMFLOAT3& position,
				const DirectX::XMFLOAT2& texCoords,
				const DirectX::XMFLOAT3& normal
			)
				: Position(position), texCoords(texCoords), Normal(normal)
			{
			}
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
}
