#include "dsrpch.h"
#include "BlenderModelLoader.h"

namespace dsr
{
	std::variant<BlenderModel, dsr_error> BlenderModelLoader::Load(const std::filesystem::path& path)
	{
		std::fstream modelFile;
		modelFile.open(path, std::ios::in);

		if (!modelFile.is_open())
		{
			std::string message = "error loading modelfile: ";
			message += path.generic_string();
			message += ". File could not be opened.";
			return dsr_error(message, ERROR_OPENMODELFILE);
		}
		//https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c

		std::string line;
		std::getline(modelFile, line);
		std::vector<std::string> versionSegments = SegmentLine(line, " ");

		if (versionSegments[1] != "Blender")
		{
			std::string message = "error loading modelfile: ";
			message += path.generic_string();
			message += ". Invalid File Format.";
			return dsr_error(message, ERROR_OPENMODELFILEINVALIDFORMAT);
		}

		// skip headerinfo
		std::getline(modelFile, line);
		std::getline(modelFile, line);
		std::getline(modelFile, line);

		std::vector<std::pair<uint32_t, DirectX::XMINT3>> storedinidces;
		uint32_t vertexIndex = 0;

		std::vector<DirectX::XMFLOAT3> vertexPositions;
		std::vector<DirectX::XMFLOAT2> vertexTxCoords;
		std::vector<DirectX::XMFLOAT3> vertexNormals;

		std::vector<Vertex3FP2FTx3FN> vertexBuffer;
		std::vector<uint32_t> indexBuffer;

		while (std::getline(modelFile, line))
		{
			std::vector<std::string> lineData = SegmentLine(line, " ");

			if (lineData[0] == "v")
			{
				vertexPositions.push_back(DirectX::XMFLOAT3(std::stof(lineData[1]), std::stof(lineData[2]), std::stof(lineData[3])));
			}
			else if (lineData[0] == "vn")
			{
				vertexNormals.push_back(DirectX::XMFLOAT3(std::stof(lineData[1]), std::stof(lineData[2]), std::stof(lineData[3])));
			}
			else if (lineData[0] == "vt")
			{
				vertexTxCoords.push_back(DirectX::XMFLOAT2(std::stof(lineData[1]), std::stof(lineData[2])));
			}
			else if (lineData[0] == "f")
			{
				std::vector<std::string> v0 = SegmentLine(lineData[1], "/");
				std::vector<std::string> v1 = SegmentLine(lineData[2], "/");
				std::vector<std::string> v2 = SegmentLine(lineData[3], "/");
				std::vector<std::string> v3 = SegmentLine(lineData[4], "/");

				DirectX::XMINT3 vertex0(std::stoi(v0[0]) -1, std::stoi(v0[1]) -1, std::stoi(v0[2]) - 1);
				DirectX::XMINT3 vertex1(std::stoi(v1[0]) - 1, std::stoi(v1[1]) - 1, std::stoi(v1[2]) - 1);
				DirectX::XMINT3 vertex2(std::stoi(v2[0]) - 1, std::stoi(v2[1]) - 1, std::stoi(v2[2]) - 1);
				DirectX::XMINT3 vertex3(std::stoi(v3[0]) - 1, std::stoi(v3[1]) - 1, std::stoi(v3[2]) - 1);

				//windingorder: clockwise
				ApplyVertexToBuffers(vertex0, vertexBuffer, indexBuffer, vertexPositions, vertexTxCoords, vertexNormals, storedinidces, vertexIndex);
				ApplyVertexToBuffers(vertex1, vertexBuffer, indexBuffer, vertexPositions, vertexTxCoords, vertexNormals, storedinidces, vertexIndex);
				ApplyVertexToBuffers(vertex2, vertexBuffer, indexBuffer, vertexPositions, vertexTxCoords, vertexNormals, storedinidces, vertexIndex);
				ApplyVertexToBuffers(vertex0, vertexBuffer, indexBuffer, vertexPositions, vertexTxCoords, vertexNormals, storedinidces, vertexIndex);
				ApplyVertexToBuffers(vertex2, vertexBuffer, indexBuffer, vertexPositions, vertexTxCoords, vertexNormals, storedinidces, vertexIndex);
				ApplyVertexToBuffers(vertex3, vertexBuffer, indexBuffer, vertexPositions, vertexTxCoords, vertexNormals, storedinidces, vertexIndex);
			}
		}

		return BlenderModel{ vertexBuffer, indexBuffer };
	}

	void BlenderModelLoader::ApplyVertexToBuffers(
		const DirectX::XMINT3& vertexIndexData,
		std::vector<Vertex3FP2FTx3FN>& vertexBuffer,
		std::vector<uint32_t>& indexBuffer,
		std::vector<DirectX::XMFLOAT3>& vertexPositions,
		std::vector<DirectX::XMFLOAT2>& vertexTxCoords,
		std::vector<DirectX::XMFLOAT3>& vertexNormals,
		std::vector<std::pair<uint32_t,
		DirectX::XMINT3>>& storedinidces,
		uint32_t& vertexIndex)
	{
		std::optional<uint32_t> searchResult = SearchVertexIndexBufferIndex(storedinidces, vertexIndexData);

		if (searchResult.has_value())
		{
			indexBuffer.push_back(searchResult.value());
		}
		else
		{
			const DirectX::XMFLOAT3& vertexPosition = vertexPositions[vertexIndexData.x];
			const DirectX::XMFLOAT2& vertexTxCoord = vertexTxCoords[vertexIndexData.y];
			const DirectX::XMFLOAT3& vertexNormal = vertexNormals[vertexIndexData.z];

			Vertex3FP2FTx3FN vertex{ vertexPosition, vertexTxCoord, vertexNormal };
			vertexBuffer.push_back(vertex);
			indexBuffer.push_back(vertexIndex);
			storedinidces.push_back(std::pair<uint32_t, DirectX::XMINT3>(vertexIndex, vertexIndexData));
			++vertexIndex;
		}
	}

	std::vector<std::string> BlenderModelLoader::SegmentLine(std::string line, const std::string& delimiter)
	{
		std::vector<std::string> tokens;
		size_t pos;

		while (true)
		{
			pos = line.find(delimiter);

			if (pos == line.npos)
			{
				tokens.push_back(line);
				break;
			}

			tokens.push_back(line.substr(0, pos));
			line.erase(0, pos + delimiter.length());
		}

		return tokens;
	}

	std::optional<uint32_t> BlenderModelLoader::SearchVertexIndexBufferIndex(const std::vector<std::pair<uint32_t, DirectX::XMINT3>>& vertexIndexBufferMap, const DirectX::XMINT3& vertexDataIndices)
	{
		//not the fastest solution. should be improved in the future
		const auto& it = std::find_if(vertexIndexBufferMap.begin(), vertexIndexBufferMap.end(), [vertexDataIndices](const std::pair<uint32_t, DirectX::XMINT3>& item) {
			return item.second.x == vertexDataIndices.x && item.second.y == vertexDataIndices.y && item.second.z == vertexDataIndices.z;
			});

		if (it == vertexIndexBufferMap.end())
			return std::nullopt;

		return it->first;
	}
}
