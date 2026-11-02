#include "ClientForm.h"
#include <Windows.h>

using namespace AlohaClient;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	System::Windows::Forms::Application::EnableVisualStyles(); 
	System::Windows::Forms::Application::SetCompatibleTextRenderingDefault(false);
	System::Windows::Forms::Application::Run(gcnew ClientForm());
	return 0;
}
