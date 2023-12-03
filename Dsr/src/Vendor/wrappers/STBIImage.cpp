#include "dsrpch.h"
#include "STBIImage.h"

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
