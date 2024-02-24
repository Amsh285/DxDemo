#include "dsrpch.h"
#include "Comparers.h"

namespace dsr
{
	bool XMVectorEqualComparer::operator()(const DirectX::XMVECTOR& lhs, const DirectX::XMVECTOR& rhs) const
	{
		using namespace DirectX;

		return XMVector4Equal(XMVectorNearEqual(lhs, rhs, XMVectorReplicate(1e-6f)), XMVectorTrueInt()) != 0;
	}
}
