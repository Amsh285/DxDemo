
#include "pch.h"

#include "Data/Vertex.h"
#include "Data/Manipulation/StaticMeshExtensions.h"
#include "Data/Structures/StaticMesh.h"

using namespace DirectX;

using namespace dsr::data;
using namespace dsr::data::manipulation;

namespace StaticMeshExtensionsTests
{
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
		XMVECTOR rayOrigin = XMVectorSet(3.0f, 3.0f, 3.0f, 1.0f);
		XMVECTOR rayDirection = XMVector3Normalize(XMVectorSubtract(XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), rayOrigin));

		std::vector<RaycastMeshHit> meshHits = GetMeshIntersections(m_mesh, rayOrigin, rayDirection);

		EXPECT_GT(meshHits.size(), 0);
	}
}
