#pragma once

#include "Data/Vertex.h"

namespace dsr
{
	namespace data
	{
		namespace manipulation
		{
			struct SubDivisionVertex
			{
				Vertex3FP2FTx3FN Vertex;
				DirectX::XMVECTOR Position;
				uint32_t IndexBufferLocation;
			};

			struct SubDivisionTriangle
			{
				SubDivisionVertex A, B, C;
			};

			struct RaycastMeshHit
			{
				uint32_t IndexBuffer0;
				uint32_t IndexBuffer1;
				uint32_t IndexBuffer2;

				float Distance;
				DirectX::XMFLOAT3 Intersection;

				RaycastMeshHit()
					: IndexBuffer0(0), IndexBuffer1(0), IndexBuffer2(0),
					Distance(std::numeric_limits<float>::infinity()),
					Intersection(
						std::numeric_limits<float>::infinity(),
						std::numeric_limits<float>::infinity(),
						std::numeric_limits<float>::infinity())
				{
				}
			};
		}
	}
}
