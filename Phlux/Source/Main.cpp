#include "Core/Application.h"
#include "ViewportLayer.h"
#include "TitleBarLayer.h"
#include "OutlinerLayer.h"

int main()
{
	Core::ApplicationSpecification appSpec;
	appSpec.Name = "Phlux";
	appSpec.WindowSpec.Width = 1920;
	appSpec.WindowSpec.Height = 1080;
	
	Core::Application application(appSpec);
	application.PushLayer<ViewportLayer>();
	application.PushLayer<TitleBarLayer>();
	application.PushLayer<OutlinerLayer>();
	application.Run();
}