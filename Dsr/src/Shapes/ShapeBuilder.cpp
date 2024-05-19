#include "dsrpch.h"
#include "ShapeBuilder.h"

namespace dsr
{
	namespace shapes
	{
		std::vector<float> Line(const DirectX::XMFLOAT3& from, const DirectX::XMFLOAT3& to, const DirectX::XMFLOAT4& color)
		{
			std::vector<float> vertexBufferData;

			vertexBufferData.push_back(from.x);
			vertexBufferData.push_back(from.y);
			vertexBufferData.push_back(from.z);

			vertexBufferData.push_back(color.x);
			vertexBufferData.push_back(color.y);
			vertexBufferData.push_back(color.z);
			vertexBufferData.push_back(color.w);

			vertexBufferData.push_back(to.x);
			vertexBufferData.push_back(to.y);
			vertexBufferData.push_back(to.z);

			vertexBufferData.push_back(color.x);
			vertexBufferData.push_back(color.y);
			vertexBufferData.push_back(color.z);
			vertexBufferData.push_back(color.w);

			return vertexBufferData;
		}
	}
}
