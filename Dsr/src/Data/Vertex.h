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

			bool operator==(const Vertex3FP2FTx3FN& other) const {
				using namespace DirectX;

				constexpr float epsilon = 1e-6f;

				return XMVector3NearEqual(XMLoadFloat3(&Position), XMLoadFloat3(&other.Position), XMVectorReplicate(epsilon))
					&& XMVector2NearEqual(XMLoadFloat2(&texCoords), XMLoadFloat2(&other.texCoords), XMVectorReplicate(epsilon))
					&& XMVector3NearEqual(XMLoadFloat3(&Normal), XMLoadFloat3(&other.Normal), XMVectorReplicate(epsilon));
			}

			size_t GetHashCode() const
			{
				size_t hash = 0;
				hash ^= std::hash<float>{}(Position.x) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
				hash ^= std::hash<float>{}(Position.y) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
				hash ^= std::hash<float>{}(Position.z) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
				hash ^= std::hash<float>{}(texCoords.x) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
				hash ^= std::hash<float>{}(texCoords.y) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
				hash ^= std::hash<float>{}(Normal.x) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
				hash ^= std::hash<float>{}(Normal.y) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
				hash ^= std::hash<float>{}(Normal.z) + 0x9e3779b9 + (hash << 6) + (hash >> 2);

				return hash;
			}
		};

		struct Vertex3FP2FTx3FNHasher
		{
			size_t operator()(const Vertex3FP2FTx3FN& vertex) const
			{
				return vertex.GetHashCode();
			}
		};

		struct Vertex3FP2FTx3FNEq
		{
			bool operator()(const Vertex3FP2FTx3FN& lhs, const Vertex3FP2FTx3FN& rhs) const
			{
				return lhs == rhs;
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
