#pragma once

#include "Data/Face.h"
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
			std::variant<std::shared_ptr<WavefrontModel>, dsr_error> LoadWavefrontModel(
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
				const std::shared_ptr<WavefrontModel> model
			);

			std::vector<Face> GetFaceData(const std::shared_ptr<WavefrontModel> model);

			std::shared_ptr<dsr::WavefrontModel> FilterUpperSurface(
				const std::shared_ptr<dsr::WavefrontModel> targetMesh,
				const float& thresholdAngle,
				const DirectX::XMVECTOR& upvector = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));

			std::unordered_map<std::string, std::shared_ptr<VertexGroup>> MapModel(const std::shared_ptr<WavefrontModel> model);
		}
	}
}
