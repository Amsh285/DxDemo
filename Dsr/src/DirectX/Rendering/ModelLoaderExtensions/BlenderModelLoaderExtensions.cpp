#include "dsrpch.h"
#include "BlenderModelLoaderExtensions.h"

namespace dsr
{
	namespace directX
	{
		namespace rendering
		{
			std::variant<GroupedVertexBuffer, dsr_error> LoadWavefrontModel(
				std::shared_ptr<Direct3dDevice> device,
				std::shared_ptr<BlenderModelLoader> modelLoader,
				const std::filesystem::path& baseDirectory,
				const std::filesystem::path& modelPath,
				const std::filesystem::path& materialPath)
			{
				using namespace dsr;
				using namespace dsr::directX;

				//std::variant<BlenderModel, dsr_error> loadModel = m_blenderModelLoader->Load(L"Assets/Map.obj");
				std::variant<BlenderModel, dsr_error> loadModel = modelLoader->Load(
					baseDirectory / modelPath,
					baseDirectory / materialPath);

				if (std::holds_alternative<dsr_error>(loadModel))
					return dsr::dsr_error::Attach("Error loading blendermodel: ", std::get<dsr_error>(loadModel));

				Direct3dShaderInputLayout inputLayout;
				inputLayout.AddVector3f("POSITION");
				inputLayout.AddVector2f("TXCOORD");
				inputLayout.AddVector3f("NORMAL");

				const BlenderModel& model = std::get<BlenderModel>(loadModel);
				std::vector<float> vertexBuffer;

				for (const Vertex3FP2FTx3FN& vertex : model.VertexBuffer)
				{
					vertexBuffer.push_back(vertex.Position.x);
					vertexBuffer.push_back(vertex.Position.y);
					vertexBuffer.push_back(vertex.Position.z);

					vertexBuffer.push_back(vertex.texCoords.x);
					vertexBuffer.push_back(vertex.texCoords.y);

					vertexBuffer.push_back(vertex.Normal.x);
					vertexBuffer.push_back(vertex.Normal.y);
					vertexBuffer.push_back(vertex.Normal.z);
				}

				std::vector<VertexGroup> vertexGroups = MapModel(device, baseDirectory, model);

				std::variant<Direct3dVertexBufferf, dsr_error> loadVertexData = SetupVertexBufferf(device, vertexBuffer, model.IndexBuffer, inputLayout);
				if (std::holds_alternative<dsr_error>(loadVertexData))
					return dsr_error::Attach("Error setup vertexbuffer: ", std::get<dsr_error>(loadVertexData));

				return GroupedVertexBuffer{ std::get<Direct3dVertexBufferf>(loadVertexData), vertexGroups };
			}

			std::vector<VertexGroup> MapModel(
				std::shared_ptr<Direct3dDevice> device,
				const std::filesystem::path& baseDirectory,
				const BlenderModel& model)
			{
				std::vector<VertexGroup> vertexGroups;

				for (const auto& item : model.MaterialGroups)
				{
					rendering::VertexGroup group;
					group.StartIndexLocation = item.StartIndexLocation;
					group.IndexCount = item.IndexCount;
					group.MaterialName = item.MaterialName;

					PixelShaderData data;
					data.SpecularExponent = item.MaterialData.SpecularExponent;
					data.AmbientColor = DirectX::XMFLOAT4(item.MaterialData.AmbientColor.x, item.MaterialData.AmbientColor.y, item.MaterialData.AmbientColor.z, 1.0f);
					data.DiffuseColor = DirectX::XMFLOAT4(item.MaterialData.DiffuseColor.x, item.MaterialData.DiffuseColor.y, item.MaterialData.DiffuseColor.z, 1.0f);
					data.EmissiveColor = DirectX::XMFLOAT4(item.MaterialData.EmissiveColor.x, item.MaterialData.EmissiveColor.y, item.MaterialData.EmissiveColor.z, 1.0f);
					data.SpecularColor = DirectX::XMFLOAT4(item.MaterialData.SpecularColor.x, item.MaterialData.SpecularColor.y, item.MaterialData.SpecularColor.z, 1.0f);
					data.OpticalDensity =  item.MaterialData.OpticalDensity;
					data.IlluminationModel = item.MaterialData.IlluminationModel;

					if (item.MaterialData.MapDiffuse.empty())
						group.DiffuseMap = std::nullopt;
					else
					{
						std::variant<Direct3dShaderTexture2D, dsr_error> loadTextureResult = Direct3dShaderTexture2D::LoadSingleRGBA(device, baseDirectory.string() + item.MaterialData.MapDiffuse);

						if (std::holds_alternative<dsr_error>(loadTextureResult))
						{
							dsr_error& error = std::get<dsr_error>(loadTextureResult);

							//Todo: improve handling
							std::cout << "error loading Texture: " << item.MaterialData.MapDiffuse << ". " << error.what() << std::endl;
							group.DiffuseMap = std::nullopt;
						}
						else
						{
							data.UseDiffuseMap = 1;
							group.DiffuseMap = std::get<Direct3dShaderTexture2D>(loadTextureResult);
						}
					}

					group.PSData = data;
					vertexGroups.push_back(group);
				}

				return vertexGroups;
			}
		}
	}
}
