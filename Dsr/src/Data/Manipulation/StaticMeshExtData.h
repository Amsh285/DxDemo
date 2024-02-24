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
		}
	}
}
