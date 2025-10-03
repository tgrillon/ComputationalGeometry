#include "AppLayer.h"
#include "Core/Application.h"

namespace Application
{
int Run()
{
	Core::ApplicationSpecification appSpec;
	appSpec.Name = "Architecture";
	appSpec.WindowSpec.Width = 800;
	appSpec.WindowSpec.Height = 500;

	Core::Application application(appSpec);
	application.PushLayer<AppLayer>();
	application.Run();

	return 0;
}
} // namespace Application
