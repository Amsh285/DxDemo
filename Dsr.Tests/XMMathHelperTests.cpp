
#include "pch.h"

#include "Infrastructure/XMathHelper.h"

using namespace DirectX;

using namespace dsr;

static constexpr float epsilon = 1e-6f;

namespace XMMathHelperTests
{
	class Vector3PlaneIntersectionTests : public testing::Test
	{
	public:
		Vector3PlaneIntersectionTests()
			: m_planeUpVectorNormal(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)),
			m_planeOrigin(XMVectorSet(0.0f, 2.0f, 0.0f, 1.0f))
		{
		}

	protected:
		XMVECTOR m_planeUpVectorNormal;
		XMVECTOR m_planeOrigin;
	private:
	};

	TEST_F(Vector3PlaneIntersectionTests, OriginUp_IntersectsWithPlane)
	{
		XMVECTOR originUp = XMVectorSet(0.0f, 10.0f, 0.0f, 0.0f);
		XMVECTOR originUpLeft = XMVectorSet(-5.0f, 10.0f, 0.0f, 0.0f);

		XMVECTOR rayDirection = XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f);

		RaycastPlaneHit originUpHit = Vector3PlaneIntersection(originUp, rayDirection, m_planeUpVectorNormal, m_planeOrigin);
		XMVECTOR originUpIntersection = XMVectorAdd(originUp, XMVectorScale(rayDirection, originUpHit.Distance));

		RaycastPlaneHit originUpLeftHit = Vector3PlaneIntersection(originUpLeft, rayDirection, m_planeUpVectorNormal, m_planeOrigin);
		XMVECTOR originUpLeftIntersection = XMVectorAdd(originUpLeft, XMVectorScale(rayDirection, originUpLeftHit.Distance));

		EXPECT_FALSE(originUpHit.IsOrthogonal);
		EXPECT_GE(originUpHit.Distance, 0.0f);
		EXPECT_TRUE(XMVector3NearEqual(originUpIntersection, XMVectorSet(0.0f, 2.0f, 0.0f, 1.0f), XMVectorReplicate(epsilon)));

		EXPECT_FALSE(originUpLeftHit.IsOrthogonal);
		EXPECT_GE(originUpLeftHit.Distance, 0.0f);
		EXPECT_TRUE(XMVector3NearEqual(originUpLeftIntersection, XMVectorSet(-5.0f, 2.0f, 0.0f, 1.0f), XMVectorReplicate(epsilon)));
	}

	TEST_F(Vector3PlaneIntersectionTests, OriginUp_Orthgonal_DoesNotIntersectWithPlane)
	{
		XMVECTOR originUp = XMVectorSet(0.0f, 10.0f, 0.0f, 0.0f);
		XMVECTOR rayDirection = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

		RaycastPlaneHit originUpHit = Vector3PlaneIntersection(originUp, rayDirection, m_planeUpVectorNormal, m_planeOrigin);

		EXPECT_TRUE(originUpHit.IsOrthogonal);
		EXPECT_EQ(originUpHit.Distance, std::numeric_limits<float>::infinity());
	}

	TEST_F(Vector3PlaneIntersectionTests, OriginUp_FacingAway_ReturnsNegativeDistance)
	{
		XMVECTOR originUp = XMVectorSet(0.0f, 10.0f, 0.0f, 0.0f);
		XMVECTOR rayDirection = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

		RaycastPlaneHit originUpHit = Vector3PlaneIntersection(originUp, rayDirection, m_planeUpVectorNormal, m_planeOrigin);

		EXPECT_FALSE(originUpHit.IsOrthogonal);
		EXPECT_LT(originUpHit.Distance, 0.0f);
	}
}
