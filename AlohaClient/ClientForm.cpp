#include "ClientForm.h"
#include <msclr/marshal_cppstd.h>

public ref class ConnectInfo : public System::Object {
public:
    System::String^ host;
    int serverPort;
    int clientPort;
    ConnectInfo(System::String^ host, int sPort, int cPort)
        : host(host), serverPort(sPort), clientPort(cPort) {
    }
};

System::Void AlohaClient::ClientForm::updateUi(bool connected) {
    if (this->IsDisposed || !this->IsHandleCreated) return;
    buttonConnect->Text = connected ? L"Quit" : L"connect";
    labelStatus->Text = connected ? L"STATUS: ONLINE" : L"STATUS: OFFLINE";
    labelStatus->ForeColor = connected ? System::Drawing::Color::DarkGreen : System::Drawing::Color::DarkRed;
    textBoxServerIp->Enabled = !connected;
    textBoxPort->Enabled = !connected;
    buttonSendMsg->Enabled = connected && !System::String::IsNullOrWhiteSpace(textBoxSend->Text);
}

System::Void AlohaClient::ClientForm::client_shutdown() {
    if (!is_connected && client == nullptr && worker == nullptr) return;
    is_connected = false;

    // Закрываем сокет — это разблокирует recv() в рабочем потоке
    if (client != nullptr) {
        client->close();
    }

    if (worker != nullptr) {
        try {
            if (worker->IsAlive) {
                worker->Join(2000);
                if (worker->IsAlive) worker->Abort();
            }
        }
        catch (...) {}
        worker = nullptr;
    }

    if (client != nullptr) {
        delete client;
        client = nullptr;
    }

    updateUi(false);
}

System::Void AlohaClient::ClientForm::client_run(System::Object^ obj) {
    ConnectInfo^ info = (ConnectInfo^)obj;
    WsaInitializer wsa;
    try {
        client = new ClientSocket(info->clientPort);
        msclr::interop::marshal_context context;
        client->connect(context.marshal_as<std::string>(info->host), info->serverPort);
        this->Invoke(gcnew log_update(this, &ClientForm::addLineLog),
            "Подключен к " + info->host + ":" + info->serverPort.ToString(),
            System::Drawing::Color::DarkMagenta);
        this->Invoke(gcnew ui_update(this, &ClientForm::updateUi), true);
    }
    catch (const std::exception& e) {
        is_connected = false;
        this->Invoke(gcnew ui_update(this, &ClientForm::updateUi), false);
        MessageBox::Show(
            "Connection failed: " + msclr::interop::marshal_as<System::String^>(e.what()),
            L"Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
        return;
    }

    try {
        while (is_connected) {
            std::string raw = client->receive(2048);
            if (raw.empty()) throw NetworkException("connection closed by server");
            this->Invoke(gcnew log_update(this, &ClientForm::addLineLog),
                msclr::interop::marshal_as<System::String^>(raw),
                System::Drawing::Color::Black);
        }
    }
    catch (const std::exception& e) {
        bool wasConnected = is_connected;
        is_connected = false;
        try { this->Invoke(gcnew ui_update(this, &ClientForm::updateUi), false); }
        catch (...) {}
        if (wasConnected) {
            MessageBox::Show(
                "Disconnected: " + msclr::interop::marshal_as<System::String^>(e.what()),
                L"Соединение", MessageBoxButtons::OK, MessageBoxIcon::Warning);
        }
    }
}

System::Void AlohaClient::ClientForm::addLine(const std::string& text, System::Drawing::Color color) {
    if (this->IsDisposed || !this->IsHandleCreated) return;
    System::String^ sysText = msclr::interop::marshal_as<System::String^>(text);
    if (this->InvokeRequired)
        this->BeginInvoke(gcnew log_update(this, &ClientForm::addLineLog), sysText, color);
    else
        addLineLog(sysText, color);
}

System::Void AlohaClient::ClientForm::addLineLog(System::String^ line, System::Drawing::Color color) {
    richTextChat->SelectionStart = richTextChat->TextLength;
    richTextChat->SelectionLength = 0;
    richTextChat->SelectionColor = color;
    richTextChat->AppendText(line + System::Environment::NewLine);
    richTextChat->ScrollToCaret();
}

System::Void AlohaClient::ClientForm::buttonConnect_Click(System::Object^ sender, System::EventArgs^ e) {
    if (is_connected) {
        client_shutdown();
        return;
    }

    int port;
    if (!System::Int32::TryParse(textBoxPort->Text, port) || port <= 0 || port > 65535) {
        MessageBox::Show(L"Некорректный порт (1..65535)", L"Ошибка ввода", MessageBoxButtons::OK, MessageBoxIcon::Warning);
        return;
    }
    if (System::String::IsNullOrWhiteSpace(textBoxServerIp->Text)) {
        MessageBox::Show(L"Введите IP-адрес сервера", L"Ошибка ввода", MessageBoxButtons::OK, MessageBoxIcon::Warning);
        return;
    }

    is_connected = true;
    updateUi(true);
    labelStatus->Text = L"STATUS: CONNECTING...";
    labelStatus->ForeColor = System::Drawing::Color::DarkOrange;

    worker = gcnew System::Threading::Thread(
        gcnew System::Threading::ParameterizedThreadStart(this, &ClientForm::client_run));
    ConnectInfo^ info = gcnew ConnectInfo(textBoxServerIp->Text, port, 8000);
    worker->IsBackground = true;
    worker->Start(info);
}

System::Void AlohaClient::ClientForm::buttonSendMsg_Click(System::Object^ sender, System::EventArgs^ e) {
    if (!is_connected || client == nullptr) {
        MessageBox::Show(L"Нет соединения с сервером", L"Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Warning);
        return;
    }
    if (System::String::IsNullOrWhiteSpace(textBoxSend->Text)) return;

    try {
        msclr::interop::marshal_context context;
        std::string message = context.marshal_as<std::string>(textBoxSend->Text);
        client->send(message);
        textBoxSend->Clear();
        textBoxSend->Focus();
    }
    catch (const std::exception& e) {
        MessageBox::Show(
            "Send failed: " + msclr::interop::marshal_as<System::String^>(e.what()),
            L"Ошибка отправки", MessageBoxButtons::OK, MessageBoxIcon::Error);
    }
}

System::Void AlohaClient::ClientForm::textBoxSend_TextChanged(System::Object^ sender, System::EventArgs^ e) {
    buttonSendMsg->Enabled = is_connected && !System::String::IsNullOrWhiteSpace(textBoxSend->Text);
}

System::Void AlohaClient::ClientForm::textBoxPort_KeyPress(System::Object^ sender, System::Windows::Forms::KeyPressEventArgs^ e) {
    if (!System::Char::IsDigit(e->KeyChar) && e->KeyChar != '\b')
        e->Handled = true;
}

System::Void AlohaClient::ClientForm::textBoxServerIp_KeyPress(System::Object^ sender, System::Windows::Forms::KeyPressEventArgs^ e) {
    if (!System::Char::IsDigit(e->KeyChar) && e->KeyChar != '.' && e->KeyChar != '\b')
        e->Handled = true;
}

System::Void AlohaClient::ClientForm::ClientForm_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e) {
    client_shutdown();
}