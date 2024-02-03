#include "dsrpch.h"
#include "ModelConfiguration.h"

namespace dsr
{
	std::vector<std::shared_ptr<directX::rendering::VertexGroup>> ModelConfiguration::GetVertexGroups() const
	{
		using namespace dsr::directX::rendering;

		std::vector<std::shared_ptr<VertexGroup>> vertexGroups;
		for (const auto& pair : m_vertexGroupMap) {
			vertexGroups.push_back(pair.second);
		}

		std::sort(
			vertexGroups.begin(),
			vertexGroups.end(),
			[](const std::shared_ptr<VertexGroup>& left, const std::shared_ptr<VertexGroup>& right) {
				return left->StartIndexLocation < right->StartIndexLocation; 
			});

		return vertexGroups;
	}

	void ModelConfiguration::ConfigureVertexGroup(const std::string& name, const std::function<void(std::shared_ptr<directX::rendering::VertexGroup>)>& configFunc)
	{
		auto it = m_vertexGroupMap.find(name);

		if (it != m_vertexGroupMap.end())
			configFunc(it->second);
	}

	std::variant<NotFoundError, DsrResult> ModelConfiguration::ConfigureVertexGroupWithResult(const std::string& name, const std::function<DsrResult(std::shared_ptr<directX::rendering::VertexGroup>)>& configFunc)
	{
		auto it = m_vertexGroupMap.find(name);

		if (it != m_vertexGroupMap.end())
			return configFunc(it->second);

		return NotFoundError("Vertexgroup with Name: " + name + " could not be found.");
	}

	ModelConfiguration::ModelConfiguration(
		const data::Transform& globalTransform,
		const directX::Direct3dVertexBufferf& vertexBuffer,
		const std::unordered_map<std::string, std::shared_ptr<directX::rendering::VertexGroup>>& vertexGroupMap)
		: GlobalTransform(globalTransform), m_vertexBuffer(vertexBuffer), m_vertexGroupMap(vertexGroupMap)
	{
	}
}
