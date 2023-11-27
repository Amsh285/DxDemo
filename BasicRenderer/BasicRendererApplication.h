#pragma once

#include "dsrpch.h"
#include "DsrApplication.h"

struct VertexPosColor
{
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT3 Color;
};

class BasicRendererApplication : public dsr::DsrApplication
{
public:
	dsr::DsrResult Setup() override;

	BasicRendererApplication();
protected:
private:
	std::variant<dsr::directX::Direct3dVertexBufferf, dsr::dsr_error> LoadContent();
	std::variant<dsr::directX::Direct3dShaderProgram, dsr::dsr_error> LoadShaderProgram();
};

