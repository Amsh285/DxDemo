#pragma once

namespace dsr
{
	namespace vendor
	{
		class STBIImage
		{
		public:
			int GetWidth() const { return m_width; }
			int GetHeight() const { return m_height; }
			int GetChannels() const { return m_channels; }
			int GetImageSize() const { return m_width * m_height * m_channels; }
			unsigned char* GetImagePtr() const { return m_imageData.get(); }

			explicit STBIImage(const std::filesystem::path& filePath, const int& req_comp = STBI_rgb_alpha);
			STBIImage(const STBIImage& other) = delete;
			STBIImage& operator=(const STBIImage& other) = delete;
			~STBIImage() = default;

			explicit operator bool() const;
		private:
			struct STBIFreeDeleter
			{
				void operator()(unsigned char* p) const { stbi_image_free(p); }
			};

			int m_width, m_height, m_channels;
			std::unique_ptr<unsigned char, STBIFreeDeleter> m_imageData;
		};
	}
}
