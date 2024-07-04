#include "dsrpch.h"
#include "PathPostProcessor.h"

namespace dsr
{
	namespace data
	{
		namespace pathfinding
		{
			std::vector<float> BuildVertexBufferCoTriangular(
				const DirectX::XMVECTOR& start,
				const DirectX::XMVECTOR& finish,
				const DirectX::XMVECTORF32& color
			)
			{
				using namespace DirectX;

				std::vector<float> vertexBuffer;
				vertexBuffer.push_back(XMVectorGetX(start));
				vertexBuffer.push_back(XMVectorGetY(start));
				vertexBuffer.push_back(XMVectorGetZ(start));

				vertexBuffer.push_back(XMVectorGetX(color));
				vertexBuffer.push_back(XMVectorGetY(color));
				vertexBuffer.push_back(XMVectorGetZ(color));
				vertexBuffer.push_back(XMVectorGetW(color));

				vertexBuffer.push_back(XMVectorGetX(finish));
				vertexBuffer.push_back(XMVectorGetY(finish));
				vertexBuffer.push_back(XMVectorGetZ(finish));

				vertexBuffer.push_back(XMVectorGetX(color));
				vertexBuffer.push_back(XMVectorGetY(color));
				vertexBuffer.push_back(XMVectorGetZ(color));
				vertexBuffer.push_back(XMVectorGetW(color));

				return vertexBuffer;
			}

			std::vector<float> BuildVertexBufferConcurrent(
				const DirectX::XMVECTOR& start,
				const DirectX::XMVECTOR& finish,
				const DirectX::XMVECTOR& connectionVertex,
				const DirectX::XMVECTORF32& color
			)
			{
				using namespace DirectX;

				std::vector<float> vertexBuffer;
				vertexBuffer.push_back(XMVectorGetX(start));
				vertexBuffer.push_back(XMVectorGetY(start));
				vertexBuffer.push_back(XMVectorGetZ(start));

				vertexBuffer.push_back(XMVectorGetX(color));
				vertexBuffer.push_back(XMVectorGetY(color));
				vertexBuffer.push_back(XMVectorGetZ(color));
				vertexBuffer.push_back(XMVectorGetW(color));

				vertexBuffer.push_back(XMVectorGetX(connectionVertex));
				vertexBuffer.push_back(XMVectorGetY(connectionVertex));
				vertexBuffer.push_back(XMVectorGetZ(connectionVertex));

				vertexBuffer.push_back(XMVectorGetX(color));
				vertexBuffer.push_back(XMVectorGetY(color));
				vertexBuffer.push_back(XMVectorGetZ(color));
				vertexBuffer.push_back(XMVectorGetW(color));
				
				vertexBuffer.push_back(XMVectorGetX(connectionVertex));
				vertexBuffer.push_back(XMVectorGetY(connectionVertex));
				vertexBuffer.push_back(XMVectorGetZ(connectionVertex));

				vertexBuffer.push_back(XMVectorGetX(color));
				vertexBuffer.push_back(XMVectorGetY(color));
				vertexBuffer.push_back(XMVectorGetZ(color));
				vertexBuffer.push_back(XMVectorGetW(color));

				vertexBuffer.push_back(XMVectorGetX(finish));
				vertexBuffer.push_back(XMVectorGetY(finish));
				vertexBuffer.push_back(XMVectorGetZ(finish));

				vertexBuffer.push_back(XMVectorGetX(color));
				vertexBuffer.push_back(XMVectorGetY(color));
				vertexBuffer.push_back(XMVectorGetZ(color));
				vertexBuffer.push_back(XMVectorGetW(color));

				return vertexBuffer;
			}
		}
	}
}
