#pragma once

#include "Data/Vertex.h"
#include "ErrorHandling/dsr_error.h"

namespace dsr
{
constexpr auto ERROR_OPENMODELFILE = 102;
constexpr auto ERROR_OPENMODELFILEINVALIDFORMAT = 103;

	struct BlenderModel
	{
		std::vector<Vertex3FP2FTx3FN> VertexBuffer;
		std::vector<uint32_t> IndexBuffer;
	};

	class BlenderModelLoader
	{
	public:
		std::variant<BlenderModel, dsr_error> Load(const std::filesystem::path& path);
	private:
		void ApplyVertexToBuffers(
			const DirectX::XMINT3& vertexIndexData,
			std::vector<Vertex3FP2FTx3FN>& vertexBuffer,
			std::vector<uint32_t>& indexBuffer,
			std::vector<DirectX::XMFLOAT3>& vertexPositions,
			std::vector<DirectX::XMFLOAT2>& vertexTxCoords,
			std::vector<DirectX::XMFLOAT3>& vertexNormals,
			std::vector<std::pair<uint32_t, DirectX::XMINT3>>& storedinidces,
			uint32_t& vertexIndex
		);

		std::vector<std::string> SegmentLine(std::string line, const std::string& delimiter);
		std::optional<uint32_t> SearchVertexIndexBufferIndex(const std::vector<std::pair<uint32_t, DirectX::XMINT3>>& vertexIndexBufferMap, const DirectX::XMINT3& vertexDataIndices);
	};
}
