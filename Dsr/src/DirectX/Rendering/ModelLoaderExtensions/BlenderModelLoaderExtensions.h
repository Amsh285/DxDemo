#pragma once

#include "Data/Transform.h"

#include "DirectX/Data/ModelConfiguration.h"
#include "DirectX/Direct3dDevice.h"
#include "DirectX/Direct3dDeviceBufferExtensions.h"

#include "ErrorHandling/dsr_error.h"
#include "ModelLoaders/BlenderModelLoader.h"

namespace dsr
{
	namespace directX
	{
		namespace rendering
		{
			std::variant<WavefrontModel, dsr_error> LoadWavefrontModel(
				std::shared_ptr<BlenderModelLoader> modelLoader,
				const std::filesystem::path& baseDirectory,
				const std::filesystem::path& modelPath,
				const std::filesystem::path& materialPath);

			std::variant<ModelConfiguration, dsr_error> LoadWavefrontModelConfiguration(
				std::shared_ptr<Direct3dDevice> device,
				std::shared_ptr<BlenderModelLoader> modelLoader,
				const std::filesystem::path& baseDirectory,
				const std::filesystem::path& modelPath,
				const std::filesystem::path& materialPath);

			std::variant<ModelConfiguration, dsr_error> LoadWavefrontModelConfiguration(
				std::shared_ptr<Direct3dDevice> device,
				const WavefrontModel& model
			);

			std::unordered_map<std::string, std::shared_ptr<VertexGroup>> MapModel(const WavefrontModel& model);
		}
	}
}
