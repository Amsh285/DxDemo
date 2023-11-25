
#include "BasicRendererApplication.h"

int main(int argc, char* argv[])
{
	try
	{
		BasicRendererApplication application;
		application.Initialize();

		dsr::DsrResult setupResult = application.Setup();
		if (setupResult.GetResultStatusCode() != RESULT_SUCCESS)
		{
			std::cout << "error setup application: " << setupResult.GetResultMessage() << std::endl;
			return EXIT_FAILURE;
		}

		application.Run();
	}
	catch (const std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
	}
}
