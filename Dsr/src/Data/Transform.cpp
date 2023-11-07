#include "dsrpch.h"
#include "Transform.h"

namespace dsr
{
	namespace data
	{
		Transform::Transform()
		{
			Position = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
			Scale = DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
			Rotation = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
			Pivot = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		}
	}
}
