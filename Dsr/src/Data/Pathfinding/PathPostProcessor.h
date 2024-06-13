#pragma once

#include "Data/Manipulation/StaticMeshExtensions.h"

#include "VertexIndexSearchResult.h"

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
			);

			std::vector<float> BuildVertexBufferConcurrent(
				const DirectX::XMVECTOR& start,
				const DirectX::XMVECTOR& finish,
				const DirectX::XMVECTOR& edge,
				const DirectX::XMVECTORF32& color
			);

			template<class TVertex>
			std::vector<float> BuildVertexBuffer(
				const DirectX::XMVECTOR& start,
				const DirectX::XMVECTOR& finish,
				const std::vector<uint32_t>& path,
				const std::vector<TVertex>& vertexData,
				const DirectX::XMVECTORF32& color
			)
			{
				using namespace DirectX;

				std::vector<float> buffer;
				buffer.push_back(XMVectorGetX(start));
				buffer.push_back(XMVectorGetY(start));
				buffer.push_back(XMVectorGetZ(start));

				buffer.push_back(XMVectorGetX(lineColor));
				buffer.push_back(XMVectorGetY(lineColor));
				buffer.push_back(XMVectorGetZ(lineColor));
				buffer.push_back(XMVectorGetW(lineColor));

				const TVertex& startnode = vertexData[path[path.size() - 1]];

				buffer.push_back(XMVectorGetX(startnode));
				buffer.push_back(XMVectorGetY(startnode));
				buffer.push_back(XMVectorGetZ(startnode));

				buffer.push_back(XMVectorGetX(lineColor));
				buffer.push_back(XMVectorGetY(lineColor));
				buffer.push_back(XMVectorGetZ(lineColor));
				buffer.push_back(XMVectorGetW(lineColor));

				for (size_t i = path.size() -1; i > 0; i--)
				{
					uint32_t currentIndex = indexPath[i];
					uint32_t nextIndex = indexPath[i - 1];

					XMVECTOR v0 = XMLoadFloat3(&vertexData[currentIndex].Position);
					XMVECTOR v1 = XMLoadFloat3(&vertexData[nextIndex].Position);

					buffer.push_back(XMVectorGetX(v0));
					buffer.push_back(XMVectorGetY(v0));
					buffer.push_back(XMVectorGetZ(v0));

					buffer.push_back(XMVectorGetX(lineColor));
					buffer.push_back(XMVectorGetY(lineColor));
					buffer.push_back(XMVectorGetZ(lineColor));
					buffer.push_back(XMVectorGetW(lineColor));

					buffer.push_back(XMVectorGetX(v1));
					buffer.push_back(XMVectorGetY(v1));
					buffer.push_back(XMVectorGetZ(v1));

					buffer.push_back(XMVectorGetX(lineColor));
					buffer.push_back(XMVectorGetY(lineColor));
					buffer.push_back(XMVectorGetZ(lineColor));
					buffer.push_back(XMVectorGetW(lineColor));
				}

				const TVertex& finishNode = vertexData[path[0]];

				buffer.push_back(XMVectorGetX(finishNode));
				buffer.push_back(XMVectorGetY(finishNode));
				buffer.push_back(XMVectorGetZ(finishNode));

				buffer.push_back(XMVectorGetX(lineColor));
				buffer.push_back(XMVectorGetY(lineColor));
				buffer.push_back(XMVectorGetZ(lineColor));
				buffer.push_back(XMVectorGetW(lineColor));

				buffer.push_back(XMVectorGetX(finish));
				buffer.push_back(XMVectorGetY(finish));
				buffer.push_back(XMVectorGetZ(finish));

				buffer.push_back(XMVectorGetX(lineColor));
				buffer.push_back(XMVectorGetY(lineColor));
				buffer.push_back(XMVectorGetZ(lineColor));
				buffer.push_back(XMVectorGetW(lineColor));
			}
		}
	}
}
