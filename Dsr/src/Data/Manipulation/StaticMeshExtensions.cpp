#include "dsrpch.h"
#include "StaticMeshExtensions.h"

#include "Data/Manipulation/StaticMeshExtData.h"

#include "Infrastructure/Comparers.h"

namespace dsr
{
	namespace data
	{
		namespace manipulation
		{
			void SetSubdivisionData(
				const SubDivisionVertex& subDivisionData,
				uint32_t& index,
				std::unordered_map<uint32_t, uint32_t>& indexMap,
				std::vector<Vertex3FP2FTx3FN>& subdividedVertexBuffer,
				std::vector<uint32_t>& subdividedIndexBuffer
			);

			void SetSubdivisionData(
				const DirectX::XMVECTOR& edgeSplitPoint,
				const Vertex3FP2FTx3FN& splitVertex,
				uint32_t& index,
				std::unordered_map<DirectX::XMVECTOR, uint32_t, XMVectorHasher, XMVectorEqualComparer<1e-6f>>& splitMap,
				std::vector<Vertex3FP2FTx3FN>& subdividedVertexBuffer,
				std::vector<uint32_t>& subdividedIndexBuffer
			);


			struct IndexBufferChangeEntry
			{
				uint32_t OldValue;
				uint32_t NewValue;
			};

			StaticMesh<Vertex3F> FilterDistinct(const StaticMesh<Vertex3FP2FTx3FN>& sourceMesh)
			{
				using namespace DirectX;

				static dsr::XMVectorEqualComparer<1e-6f> comparer;

				const std::vector<Vertex3FP2FTx3FN>& vertexBuffer = sourceMesh.GetVertexBuffer();
				std::vector<uint32_t> indices = sourceMesh.GetIndexBuffer();
				std::unordered_map<size_t, IndexBufferChangeEntry> duplicateIndices;

				for (size_t i = 0; i < indices.size(); i++)
				{
					if (duplicateIndices.find(i) != duplicateIndices.end())
						continue;

					uint32_t currentIndex = indices[i];
					XMVECTOR v0 = XMLoadFloat3(&vertexBuffer[currentIndex].Position);

					for (size_t j = 0; j < indices.size(); j++)
					{
						if (i == j)
							continue;

						uint32_t nextIndex = indices[j];
						XMVECTOR v1 = XMLoadFloat3(&vertexBuffer[nextIndex].Position);

						if (comparer.operator()(v0, v1) && currentIndex != nextIndex)
						{
							duplicateIndices[j] = { nextIndex, currentIndex };
							indices[j] = currentIndex;
						}
					}
				}

				std::vector<Vertex3F> vertices;

				for (const Vertex3FP2FTx3FN& v : vertexBuffer)
					vertices.push_back(Vertex3F(v.Position));

				StaticMesh<Vertex3F> filteredMesh;
				filteredMesh.SetVertexBuffer(vertices);
				filteredMesh.SetIndexBuffer(indices);
				return filteredMesh;
			}

