#include "dsrpch.h"
#include "ModelConfiguration.h"

namespace dsr
{
	ModelConfiguration::ModelConfiguration(
		const data::Transform& globalTransform,
		const directX::Direct3dVertexBufferf& vertexBuffer,
		const std::unordered_map<std::string, std::shared_ptr<directX::rendering::VertexGroup>>& vertexGroupMap)
		: GlobalTransform(globalTransform), m_vertexBuffer(vertexBuffer), m_vertexGroupMap(vertexGroupMap)
	{
	}
}
