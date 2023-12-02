#include "dsrpch.h"
#include "BlenderMTLLoader.h"

namespace dsr
{
	std::variant<std::unordered_map<std::string, BlenderMaterialData>, dsr_error> BlenderMTLLoader::Load(const std::filesystem::path& path)
	{
		std::unordered_map<std::string, BlenderMaterialData> materialDataMap;

		std::fstream materialDataFile;
		materialDataFile.open(path, std::ios::in);

		if (!materialDataFile.is_open())
		{
			std::string message = "error opening materialFile: ";
			message += path.generic_string();
			message += ". File could not be opened.";
			return dsr_error(message, ERROR_OPENMATERIALFILE);
		}

		std::string line;
		std::getline(materialDataFile, line);
		std::vector<std::string> versionSegments = SegmentLine(line, " ");

		if (versionSegments[1] != "Blender")
		{
			std::string message = "error loading materialFile: ";
			message += path.generic_string();
			message += ". Invalid File Format.";
			return dsr_error(message, ERROR_OPENMATERIALFILEINVALIDFORMAT);
		}

		std::getline(materialDataFile, line);
		std::getline(materialDataFile, line);

		std::string materialName;
		BlenderMaterialData currentMaterial;
		memset(&currentMaterial, 0x00, sizeof(BlenderMaterialData));

		uint32_t rowIndex = 2;

		while (std::getline(materialDataFile, line))
		{
			++rowIndex;

			if (!line.empty())
			{
				std::vector<std::string> lineData = SegmentLine(line, " ");

				if (lineData[0] == "newmtl")
				{
					if (lineData.size() != 2)
					{
						std::string errorMessage = "Handle MaterialName: unexpected number of lineData elements: ";
						errorMessage += lineData.size();
						errorMessage += ". RowIndex: " + rowIndex;
						return dsr_error(errorMessage, ERROR_PARSEMATERIALFILE_INVALID_MATERIALNAMEFORMAT);
					}

					materialName = lineData[1];
				}
				else if (lineData[0] == "Ns")
				{
					if (lineData.size() != 2)
					{
						std::string errorMessage = "Handle SpecularExponent: unexpected number of lineData elements: ";
						errorMessage += lineData.size();
						errorMessage += ". RowIndex: " + rowIndex;
						return dsr_error(errorMessage, ERROR_PARSEMATERIALFILE_INVALID_SPECULAREXPONENTFORMAT);
					}

					currentMaterial.SpecularExponent = std::stof(lineData[1]);
				}
				else if (lineData[0] == "Ka")
				{
					if (lineData.size() != 4)
					{
						std::string errorMessage = "Handle AmbientColor: unexpected number of lineData elements: ";
						errorMessage += lineData.size();
						errorMessage += ". RowIndex: " + rowIndex;
						return dsr_error(errorMessage, ERROR_PARSEMATERIALFILE_INVALID_AMBIENTCOLORFORMAT);
					}

					currentMaterial.AmbientColor = DirectX::XMFLOAT3(std::stof(lineData[1]), std::stof(lineData[2]), std::stof(lineData[3]));
				}
				else if (lineData[0] == "Kd")
				{
					if (lineData.size() != 4)
					{
						std::string errorMessage = "Handle DiffuseColor: unexpected number of lineData elements: ";
						errorMessage += lineData.size();
						errorMessage += ". RowIndex: " + rowIndex;
						return dsr_error(errorMessage, ERROR_PARSEMATERIALFILE_INVALID_DIFFUSECOLORFORMAT);
					}

					currentMaterial.DiffuseColor = DirectX::XMFLOAT3(std::stof(lineData[1]), std::stof(lineData[2]), std::stof(lineData[3]));
				}
				else if (lineData[0] == "Ke")
				{
					if (lineData.size() != 4)
					{
						std::string errorMessage = "Handle EmissiveColor: unexpected number of lineData elements: ";
						errorMessage += lineData.size();
						errorMessage += ". RowIndex: " + rowIndex;
						return dsr_error(errorMessage, ERROR_PARSEMATERIALFILE_INVALID_EMISSIVECOLORFORMAT);
					}

					currentMaterial.EmissiveColor = DirectX::XMFLOAT3(std::stof(lineData[1]), std::stof(lineData[2]), std::stof(lineData[3]));
				}
				else if (lineData[0] == "Ni")
				{
					if (lineData.size() != 2)
					{
						std::string errorMessage = "Handle OpticalDensity: unexpected number of lineData elements: ";
						errorMessage += lineData.size();
						errorMessage += ". RowIndex: " + rowIndex;
						return dsr_error(errorMessage, ERROR_PARSEMATERIALFILE_INVALID_OPTICALDENSITYFORMAT);
					}

					currentMaterial.OpticalDensity = std::stof(lineData[1]);
				}
				else if (lineData[0] == "illum")
				{
					if (lineData.size() != 2)
					{
						std::string errorMessage = "Handle IlluminationModel: unexpected number of lineData elements: ";
						errorMessage += lineData.size();
						errorMessage += ". RowIndex: " + rowIndex;
						return dsr_error(errorMessage, ERROR_PARSEMATERIALFILE_INVALID_ILLUMINATIONMODELFORMAT);
					}

					currentMaterial.IlluminationModel = std::stoul(lineData[1]);
				}
				else if (lineData[0] == "map_Ks")
				{
					if (lineData.size() != 2)
					{
						std::string errorMessage = "Handle MapSpecular: unexpected number of lineData elements: ";
						errorMessage += lineData.size();
						errorMessage += ". RowIndex: " + rowIndex;
						return dsr_error(errorMessage, ERROR_PARSEMATERIALFILE_INVALID_MAPSPECULARFORMAT);
					}

					currentMaterial.MapSpecular = lineData[1];
				}
				else if (lineData[0] == "map_d")
				{
					if (lineData.size() != 2)
					{
						std::string errorMessage = "Handle MapAlpha: unexpected number of lineData elements: ";
						errorMessage += lineData.size();
						errorMessage += ". RowIndex: " + rowIndex;
						return dsr_error(errorMessage, ERROR_PARSEMATERIALFILE_INVALID_MAPALPHAFORMAT);
					}

					currentMaterial.MapAlpha = lineData[1];
				}
				else if (lineData[0] == "map_Kd")
				{
					if (lineData.size() != 2)
					{
						std::string errorMessage = "Handle MapDiffuse: unexpected number of lineData elements: ";
						errorMessage += lineData.size();
						errorMessage += ". RowIndex: " + rowIndex;
						return dsr_error(errorMessage, ERROR_PARSEMATERIALFILE_INVALID_MAPDIFFUSEFORMAT);
					}

					currentMaterial.MapDiffuse = lineData[1];
				}
				else if (lineData[0] == "map_Bump")
				{
					if (lineData.size() != 4)
					{
						std::string errorMessage = "Handle MapBump: unexpected number of lineData elements: ";
						errorMessage += lineData.size();
						errorMessage += ". RowIndex: " + rowIndex;
						return dsr_error(errorMessage, ERROR_PARSEMATERIALFILE_INVALID_MAPBUMPFORMAT);
					}

					currentMaterial.BumpMultiplier = std::stof(lineData[2]);
					currentMaterial.MapBump = lineData[3];
				}
			}
			else
			{
				if (materialName.empty())
				{
					std::string errorMessage = "error mapping materialEntry: name has no value";
					errorMessage += ". RowIndex: " + rowIndex;
					return dsr_error(errorMessage, ERRO_STOREMATERIAL_MATERIALNAME_MISSING);
				}

				materialDataMap[materialName] = currentMaterial;
				memset(&currentMaterial, 0x00, sizeof(BlenderMaterialData));
			}
		}

		return materialDataMap;
	}

	std::vector<std::string> BlenderMTLLoader::SegmentLine(std::string line, const std::string& delimiter)
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
}

