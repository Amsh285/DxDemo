#include "dsrpch.h"
#include "Face.h"

namespace dsr
{
	namespace data
	{
		Face::Face()
			: Centroid(0.0f, 0.0f, 0.0f), Normal(0.0f, 0.0f, 0.0f)
		{
		}

		Face::Face(const DirectX::XMFLOAT3& centroid, const DirectX::XMFLOAT3& normal)
			: Centroid(centroid), Normal(normal)
		{
		}
	}
}
