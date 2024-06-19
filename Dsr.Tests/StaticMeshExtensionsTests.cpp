
#include "pch.h"

#include "Data/Vertex.h"
#include "Data/Manipulation/StaticMeshExtensions.h"
#include "Data/Structures/StaticMesh.h"

using namespace DirectX;

using namespace dsr::data;
using namespace dsr::data::manipulation;

namespace StaticMeshExtensionsTests
{
	static constexpr float epsilon = 1e-6f;

	class StaticMeshIntersectionTests : public testing::Test
	{
	public:
		StaticMeshIntersectionTests()
		{
			std::vector<Vertex3F> vertexBuffer;
			vertexBuffer.push_back(a = XMFLOAT3(1.0f, 0.0f, 0.0f));
			vertexBuffer.push_back(b = XMFLOAT3(0.0f, 1.0f, 0.0f));
			vertexBuffer.push_back(c = XMFLOAT3(0.0f, 0.0f, 1.0f));

			std::vector<uint32_t> indexBuffer;
			indexBuffer.push_back(0);
			indexBuffer.push_back(1);
			indexBuffer.push_back(2);

			m_mesh = std::make_shared<StaticMesh<Vertex3F>>();
			m_mesh->SetVertexBuffer(vertexBuffer);
			m_mesh->SetIndexBuffer(indexBuffer);
			m_mesh->RefreshHitTestCache();
		}
	protected:
		XMFLOAT3 a, b, c;

		std::shared_ptr<StaticMesh<Vertex3F>> m_mesh;
	private:
	};

	TEST_F(StaticMeshIntersectionTests, RaycastHit)
	{
		constexpr float scale = 1.0f / 3.0f;

		XMVECTOR barycenter = XMVectorAdd(
			XMVectorScale(XMLoadFloat3(&a), scale),
			XMVectorAdd(
				XMVectorScale(XMLoadFloat3(&b), scale),
				XMVectorScale(XMLoadFloat3(&c), scale)
			)
		);

		XMVECTOR rayOrigin = XMVectorSet(3.0f, 3.0f, 3.0f, 1.0f);
		XMVECTOR rayDirection = XMVector3Normalize(XMVectorSubtract(XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), rayOrigin));

		std::vector<RaycastMeshHit> meshHits = GetMeshIntersections(m_mesh, rayOrigin, rayDirection);

