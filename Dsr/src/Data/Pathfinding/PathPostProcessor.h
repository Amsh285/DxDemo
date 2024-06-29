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

				if (path.empty())
					return std::vector<float>();

				std::vector<float> buffer;
				buffer.push_back(XMVectorGetX(start));
				buffer.push_back(XMVectorGetY(start));
				buffer.push_back(XMVectorGetZ(start));

				buffer.push_back(XMVectorGetX(color));
				buffer.push_back(XMVectorGetY(color));
				buffer.push_back(XMVectorGetZ(color));
				buffer.push_back(XMVectorGetW(color));

				const XMVECTOR startnode = XMLoadFloat3(&vertexData[path[path.size() - 1]].Position);

				buffer.push_back(XMVectorGetX(startnode));
				buffer.push_back(XMVectorGetY(startnode));
				buffer.push_back(XMVectorGetZ(startnode));

				buffer.push_back(XMVectorGetX(color));
				buffer.push_back(XMVectorGetY(color));
				buffer.push_back(XMVectorGetZ(color));
				buffer.push_back(XMVectorGetW(color));

				for (size_t i = path.size() - 1; i > 0; i--)
				{
					const uint32_t currentIndex = path[i];
					const uint32_t nextIndex = path[i - 1];

					const XMVECTOR v0 = XMLoadFloat3(&vertexData[currentIndex].Position);
					const XMVECTOR v1 = XMLoadFloat3(&vertexData[nextIndex].Position);

					buffer.push_back(XMVectorGetX(v0));
					buffer.push_back(XMVectorGetY(v0));
					buffer.push_back(XMVectorGetZ(v0));

					buffer.push_back(XMVectorGetX(color));
					buffer.push_back(XMVectorGetY(color));
					buffer.push_back(XMVectorGetZ(color));
					buffer.push_back(XMVectorGetW(color));

					buffer.push_back(XMVectorGetX(v1));
					buffer.push_back(XMVectorGetY(v1));
					buffer.push_back(XMVectorGetZ(v1));

					buffer.push_back(XMVectorGetX(color));
					buffer.push_back(XMVectorGetY(color));
					buffer.push_back(XMVectorGetZ(color));
					buffer.push_back(XMVectorGetW(color));
				}

				const XMVECTOR finishNode = XMLoadFloat3(&vertexData[path[0]].Position);

				buffer.push_back(XMVectorGetX(finishNode));
				buffer.push_back(XMVectorGetY(finishNode));
				buffer.push_back(XMVectorGetZ(finishNode));

				buffer.push_back(XMVectorGetX(color));
				buffer.push_back(XMVectorGetY(color));
				buffer.push_back(XMVectorGetZ(color));
				buffer.push_back(XMVectorGetW(color));

				buffer.push_back(XMVectorGetX(finish));
				buffer.push_back(XMVectorGetY(finish));
				buffer.push_back(XMVectorGetZ(finish));

				buffer.push_back(XMVectorGetX(color));
				buffer.push_back(XMVectorGetY(color));
				buffer.push_back(XMVectorGetZ(color));
				buffer.push_back(XMVectorGetW(color));

				return buffer;
			}
		}
	}
}
