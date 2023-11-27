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

		while (std::getline(modelFile, line))
		{
			std::vector<std::string> lineData = SegmentLine(line, " ");
		
			if (lineData[0] == "v")
			{
				//VertexPosition
			}
		}

		return BlenderModel();
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
}