			std::shared_ptr<StaticMesh<Vertex3FP2FTx3FN>> SubDivide(const std::shared_ptr<StaticMesh<Vertex3FP2FTx3FN>> sourceMesh)
			{
				using namespace DirectX;

				const std::vector<Vertex3FP2FTx3FN>& sourceVertexBuffer = sourceMesh->GetVertexBuffer();
				const std::vector<uint32_t>& sourceIndexBuffer = sourceMesh->GetIndexBuffer();

				if (sourceIndexBuffer.size() % 3 != 0)
				{
					return std::make_shared<StaticMesh<Vertex3FP2FTx3FN>>();
				}

				std::shared_ptr<StaticMesh<Vertex3FP2FTx3FN>> subdividedMesh = std::make_shared<StaticMesh<Vertex3FP2FTx3FN>>();
				std::vector<Vertex3FP2FTx3FN> subdividedVertexBuffer;
				std::vector<uint32_t> subdividedIndexBuffer;

				SubDivisionTriangle subDivisionData;

				uint32_t index = 0;
				std::unordered_map<uint32_t, uint32_t> indexMap;
				std::unordered_map<XMVECTOR, uint32_t, XMVectorHasher, XMVectorEqualComparer<1e-6f>> splitMap;

				for (size_t i = 0; i < sourceIndexBuffer.size(); i += 3)
				{
					const Vertex3FP2FTx3FN& vertex0 = sourceVertexBuffer[sourceIndexBuffer[i]];
					const Vertex3FP2FTx3FN& vertex1 = sourceVertexBuffer[sourceIndexBuffer[i + 1]];
					const Vertex3FP2FTx3FN& vertex2 = sourceVertexBuffer[sourceIndexBuffer[i + 2]];

					XMVECTOR v0 = XMLoadFloat3(&vertex0.Position);
					XMVECTOR v1 = XMLoadFloat3(&vertex1.Position);
					XMVECTOR v2 = XMLoadFloat3(&vertex2.Position);

					XMVECTOR u = XMVectorSubtract(v1, v0);
					XMVECTOR v = XMVectorSubtract(v2, v0);
					float largestAngle = std::atan2(XMVectorGetX(XMVector3Length(XMVector3Cross(u, v))), XMVectorGetX(XMVector3Dot(u, v)));
					subDivisionData.A = { vertex0, v0, sourceIndexBuffer[i] };
					subDivisionData.B = { vertex1, v1, sourceIndexBuffer[i + 1] };
					subDivisionData.C = { vertex2, v2, sourceIndexBuffer[i + 2] };

					//find the side opposite to the largest angle
					u = XMVectorSubtract(v0, v1);
					v = XMVectorSubtract(v2, v1);
					float angle = std::atan2(XMVectorGetX(XMVector3Length(XMVector3Cross(u, v))), XMVectorGetX(XMVector3Dot(u, v)));

					//arcos - crossproduct

					if (angle > largestAngle)
					{
						subDivisionData.A = { vertex1, v1, sourceIndexBuffer[i + 1] };
						subDivisionData.B = { vertex2, v2, sourceIndexBuffer[i + 2] };
						subDivisionData.C = { vertex0, v0, sourceIndexBuffer[i] };
						largestAngle = angle;
					}

					u = XMVectorSubtract(v0, v2);
					v = XMVectorSubtract(v1, v2);
					angle = std::atan2(XMVectorGetX(XMVector3Length(XMVector3Cross(u, v))), XMVectorGetX(XMVector3Dot(u, v)));

					if (angle > largestAngle)
					{
						subDivisionData.A = { vertex2, v2, sourceIndexBuffer[i + 2] };
						subDivisionData.B = { vertex0, v0, sourceIndexBuffer[i] };
						subDivisionData.C = { vertex1, v1, sourceIndexBuffer[i + 1] };
						largestAngle = angle;
					}

					// Get the new Subdivision Splitvector
					XMVECTOR edge = XMVectorSubtract(subDivisionData.B.Position, subDivisionData.C.Position);
					edge = XMVectorScale(edge, 0.5f);
					XMVECTOR edgeSplitPoint = XMVectorAdd(subDivisionData.C.Position, edge);

					// Interpolate Vertexdata for the Splitvector
					XMVECTOR interpolatedNormal = XMVectorLerp(
						XMLoadFloat3(&subDivisionData.B.Vertex.Normal),
						XMLoadFloat3(&subDivisionData.C.Vertex.Normal),
						0.5f
					);

					XMVECTOR interpolatedTx = XMVectorLerp(
						XMLoadFloat2(&subDivisionData.B.Vertex.texCoords),
						XMLoadFloat2(&subDivisionData.C.Vertex.texCoords),
						0.5f
					);

					Vertex3FP2FTx3FN splitVertex;
					XMStoreFloat3(&splitVertex.Position, edgeSplitPoint);
					XMStoreFloat3(&splitVertex.Normal, interpolatedNormal);
					XMStoreFloat2(&splitVertex.texCoords, interpolatedTx);

					SetSubdivisionData(subDivisionData.A, index, indexMap, subdividedVertexBuffer, subdividedIndexBuffer);
					SetSubdivisionData(subDivisionData.B, index, indexMap, subdividedVertexBuffer, subdividedIndexBuffer);
					SetSubdivisionData(edgeSplitPoint, splitVertex, index, splitMap, subdividedVertexBuffer, subdividedIndexBuffer);

					SetSubdivisionData(subDivisionData.A, index, indexMap, subdividedVertexBuffer, subdividedIndexBuffer);
					SetSubdivisionData(edgeSplitPoint, splitVertex, index, splitMap, subdividedVertexBuffer, subdividedIndexBuffer);
					SetSubdivisionData(subDivisionData.C, index, indexMap, subdividedVertexBuffer, subdividedIndexBuffer);
				}

				subdividedMesh->SetVertexBuffer(subdividedVertexBuffer);
				subdividedMesh->SetIndexBuffer(subdividedIndexBuffer);
				subdividedMesh->SetWindingOrder(sourceMesh->GetWindingOrder());
				return subdividedMesh;
			}

