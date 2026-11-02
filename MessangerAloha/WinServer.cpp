#include "ServerForm.h"
#include <Windows.h>

using namespace MessengerAloha;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Application::Run(gcnew ServerForm());
	return 0;
}