		EXPECT_GT(meshHits.size(), 0);
		EXPECT_TRUE(XMVector3NearEqual(barycenter, XMLoadFloat3(&meshHits[0].Intersection), XMVectorReplicate(epsilon)));
	}

	TEST_F(StaticMeshIntersectionTests, RaycastMiss)
	{
		XMVECTOR rayOrigin = XMVectorSet(3.0f, 3.0f, 3.0f, 1.0f);
		XMVECTOR rayDirection = XMVector3Normalize(XMVectorSubtract(XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), rayOrigin));
		rayDirection = XMVectorNegate(rayDirection);

		std::vector<RaycastMeshHit> meshHits = GetMeshIntersections(m_mesh, rayOrigin, rayDirection);

		EXPECT_EQ(meshHits.size(), 0);
	}

	class BarycentricSubDivisionTests : public testing::Test
	{
	public:
		BarycentricSubDivisionTests()
		{
			m_connectedMesh.SetVertexBuffer({
				{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
				{ XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
				{ XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
				{ XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) }
				});
			m_connectedMesh.SetIndexBuffer({ 0, 1, 2, 1, 3, 2 });

			m_irregularOrderMesh.SetVertexBuffer({
				{ XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },
				{ XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },
				{ XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },
				});
			m_irregularOrderMesh.SetIndexBuffer({ 0, 2, 1});

			m_disconnectedMesh.SetVertexBuffer({
				{ XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
				{ XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
				{ XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },

				{ XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },
				{ XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },
				{ XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },
				});
			m_disconnectedMesh.SetIndexBuffer({ 0, 1, 2, 3, 5, 4 });
		}
	protected:
		StaticMesh<Vertex3FP2FTx3FN> m_connectedMesh;
		StaticMesh<Vertex3FP2FTx3FN> m_irregularOrderMesh;
		StaticMesh<Vertex3FP2FTx3FN> m_disconnectedMesh;
	private:
	};

	TEST_F(BarycentricSubDivisionTests, SubDivision_ConnectedMesh_ValidVertexAndIndexBuffer)
	{
		using namespace dsr::data;
		using namespace dsr::data::manipulation;

		StaticMesh<Vertex3FP2FTx3FN> subDividedMesh = SubDivideBarycentric(m_connectedMesh);
		std::vector<Vertex3FP2FTx3FN> vertexBuffer = subDividedMesh.GetVertexBuffer();
		std::vector<uint32_t> indexBuffer = subDividedMesh.GetIndexBuffer();

		EXPECT_EQ(vertexBuffer.size(), 6); // Original 4 vertices + 2 centroid vertices
		EXPECT_EQ(indexBuffer.size(), 18); // 6 new triangles, 3 indices each

		EXPECT_EQ(indexBuffer[0], 0);
		EXPECT_EQ(indexBuffer[1], 1);
		EXPECT_EQ(indexBuffer[2], 3); // Centroid vertex index for first triangle

		EXPECT_EQ(indexBuffer[3], 1);
		EXPECT_EQ(indexBuffer[4], 2);
		EXPECT_EQ(indexBuffer[5], 3); // Centroid vertex index for first triangle

		EXPECT_EQ(indexBuffer[6], 2);
		EXPECT_EQ(indexBuffer[7], 0);
		EXPECT_EQ(indexBuffer[8], 3); // Centroid vertex index for first triangle

		EXPECT_EQ(indexBuffer[9], 1);
		EXPECT_EQ(indexBuffer[10], 4);
		EXPECT_EQ(indexBuffer[11], 5); // Centroid vertex index for first triangle

		EXPECT_EQ(indexBuffer[12], 4);
		EXPECT_EQ(indexBuffer[13], 2);
		EXPECT_EQ(indexBuffer[14], 5); // Centroid vertex index for first triangle

		EXPECT_EQ(indexBuffer[15], 2);
		EXPECT_EQ(indexBuffer[16], 1);
		EXPECT_EQ(indexBuffer[17], 5); // Centroid vertex index for first triangle

		// Check the positions of the centroid vertices
		EXPECT_FLOAT_EQ(vertexBuffer[3].Position.x, (0.0f + 0.0f + 1.0f) / 3.0f);
		EXPECT_FLOAT_EQ(vertexBuffer[3].Position.y, (0.0f + 1.0f + 0.0f) / 3.0f);
		EXPECT_FLOAT_EQ(vertexBuffer[3].Position.z, 0.0f);

		EXPECT_FLOAT_EQ(vertexBuffer[5].Position.x, (0.0f + 1.0f + 1.0f) / 3.0f);
		EXPECT_FLOAT_EQ(vertexBuffer[5].Position.y, (1.0f + 1.0f + 0.0f) / 3.0f);
		EXPECT_FLOAT_EQ(vertexBuffer[5].Position.z, 0.0f);
	}

	TEST_F(BarycentricSubDivisionTests, SubDivision_IrregularMesh_ValidVertexAndIndexBuffer)
	{
		using namespace dsr::data;
		using namespace dsr::data::manipulation;

		StaticMesh<Vertex3FP2FTx3FN> subDividedMesh = SubDivideBarycentric(m_irregularOrderMesh);
		std::vector<Vertex3FP2FTx3FN> vertexBuffer = subDividedMesh.GetVertexBuffer();
		std::vector<uint32_t> indexBuffer = subDividedMesh.GetIndexBuffer();

		EXPECT_EQ(vertexBuffer.size(), 4); // Original 3 vertices + 1 centroid vertex
		EXPECT_EQ(indexBuffer.size(), 9); // 3 new triangles, 3 indices each

		EXPECT_EQ(indexBuffer[0], 0);
		EXPECT_EQ(indexBuffer[1], 1);
		EXPECT_EQ(indexBuffer[2], 3); // Centroid vertex index for first triangle

		EXPECT_EQ(indexBuffer[3], 1);
		EXPECT_EQ(indexBuffer[4], 2);
		EXPECT_EQ(indexBuffer[5], 3); // Centroid vertex index for second triangle

		EXPECT_EQ(indexBuffer[6], 2);
		EXPECT_EQ(indexBuffer[7], 0);
		EXPECT_EQ(indexBuffer[8], 3); // Centroid vertex index for third triangle
	}

	TEST_F(BarycentricSubDivisionTests, SubDivision_DisconnectedMesh_ValidVertexAndIndexBuffer)
	{
		using namespace dsr::data;
		using namespace dsr::data::manipulation;

		StaticMesh<Vertex3FP2FTx3FN> subDividedMesh = SubDivideBarycentric(m_disconnectedMesh);
		std::vector<Vertex3FP2FTx3FN> vertexBuffer = subDividedMesh.GetVertexBuffer();
		std::vector<uint32_t> indexBuffer = subDividedMesh.GetIndexBuffer();

		EXPECT_EQ(vertexBuffer.size(), 8); // Original 6 vertices + 2 centroid vertex
		EXPECT_EQ(indexBuffer.size(), 18); // 6 new triangles, 3 indices each

		EXPECT_EQ(indexBuffer[0], 0);
		EXPECT_EQ(indexBuffer[1], 1);
		EXPECT_EQ(indexBuffer[2], 3); // Centroid vertex index for first triangle

		EXPECT_EQ(indexBuffer[3], 1);
		EXPECT_EQ(indexBuffer[4], 2);
		EXPECT_EQ(indexBuffer[5], 3); // Centroid vertex index for second triangle

		EXPECT_EQ(indexBuffer[6], 2);
		EXPECT_EQ(indexBuffer[7], 0);
		EXPECT_EQ(indexBuffer[8], 3); // Centroid vertex index for third triangle

		EXPECT_EQ(indexBuffer[9], 4);
		EXPECT_EQ(indexBuffer[10], 5);
		EXPECT_EQ(indexBuffer[11], 7); // Centroid vertex index for fourth triangle

		EXPECT_EQ(indexBuffer[12], 5);
		EXPECT_EQ(indexBuffer[13], 6);
		EXPECT_EQ(indexBuffer[14], 7); // Centroid vertex index for fifth triangle

		EXPECT_EQ(indexBuffer[15], 6);
		EXPECT_EQ(indexBuffer[16], 4);
		EXPECT_EQ(indexBuffer[17], 7); // Centroid vertex index for sixth triangle
	}
}
