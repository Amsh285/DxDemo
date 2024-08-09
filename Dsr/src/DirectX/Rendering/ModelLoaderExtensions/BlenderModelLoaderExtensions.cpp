#include "dsrpch.h"
#include "BlenderModelLoaderExtensions.h"

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
				const std::filesystem::path& materialPath)
			{
				return modelLoader->Load(baseDirectory / modelPath, baseDirectory / materialPath);
			}

			std::variant<ModelConfiguration, dsr_error> LoadWavefrontModelConfiguration(
				std::shared_ptr<Direct3dDevice> device,
				std::shared_ptr<BlenderModelLoader> modelLoader,
				const std::filesystem::path& baseDirectory,
				const std::filesystem::path& modelPath,
				const std::filesystem::path& materialPath)
			{
				using namespace dsr;
				using namespace dsr::directX;

				std::variant<std::shared_ptr<WavefrontModel>, dsr_error> loadModel = LoadWavefrontModel(
					modelLoader,
					baseDirectory,
					modelPath,
					materialPath
				);

				if (std::holds_alternative<dsr_error>(loadModel))
					return dsr::dsr_error::Attach("Error loading blendermodel: ", std::get<dsr_error>(loadModel));

				std::shared_ptr<WavefrontModel> model = std::get<std::shared_ptr<WavefrontModel>>(loadModel);
				return LoadWavefrontModelConfiguration(device, model);
			}

			std::variant<ModelConfiguration, dsr_error> LoadWavefrontModelConfiguration(
				std::shared_ptr<Direct3dDevice> device,
				const std::shared_ptr<WavefrontModel> model)
			{
				using namespace dsr::data;

				Direct3dShaderInputLayout inputLayout;
				inputLayout.AddVector3f("POSITION");
				inputLayout.AddVector2f("TXCOORD");
				inputLayout.AddVector3f("NORMAL");

				const std::vector<Vertex3FP2FTx3FN>& srcVertexBuffer = model->Mesh->GetVertexBuffer();
				std::vector<float> vertexBuffer;

				for (const dsr::data::Vertex3FP2FTx3FN& vertex : srcVertexBuffer)
				{
					vertexBuffer.push_back(DirectX::XMVectorGetX(vertex.Position));
					vertexBuffer.push_back(DirectX::XMVectorGetY(vertex.Position));
					vertexBuffer.push_back(DirectX::XMVectorGetZ(vertex.Position));

					vertexBuffer.push_back(vertex.texCoords.x);
					vertexBuffer.push_back(vertex.texCoords.y);

					vertexBuffer.push_back(vertex.Normal.x);
					vertexBuffer.push_back(vertex.Normal.y);
					vertexBuffer.push_back(vertex.Normal.z);
				}

				std::unordered_map<std::string, std::shared_ptr<VertexGroup>> namedVertexGroups = MapModel(model);

				std::variant<Direct3dVertexBufferf, dsr_error> loadVertexData = SetupVertexBufferf(device, vertexBuffer, model->Mesh->GetIndexBuffer(), inputLayout);
				if (std::holds_alternative<dsr_error>(loadVertexData))
					return dsr_error::Attach("Error setup vertexbuffer: ", std::get<dsr_error>(loadVertexData));

				return ModelConfiguration(dsr::data::Transform(), std::get<Direct3dVertexBufferf>(loadVertexData), namedVertexGroups);
			}

			std::vector<dsr::data::Face> GetFaceData(const std::shared_ptr<WavefrontModel> model)
			{
				constexpr float denom = 1.0f / 3.0f;

				using namespace dsr::data;

				using namespace DirectX;

				const std::vector<Vertex3FP2FTx3FN>& vertexBuffer = model->Mesh->GetVertexBuffer();
				const std::vector<uint32_t>& indexBuffer = model->Mesh->GetIndexBuffer();

				if (indexBuffer.size() % 3 != 0)
				{
					// Todo Log warn xD
					return std::vector<Face>();
				}

				std::vector<Face> faces;

				for (std::size_t i = 0; i < indexBuffer.size(); i += 3)
				{
					
					const Vertex3FP2FTx3FN& v1 = vertexBuffer[indexBuffer[i]];
					const Vertex3FP2FTx3FN& v2 = vertexBuffer[indexBuffer[i + 1]];
					const Vertex3FP2FTx3FN& v3 = vertexBuffer[indexBuffer[i + 2]];

					XMVECTOR u = XMVectorSubtract(v3.Position, v2.Position);
					XMVECTOR v = XMVectorSubtract(v1.Position, v2.Position);

					Face f;
					XMStoreFloat3(&f.Normal, XMVector3Normalize(XMVector3Cross(u, v)));

					XMVECTOR cent = XMVectorScale(XMVectorAdd(XMVectorAdd(v1.Position, v2.Position), v3.Position), denom);
					XMStoreFloat3(&f.Centroid, cent);

					faces.push_back(f);
				}

				return faces;
			}

			std::unordered_map<std::string, std::shared_ptr<VertexGroup>> MapModel(const std::shared_ptr<WavefrontModel> model)
			{
				std::unordered_map<std::string, std::shared_ptr<VertexGroup>> namedVertexGroups;
				uint32_t sortOrder = 0;

				for (const auto& item : model->MaterialGroups)
				{
					std::shared_ptr<VertexGroup> group = std::make_shared<VertexGroup>();
					group->StartIndexLocation = item.StartIndexLocation;
					group->IndexCount = item.IndexCount;
					group->MaterialName = item.MaterialName;

					PixelShaderData data;
					data.SpecularExponent = item.MaterialData.SpecularExponent;
					data.AmbientColor = DirectX::XMFLOAT4(item.MaterialData.AmbientColor.x, item.MaterialData.AmbientColor.y, item.MaterialData.AmbientColor.z, 1.0f);
					data.DiffuseColor = DirectX::XMFLOAT4(item.MaterialData.DiffuseColor.x, item.MaterialData.DiffuseColor.y, item.MaterialData.DiffuseColor.z, 1.0f);
					data.EmissiveColor = DirectX::XMFLOAT4(item.MaterialData.EmissiveColor.x, item.MaterialData.EmissiveColor.y, item.MaterialData.EmissiveColor.z, 1.0f);
					data.SpecularColor = DirectX::XMFLOAT4(item.MaterialData.SpecularColor.x, item.MaterialData.SpecularColor.y, item.MaterialData.SpecularColor.z, 1.0f);
					data.OpticalDensity = item.MaterialData.OpticalDensity;
					data.IlluminationModel = item.MaterialData.IlluminationModel;
					group->PSData = data;

					namedVertexGroups[group->MaterialName] = group;
				}

				return namedVertexGroups;
			}
		}
	}
}
