#include "vkRenderer.h"

int main()
{
	Application app(1920, 1080, "Renderer");

	app.Run();
	app.Destroy();

	return 0;
}