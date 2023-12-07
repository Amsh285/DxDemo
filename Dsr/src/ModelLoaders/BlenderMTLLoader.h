#pragma once

#include "ErrorHandling/dsr_error.h"

namespace dsr
{
	constexpr auto ERROR_OPENMATERIALFILE = 102;
	constexpr auto ERROR_OPENMATERIALFILEINVALIDFORMAT = 103;

	constexpr auto ERROR_PARSEMATERIALFILE_INVALID_MATERIALNAMEFORMAT = 1001;
	constexpr auto ERROR_PARSEMATERIALFILE_INVALID_SPECULAREXPONENTFORMAT = 1002;
	constexpr auto ERROR_PARSEMATERIALFILE_INVALID_AMBIENTCOLORFORMAT = 1003;
	constexpr auto ERROR_PARSEMATERIALFILE_INVALID_DIFFUSECOLORFORMAT = 1004;
	constexpr auto ERROR_PARSEMATERIALFILE_INVALID_EMISSIVECOLORFORMAT = 1005;
	constexpr auto ERROR_PARSEMATERIALFILE_INVALID_SPECULARCOLORFORMAT = 1006;
	constexpr auto ERROR_PARSEMATERIALFILE_INVALID_OPTICALDENSITYFORMAT = 1007;
	constexpr auto ERROR_PARSEMATERIALFILE_INVALID_DISSOLVEFACTORFORMAT = 1008;
	constexpr auto ERROR_PARSEMATERIALFILE_INVALID_ILLUMINATIONMODELFORMAT = 1009;
	constexpr auto ERROR_PARSEMATERIALFILE_INVALID_MAPSPECULARFORMAT = 1010;
	constexpr auto ERROR_PARSEMATERIALFILE_INVALID_MAPALPHAFORMAT = 1011;
	constexpr auto ERROR_PARSEMATERIALFILE_INVALID_MAPDIFFUSEFORMAT = 1012;
	constexpr auto ERROR_PARSEMATERIALFILE_INVALID_MAPBUMPFORMAT = 1013;

	constexpr auto ERRO_STOREMATERIAL_MATERIALNAME_MISSING = 2001;

	struct BlenderMaterialData
	{
		float SpecularExponent; // Ns
		DirectX::XMFLOAT3 AmbientColor; //Ka
		DirectX::XMFLOAT4 DiffuseColor; //Kd
		DirectX::XMFLOAT3 EmissiveColor; //Ke
		DirectX::XMFLOAT3 SpecularColor; // Ks
		float OpticalDensity; //Ni
		float DissolveFactor; // d
		uint32_t IlluminationModel; //illum

		std::string MapSpecular; //map_Ks 
		std::string MapAlpha; //map_d
		std::string MapBump; // map_Bump
		float BumpMultiplier; // -bm same line as map_Bump
		std::string MapDiffuse; // map_Kd
	};

	class BlenderMTLLoader
	{
	public:
		std::variant<std::unordered_map<std::string, BlenderMaterialData>, dsr_error> Load(const std::filesystem::path& path);
	private:
		std::vector<std::string> SegmentLine(std::string line, const std::string& delimiter);
	};
}

