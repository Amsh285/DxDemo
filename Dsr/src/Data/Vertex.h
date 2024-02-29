#pragma once

namespace dsr
{
	namespace data
	{
		struct Vertex3F
		{
			DirectX::XMFLOAT3 Position;

			Vertex3F()
				: Position(0.0f, 0.0f, 0.0f)
			{
			}

			Vertex3F(const DirectX::XMFLOAT3& position)
				: Position(position)
			{
			}

			~Vertex3F() = default;
		};

		struct Vertex3FP2FTx3FN : public Vertex3F
		{
			DirectX::XMFLOAT2 texCoords;
			DirectX::XMFLOAT3 Normal;

			Vertex3FP2FTx3FN()
				: texCoords(0.0f, 0.0f), Normal(0.0f, 0.0f, 0.0f)
			{
			}

			Vertex3FP2FTx3FN(
				const DirectX::XMFLOAT3& position,
				const DirectX::XMFLOAT2& texCoords,
				const DirectX::XMFLOAT3& normal
			)
				: Vertex3F(position), texCoords(texCoords), Normal(normal)
			{
			}
		};

		struct Vertex3FP4FC : public Vertex3F
		{
			DirectX::XMFLOAT4 Color;

			Vertex3FP4FC()
				: Color(1.0f, 1.0f, 1.0f, 1.0f)
			{
			}

			Vertex3FP4FC(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT4& color)
				: Vertex3F(position), Color(color)
			{
			}
		};
	}
}
