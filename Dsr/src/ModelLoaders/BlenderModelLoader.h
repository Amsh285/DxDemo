#pragma once

#include "Data/Vertex.h"
#include "ErrorHandling/dsr_error.h"
#include "ModelLoaders/BlenderMTLLoader.h"

namespace dsr
{
constexpr auto ERROR_OPENMODELFILE = 102;
constexpr auto ERROR_OPENMODELFILEINVALIDFORMAT = 103;

constexpr auto ERROR_PARSEMODELFILEINVALIDFACEFORMAT = 1001;

	struct BlenderModel
	{
		std::vector<Vertex3FP2FTx3FN> VertexBuffer;
		std::vector<uint32_t> IndexBuffer;
	};

	struct FaceVertex
	{
		uint32_t VertexIndex;
		uint32_t TxCoordIndex;
		uint32_t NormalIndex;

		bool operator==(const FaceVertex& rhs) const
		{
			return VertexIndex == rhs.VertexIndex &&
				TxCoordIndex == rhs.TxCoordIndex &&
				NormalIndex == rhs.NormalIndex;
		}
	};

	struct FaceVertexEq
	{
		bool operator()(const FaceVertex& lhs, const FaceVertex& rhs)
		{
			return lhs.VertexIndex == rhs.VertexIndex &&
				lhs.TxCoordIndex == rhs.TxCoordIndex &&
				lhs.NormalIndex == rhs.NormalIndex;
		}
	};

	struct FaceVertexHash
	{
		size_t operator()(const FaceVertex& fv) const
		{
			size_t hash = std::hash<int>()(fv.VertexIndex);
			hash_combine(hash, std::hash<int>()(fv.TxCoordIndex));
			hash_combine(hash, std::hash<int>()(fv.NormalIndex));
			return hash;
		}

		// Utility function for combining hash values
		static void hash_combine(size_t& seed, size_t hash)
		{
			seed ^= hash + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		}
	};

	class BlenderModelLoader
	{
	public:
		std::variant<BlenderModel, dsr_error> Load(
			const std::filesystem::path& modelPath,
			const std::filesystem::path& materialPath);
	private:
		void ApplyVertexToBuffers(
			const FaceVertex& vertexIndexData,
			std::vector<Vertex3FP2FTx3FN>& vertexBuffer,
			std::vector<uint32_t>& indexBuffer,
			std::vector<DirectX::XMFLOAT3>& vertexPositions,
			std::vector<DirectX::XMFLOAT2>& vertexTxCoords,
			std::vector<DirectX::XMFLOAT3>& vertexNormals,
			std::unordered_map<FaceVertex, uint32_t, FaceVertexHash>& storedinidces,
			uint32_t& vertexIndex
		);

		std::vector<std::string> SegmentLine(std::string line, const std::string& delimiter);
		std::optional<uint32_t> SearchVertexIndexBufferIndex(const std::unordered_map<FaceVertex, uint32_t, FaceVertexHash>& vertexIndexBufferMap, const FaceVertex& vertexDataIndices) const;

		BlenderMTLLoader m_materialLoader;
	};
}
