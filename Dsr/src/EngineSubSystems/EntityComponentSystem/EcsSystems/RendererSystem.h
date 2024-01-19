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

#include "Events/Application/WindowEvents.h"

namespace dsr
{
	namespace ecs
	{
		class RendererSystem : public System
		{
		public:
			virtual std::vector<std::type_index> GetRequiredComponents() const override;

			RendererSystem(const std::shared_ptr<directX::Direct3dDevice>& device);
			RendererSystem(const RendererSystem& other) = delete;
			RendererSystem& operator=(const RendererSystem& other) = delete;

			DsrResult Initialize();
			void SetDefaultSamplerState();

			void PrepareUpdate(const events::PrepareUdateFrameEvent& args);
			void Update(const EngineContext& context);
			void UpdateFinished(const events::UpdateFrameFinishedEvent& args);
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
