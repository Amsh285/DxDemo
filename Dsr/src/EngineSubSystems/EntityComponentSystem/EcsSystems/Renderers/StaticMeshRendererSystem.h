#pragma once

#include "DirectX/Direct3dDevice.h"
#include "DirectX/Direct3dDeviceShaderExtensions.h"
#include "DirectX/Shader/Data/VertexShaderData.h"
#include "DirectX/Textures/Direct3dSamplerState.h"

#include "ErrorHandling/DsrResult.h"

#include "EngineSubSystems/EntityComponentSystem/System.h"
#include "EngineSubSystems/EntityComponentSystem/Components/ShaderProgramComponent.h"
#include "EngineSubSystems/EntityComponentSystem/Components/StaticMeshComponent.h"
#include "EngineSubSystems/EntityComponentSystem/Components/TransformComponent.h"
#include "EngineSubSystems/EntityComponentSystem/EcsSystems/Renderers/RendererSystem.h"

#include "Events/Application/WindowEvents.h"
#include "Events/Application/FrameEvents.h"

namespace dsr
{
	namespace ecs
	{
		class StaticMeshRendererSystem : public RendererSystem
		{
		public:
			virtual std::vector<std::type_index> GetRequiredComponents() const override;

			StaticMeshRendererSystem(const std::shared_ptr<directX::Direct3dDevice>& device);
			StaticMeshRendererSystem(const StaticMeshRendererSystem& other) = delete;
			StaticMeshRendererSystem& operator=(const StaticMeshRendererSystem& other) = delete;

			DsrResult Initialize();
			void SetDefaultSamplerState();

			void PrepareRendererUpdate();
			void Update(const EngineContext& context);
		private:
			template<class TShader>
			void ApplyConstantBuffers();

			template<>
			void ApplyConstantBuffers<ID3D11VertexShader>()
			{
				std::vector<ID3D11Buffer*> vsConstantBuffers;
				for (auto& pair : m_vsConstantBuffers)
					vsConstantBuffers.push_back(pair.second.GetBufferPtr().get());

				m_device->UseConstantBuffers<ID3D11VertexShader>(0, vsConstantBuffers.size(), vsConstantBuffers.data());
			}

			template<>
			void ApplyConstantBuffers<ID3D11PixelShader>()
			{
				std::vector<ID3D11Buffer*> psConstantBuffers;
				for (auto& pair : m_psConstantBuffers)
					psConstantBuffers.push_back(pair.second.GetBufferPtr().get());

				m_device->UseConstantBuffers<ID3D11PixelShader>(0, psConstantBuffers.size(), psConstantBuffers.data());
			}

			void SetupMvp(const EngineContext& context, const Entity& camera, const RenderTransform& renderTransform);
			void SetupTextures(std::shared_ptr<dsr::directX::rendering::VertexGroup> vertexGroup);

			directX::Direct3dSamplerState m_defaultSamplerState;

			std::shared_ptr<directX::Direct3dDevice> m_device;

			std::map<size_t, directX::Direct3dBuffer> m_vsConstantBuffers;
			std::map<size_t, directX::Direct3dBuffer> m_psConstantBuffers;
		};
	}
}
