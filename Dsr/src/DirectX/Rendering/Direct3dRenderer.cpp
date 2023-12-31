#include "dsrpch.h"
#include "Direct3dRenderer.h"

namespace dsr
{
	namespace directX
	{
		namespace rendering
		{
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
						DsrResult setModelMatrixResult = SetConstantBuffer(m_device, iteratorUoW->Shaders.VertexShader, 2, model);

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
						for (auto pair : iteratorUoW->Shaders.VertexShader->ConstantBuffers)
							vsConstantBuffers.push_back(pair.second.GetBufferPtr().get());

						m_device->UseConstantBuffers<ID3D11VertexShader>(0, vsConstantBuffers.size(), vsConstantBuffers.data());
						m_device->UseShader(iteratorUoW->Shaders.PixelShader->GetShaderPtr().get(), nullptr, 0);

						uint32_t indexBufferSize = iteratorRenderData->VertexBuffer.GetIndexBuffer().GetBufferSize();
						m_device->DrawIndexed(indexBufferSize, 0, 0);
					}
				}

				// switch the back buffer and the front buffer
				m_device->SwapBuffers();
			}
		}
	}
}
