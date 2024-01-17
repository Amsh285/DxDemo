
#include <iostream>

#include "ModelloaderApplication.h"

int main(int argc, const char* const* argv)
{
	try
	{
		ModelloaderApplication application;
		application.Initialize(argc, argv);

		dsr::DsrResult setupResult = application.Setup();
		if (setupResult.GetResultStatusCode() != RESULT_SUCCESS)
		{
			std::cout << "Setup failed: " << setupResult.GetResultMessage() << std::endl;
			return EXIT_FAILURE;
		}

		application.Run();
	}
	catch (const std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
	}
}