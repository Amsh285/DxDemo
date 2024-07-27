#include "NavMeshSimulationSceneBenchmarkView.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "Data/Pathfinding/VertexIndexSearchResult.h"


NavMeshSimulationSceneBenchmarkView::NavMeshSimulationSceneBenchmarkView(const std::string& controlId, const BenchmarkViewType viewType)
	: m_controlId(controlId), m_viewType(viewType)
{
	m_benchmarkEnabledLabel = "Benchmark Enabled: ##" + controlId + "EnabledColor";
	m_benchmarkEuclideanIterationTimesLabel = "Benchmark Iterations##" + controlId + "Euclidean";
	m_benchmarkDijkstraIterationTimesLabel = "Benchmark Iterations##" + controlId + "Dijkstra";
	m_benchmarkEuclideanRunButtonLabel = "Run Benchmark##" + controlId + "Euclidean";
	m_benchmarkDijkstraRunButtonLabel = "Run Benchmark##" + controlId + "Dijkstra";

	m_euclideanChildControlId = controlId + "EuclideanChild";
	m_dijkstraChildControlId = controlId + "DijkstraChild";
}

void NavMeshSimulationSceneBenchmarkView::Update(
	std::shared_ptr<NavMeshSimulationSceneBase> selectedScene,
	const TimeUnit selectedTimeUnit,
	const dsr::SyncHandle<NavMeshSimulationSceneBenchmarkResult>& euclideanBenchmarkHandle,
	const dsr::SyncHandle<NavMeshSimulationSceneBenchmarkResult>& dijkstraBenchmarkHandle
)
{
	using namespace dsr::data::pathfinding;

	NavMeshSimulationSceneBenchmarkStats stats = GetStats(selectedScene);
	bool canExecuteBenchmark = stats.GetVertexIndexSearchResultType() == VertexIndexSearchResultType::PathSearchRequired;

	ImGui::Text("Benchmark Enabled:");
	ImGui::SameLine();
	ImGui::ColorButton(
		"##BenchmarkUpperSurfaceEnabledColor",
		canExecuteBenchmark ? ImVec4(0.0f, 1.0f, 0.0f, 1.0f) : ImVec4(1.0f, 0.0f, 0.0f, 1.0f),
		ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop,
		ImVec2(15, 15)
	);

	ImGui::Text("Index Search: %s", stats.GetVertexIndexSearchResultTypeText().c_str());

	ImGui::Text("Triangle Count: %d", stats.GetNavMeshTriangleCount());
	ImGui::Text("Avg Branching Factor: %.4f", stats.GetAverageBranchingFactor());
	ImGui::Text("Nodes Traveled: %d", stats.GetNodesTraveled());
	ImGui::Text("Path Length: %.4f", stats.GetPathLength());

	ImGui::NewLine();

	ImGui::BeginChild(m_euclideanChildControlId.c_str(), ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, 180.0f), true);
	ImGui::Text("Euclidean Heuristic");
	ImGui::InputInt(m_benchmarkEuclideanIterationTimesLabel.c_str(), &m_euclideanBenchmarkIterations);

	m_euclideanBenchmarkIterations = std::clamp(m_euclideanBenchmarkIterations, 1, 100000);

	ImGui::BeginDisabled(!canExecuteBenchmark || m_isEuclideanBenchmarkRunning.load());

	if (ImGui::Button(m_benchmarkEuclideanRunButtonLabel.c_str()) && canExecuteBenchmark)
		RunEuclideanBenchmark(selectedScene);

	ImGui::EndDisabled();

	DisplayBenchmarkResult(euclideanBenchmarkHandle.GetData(), selectedTimeUnit);
	ImGui::EndChild();

	ImGui::SameLine();

	ImGui::BeginChild(m_dijkstraChildControlId.c_str(), ImVec2(0, 180.0f), true);
	ImGui::Text("Dijkstra Heuristic");
	ImGui::InputInt(m_benchmarkDijkstraIterationTimesLabel.c_str(), &m_dijkstraBenchmarkIterations);

	m_dijkstraBenchmarkIterations = std::clamp(m_dijkstraBenchmarkIterations, 1, 100000);

	ImGui::BeginDisabled(!canExecuteBenchmark || m_isDijkstraBenchmarkRunning.load());

	if (ImGui::Button(m_benchmarkDijkstraRunButtonLabel.c_str()) && canExecuteBenchmark)
		RunDijkstraBenchmark(selectedScene);

	ImGui::EndDisabled();

	DisplayBenchmarkResult(dijkstraBenchmarkHandle.GetData(), selectedTimeUnit);
	ImGui::EndChild();
}

NavMeshSimulationSceneBenchmarkStats NavMeshSimulationSceneBenchmarkView::GetStats(std::shared_ptr<NavMeshSimulationSceneBase> selectedScene) const
{
	switch (m_viewType)
	{
	case BenchmarkViewType::UpperSurface:
	{
		return selectedScene->GetBenchmarks()->UpperSurfaceStats;
	}
	case BenchmarkViewType::UpperSurfaceSubDivision:
	{
		return selectedScene->GetBenchmarks()->UpperSurfaceSubDivisionStats;
	}
	case BenchmarkViewType::UpperSurfaceBarycentricSubDivision:
	{
		return selectedScene->GetBenchmarks()->UpperSurfaceBarycentricSubDivisionStats;
	}
	}
}

