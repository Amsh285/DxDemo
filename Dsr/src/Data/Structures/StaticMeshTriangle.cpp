#include "dsrpch.h"
#include "StaticMeshTriangle.h"

namespace dsr
{
	namespace data
	{
		StaticMeshTriangle::StaticMeshTriangle()
			:	V0(DirectX::XMVectorZero()), Index0(0),
			V1(DirectX::XMVectorZero()), Index1(0),
			V2(DirectX::XMVectorZero()), Index2(0)
		{
		}

		bool StaticMeshTriangle::Equal(const StaticMeshTriangle& rhs) const
		{
			return Index0 == rhs.Index0
				&& Index1 == rhs.Index1
				&& Index2 == rhs.Index2;
		}
	}
}
