#pragma once

#include "Data/Structures/StaticMesh.h"

namespace dsr
{
	namespace data
	{
		namespace manipulation
		{
			template<class TVertex>
			std::shared_ptr<StaticMesh<TVertex>> FilterByNormalAngles(
				const std::shared_ptr<StaticMesh<TVertex>> sourceMesh,
				const float& thresholdAngle,
				const DirectX::XMVECTOR& comparisonNormal = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f))
			{
				
			}
		}
	}
}
