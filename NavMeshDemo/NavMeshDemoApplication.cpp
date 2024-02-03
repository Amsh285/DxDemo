#include "NavMeshDemoApplication.h"

NavMeshDemoApplication::NavMeshDemoApplication()
	: DsrApplication(L"Nav Mesh Demo", 100, 100, 1280, 768)
{
}

dsr::DsrResult NavMeshDemoApplication::Setup()
{
	dsr::DsrResult baseResult = DsrApplication::Setup();
	if (baseResult.GetResultStatusCode() != RESULT_SUCCESS)
		return baseResult;

	return dsr::DsrResult::Success("");
}
