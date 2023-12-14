#pragma once

#include "Data/Transform.h"

#include "DirectX/Direct3dDevice.h"
#include "DirectX/Direct3dDeviceBufferExtensions.h"
#include "DirectX/Rendering/Direct3dRenderUoW.h"

#include "ErrorHandling/dsr_error.h"
#include "ModelLoaders/BlenderModelLoader.h"

namespace dsr
{
	namespace directX
	{
		namespace rendering
		{
			struct GroupedVertexBuffer
			{
				dsr::data::Transform GlobalTransform;
				Direct3dVertexBufferf Vertexbuffer;
				std::unordered_map<std::string, std::shared_ptr<VertexGroup>> VertexGroups;
			};

			std::variant<GroupedVertexBuffer, dsr_error> LoadWavefrontModel(
				std::shared_ptr<Direct3dDevice> device,
				std::shared_ptr<BlenderModelLoader> modelLoader,
				const std::filesystem::path& baseDirectory,
				const std::filesystem::path& modelPath,
				const std::filesystem::path& materialPath);

			std::unordered_map<std::string, std::shared_ptr<VertexGroup>> MapModel(
				const std::filesystem::path& baseDirectory,
				const BlenderModel& model);
		}
	}
}
