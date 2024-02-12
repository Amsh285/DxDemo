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
				Direct3dShaderInputLayout inputLayout;
				inputLayout.AddVector3f("POSITION");
				inputLayout.AddVector2f("TXCOORD");
				inputLayout.AddVector3f("NORMAL");

				std::vector<float> vertexBuffer;

				for (const Vertex3FP2FTx3FN& vertex : model->VertexBuffer)
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

				std::unordered_map<std::string, std::shared_ptr<VertexGroup>> namedVertexGroups = MapModel(model);

				std::variant<Direct3dVertexBufferf, dsr_error> loadVertexData = SetupVertexBufferf(device, vertexBuffer, model->IndexBuffer, inputLayout);
				if (std::holds_alternative<dsr_error>(loadVertexData))
					return dsr_error::Attach("Error setup vertexbuffer: ", std::get<dsr_error>(loadVertexData));

				return ModelConfiguration(dsr::data::Transform(), std::get<Direct3dVertexBufferf>(loadVertexData), namedVertexGroups);
			}

			std::vector<Face> GetFaceData(const std::shared_ptr<WavefrontModel> model)
			{
				using namespace DirectX;

				if (model->IndexBuffer.size() % 3 != 0)
				{
					// Todo Log warn xD
					return std::vector<Face>();
				}

				std::vector<Face> faces;

				for (std::size_t i = 0; i < model->IndexBuffer.size(); i += 3)
				{
					const Vertex3FP2FTx3FN& v1 = model->VertexBuffer[model->IndexBuffer[i]];
					const Vertex3FP2FTx3FN& v2 = model->VertexBuffer[model->IndexBuffer[i + 1]];
					const Vertex3FP2FTx3FN& v3 = model->VertexBuffer[model->IndexBuffer[i + 2]];

					XMVECTOR u = XMVectorSubtract(XMLoadFloat3(&v3.Position), XMLoadFloat3(&v2.Position));
					XMVECTOR v = XMVectorSubtract(XMLoadFloat3(&v1.Position), XMLoadFloat3(&v2.Position));

					Face f;
					XMStoreFloat3(&f.Normal, XMVector3Normalize(XMVector3Cross(u, v)));

					f.Centroid = XMFLOAT3(
						(v1.Position.x + v2.Position.x + v3.Position.x) / 3.0f,
						(v1.Position.y + v2.Position.y + v3.Position.y) / 3.0f,
						(v1.Position.z + v2.Position.z + v3.Position.z) / 3.0f
					);

					faces.push_back(f);
				}

				return faces;
			}

			std::shared_ptr<dsr::WavefrontModel> FilterUpperSurface(const std::shared_ptr<dsr::WavefrontModel> targetMesh, const float& thresholdAngle, const DirectX::XMVECTOR& upvector)
			{
				using namespace DirectX;

				if (targetMesh->IndexBuffer.size() % 3 != 0)
				{
					// log
					return std::make_shared<dsr::WavefrontModel>();
				}

				std::vector<Vertex3FP2FTx3FN> vertexBuffer;
				std::vector<uint32_t> indexBuffer;

				uint32_t index = 0;

				for (int i = 0; i < targetMesh->IndexBuffer.size(); i += 3)
				{
					const Vertex3FP2FTx3FN& v1 = targetMesh->VertexBuffer[targetMesh->IndexBuffer[i]];
					const Vertex3FP2FTx3FN& v2 = targetMesh->VertexBuffer[targetMesh->IndexBuffer[i + 1]];
					const Vertex3FP2FTx3FN& v3 = targetMesh->VertexBuffer[targetMesh->IndexBuffer[i + 2]];

					XMVECTOR u = XMVectorSubtract(XMLoadFloat3(&v3.Position), XMLoadFloat3(&v2.Position));
					XMVECTOR v = XMVectorSubtract(XMLoadFloat3(&v1.Position), XMLoadFloat3(&v2.Position));

					XMVECTOR faceNormal = XMVector3Normalize(XMVector3Cross(u, v));

					float angleRadiants = std::atan2(XMVectorGetX(XMVector3Length(XMVector3Cross(faceNormal, upvector))), XMVectorGetX(XMVector3Dot(upvector, faceNormal)));
					float angle = XMConvertToDegrees(angleRadiants);

					if (angle <= thresholdAngle)
					{
						vertexBuffer.push_back(v1);
						vertexBuffer.push_back(v2);
						vertexBuffer.push_back(v3);

						/*vertex2
							vertex1
							vertex0
							vertex3
							vertex2
							vertex0*/

						indexBuffer.push_back(index + 2);
						indexBuffer.push_back(index + 1);
						indexBuffer.push_back(index);

						index += 3;
					}
				}

				std::shared_ptr<WavefrontModel> model = std::make_shared<WavefrontModel>();
				model->VertexBuffer = vertexBuffer;
				model->IndexBuffer = indexBuffer;
				return model;
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
