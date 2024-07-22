#include "EditorUISystem.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "Infrastructure/XMathHelper.h"
#include <Vendor/implot/implot.h>

std::vector<std::type_index> EditorUISystem::GetRequiredComponents() const
{
	return { std::type_index(typeid(EditorUIComponent)) };
}

EditorUISystem::EditorUISystem(
	const std::shared_ptr<dsr::input::Input>& input,
	const std::vector<std::shared_ptr<NavMeshSimulationSceneBase>>& scenes)
	: dsr::ecs::System(typeid(EditorUISystem)),
	m_input(input), m_scenes(scenes), m_sceneSelectedIdx(0),
	m_subDivisionCount(1), m_barycentricSubDivisionCount(1)
{
	//OnStart = std::bind(&EditorUISystem::Start, this, std::placeholders::_1);
	OnUpdate = std::bind(&EditorUISystem::Update, this, std::placeholders::_1);

	m_timeUnits = {
		{ "Nanoseconds", TimeUnit::Nanoseconds },
		{ "Microseconds", TimeUnit::Microseconds },
		{ "Milliseconds", TimeUnit::Milliseconds },
		{ "Seconds", TimeUnit::Seconds }
	};

	//m_upperSurfaceBenchmark = m_scenes[m_sceneSelectedIdx]->GetBenchmarks()->UpperSurfaceBenchmark;
}

//void EditorUISystem::Start(const dsr::ecs::EngineStartupContext& context)
//{
//	
//}

