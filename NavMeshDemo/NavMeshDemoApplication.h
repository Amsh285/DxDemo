#pragma once

#include "dsrpch.h"
#include "DsrApplication.h"

class NavMeshDemoApplication : public dsr::DsrApplication
{
public:
	dsr::DsrResult Setup() override;
	
	NavMeshDemoApplication();
private:
};
