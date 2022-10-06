#include "AppWindow.h"

int main()
{
	AppWindow::initialize();
	AppWindow* window = AppWindow::getInstance();

	if (window->init())
	{
		while (window->isRun())
		{
			window->broadcast();
		}
	}

	return 0;
}