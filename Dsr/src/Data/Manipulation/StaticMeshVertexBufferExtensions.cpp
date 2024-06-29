#include "dsrpch.h"
#include "StaticMeshVertexBufferExtensions.h"

std::vector<float> dsr::data::manipulation::GetVertexBufferF(const StaticMesh<Vertex3FP2FTx3FN>& mesh)
{
	std::vector<float> vertexBuffer;

	for (const auto& vertex : mesh.GetVertexBuffer())
	{
		vertexBuffer.push_back(vertex.Position.x);
		vertexBuffer.push_back(vertex.Position.y);
		vertexBuffer.push_back(vertex.Position.z);

		vertexBuffer.push_back(vertex.texCoords.x);
		vertexBuffer.push_back(vertex.texCoords.y);

		vertexBuffer.push_back(vertex.Normal.x);
		vertexBuffer.push_back(vertex.Normal.y);
		vertexBuffer.push_back(vertex.Normal.z);
	}

	return vertexBuffer;
}
