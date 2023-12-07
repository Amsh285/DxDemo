#include "dsrpch.h"
#include "Direct3dRenderer.h"

namespace dsr
{
	namespace directX
	{
		namespace rendering
		{
			DsrResult Direct3dRenderer::Initialize()
			{
				std::variant<Direct3dSamplerState, dsr_error> createSamplerStateResult = Direct3dSamplerState::CreateDefault(m_device);
				if (std::holds_alternative<dsr_error>(createSamplerStateResult))
				{
					const dsr_error& error = std::get<dsr_error>(createSamplerStateResult);
					std::string errorMessage = "Error initializing Renderer: ";
					errorMessage += error.what();
					return DsrResult(errorMessage, error.GetResult());
				}

				m_defaultSamplerState = std::get<Direct3dSamplerState>(createSamplerStateResult);
				SetDefaultSamplerState();

				return DsrResult::Success("Initializing Renderer Successful.");
			}

			void Direct3dRenderer::SetDefaultSamplerState()
			{
				ID3D11SamplerState* state = m_defaultSamplerState.GetSamplerStatePtr().get();
				m_device->SetSamplers<ID3D11PixelShader>(0, 1, &state);
			}

			Direct3dRenderer::Direct3dRenderer(const std::shared_ptr<Direct3dDevice>& device)
				: m_device(device)
			{
				assert(device, "device cannot be null");
			}

			void Direct3dRenderer::AddUnitOfWork(const Direct3dRenderUoW& uow)
			{
				m_units.push_back(uow);
			}

			void Direct3dRenderer::OnUpdate(const dsr::events::UpdateFrameEvent& updateEvent)
			{
				using namespace DirectX;

				m_device->Clear(0.0f, 0.2f, 0.4f, 1.0f);
				m_device->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

				for (auto iteratorUoW = m_units.begin(); iteratorUoW < m_units.end(); ++iteratorUoW)
				{
					//Just to get something going now.. optimize that later
					//setup viewmatrix
					/*DirectX::XMVECTOR eyePosition = DirectX::XMVectorSet(0, 0, -10, 1);
					DirectX::XMVECTOR focusPoint = DirectX::XMVectorSet(0, 0, -9, 1);
					DirectX::XMVECTOR upDirection = DirectX::XMVectorSet(0, 1, 0, 0);
					DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtLH(eyePosition, focusPoint, upDirection);
					DsrResult setViewMatrixResult = SetConstantBuffer(m_device, iteratorUoW->Shaders.VertexShader, 1, viewMatrix);*/

					if (m_activeCamera)
					{
						XMMATRIX projectionMatrix = m_activeCamera->GetProjectionMatrix();
						XMMATRIX viewMatrix = m_activeCamera->GetViewMatrix();

						DsrResult setProjectionMatrixResult = SetConstantBuffer(m_device, iteratorUoW->Shaders.VertexShader, 0, projectionMatrix);
						DsrResult setViewMatrixResult = SetConstantBuffer(m_device, iteratorUoW->Shaders.VertexShader, 1, viewMatrix);
					}

					for (auto iteratorRenderData = iteratorUoW->RenderData.begin(); iteratorRenderData < iteratorUoW->RenderData.end(); ++iteratorRenderData)
					{
						// use instanced rendering later
						XMMATRIX model = iteratorRenderData->Transform.CalculateModelMatrix();
						XMMATRIX normal = iteratorRenderData->Transform.CalculateNormalMatrix();

						DsrResult setModelMatrixResult = SetVertexShaderPerObjectData(m_device, iteratorUoW->Shaders.VertexShader, 2, { model, normal });

						//DsrResult setModelMatrixResult = SetConstantBuffer(m_device, iteratorUoW->Shaders.VertexShader, 2, model);

						std::shared_ptr<ID3D11Buffer> vertexBufferPtr = iteratorRenderData->VertexBuffer.GetVertexBuffer().GetBufferPtr();
						std::shared_ptr<ID3D11Buffer> indexBufferPtr = iteratorRenderData->VertexBuffer.GetIndexBuffer().GetBufferPtr();
						Direct3dShaderInputLayout layout = iteratorRenderData->VertexBuffer.GetLayout();
						uint32_t vertexStride = layout.GetTotalStride();
						uint32_t offset = 0;

						ID3D11Buffer* vertexShaderRawPtr = vertexBufferPtr.get();
						m_device->SetInputLayout(iteratorUoW->Shaders.VertexShaderInputLayout.get());
						m_device->SetVertexBuffers(0, 1, &vertexShaderRawPtr, &vertexStride, &offset);
						m_device->SetIndexBuffer(indexBufferPtr.get());

						m_device->UseShader(iteratorUoW->Shaders.VertexShader->GetShaderPtr().get(), nullptr, 0);

						std::vector<ID3D11Buffer*> vsConstantBuffers;
						for (auto& pair : iteratorUoW->Shaders.VertexShader->ConstantBuffers)
							vsConstantBuffers.push_back(pair.second.GetBufferPtr().get());

						m_device->UseConstantBuffers<ID3D11VertexShader>(0, vsConstantBuffers.size(), vsConstantBuffers.data());
						m_device->UseShader(iteratorUoW->Shaders.PixelShader->GetShaderPtr().get(), nullptr, 0);

						if (iteratorRenderData->VertexGroups.empty())
						{
							uint32_t indexBufferSize = iteratorRenderData->VertexBuffer.GetIndexBuffer().GetBufferSize();
							m_device->DrawIndexed(indexBufferSize, 0, 0);
						}
						else
						{
							for (auto& vertexGroup : iteratorRenderData->VertexGroups)
							{
								if (vertexGroup.PixelShader)
									m_device->UseShader(vertexGroup.PixelShader->GetShaderPtr().get(), nullptr, 0);

								XMStoreFloat4(&vertexGroup.PSData.CameraPosition, m_activeCamera->Transform.Position);
								SetConstantBuffer(m_device, iteratorUoW->Shaders.PixelShader, 0, &vertexGroup.PSData, sizeof(PixelShaderData));

								std::vector<ID3D11Buffer*> psConstantBuffers;
								for (auto& pair : iteratorUoW->Shaders.PixelShader->ConstantBuffers)
									psConstantBuffers.push_back(pair.second.GetBufferPtr().get());
								 
								std::vector<ID3D11ShaderResourceView*> psResourceViews;
								for (const Direct3dShaderTexture2D& texture : vertexGroup.PSTextures2D)
									psResourceViews.push_back(texture.GetShaderResourceViewPtr().get());

								m_device->UseConstantBuffers<ID3D11PixelShader>(0, psConstantBuffers.size(), psConstantBuffers.data());
								m_device->SetShaderResources<ID3D11PixelShader>(0, psResourceViews.size(), psResourceViews.data());
								m_device->DrawIndexed(vertexGroup.IndexCount, vertexGroup.StartIndexLocation, 0);
							}
						}
					}
				}

				// switch the back buffer and the front buffer
				m_device->SwapBuffers();
			}

			DsrResult Direct3dRenderer::SetVertexShaderPerObjectData(
				const std::shared_ptr<Direct3dDevice> device,
				std::shared_ptr<Direct3dShader<ID3D11VertexShader>> target,
				const size_t& bRegister,
				const dsr::shader::PerObject& data)
			{
				return SetConstantBuffer<ID3D11VertexShader>(device, target, bRegister, &data, sizeof(dsr::shader::PerObject));
			}
		}
	}
}