void EditorUISystem::Update(const dsr::ecs::EngineContext& context)
{
	using namespace dsr::ecs;
	using namespace dsr::events;
	using namespace dsr::input;

	using namespace dsr::data::pathfinding;

	using namespace DirectX;

	std::shared_ptr<EditorUIComponent> uiData = context.GetComponent<EditorUIComponent>();

	if (m_input->GetKeyDown(KeyCode::MouseLeft) && !ImGui::GetIO().WantCaptureMouse)
	{
		EditorScreenClickEvent screenClickEvent(
			m_input->GetMouse()->GetCurrentClientAreaPosition(),
			*m_input->GetScreen(),
			(PathSelectType)uiData->PathSelectMode);

		m_scenes[m_sceneSelectedIdx]->OnScreenClick(screenClickEvent);
	}

	//ImPlot::ShowDemoWindow();

	ImGui::Begin("Editor", nullptr);

	ImGui::BeginDisabled(IsBackgroundThreadRunning());

	if (ImGui::BeginListBox("Scenes"))
	{
		for (size_t i = 0; i < m_scenes.size(); i++)
		{
			//m_sceneViewData[i].IsSelected = itemCurrentIndex == i;
			const bool isSelected = m_sceneSelectedIdx == i;

			if (ImGui::Selectable(m_scenes[i]->GetSceneName().c_str(), isSelected))
			{
				if (m_sceneSelectedIdx != i)
					m_scenes[i]->SetActive();

				m_sceneSelectedIdx = i;

				m_subDivisionCount = m_scenes[m_sceneSelectedIdx]->GetUpperSurfaceSubDivisionCount();
				m_barycentricSubDivisionCount = m_scenes[m_sceneSelectedIdx]->GetUpperSurfaceBarycentricSubDivisionCount();
			}

			if (isSelected)
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndListBox();
	}

	ImGui::EndDisabled();

	ImGui::NewLine();

	if (ImGui::CollapsingHeader("Debug"))
	{
		ImGui::Checkbox("Show Colliders", &uiData->ShowColliders);
	}

	if (ImGui::CollapsingHeader("Mesh Subdivision"))
	{
		ImGui::BeginDisabled(IsBackgroundThreadRunning());

		if (ImGui::SliderInt("Subdivision Count", &m_subDivisionCount, 1, 5))
		{
			assert(m_subDivisionCount >= 1 && m_subDivisionCount <= 5);

			m_scenes[m_sceneSelectedIdx]->UpdateUpperSurfaceSubDivision(static_cast<uint32_t>(m_subDivisionCount));
		}

		if (ImGui::SliderInt("Barycentric Subdivision Count", &m_barycentricSubDivisionCount, 1, 5))
		{
			assert(m_barycentricSubDivisionCount >= 1 && m_barycentricSubDivisionCount <= 5);

			m_scenes[m_sceneSelectedIdx]->UpdateUpperSurfaceBarycentricSubDivision(static_cast<uint32_t>(m_barycentricSubDivisionCount));
		}

		ImGui::EndDisabled();
	}

	if (ImGui::CollapsingHeader("Pathfinding"))
	{
		ImGui::Text("Node select:");
		ImGui::RadioButton("Start", &uiData->PathSelectMode, 0);
		ImGui::RadioButton("Finish", &uiData->PathSelectMode, 1);
	}

	ImGui::End();

	m_scenes[m_sceneSelectedIdx]->GetBenchmarks()->Update();
	const dsr::SyncHandle<NavMeshSimulationSceneBenchmarkResult>& upperSurfaceBenchmarkHandle = m_scenes[m_sceneSelectedIdx]->GetBenchmarks()->UpperSurfaceBenchmarkHandle;
	const dsr::SyncHandle<NavMeshSimulationSceneBenchmarkResult>& upperSurfaceSubDivisionBenchmarkHandle = m_scenes[m_sceneSelectedIdx]->GetBenchmarks()->UpperSurfaceSubDivisionBenchmarkHandle;
	const dsr::SyncHandle<NavMeshSimulationSceneBenchmarkResult>& upperSurfaceBarycentricSubDivisionBenchmarkHandle = m_scenes[m_sceneSelectedIdx]->GetBenchmarks()->UpperSurfaceBarycentricSubDivisionBenchmarkHandle;

	ImGui::Begin("Benchmark", nullptr);

	ImGui::Text("Scene: %s", m_scenes[m_sceneSelectedIdx]->GetSceneName().c_str());

	if (ImGui::BeginCombo("Benchmark Timeunits: ", m_timeUnits[m_timeUnitSelectedIdx].first.c_str()))
	{
		for (size_t i = 0; i < m_timeUnits.size(); i++)
		{
			const bool isSelected = m_timeUnitSelectedIdx == i;

			if (ImGui::Selectable(m_timeUnits[i].first.c_str(), isSelected))
			{
				m_timeUnitSelectedIdx = i;
			}

			if (isSelected)
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}

	if (ImGui::CollapsingHeader("Upper Surface"))
	{
		NavMeshSimulationSceneBenchmarkStats stats = m_scenes[m_sceneSelectedIdx]->GetBenchmarks()->UpperSurfaceStats;
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

		ImGui::Separator();
		ImGui::InputInt("Benchmark Iterations##UpperSurface", &m_upperSurfaceBenchmarkIterations);

		m_upperSurfaceBenchmarkIterations = std::clamp(m_upperSurfaceBenchmarkIterations, 1, 100000);

		ImGui::BeginDisabled(!canExecuteBenchmark || m_isUpperSurfaceBenchmarkRunning.load());

		if (ImGui::Button("Run Benchmark##UpperSurface") && canExecuteBenchmark)
		{
			m_isUpperSurfaceBenchmarkRunning.store(true);


			std::thread([this]() {
				m_scenes[m_sceneSelectedIdx]->RunUpperSurfaceBenchmark(m_upperSurfaceBenchmarkIterations);
				m_isUpperSurfaceBenchmarkRunning.store(false);
			}).detach();
		}

		ImGui::EndDisabled();

		DisplayBenchmarkResult(upperSurfaceBenchmarkHandle.GetData(), m_timeUnits[m_timeUnitSelectedIdx].second);
	}

	if(ImGui::CollapsingHeader("Upper Surface Subdivision"))
	{
		NavMeshSimulationSceneBenchmarkStats stats = m_scenes[m_sceneSelectedIdx]->GetBenchmarks()->UpperSurfaceSubDivisionStats;
		bool canExecuteBenchmark = stats.GetVertexIndexSearchResultType() == VertexIndexSearchResultType::PathSearchRequired;

		ImGui::Text("Benchmark Enabled:");
		ImGui::SameLine();
		ImGui::ColorButton(
			"##BenchmarkUpperSurfaceSubDivisionEnabledColor",
			canExecuteBenchmark ? ImVec4(0.0f, 1.0f, 0.0f, 1.0f) : ImVec4(1.0f, 0.0f, 0.0f, 1.0f),
			ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop,
			ImVec2(15, 15)
		);

		ImGui::Text("Index Search: %s", stats.GetVertexIndexSearchResultTypeText().c_str());

		ImGui::Text("Triangle Count: %d", stats.GetNavMeshTriangleCount());
		ImGui::Text("Avg Branching Factor: %.4f", stats.GetAverageBranchingFactor());
		ImGui::Text("Nodes Traveled: %d", stats.GetNodesTraveled());

		ImGui::Separator();
		ImGui::InputInt("Benchmark Iterations##UpperSurfaceSubDivision", &m_upperSurfaceSubdivisionBenchmarkIterations);

		m_upperSurfaceSubdivisionBenchmarkIterations = std::clamp(m_upperSurfaceSubdivisionBenchmarkIterations, 1, 100000);

		ImGui::BeginDisabled(!canExecuteBenchmark || m_isUpperSurfaceSubdivisionBenchmarkRunning.load());

		if (ImGui::Button("Run Benchmark##UpperSurfaceSubDivision") && canExecuteBenchmark)
		{
			m_isUpperSurfaceSubdivisionBenchmarkRunning.store(true);

			std::thread([this]() {
				m_scenes[m_sceneSelectedIdx]->RunUpperSurfaceSubDivisionBenchmark(m_upperSurfaceBenchmarkIterations);
				m_isUpperSurfaceSubdivisionBenchmarkRunning.store(false);
				}).detach();
		}

		ImGui::EndDisabled();

		DisplayBenchmarkResult(upperSurfaceSubDivisionBenchmarkHandle.GetData(), m_timeUnits[m_timeUnitSelectedIdx].second);
	}

	if (ImGui::CollapsingHeader("Upper Surface barycentric SubDivision"))
	{
		NavMeshSimulationSceneBenchmarkStats stats = m_scenes[m_sceneSelectedIdx]->GetBenchmarks()->UpperSurfaceBarycentricSubDivisionStats;
		bool canExecuteBenchmark = stats.GetVertexIndexSearchResultType() == VertexIndexSearchResultType::PathSearchRequired;

		ImGui::Text("Benchmark Enabled:");
		ImGui::SameLine();
		ImGui::ColorButton(
			"##BenchmarkUpperSurfaceBarycentricSubDivisionEnabledColor",
			canExecuteBenchmark ? ImVec4(0.0f, 1.0f, 0.0f, 1.0f) : ImVec4(1.0f, 0.0f, 0.0f, 1.0f),
			ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop,
			ImVec2(15, 15)
		);

		ImGui::Text("Index Search: %s", stats.GetVertexIndexSearchResultTypeText().c_str());

		ImGui::Text("Triangle Count: %d", stats.GetNavMeshTriangleCount());
		ImGui::Text("Avg Branching Factor: %.4f", stats.GetAverageBranchingFactor());
		ImGui::Text("Nodes Traveled: %d", stats.GetNodesTraveled());

		ImGui::Separator();
		ImGui::InputInt("Benchmark Iterations##UpperSurfaceBarycentricSubDivision", &m_upperSurfaceBarycentricSubdivisionBenchmarkIterations);

		m_upperSurfaceBarycentricSubdivisionBenchmarkIterations = std::clamp(m_upperSurfaceBarycentricSubdivisionBenchmarkIterations, 1, 100000);

		ImGui::BeginDisabled(!canExecuteBenchmark || m_isUpperSurfaceBarycentricSubdivisionBenchmarkRunning.load());

		if (ImGui::Button("Run Benchmark##UpperSurfaceBarycentricSubDivision") && canExecuteBenchmark)
		{
			m_isUpperSurfaceBarycentricSubdivisionBenchmarkRunning.store(true);

			std::thread([this]() {
				m_scenes[m_sceneSelectedIdx]->RunUpperSurfaceBarycentricSubDivisionBenchmark(m_upperSurfaceBarycentricSubdivisionBenchmarkIterations);
				m_isUpperSurfaceBarycentricSubdivisionBenchmarkRunning.store(false);
				}).detach();
		}

		ImGui::EndDisabled();

		DisplayBenchmarkResult(upperSurfaceBarycentricSubDivisionBenchmarkHandle.GetData(), m_timeUnits[m_timeUnitSelectedIdx].second);
	}

	if(ImPlot::BeginPlot("Avg Iteration times (double click to autofit):"))
	{
		ImPlot::SetupAxes("Iteration", "Time (ns)");

		const std::vector<std::chrono::duration<double, std::nano>>& upperSurfaceIterationTimes = upperSurfaceBenchmarkHandle.GetData().IterationTimes;
		std::vector<double> upperSurfaceIterationTimesDouble;
		std::vector<double> upperSurfaceIterationTimesXAxis;
		upperSurfaceIterationTimesDouble.reserve(upperSurfaceIterationTimes.size());
		upperSurfaceIterationTimesXAxis.reserve(upperSurfaceIterationTimes.size());

		for (size_t i = 0; i < upperSurfaceIterationTimes.size(); ++i)
		{
			upperSurfaceIterationTimesDouble.push_back(upperSurfaceIterationTimes[i].count());
			upperSurfaceIterationTimesXAxis.push_back(i + 1);
		}

		ImPlot::PlotLine("Upper Surface", upperSurfaceIterationTimesXAxis.data(), upperSurfaceIterationTimesDouble.data(), upperSurfaceIterationTimes.size());

		ImPlot::EndPlot();
	}

	ImGui::End();
}

bool EditorUISystem::IsBackgroundThreadRunning() const
{
	return m_isUpperSurfaceBenchmarkRunning.load() || m_isUpperSurfaceSubdivisionBenchmarkRunning.load() ||
		m_isUpperSurfaceBarycentricSubdivisionBenchmarkRunning.load();
}

void EditorUISystem::DisplayBenchmarkResult(const NavMeshSimulationSceneBenchmarkResult& benchmarkResult, const TimeUnit unit)
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
}