void NavMeshSimulationSceneBenchmarkView::RunEuclideanBenchmark(std::shared_ptr<NavMeshSimulationSceneBase> selectedScene)
{
	switch (m_viewType)
	{
	case BenchmarkViewType::UpperSurface:
	{
		m_isEuclideanBenchmarkRunning.store(true);

		std::thread([&, this, selectedScene]() {
			selectedScene->RunUpperSurfaceBenchmark(m_euclideanBenchmarkIterations);
			m_isEuclideanBenchmarkRunning.store(false);
			}).detach();
		break;
	}
	case BenchmarkViewType::UpperSurfaceSubDivision:
	{
		m_isEuclideanBenchmarkRunning.store(true);

		std::thread([&, this, selectedScene]() {
			selectedScene->RunUpperSurfaceSubDivisionBenchmark(m_euclideanBenchmarkIterations);
			m_isEuclideanBenchmarkRunning.store(false);
			}).detach();

		break;
	}
	case BenchmarkViewType::UpperSurfaceBarycentricSubDivision:
	{
		m_isEuclideanBenchmarkRunning.store(true);

		std::thread([&, this, selectedScene]() {
			selectedScene->RunUpperSurfaceBarycentricSubDivisionBenchmark(m_euclideanBenchmarkIterations);
			m_isEuclideanBenchmarkRunning.store(false);
			}).detach();
		break;
	}
	}
}

void NavMeshSimulationSceneBenchmarkView::RunDijkstraBenchmark(std::shared_ptr<NavMeshSimulationSceneBase> selectedScene)
{
	switch (m_viewType)
	{
	case BenchmarkViewType::UpperSurface:
	{
		m_isDijkstraBenchmarkRunning.store(true);

		std::thread([&, this, selectedScene]() {
			selectedScene->RunUpperSurfaceDijkstraBenchmark(m_dijkstraBenchmarkIterations);
			m_isDijkstraBenchmarkRunning.store(false);
			}).detach();
		break;
	}
	case BenchmarkViewType::UpperSurfaceSubDivision:
	{
		m_isDijkstraBenchmarkRunning.store(true);

		std::thread([&, this, selectedScene]() {
			selectedScene->RunUpperSurfaceSubDivisionDijkstraBenchmark(m_dijkstraBenchmarkIterations);
			m_isDijkstraBenchmarkRunning.store(false);
			}).detach();
		break;
	}
	case BenchmarkViewType::UpperSurfaceBarycentricSubDivision:
	{
		m_isDijkstraBenchmarkRunning.store(true);

		std::thread([&, this, selectedScene]() {
			selectedScene->RunUpperSurfaceBarycentricSubDivisionDijkstraBenchmark(m_dijkstraBenchmarkIterations);
			m_isDijkstraBenchmarkRunning.store(false);
			}).detach();
		break;
	}
	}

}

void NavMeshSimulationSceneBenchmarkView::DisplayBenchmarkResult(const NavMeshSimulationSceneBenchmarkResult& benchmarkResult, const TimeUnit unit)
{
	switch (unit)
	{
	case TimeUnit::Nanoseconds:
	{
		ImGui::Text("Total Time: %.4f ns", benchmarkResult.TotalTime.count());
		ImGui::Text("Avg Iteration Time: %.4f ns", benchmarkResult.AverageIterationTime.count());
		ImGui::Text("Standard Deviation: %.4f ns", benchmarkResult.StandardDeviationTime.count());
		break;
	}
	case TimeUnit::Microseconds:
	{
		ImGui::Text("Total Time: %.4f us", benchmarkResult.TotalTime.count() / 1.0e3);
		ImGui::Text("Avg Iteration Time: %.4f us", benchmarkResult.AverageIterationTime.count() / 1.0e3);
		ImGui::Text("Standard Deviation: %.4f us", benchmarkResult.StandardDeviationTime.count() / 1.0e3);
		break;
	}
	case TimeUnit::Milliseconds:
	{
		ImGui::Text("Total Time: %.4f ms", benchmarkResult.TotalTime.count() / 1.0e6);
		ImGui::Text("Avg Iteration Time: %.4f ms", benchmarkResult.AverageIterationTime.count() / 1.0e6);
		ImGui::Text("Standard Deviation: %.4f ms", benchmarkResult.StandardDeviationTime.count() / 1.0e6);
		break;
	}
	case TimeUnit::Seconds:
	{
		ImGui::Text("Total Time: %.4f s", benchmarkResult.TotalTime.count() / 1.0e9);
		ImGui::Text("Avg Iteration Time: %.4f s", benchmarkResult.AverageIterationTime.count() / 1.0e9);
		ImGui::Text("Standard Deviation: %.4f s", benchmarkResult.StandardDeviationTime.count() / 1.0e9);
		break;
	}
	}

	ImGui::Text("Nodes Traveled: %d", benchmarkResult.NodesTraveled);
	ImGui::Text("Path Length: %.4f", benchmarkResult.PathLength);
}

