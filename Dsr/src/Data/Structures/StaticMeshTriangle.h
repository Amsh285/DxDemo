#pragma once

#include "Data/Vertex.h"

namespace dsr
{
	namespace data
	{
		struct StaticMeshTriangle
		{
			DirectX::XMVECTOR V0;
			uint32_t Index0;

			DirectX::XMVECTOR V1;
			uint32_t Index1;

			DirectX::XMVECTOR V2;
			uint32_t Index2;

			StaticMeshTriangle();

			bool Equal(const StaticMeshTriangle& rhs) const;
		};
	}
}
