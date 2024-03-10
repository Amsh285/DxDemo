
#include <iostream>


#include "NavMeshDemoApplication.h"

int main(int argc, const char* const* argv)
{
	try
	{
		std::shared_ptr<NavMeshDemoApplication> app = std::make_shared<NavMeshDemoApplication>();
		app->Initialize(argc, argv);

		dsr::DsrResult setupResult = app->Setup();
		if (setupResult.GetResultStatusCode() != RESULT_SUCCESS)
		{
			std::cout << "Setup failed: " << setupResult.GetResultMessage() << std::endl;
			return EXIT_FAILURE;
		}

		app->Run();
	}
	catch (const std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
	}
}
