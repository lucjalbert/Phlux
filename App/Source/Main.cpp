#include "Core/Application.h"
#include "ViewportLayer.h"

int main()
{
	Core::ApplicationSpecification appSpec;
	appSpec.Name = "Phlux";
	appSpec.WindowSpec.Width = 1920;
	appSpec.WindowSpec.Height = 1080;
	
	Core::Application application(appSpec);
	application.PushLayer<ViewportLayer>();
	application.Run();
}