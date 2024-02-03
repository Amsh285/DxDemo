#include "dsrpch.h"
#include "STBIImage.h"

// Das ist mit ABSTAND die dümmste zurückgebliebenste SCHEISSE die ich in meinem ganzen Leben je gesehen habe.
// Die Leute die so ne scheisse verbrechen gehören gehängt!
// Wenn ich jemals die specs für die image files finde Programmier ich die scheisse Lieber selber bevor ich diesen SCHEISSDRECK nochmal verwende.
#define STB_IMAGE_IMPLEMENTATION
#include "Vendor/stb/stb_image.h"

namespace dsr
{
	namespace vendor
	{
		STBIImage::STBIImage(const std::filesystem::path& filePath, const int& req_comp)
			: m_width(0), m_height(0), m_channels(0), m_imageData(nullptr)
		{
			m_imageData.reset(stbi_load(filePath.generic_string().c_str(), &m_width, &m_height, &m_channels, req_comp));
		}

		STBIImage::operator bool() const
		{
			return m_imageData != nullptr;
		}
	}
}
