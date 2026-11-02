#include <Windows.h>
#include "ClientForm.h"

using namespace AlohaClient;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Application::Run(gcnew ClientForm());
	return 0;
}