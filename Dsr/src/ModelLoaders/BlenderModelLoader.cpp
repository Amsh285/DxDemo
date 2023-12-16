#include "dsrpch.h"
#include "BlenderModelLoader.h"

namespace dsr
{
	std::variant<WavefrontModel, dsr_error> BlenderModelLoader::Load(
		const std::filesystem::path& path,
		const std::filesystem::path& materialPath)
	{
		std::variant<std::unordered_map<std::string, BlenderMaterialData>, dsr_error> loadMaterial = m_materialLoader.Load(materialPath);

		if (std::holds_alternative<dsr_error>(loadMaterial))
			return std::get<dsr_error>(loadMaterial);

		std::unordered_map<std::string, BlenderMaterialData> materialMap = std::get<std::unordered_map<std::string, BlenderMaterialData>>(loadMaterial);

		std::fstream modelFile;
		modelFile.open(path, std::ios::in);

		if (!modelFile.is_open())
		{
			std::string message = "error opening modelfile: ";
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

		//std::vector<std::pair<uint32_t, DirectX::XMINT3>> storedinidces;
		std::unordered_map<FaceVertex, uint32_t, FaceVertexHash> storedinidces;
		uint32_t vertexIndex = 0;

		std::vector<DirectX::XMFLOAT3> vertexPositions;
		std::vector<DirectX::XMFLOAT2> vertexTxCoords;
		std::vector<DirectX::XMFLOAT3> vertexNormals;

		std::vector<Vertex3FP2FTx3FN> vertexBuffer;
		std::vector<uint32_t> indexBuffer;

		uint32_t rowIndex = 3, startIndexLocation = 0;
		std::vector<WavefrontModelMaterialGroup> materialGroups;

		//cheers
		//https://stackoverflow.com/questions/16986017/how-do-i-make-blender-operate-in-left-hand
		//// Invert every Z coord of v entry:
		//z = -1.0f * input.z;

		//// Invert every Z coord of vn entry:
		//nz = -1.0f * input.nz;

		//// Invert every V coord of vt entry:
		//tv = 1.0f - input.tv;

		//...
		//// When assembling vertices:
		//// Revert face order from 0->1->2 to 2->1->0, depending on your implementation.

		while (std::getline(modelFile, line))
		{
			++rowIndex;
			std::vector<std::string> lineData = SegmentLine(line, " ");

			if (lineData[0] == "v")
			{
				//DirectX::XMFLOAT3 position(std::stof(lineData[1]), std::stof(lineData[2]), std::stof(lineData[3]));
				DirectX::XMFLOAT3 position(std::stof(lineData[1]), std::stof(lineData[2]), std::stof(lineData[3]) *-1.0f);
				vertexPositions.push_back(position);
			}
			else if (lineData[0] == "vn")
			{
				//DirectX::XMFLOAT3 normal(std::stof(lineData[1]), std::stof(lineData[2]), std::stof(lineData[3]));
				DirectX::XMFLOAT3 normal(std::stof(lineData[1]), std::stof(lineData[2]), std::stof(lineData[3]) *-1.0f);

				vertexNormals.push_back(normal);
			}
			else if (lineData[0] == "vt")
			{
				//DirectX::XMFLOAT2  txCoord(std::stof(lineData[1]), std::stof(lineData[2]));
				DirectX::XMFLOAT2  txCoord(std::stof(lineData[1]), 1.0f - std::stof(lineData[2]));
				vertexTxCoords.push_back(txCoord);
			}
			else if (lineData[0] == "f")
			{
				if (lineData.size() == 5)
				{
					//handle quadrilateral polygon
					std::vector<std::string> v0 = SegmentLine(lineData[1], "/");
					std::vector<std::string> v1 = SegmentLine(lineData[2], "/");
					std::vector<std::string> v2 = SegmentLine(lineData[3], "/");
					std::vector<std::string> v3 = SegmentLine(lineData[4], "/");

					FaceVertex vertex0 = { std::stoi(v0[0]) - 1, std::stoi(v0[1]) - 1, std::stoi(v0[2]) - 1 };
					FaceVertex vertex1 = { std::stoi(v1[0]) - 1, std::stoi(v1[1]) - 1, std::stoi(v1[2]) - 1 };
					FaceVertex vertex2 = { std::stoi(v2[0]) - 1, std::stoi(v2[1]) - 1, std::stoi(v2[2]) - 1 };
					FaceVertex vertex3 = { std::stoi(v3[0]) - 1, std::stoi(v3[1]) - 1, std::stoi(v3[2]) - 1 };

					//windingorder: clockwise
					/*ApplyVertexToBuffers(vertex0, vertexBuffer, indexBuffer, vertexPositions, vertexTxCoords, vertexNormals, storedinidces, vertexIndex);
					ApplyVertexToBuffers(vertex1, vertexBuffer, indexBuffer, vertexPositions, vertexTxCoords, vertexNormals, storedinidces, vertexIndex);
					ApplyVertexToBuffers(vertex2, vertexBuffer, indexBuffer, vertexPositions, vertexTxCoords, vertexNormals, storedinidces, vertexIndex);
					ApplyVertexToBuffers(vertex0, vertexBuffer, indexBuffer, vertexPositions, vertexTxCoords, vertexNormals, storedinidces, vertexIndex);
					ApplyVertexToBuffers(vertex2, vertexBuffer, indexBuffer, vertexPositions, vertexTxCoords, vertexNormals, storedinidces, vertexIndex);
					ApplyVertexToBuffers(vertex3, vertexBuffer, indexBuffer, vertexPositions, vertexTxCoords, vertexNormals, storedinidces, vertexIndex);*/

					//had to be changed to convert the blender rhs
					ApplyVertexToBuffers(vertex2, vertexBuffer, indexBuffer, vertexPositions, vertexTxCoords, vertexNormals, storedinidces, vertexIndex);
					ApplyVertexToBuffers(vertex1, vertexBuffer, indexBuffer, vertexPositions, vertexTxCoords, vertexNormals, storedinidces, vertexIndex);
					ApplyVertexToBuffers(vertex0, vertexBuffer, indexBuffer, vertexPositions, vertexTxCoords, vertexNormals, storedinidces, vertexIndex);
					ApplyVertexToBuffers(vertex3, vertexBuffer, indexBuffer, vertexPositions, vertexTxCoords, vertexNormals, storedinidces, vertexIndex);
					ApplyVertexToBuffers(vertex2, vertexBuffer, indexBuffer, vertexPositions, vertexTxCoords, vertexNormals, storedinidces, vertexIndex);
					ApplyVertexToBuffers(vertex0, vertexBuffer, indexBuffer, vertexPositions, vertexTxCoords, vertexNormals, storedinidces, vertexIndex);
				}
				else if (lineData.size() == 4)
				{
					//handle triangle
					std::vector<std::string> v0 = SegmentLine(lineData[1], "/");
					std::vector<std::string> v1 = SegmentLine(lineData[2], "/");
					std::vector<std::string> v2 = SegmentLine(lineData[3], "/");

					FaceVertex vertex0 = { std::stoi(v0[0]) - 1, std::stoi(v0[1]) - 1, std::stoi(v0[2]) - 1 };
					FaceVertex vertex1 = { std::stoi(v1[0]) - 1, std::stoi(v1[1]) - 1, std::stoi(v1[2]) - 1 };
					FaceVertex vertex2 = { std::stoi(v2[0]) - 1, std::stoi(v2[1]) - 1, std::stoi(v2[2]) - 1 };

					/*ApplyVertexToBuffers(vertex0, vertexBuffer, indexBuffer, vertexPositions, vertexTxCoords, vertexNormals, storedinidces, vertexIndex);
					ApplyVertexToBuffers(vertex1, vertexBuffer, indexBuffer, vertexPositions, vertexTxCoords, vertexNormals, storedinidces, vertexIndex);
					ApplyVertexToBuffers(vertex2, vertexBuffer, indexBuffer, vertexPositions, vertexTxCoords, vertexNormals, storedinidces, vertexIndex);*/

					ApplyVertexToBuffers(vertex2, vertexBuffer, indexBuffer, vertexPositions, vertexTxCoords, vertexNormals, storedinidces, vertexIndex);
					ApplyVertexToBuffers(vertex1, vertexBuffer, indexBuffer, vertexPositions, vertexTxCoords, vertexNormals, storedinidces, vertexIndex);
					ApplyVertexToBuffers(vertex0, vertexBuffer, indexBuffer, vertexPositions, vertexTxCoords, vertexNormals, storedinidces, vertexIndex);
				}
				else
				{
					std::string errorMessage = "Handle Face: unexpected number of lineData elements: ";
					errorMessage += lineData.size();
					return dsr_error(errorMessage, ERROR_PARSEMODELFILE_INVALIDFACEFORMAT);
				}
			}
			else if (lineData[0] == "usemtl")
			{
				if (lineData.size() != 2)
				{
					std::string errorMessage = "Handle UseMaterial: unexpected number of lineData elements: ";
					errorMessage += lineData.size();
					errorMessage += ". RowIndex: " + rowIndex;
					return dsr_error(errorMessage, ERROR_PARSEMODELFILE_INVALIDUSEMATERIALINSTRUCTIONFORMAT);
				}

				WavefrontModelMaterialGroup materialGroup;
				materialGroup.MaterialData = materialMap[lineData[1]];
				materialGroup.MaterialName = lineData[1];

				if (materialGroups.empty())
				{
					materialGroup.StartIndexLocation = 0;
					materialGroup.IndexCount = 0;
				}
				else
				{
					WavefrontModelMaterialGroup& lastGroup = materialGroups.back();
					lastGroup.IndexCount = indexBuffer.size() - lastGroup.StartIndexLocation;

					materialGroup.StartIndexLocation = lastGroup.StartIndexLocation + lastGroup.IndexCount;
					materialGroup.IndexCount = 0;
				}

				materialGroups.push_back(materialGroup);
			}
		}

		if (materialGroups.empty())
		{
			WavefrontModelMaterialGroup materialGroup;
			materialGroup.StartIndexLocation = 0;
			materialGroup.IndexCount = indexBuffer.size();
			materialGroups.push_back(materialGroup);
		}
		else
		{
			WavefrontModelMaterialGroup& lastGroup = materialGroups.back();
			lastGroup.IndexCount = indexBuffer.size() - lastGroup.StartIndexLocation;
		}

		return WavefrontModel{ vertexBuffer, indexBuffer, materialGroups };
	}

	void BlenderModelLoader::ApplyVertexToBuffers(
		const FaceVertex& vertexIndexData,
		std::vector<Vertex3FP2FTx3FN>& vertexBuffer,
		std::vector<uint32_t>& indexBuffer,
		std::vector<DirectX::XMFLOAT3>& vertexPositions,
		std::vector<DirectX::XMFLOAT2>& vertexTxCoords,
		std::vector<DirectX::XMFLOAT3>& vertexNormals,
		std::unordered_map<FaceVertex, uint32_t, FaceVertexHash>& storedinidces,
		uint32_t& vertexIndex)
	{
		std::optional<uint32_t> searchResult = SearchVertexIndexBufferIndex(storedinidces, vertexIndexData);

		if (searchResult.has_value())
		{
			indexBuffer.push_back(searchResult.value());
		}
		else
		{
			const DirectX::XMFLOAT3& vertexPosition = vertexPositions[vertexIndexData.VertexIndex];
			const DirectX::XMFLOAT2& vertexTxCoord = vertexTxCoords[vertexIndexData.TxCoordIndex];
			const DirectX::XMFLOAT3& vertexNormal = vertexNormals[vertexIndexData.NormalIndex];

			Vertex3FP2FTx3FN vertex{ vertexPosition, vertexTxCoord, vertexNormal };
			vertexBuffer.push_back(vertex);
			indexBuffer.push_back(vertexIndex);
			storedinidces[vertexIndexData] = vertexIndex;
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

	std::optional<uint32_t> BlenderModelLoader::SearchVertexIndexBufferIndex(const std::unordered_map<FaceVertex, uint32_t, FaceVertexHash>& vertexIndexBufferMap, const FaceVertex& vertexDataIndices) const
	{
		auto it = vertexIndexBufferMap.find(vertexDataIndices);

		if (it == vertexIndexBufferMap.end())
			return std::nullopt;

		return it->second;
	}
}
