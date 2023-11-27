#include "ModelloaderApplication.h"

dsr::DsrResult ModelloaderApplication::Setup()
{
	dsr::DsrResult baseResult = DsrApplication::Setup();
	if (baseResult.GetResultStatusCode() != RESULT_SUCCESS)
		return baseResult;

	return dsr::DsrResult::Success("Setup Successful.");
}

ModelloaderApplication::ModelloaderApplication()
	: DsrApplication(L"Model loader", 100, 100, 1280, 768)
{
}
