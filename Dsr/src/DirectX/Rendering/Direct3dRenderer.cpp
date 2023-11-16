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
				m_device->Clear(0.0f, 0.2f, 0.4f, 1.0f);

				// do 3D rendering on the back buffer here

				for (auto iteratorUoW = m_units.begin(); iteratorUoW < m_units.end(); ++iteratorUoW)
				{
					//Copy!!!!!
					//Direct3dShaderProgram program = iteratorUoW->GetProgram();

					//Just to get something going now.. optimize that later
					//setup viewmatrix
					DirectX::XMVECTOR eyePosition = DirectX::XMVectorSet(0, 0, -10, 1);
					DirectX::XMVECTOR focusPoint = DirectX::XMVectorSet(0, 0, 0, 1);
					DirectX::XMVECTOR upDirection = DirectX::XMVectorSet(0, 1, 0, 0);
					DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtLH(eyePosition, focusPoint, upDirection);
					DsrResult setViewMatrixResult = SetConstantBuffer(m_device, iteratorUoW->Shaders.VertexShader, 1, viewMatrix);

					for (auto iteratorRenderData = iteratorUoW->RenderData.begin(); iteratorRenderData < iteratorUoW->RenderData.end(); ++iteratorRenderData)
					{
						// user instanced rendering later
						DirectX::XMMATRIX model = iteratorRenderData->Transform.CalculateModelMatrix();
						DsrResult setModelMatrixResult = SetConstantBuffer(m_device, iteratorUoW->Shaders.VertexShader, 2, model);
					}
				}

				// switch the back buffer and the front buffer
				m_device->SwapBuffers();
			}
		}
	}
}
