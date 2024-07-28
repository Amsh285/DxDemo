#include "EditorUISystem.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "Infrastructure/XMathHelper.h"
#include "Vendor/implot/implot.h"

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

	m_upperSurfaceBenchmarkView = std::make_unique<NavMeshSimulationSceneBenchmarkView>("UpperSurface", BenchmarkViewType::UpperSurface);
	m_upperSurfaceSubDivisionBenchmarkView = std::make_unique<NavMeshSimulationSceneBenchmarkView>("UpperSurfaceSubDivision", BenchmarkViewType::UpperSurfaceSubDivision);
	m_upperSurfaceBarycentricSubDivisionBenchmarkView = std::make_unique<NavMeshSimulationSceneBenchmarkView>("UpperSurfaceBarycentricSubDivision", BenchmarkViewType::UpperSurfaceBarycentricSubDivision);
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

	const dsr::SyncHandle<NavMeshSimulationSceneBenchmarkResult>& upperSurfaceDijkstraBenchmarkHandle = m_scenes[m_sceneSelectedIdx]->GetBenchmarks()->UpperSurfaceDijkstraBenchmarkHandle;
	const dsr::SyncHandle<NavMeshSimulationSceneBenchmarkResult>& upperSurfaceSubDivisionDijkstraBenchmarkHandle = m_scenes[m_sceneSelectedIdx]->GetBenchmarks()->UpperSurfaceSubDivisionDijkstraBenchmarkHandle;
	const dsr::SyncHandle<NavMeshSimulationSceneBenchmarkResult>& upperSurfaceBarycentricSubDivisionDijkstraBenchmarkHandle = m_scenes[m_sceneSelectedIdx]->GetBenchmarks()->UpperSurfaceBarycentricSubDivisionDijkstraBenchmarkHandle;

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
		m_upperSurfaceBenchmarkView->Update(
			m_scenes[m_sceneSelectedIdx],
			m_timeUnits[m_timeUnitSelectedIdx].second,
			upperSurfaceBenchmarkHandle,
			upperSurfaceDijkstraBenchmarkHandle
		);
	}

	if(ImGui::CollapsingHeader("Upper Surface Subdivision"))
	{
		m_upperSurfaceSubDivisionBenchmarkView->Update(
			m_scenes[m_sceneSelectedIdx],
			m_timeUnits[m_timeUnitSelectedIdx].second,
			upperSurfaceSubDivisionBenchmarkHandle,
			upperSurfaceSubDivisionDijkstraBenchmarkHandle
		);
	}

	if (ImGui::CollapsingHeader("Upper Surface barycentric SubDivision"))
	{
		m_upperSurfaceBarycentricSubDivisionBenchmarkView->Update(
			m_scenes[m_sceneSelectedIdx],
			m_timeUnits[m_timeUnitSelectedIdx].second,
			upperSurfaceBarycentricSubDivisionBenchmarkHandle,
			upperSurfaceBarycentricSubDivisionDijkstraBenchmarkHandle
		);
	}

	if (ImGui::CollapsingHeader("Plot"))
	{
		ImGui::NewLine();
		ImGui::Text("Euclidean Distance Heuristic:");

		if (ImPlot::BeginPlot("Avg Iteration times (double click to autofit):##Euclidean"))
		{
			//Not optimal only convert once. Fix that later.
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

			const std::vector<std::chrono::duration<double, std::nano>>& upperSurfaceSubDivisionIterationTimes = upperSurfaceSubDivisionBenchmarkHandle
				.GetData().IterationTimes;

			std::vector<double> upperSurfaceSubDivisionIterationTimesDouble;
			std::vector<double> upperSurfaceSubDivisionIterationTimesXAxis;

			upperSurfaceSubDivisionIterationTimesDouble.reserve(upperSurfaceSubDivisionIterationTimes.size());
			upperSurfaceSubDivisionIterationTimesXAxis.reserve(upperSurfaceSubDivisionIterationTimes.size());

			for (size_t i = 0; i < upperSurfaceSubDivisionIterationTimes.size(); ++i)
			{
				upperSurfaceSubDivisionIterationTimesDouble.push_back(upperSurfaceSubDivisionIterationTimes[i].count());
				upperSurfaceSubDivisionIterationTimesXAxis.push_back(i + 1);
			}

			ImPlot::PlotLine("Upper Surface SubDivision", upperSurfaceSubDivisionIterationTimesXAxis.data(), upperSurfaceSubDivisionIterationTimesDouble.data(), upperSurfaceSubDivisionIterationTimes.size());

			const std::vector<std::chrono::duration<double, std::nano>>& upperSurfaceBarycentricSubDivisionIterationTimes = upperSurfaceBarycentricSubDivisionBenchmarkHandle
				.GetData().IterationTimes;

			std::vector<double> upperSurfaceBarycentricSubDivisionIterationTimesDouble;
			std::vector<double> upperSurfaceBarycentricSubDivisionIterationTimesXAxis;

			upperSurfaceBarycentricSubDivisionIterationTimesDouble.reserve(upperSurfaceBarycentricSubDivisionIterationTimes.size());
			upperSurfaceBarycentricSubDivisionIterationTimesXAxis.reserve(upperSurfaceBarycentricSubDivisionIterationTimes.size());

			for (size_t i = 0; i < upperSurfaceBarycentricSubDivisionIterationTimes.size(); ++i)
			{
				upperSurfaceBarycentricSubDivisionIterationTimesDouble.push_back(upperSurfaceBarycentricSubDivisionIterationTimes[i].count());
				upperSurfaceBarycentricSubDivisionIterationTimesXAxis.push_back(i + 1);
			}

			ImPlot::PlotLine("Upper Surface Barycentric SubDivision", upperSurfaceBarycentricSubDivisionIterationTimesXAxis.data(), upperSurfaceBarycentricSubDivisionIterationTimesDouble.data(), upperSurfaceBarycentricSubDivisionIterationTimes.size());

			ImPlot::EndPlot();
		}

		ImGui::NewLine();
		ImGui::Text("Dijkstra Heuristic:");

		if (ImPlot::BeginPlot("Avg Iteration times (double click to autofit):##Dijkstra"))
		{
			ImPlot::SetupAxes("Iteration", "Time (ns)");

			const std::vector<std::chrono::duration<double, std::nano>>& upperSurfaceDijkstraIterationTimes = upperSurfaceDijkstraBenchmarkHandle.GetData().IterationTimes;
			std::vector<double> upperSurfaceDijkstraIterationTimesDouble;
			std::vector<double> upperSurfaceDijkstraIterationTimesXAxis;
			upperSurfaceDijkstraIterationTimesDouble.reserve(upperSurfaceDijkstraIterationTimes.size());
			upperSurfaceDijkstraIterationTimesXAxis.reserve(upperSurfaceDijkstraIterationTimes.size());

			for (size_t i = 0; i < upperSurfaceDijkstraIterationTimes.size(); ++i)
			{
				upperSurfaceDijkstraIterationTimesDouble.push_back(upperSurfaceDijkstraIterationTimes[i].count());
				upperSurfaceDijkstraIterationTimesXAxis.push_back(i + 1);
			}

			ImPlot::PlotLine("Upper Surface", upperSurfaceDijkstraIterationTimesXAxis.data(), upperSurfaceDijkstraIterationTimesDouble.data(), upperSurfaceDijkstraIterationTimes.size());

			const std::vector<std::chrono::duration<double, std::nano>>& upperSurfaceSubDivisionDijkstraIterationTimes = upperSurfaceSubDivisionDijkstraBenchmarkHandle
				.GetData().IterationTimes;

			std::vector<double> upperSurfaceSubDivisionDijkstraIterationTimesDouble;
			std::vector<double> upperSurfaceSubDivisionDijkstraIterationTimesXAxis;

			upperSurfaceSubDivisionDijkstraIterationTimesDouble.reserve(upperSurfaceSubDivisionDijkstraIterationTimes.size());
			upperSurfaceSubDivisionDijkstraIterationTimesXAxis.reserve(upperSurfaceSubDivisionDijkstraIterationTimes.size());

			for (size_t i = 0; i < upperSurfaceSubDivisionDijkstraIterationTimes.size(); ++i)
			{
				upperSurfaceSubDivisionDijkstraIterationTimesDouble.push_back(upperSurfaceSubDivisionDijkstraIterationTimes[i].count());
				upperSurfaceSubDivisionDijkstraIterationTimesXAxis.push_back(i + 1);
			}

			ImPlot::PlotLine("Upper Surface SubDivision", upperSurfaceSubDivisionDijkstraIterationTimesXAxis.data(), upperSurfaceSubDivisionDijkstraIterationTimesDouble.data(), upperSurfaceSubDivisionDijkstraIterationTimes.size());

			const std::vector<std::chrono::duration<double, std::nano>>& upperSurfaceBarycentricSubDivisionDijkstraIterationTimes = upperSurfaceBarycentricSubDivisionDijkstraBenchmarkHandle
				.GetData().IterationTimes;

			std::vector<double> upperSurfaceBarycentricSubDivisionDijkstraIterationTimesDouble;
			std::vector<double> upperSurfaceBarycentricSubDivisionDijkstraIterationTimesXAxis;

			upperSurfaceBarycentricSubDivisionDijkstraIterationTimesDouble.reserve(upperSurfaceBarycentricSubDivisionDijkstraIterationTimes.size());
			upperSurfaceBarycentricSubDivisionDijkstraIterationTimesXAxis.reserve(upperSurfaceBarycentricSubDivisionDijkstraIterationTimes.size());

			for (size_t i = 0; i < upperSurfaceBarycentricSubDivisionDijkstraIterationTimes.size(); ++i)
			{
				upperSurfaceBarycentricSubDivisionDijkstraIterationTimesDouble.push_back(upperSurfaceBarycentricSubDivisionDijkstraIterationTimes[i].count());
				upperSurfaceBarycentricSubDivisionDijkstraIterationTimesXAxis.push_back(i + 1);
			}

			ImPlot::PlotLine("Upper Surface Barycentric SubDivision", upperSurfaceBarycentricSubDivisionDijkstraIterationTimesXAxis.data(), upperSurfaceBarycentricSubDivisionDijkstraIterationTimesDouble.data(), upperSurfaceBarycentricSubDivisionDijkstraIterationTimes.size());

			ImPlot::EndPlot();
		}
	}

	ImGui::End();
}

bool EditorUISystem::IsBackgroundThreadRunning() const
{
	return m_upperSurfaceBenchmarkView->IsBenchmarkRunning() || m_upperSurfaceSubDivisionBenchmarkView->IsBenchmarkRunning() ||
		m_upperSurfaceBarycentricSubDivisionBenchmarkView->IsBenchmarkRunning();
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
