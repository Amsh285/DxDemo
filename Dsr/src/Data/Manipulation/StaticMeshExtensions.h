#pragma once

#include "Data/Vertex.h"
#include "Data/Structures/StaticMesh.h"

#include "Infrastructure/DsrTypes.h"

namespace dsr
{
	namespace data
	{
		namespace manipulation
		{
			template<class TVertex>
			std::shared_ptr<StaticMesh<TVertex>> FilterByNormalAngles(
				const std::shared_ptr<StaticMesh<TVertex>> sourceMesh,
				const Radiansf& thresholdAngle,
				const DirectX::XMVECTOR& comparisonNormal = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f))
			{
				using namespace DirectX;

				const std::vector<TVertex>& sourceVertexBuffer = sourceMesh->GetVertexBuffer();
				const std::vector<uint32_t>& sourceIndexBuffer = sourceMesh->GetIndexBuffer();

				if (sourceIndexBuffer.size() % 3 != 0)
				{
					return std::make_shared<StaticMesh<TVertex>>();
				}

				std::shared_ptr<StaticMesh<TVertex>> filteredMesh = std::make_shared<StaticMesh<TVertex>>();
				std::vector<TVertex> filteredVertexBuffer;
				std::vector<uint32_t> filteredIndexBuffer;

				std::unordered_map<uint32_t, uint32_t> indexMap;
				uint32_t index = 0;

				for (size_t i = 0; i < sourceIndexBuffer.size(); i += 3)
				{
					const TVertex& v0 = sourceVertexBuffer[sourceIndexBuffer[i]];
					const TVertex& v1 = sourceVertexBuffer[sourceIndexBuffer[i + 1]];
					const TVertex& v2 = sourceVertexBuffer[sourceIndexBuffer[i + 2]];

					XMVECTOR u = XMVectorSubtract(XMLoadFloat3(&v2.Position), XMLoadFloat3(&v1.Position));
					XMVECTOR v = XMVectorSubtract(XMLoadFloat3(&v0.Position), XMLoadFloat3(&v1.Position));

					XMVECTOR faceNormal = XMVector3Normalize(XMVector3Cross(u, v));

					float angleRadians = std::atan2(XMVectorGetX(XMVector3Length(XMVector3Cross(faceNormal, comparisonNormal))), XMVectorGetX(XMVector3Dot(faceNormal, comparisonNormal)));

					if (angleRadians <= thresholdAngle)
					{
						for (size_t j = i; j < i + 3; j++)
						{
							auto it = indexMap.find(sourceIndexBuffer[j]);

							if (it == indexMap.end())
							{
								indexMap[sourceIndexBuffer[j]] = index++;
								filteredVertexBuffer.push_back(sourceVertexBuffer[sourceIndexBuffer[j]]);
							}

							filteredIndexBuffer.push_back(indexMap.at(sourceIndexBuffer[j]));
						}
					}
				}

				filteredMesh->SetVertexBuffer(filteredVertexBuffer);
				filteredMesh->SetIndexBuffer(filteredIndexBuffer);
				filteredMesh->SetWindingOrder(sourceMesh->GetWindingOrder());
				return filteredMesh;
			}

			std::shared_ptr<StaticMesh<Vertex3FP2FTx3FN>> SubDivide(const std::shared_ptr<StaticMesh<Vertex3FP2FTx3FN>> sourceMesh);
		}
	}
}
