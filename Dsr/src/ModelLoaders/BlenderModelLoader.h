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
		std::vector<std::string> SegmentLine(std::string line, const std::string& delimiter);
	};
}
