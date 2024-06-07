#pragma once

#include "dsrpch.h"

struct NavMeshSimulationSceneSettings
{
	std::filesystem::path AssetBaseDirectory;
	std::filesystem::path BaseMeshFileName;
	std::filesystem::path BaseMeshMaterialFileName;

	DirectX::XMMATRIX BaseMeshModel;
	DirectX::XMMATRIX UpperSurfaceModel;
	DirectX::XMMATRIX UpperSurfaceSubDivisonModel;
	DirectX::XMMATRIX UpperSurfaceBarycentricSubDivisionModel;

	NavMeshSimulationSceneSettings()
		: AssetBaseDirectory("Assets/"),
		BaseMeshFileName(""), BaseMeshMaterialFileName(""),
		BaseMeshModel(DirectX::XMMatrixIdentity()), UpperSurfaceModel(DirectX::XMMatrixIdentity()),
		UpperSurfaceSubDivisonModel(DirectX::XMMatrixIdentity()),
		UpperSurfaceBarycentricSubDivisionModel(DirectX::XMMatrixIdentity())
	{
	}
};
