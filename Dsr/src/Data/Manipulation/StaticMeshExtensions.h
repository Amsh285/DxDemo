#pragma once

#include "Data/Vertex.h"
#include "Data/Manipulation/StaticMeshExtData.h"
#include "Data/Structures/StaticMesh.h"
#include "Data/Structures/StaticMeshTriangle.h"

#include "Infrastructure/DsrTypes.h"
#include "Infrastructure/XMathHelper.h"

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

			StaticMesh<Vertex3F> FilterDistinct(const StaticMesh<Vertex3FP2FTx3FN>& sourceMesh);

			std::shared_ptr<StaticMesh<Vertex3FP2FTx3FN>> SubDivide(const std::shared_ptr<StaticMesh<Vertex3FP2FTx3FN>> sourceMesh);
			std::shared_ptr<StaticMesh<Vertex3FP2FTx3FN>> SubDivideBarycentric(const std::shared_ptr<StaticMesh<Vertex3FP2FTx3FN>> sourceMesh);

			template<class TVertex>
			std::vector<RaycastMeshHit> GetMeshIntersections(
				const std::shared_ptr<StaticMesh<TVertex>>& mesh,
				const DirectX::XMVECTOR& rayOrigin,
				const DirectX::XMVECTOR& rayDirection)
			{ 
				using namespace DirectX;

				XMVECTOR rayDirectionNormalized = XMVector4Normalize(rayDirection);

				const std::vector<float>& hitTestCache = mesh->GetHitTestCache();
				const std::vector<TVertex>& sourceVertexBuffer = mesh->GetVertexBuffer();
				const std::vector<uint32_t>& sourceIndexBuffer = mesh->GetIndexBuffer();
				size_t hitTestCacheIndex = 0;

				std::vector<RaycastMeshHit> hits;

				for (size_t i = 0; i < sourceIndexBuffer.size(); i += 3)
				{
					XMVECTOR v0 = XMLoadFloat3(&sourceVertexBuffer[sourceIndexBuffer[i]].Position);
					XMVECTOR v1 = XMLoadFloat3(&sourceVertexBuffer[sourceIndexBuffer[i + 1]].Position);
					XMVECTOR v2 = XMLoadFloat3(&sourceVertexBuffer[sourceIndexBuffer[i + 2]].Position);

					XMVECTOR planeNormal = XMVector3Normalize(XMVector3Cross(XMVectorSubtract(v1, v0), XMVectorSubtract(v2, v0)));
					RaycastPlaneHit planeHit = Vector3PlaneIntersection(rayOrigin, rayDirectionNormalized, planeNormal, v0);

					if (!planeHit.IsOrthogonal && planeHit.Distance >= 0.0f)
					{
						XMVECTOR interSection = XMVectorAdd(rayOrigin, XMVectorScale(rayDirectionNormalized, planeHit.Distance));

						float denom = hitTestCache[hitTestCacheIndex];
						float v = dsr::Vector3Determinant(v0, interSection, v2) * denom;
						float w = dsr::Vector3Determinant(v0, v1, interSection) * denom;
						float u = 1.0f - v - w;

						if (u >= 0 && v >= 0 && w >= 0)
						{
							RaycastMeshHit meshHitData;
							meshHitData.IndexBuffer0 = sourceIndexBuffer[i];
							meshHitData.IndexBuffer1 = sourceIndexBuffer[i + 1];
							meshHitData.IndexBuffer2 = sourceIndexBuffer[i + 2];
							meshHitData.Distance = planeHit.Distance;
							XMStoreFloat3(&meshHitData.Intersection, interSection);
							hits.push_back(meshHitData);
						}
					}

					++hitTestCacheIndex;
				}

				std::sort(
					hits.begin(),
					hits.end(),
					[](const RaycastMeshHit& meshHit1, const RaycastMeshHit& meshHit2) {
						return meshHit1.Distance < meshHit2.Distance;
					}
				);

				return hits;
			}

			std::optional<StaticMeshTriangle> FindIntersectionTriangle(
				const DirectX::XMVECTOR& value,
				const dsr::data::StaticMesh<dsr::data::Vertex3F>& mesh
			);

			std::vector<float> GetLinePath(
				const StaticMesh<Vertex3F>& sourcemesh,
				const std::vector<uint32_t>& indexPath,
				const std::array<float, 4>& color);
		}
	}
}
