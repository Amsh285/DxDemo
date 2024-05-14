
#include "pch.h"

#include "Data/Structures/StaticMesh.h"
#include "Data/Vertex.h"

#include "Infrastructure/XMathHelper.h"

using namespace DirectX;

using namespace dsr::data;

namespace StaticMeshTests
{
	static constexpr float epsilon = 1e-6f;

	class BarycentricParameterCalculationTests : public testing::Test
	{
	public:
		XMFLOAT3 CalculateCoefficients(const XMVECTOR& p) const
		{
			const std::vector<float>& hitTestCache = m_mesh.GetHitTestCache();
			const std::vector<Vertex3F>& vertexBuffer = m_mesh.GetVertexBuffer();
			const std::vector<uint32_t>& indexBuffer = m_mesh.GetIndexBuffer();

			XMVECTOR a = XMLoadFloat3(&vertexBuffer[indexBuffer[0]].Position);
			XMVECTOR b = XMLoadFloat3(&vertexBuffer[indexBuffer[1]].Position);
			XMVECTOR c = XMLoadFloat3(&vertexBuffer[indexBuffer[2]].Position);

			float denom = hitTestCache[0];
			float v = dsr::Vector3Determinant(a, p, c) * denom;
			float w = dsr::Vector3Determinant(a, b, p) * denom;

			return XMFLOAT3(1.0f - v - w, v, w);
		}

		XMVECTOR Reconstruct(const XMVECTOR& a, const XMVECTOR& b, const XMVECTOR& c, const XMFLOAT3& barycentrics)
		{
			return XMVectorAdd(
				XMVectorAdd(
					XMVectorScale(a, barycentrics.x),
					XMVectorScale(b, barycentrics.y)
				),
				XMVectorScale(c, barycentrics.z)
			);
		}

		BarycentricParameterCalculationTests()
		{
			std::vector<Vertex3F> vertexBuffer;
			vertexBuffer.push_back(x = XMFLOAT3(1.0f, 0.0f, 0.0f));
			vertexBuffer.push_back(y = XMFLOAT3(0.0f, 1.0f, 0.0f));
			vertexBuffer.push_back(z = XMFLOAT3(0.0f, 0.0f, 1.0f));

			std::vector<uint32_t> indexBuffer = { 0, 1, 2 };

			m_mesh.SetVertexBuffer(vertexBuffer);
			m_mesh.SetIndexBuffer(indexBuffer);
			m_mesh.RefreshHitTestCache();
		}
	protected:
		XMFLOAT3 x, y, z;

		StaticMesh<Vertex3F> m_mesh;
	private:
	};

	TEST_F(BarycentricParameterCalculationTests, Coordinates_CanReconstructBarycenter)
	{
		constexpr float c = 1.0f / 3.0f;

		XMVECTOR vX = XMLoadFloat3(&x);
		XMVECTOR vY = XMLoadFloat3(&y);
		XMVECTOR vZ = XMLoadFloat3(&z);

		XMVECTOR barycenter = XMVectorAdd(
			XMVectorAdd(
				XMVectorScale(vX, c),
				XMVectorScale(vY, c)
			),
			XMVectorScale(vZ, c)
		);

		XMFLOAT3 coefficients = CalculateCoefficients(barycenter);
		XMVECTOR reconstructed = XMVectorAdd(
			XMVectorAdd(
				XMVectorScale(vX, coefficients.x),
				XMVectorScale(vY, coefficients.y)
			),
			XMVectorScale(vZ, coefficients.z)
		);

		bool consideredEqual = XMVector4NearEqual(barycenter, reconstructed, XMVectorReplicate(epsilon));
		EXPECT_TRUE(consideredEqual);
		EXPECT_EQ(1.0f, coefficients.x + coefficients.y + coefficients.z);
		EXPECT_TRUE(coefficients.x >= 0.0f && coefficients.y >= 0 && coefficients.z >= 0);
	}

	TEST_F(BarycentricParameterCalculationTests, Coordinates_CanReconstructEdges)
	{
		XMVECTOR vX = XMLoadFloat3(&x);
		XMVECTOR vY = XMLoadFloat3(&y);
		XMVECTOR vZ = XMLoadFloat3(&z);

		XMFLOAT3 coeffVX = CalculateCoefficients(vX);
		XMFLOAT3 coeffVY = CalculateCoefficients(vY);
		XMFLOAT3 coeffVZ = CalculateCoefficients(vZ);

		XMVECTOR reconstructedVX = Reconstruct(vX, vY, vZ, coeffVX);
		XMVECTOR reconstructedVY = Reconstruct(vX, vY, vZ, coeffVY);
		XMVECTOR reconstructedVZ = Reconstruct(vX, vY, vZ, coeffVZ);

		EXPECT_TRUE(XMVector4NearEqual(vX, reconstructedVX, XMVectorReplicate(epsilon)));
		EXPECT_TRUE(XMVector4NearEqual(vY, reconstructedVY, XMVectorReplicate(epsilon)));
		EXPECT_TRUE(XMVector4NearEqual(vZ, reconstructedVZ, XMVectorReplicate(epsilon)));

		EXPECT_EQ(1.0f, coeffVX.x + coeffVX.y + coeffVX.z);
		EXPECT_EQ(1.0f, coeffVY.x + coeffVY.y + coeffVY.z);
		EXPECT_EQ(1.0f, coeffVZ.x + coeffVZ.y + coeffVZ.z);

		EXPECT_TRUE(coeffVX.x >= 0.0f && coeffVX.y >= 0 && coeffVX.z >= 0);
		EXPECT_TRUE(coeffVY.x >= 0.0f && coeffVY.y >= 0 && coeffVY.z >= 0);
		EXPECT_TRUE(coeffVZ.x >= 0.0f && coeffVZ.y >= 0 && coeffVZ.z >= 0);
	}

	TEST_F(BarycentricParameterCalculationTests, Coordinates_CanReconstructOutsideOnPlane)
	{
		XMVECTOR vX = XMLoadFloat3(&x);
		XMVECTOR vY = XMLoadFloat3(&y);
		XMVECTOR vZ = XMLoadFloat3(&z);

		XMVECTOR vXY = XMVectorSubtract(vY, vX);
		XMVECTOR vXYOutside = XMVectorAdd(vX, XMVectorNegate(vXY));

		XMFLOAT3 coeff = CalculateCoefficients(vXYOutside);
		XMVECTOR reconstructed = Reconstruct(vX, vY, vZ, coeff);

		EXPECT_TRUE(XMVector4NearEqual(vXYOutside, reconstructed, XMVectorReplicate(epsilon)));
		EXPECT_FALSE(coeff.x >= 0.0f && coeff.y >= 0 && coeff.z >= 0);
	}
}