			std::shared_ptr<StaticMesh<Vertex3FP2FTx3FN>> SubDivideBarycentric(const std::shared_ptr<StaticMesh<Vertex3FP2FTx3FN>> sourceMesh)
			{
				using namespace DirectX;

				constexpr float c = 1.0f / 3.0f;

				const std::vector<Vertex3FP2FTx3FN>& sourceVertexBuffer = sourceMesh->GetVertexBuffer();
				const std::vector<uint32_t>& sourceIndexBuffer = sourceMesh->GetIndexBuffer();

				if (sourceIndexBuffer.size() % 3 != 0)
				{
					return std::make_shared<StaticMesh<Vertex3FP2FTx3FN>>();
				}

				std::shared_ptr<StaticMesh<Vertex3FP2FTx3FN>> subdividedMesh = std::make_shared<StaticMesh<Vertex3FP2FTx3FN>>();
				std::vector<Vertex3FP2FTx3FN> subdividedVertexBuffer;
				std::vector<uint32_t> subdividedIndexBuffer;

				SubDivisionTriangle subDivisionData;

				uint32_t index = 0;
				std::unordered_map<uint32_t, uint32_t> indexMap;
				std::unordered_map<XMVECTOR, uint32_t, XMVectorHasher, XMVectorEqualComparer<1e-6f>> splitMap;

				for (size_t i = 0; i < sourceIndexBuffer.size(); i += 3)
				{
					const Vertex3FP2FTx3FN& vertex0 = sourceVertexBuffer[sourceIndexBuffer[i]];
					const Vertex3FP2FTx3FN& vertex1 = sourceVertexBuffer[sourceIndexBuffer[i + 1]];
					const Vertex3FP2FTx3FN& vertex2 = sourceVertexBuffer[sourceIndexBuffer[i + 2]];

					XMVECTOR v0 = XMLoadFloat3(&vertex0.Position);
					XMVECTOR v1 = XMLoadFloat3(&vertex1.Position);
					XMVECTOR v2 = XMLoadFloat3(&vertex2.Position);

					XMVECTOR centroid = XMVectorAdd(
						XMVectorAdd(XMVectorScale(v0, c), XMVectorScale(v1, c)),
						XMVectorScale(v2, c)
					);


				}

				subdividedMesh->SetVertexBuffer(subdividedVertexBuffer);
				subdividedMesh->SetIndexBuffer(subdividedIndexBuffer);
				subdividedMesh->SetWindingOrder(sourceMesh->GetWindingOrder());
				return subdividedMesh;
			}

			std::vector<float> GetLinePath(
				const StaticMesh<Vertex3F>& sourcemesh,
				const std::vector<uint32_t>& indexPath,
				const std::array<float, 4>& color)
			{
				using namespace DirectX;

				std::vector<Vertex3F> vertexBuffer = sourcemesh.GetVertexBuffer();
				std::vector<float> vertexData;

				for (size_t i = 0; i < indexPath.size() - 1; i++)
				{
					uint32_t currentIndex = indexPath[i];
					uint32_t nextIndex = indexPath[i + 1];

					XMVECTOR v0 = XMLoadFloat3(&vertexBuffer[currentIndex].Position);
					XMVECTOR v1 = XMLoadFloat3(&vertexBuffer[nextIndex].Position);

					vertexData.push_back(XMVectorGetX(v0));
					vertexData.push_back(XMVectorGetY(v0));
					vertexData.push_back(XMVectorGetZ(v0));

					vertexData.push_back(color[0]);
					vertexData.push_back(color[1]);
					vertexData.push_back(color[2]);
					vertexData.push_back(color[3]);

					vertexData.push_back(XMVectorGetX(v1));
					vertexData.push_back(XMVectorGetY(v1));
					vertexData.push_back(XMVectorGetZ(v1));

					vertexData.push_back(color[0]);
					vertexData.push_back(color[1]);
					vertexData.push_back(color[2]);
					vertexData.push_back(color[3]);
				}

				return vertexData;
			}

			void SetSubdivisionData(
				const SubDivisionVertex& subDivisionData,
				uint32_t& index,
				std::unordered_map<uint32_t, uint32_t>& indexMap,
				std::vector<Vertex3FP2FTx3FN>& subdividedVertexBuffer,
				std::vector<uint32_t>& subdividedIndexBuffer
			)
			{
				auto it = indexMap.find(subDivisionData.IndexBufferLocation);

				if (it == indexMap.end())
				{
					int newIndex = index++;
					indexMap[subDivisionData.IndexBufferLocation] = newIndex;
					subdividedIndexBuffer.push_back(newIndex);
					subdividedVertexBuffer.push_back(subDivisionData.Vertex);
				}
				else
				{
					subdividedIndexBuffer.push_back(it->second);
				}
			}

			void SetSubdivisionData(
				const DirectX::XMVECTOR& edgeSplitPoint,
				const Vertex3FP2FTx3FN& splitVertex,
				uint32_t& index,
				std::unordered_map<DirectX::XMVECTOR, uint32_t, XMVectorHasher, XMVectorEqualComparer<1e-6f>>& splitMap,
				std::vector<Vertex3FP2FTx3FN>& subdividedVertexBuffer,
				std::vector<uint32_t>& subdividedIndexBuffer
			)
			{
				auto it = splitMap.find(edgeSplitPoint);

				if (it == splitMap.end())
				{
					uint32_t newIndex = index++;
					splitMap[edgeSplitPoint] = newIndex;
					subdividedIndexBuffer.push_back(newIndex);
					subdividedVertexBuffer.push_back(splitVertex);
				}
				else
				{
					subdividedIndexBuffer.push_back(it->second);
				}
			}
		}
	}
}
