#include "NavMeshSimulationSceneOverallBenchmarkView.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "Data/Pathfinding/VertexIndexSearchResult.h"

void NavMeshSimulationSceneOverallBenchmarkView::Update(
	const std::shared_ptr<NavMeshSimulationSceneBase> selectedScene
)
{
	using namespace dsr::data::pathfinding;

	std::shared_ptr<NavMeshSimulationSceneBenchmarks> benchmarks = selectedScene->GetBenchmarks();
	bool canRunBenchmarks = benchmarks->UpperSurfaceStats.GetVertexIndexSearchResultType() == VertexIndexSearchResultType::PathSearchRequired
		&& benchmarks->UpperSurfaceSubDivisionStats.GetVertexIndexSearchResultType() == VertexIndexSearchResultType::PathSearchRequired
		&& benchmarks->UpperSurfaceBarycentricSubDivisionStats.GetVertexIndexSearchResultType() == VertexIndexSearchResultType::PathSearchRequired;

	ImGui::InputInt("Benchmark Iterations##overallBenchmark", &m_benchmarkIterations, 1, 100000);

	ImGui::BeginDisabled(!canRunBenchmarks || m_benchmarksRunning.load());

	if (ImGui::Button("Run All Benchmarks##overallBenchmark"))
	{
		m_benchmarksRunning.store(true);

		std::thread([&, this, selectedScene]() {
			selectedScene->RunAllBenchmarks(m_benchmarkIterations);
			m_benchmarksRunning.store(false);
			}).detach();
	}

	ImGui::EndDisabled();

	ImGui::BeginDisabled(m_benchmarksRunning.load());

	if (ImGui::Button("Save Benchmark Results##overallBenchmark"))
	{
		selectedScene->SaveBenchmarkResults();
	}

	ImGui::EndDisabled();

	std::string sceneName = selectedScene->GetSceneName();
}
