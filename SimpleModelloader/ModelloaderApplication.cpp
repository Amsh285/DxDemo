#include "ModelloaderApplication.h"

dsr::DsrResult ModelloaderApplication::Setup()
{
	using namespace dsr;

	dsr::DsrResult baseResult = DsrApplication::Setup();
	if (baseResult.GetResultStatusCode() != RESULT_SUCCESS)
		return baseResult;

	std::variant<BlenderModel, dsr_error> loadModel = m_blenderModelLoader->Load(L"Assets/MapTest.obj");
	if (std::holds_alternative<dsr_error>(loadModel))
		std::cout << std::get<dsr_error>(loadModel).what() << std::endl;

	return dsr::DsrResult::Success("Setup Successful.");
}

ModelloaderApplication::ModelloaderApplication()
	: DsrApplication(L"Model loader", 100, 100, 1280, 768)
{
}
