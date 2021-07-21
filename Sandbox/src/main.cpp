#include "vkRenderer.h"
#include "Device.h"

int main()
{
	Application app(1200, 720, "Renderer");
	Device device(app.GetWindow());
	Pipeline pipeline(device, "../../vkRenderer/res/shaders/vertex_shader.spv", "../../vkRenderer/res/shaders/fragment_shader.spv", Pipeline::DefaultPipelineConfigInfo(1200, 720));

	app.Run();
	app.Destroy();

	return 0;
}
