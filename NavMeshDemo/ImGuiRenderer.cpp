#include "ImGuiRenderer.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void ImGuiRenderer::OnPrepareUpdate(const dsr::events::PrepareUpdateFrameEvent& prepareUpdateEvent)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ImGuiRenderer::OnUpdate(const dsr::events::UpdateFrameEvent& updateFrameEvent)
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
