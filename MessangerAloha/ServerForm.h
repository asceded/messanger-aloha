#pragma once
#include <SocketWrapper/Server.h>
namespace MessengerAloha {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public delegate void ui_update(bool is_online);
	public delegate System::Void status_update(System::String^ text);
	public delegate void log_update(System::String^ text, System::Drawing::Color);

	/// <summary>
	/// ������ ��� ServerForm
	/// </summary>
	public ref class ServerForm : public System::Windows::Forms::Form
	{
	public:
		ServerForm(void)
		{
			InitializeComponent();
			server = nullptr;
			is_online = false;
		}

	protected:
		/// <summary>
		/// ���������� ��� ������������ �������.
		/// </summary>
		~ServerForm()
		{
			if (components) {
				delete components;
			}
			server_shutdown();
		}
	private: System::Windows::Forms::Button^ buttonRun;
	private: System::Windows::Forms::RichTextBox^ logTextBox;
	private: System::Windows::Forms::Label^ labelStatus;
	private: System::Threading::Thread^ server;
	private: bool is_online;



	protected:


	private:
		/// <summary>
		/// ������������ ���������� ������������.
		/// </summary>
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// ��������� ����� ��� ��������� ������������ � �� ��������� 
		/// ���������� ����� ������ � ������� ��������� ����.
		/// </summary>
		void InitializeComponent(void)
		{
			this->buttonRun = (gcnew System::Windows::Forms::Button());
			this->logTextBox = (gcnew System::Windows::Forms::RichTextBox());
			this->labelStatus = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// buttonRun
			// 
			this->buttonRun->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 15.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->buttonRun->Location = System::Drawing::Point(289, 410);
			this->buttonRun->Name = L"buttonRun";
			this->buttonRun->Size = System::Drawing::Size(285, 39);
			this->buttonRun->TabIndex = 0;
			this->buttonRun->Text = L"START MAX SERVER";
			this->buttonRun->UseVisualStyleBackColor = true;
			this->buttonRun->Click += gcnew System::EventHandler(this, &ServerForm::buttonRun_Click);
			// 
			// logTextBox
			// 
			this->logTextBox->Font = (gcnew System::Drawing::Font(L"Courier New", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->logTextBox->Location = System::Drawing::Point(29, 29);
			this->logTextBox->Name = L"logTextBox";
			this->logTextBox->ReadOnly = true;
			this->logTextBox->Size = System::Drawing::Size(296, 351);
			this->logTextBox->TabIndex = 1;
			this->logTextBox->Text = L"";
			this->logTextBox->TextChanged += gcnew System::EventHandler(this, &ServerForm::logTextBox_TextChanged);
			// 
			// labelStatus
			// 
			this->labelStatus->AutoSize = true;
			this->labelStatus->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 15.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->labelStatus->Location = System::Drawing::Point(406, 29);
			this->labelStatus->Name = L"labelStatus";
			this->labelStatus->Size = System::Drawing::Size(210, 25);
			this->labelStatus->TabIndex = 2;
			this->labelStatus->Text = L"STATUS: OFFLINE";
			// 
			// ServerForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(687, 519);
			this->Controls->Add(this->labelStatus);
			this->Controls->Add(this->logTextBox);
			this->Controls->Add(this->buttonRun);
			this->Name = L"ServerForm";
			this->Text = L"ServerForm";
			this->Load += gcnew System::EventHandler(this, &ServerForm::ServerForm_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void server_run(Object^ port);
	private: System::Void server_shutdown();
	private: System::Void label_update(System::String^ text);
	private: System::Void addLine(const std::string& text, System::Drawing::Color color);
	private: System::Void addLineLog(System::String^ line, System::Drawing::Color color);
	private: System::Void buttonRun_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void logTextBox_TextChanged(System::Object^ sender, System::EventArgs^ e) {
	}
private: System::Void ServerForm_Load(System::Object^ sender, System::EventArgs^ e) {
}
};
};