#pragma once

#include "Data/Vertex.h"
#include "Data/Structures/StaticMesh.h"

namespace dsr
{
	namespace data
	{
		namespace manipulation
		{
			std::vector<float> GetVertexBufferF(const StaticMesh<Vertex3FP2FTx3FN>& mesh);
		}
	}
}
