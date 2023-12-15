#pragma once

#include "Data/Transform.h"
#include "DirectX/Direct3dVertexBufferf.h"
#include "DirectX/Rendering/Data/VertexGroup.h"

namespace dsr
{

	class ModelConfiguration
	{
	public:
		directX::Direct3dVertexBufferf GetVertexBuffer() const { return m_vertexBuffer; }
		std::unordered_map<std::string, std::shared_ptr<directX::rendering::VertexGroup>> GetVertexGroupMap() const { return m_vertexGroupMap; }

		data::Transform GlobalTransform;

		void RemoveVertexGroup(const std::string& name) { m_vertexGroupMap.erase(name); }

		ModelConfiguration() = default;
		ModelConfiguration(
			const data::Transform& globalTransform,
			const directX::Direct3dVertexBufferf& vertexBuffer,
			const std::unordered_map<std::string, std::shared_ptr<directX::rendering::VertexGroup>>& vertexGroupMap);
	private:
		directX::Direct3dVertexBufferf m_vertexBuffer;
		std::unordered_map<std::string, std::shared_ptr<directX::rendering::VertexGroup>> m_vertexGroupMap;
	};
}
