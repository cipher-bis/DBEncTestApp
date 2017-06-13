//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "uMain.h"
#include "ICiFbEncActivator.h"
#include <memory>

using namespace std;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;

// От куда-то у нас есть ключ 192 бита
const uint8_t key[] =
{
	0x06,0xDE,0x81,0xA1,0x30,0x55,0x1A,0xC9,
	0x9C,0xA3,0x42,0xA9,0xB6,0x0F,0x54,0xF0,
	0xB6,0xF9,0x70,0x18,0x85,0x04,0x83,0xBF
};
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
	// Загружаем модуль
 	std::unique_ptr<HINSTANCE__, decltype(&::FreeLibrary)> mHandle(
		::LoadLibraryEx(L"C:\\TESTAPP\\CiFbEnc_x86.dll", 0, LOAD_WITH_ALTERED_SEARCH_PATH),
		&::FreeLibrary);

	if (!mHandle)
	{
		MessageBox(
			NULL,
			L"Модуль CiFbEnc_x86.dll не найден или рядом с ним нет fbclient.dll",
			L"Загрузка модуля",
			MB_OK|MB_ICONERROR);
		return;
	}

	// Получаем активатор
	typedef CALL_CONV int(__stdcall *CREATEFUNCPTR)(CI::ICiFbEncActivator**);
	CREATEFUNCPTR GetActivator = (CREATEFUNCPTR)::GetProcAddress(mHandle.get(), "createCiFBEncActivator");
	if (!GetActivator)
	{
		MessageBox(
			NULL,
			L"Не удалось получить из модуля CiFbEnc_x86.dll процедуру createCiFBEncActivator"
			 " - попробуйте посмотреть, что пишет tdump.",
			L"Загрузка модуля",
			MB_OK|MB_ICONERROR);
		return;
	}

	CI::ICiFbEncActivator* pActivator = NULL;
	GetActivator(&pActivator);
	if (!pActivator) { ShowMessage("ERROR GetActivator!"); return; }

	// Устанавливаем параметры подключения к БД
	pActivator->SetDBAccess("localhost:TESTDB", "SYSDBA", "masterkey");

	// Устанавливаем ключ активатору
	int res = pActivator->SetKey(&key, sizeof(key));
	if (Err_OK != res) { ShowMessage("ERROR SetKey!"); pActivator->Destroy(); return; }

	// Активируем доступ к ключу
	res = pActivator->Activate();
	if (Err_OK != res)
	{
		// Обрабатываем ошибку
		char errmsg[512] = {0};
		size_t esize = sizeof(errmsg);
		pActivator->GetFBStat(errmsg, esize);
		String sErrMsg = "ERROR Activate: " + String(errmsg);
		MessageBox(
			NULL,
			sErrMsg.w_str(),
			L"Активация доступа к ключу",
			MB_OK|MB_ICONERROR);
		pActivator->Destroy();
		return;
	}

	// Подключаемся к БД
	try
	{
		FDConnection1->Connected = true;
		FDTable1->Active = true;
	}
	catch(EFDDBEngineException &e)
	{
		String sErrMsg = L"Не удалось подключиться к БД. " + e.Message;
        MessageBox(
			NULL,
			sErrMsg.w_str(),
			L"Подключение к БД",
			MB_OK|MB_ICONERROR);
	}

	// Уничтожаем экземпляр активатора
	pActivator->Destroy();
	pActivator = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button2Click(TObject *Sender)
{
	FDTable1->Active = false;
	FDConnection1->Connected = false;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button5Click(TObject *Sender)
{
	// Загружаем модуль
	std::unique_ptr<HINSTANCE__, decltype(&::FreeLibrary)> mHandle(
		::LoadLibraryEx(L"C:\\TESTAPP\\CiFbEnc_x86.dll", 0, LOAD_WITH_ALTERED_SEARCH_PATH),
		&::FreeLibrary);

	if (!mHandle)
	{
		MessageBox(
			NULL,
			L"Модуль CiFbEnc_x86.dll не найден или рядом с ним нет fbclient.dll",
			L"Загрузка модуля",
			MB_OK|MB_ICONERROR);
		return;
	}

	// Получаем активатор
	typedef CALL_CONV int(__stdcall *CREATEFUNCPTR)(CI::ICiFbEncActivator**);
	CREATEFUNCPTR GetActivator = (CREATEFUNCPTR)::GetProcAddress(mHandle.get(), "createCiFBEncActivator");
	if (!GetActivator)
	{
		MessageBox(
			NULL,
			L"Не удалось получить из модуля CiFbEnc_x86.dll процедуру createCiFBEncActivator"
			 " - попробуйте посмотреть, что пишет tdump.",
			L"Загрузка модуля",
			MB_OK|MB_ICONERROR);
		return;
	}

	CI::ICiFbEncActivator* pActivator = NULL;
	GetActivator(&pActivator);
	if (!pActivator) { ShowMessage("ERROR GetActivator!"); return; }

	// Устанавливаем параметры подключения к БД
	pActivator->SetDBAccess("localhost:TESTDB", "SYSDBA", "masterkey");

	// Посмотрим, что нам расскажет о БД сервис Firebird
	char stat_buf[1024] = { 0 };
	size_t bufsize = sizeof(stat_buf);

	int res = pActivator->GetStateSVC(stat_buf, bufsize);
	String sStatMsg = (String)stat_buf;
	if (Err_OK == res)
	{
		MessageBox(NULL, sStatMsg.c_str(), L"Статус БД", MB_OK|MB_ICONINFORMATION);
	}
	else
	{
		String sErrMsg = L"ERROR GetStateSVC: " + sStatMsg;
		MessageBox(
			NULL,
			sErrMsg.c_str(),
			L"Статус БД",
			MB_OK|MB_ICONERROR);

	}

	// Уничтожаем экземпляр активатора
	pActivator->Destroy();
	pActivator = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button3Click(TObject *Sender)
{
	// Загружаем модуль
	std::unique_ptr<HINSTANCE__, decltype(&::FreeLibrary)> mHandle(
		::LoadLibraryEx(L"C:\\TESTAPP\\CiFbEnc_x86.dll", 0, LOAD_WITH_ALTERED_SEARCH_PATH),
		&::FreeLibrary);

	if (!mHandle)
	{
		MessageBox(
			NULL,
			L"Модуль CiFbEnc_x86.dll не найден или рядом с ним нет fbclient.dll",
			L"Загрузка модуля",
			MB_OK|MB_ICONERROR);
		return;
	}

	// Получаем активатор
	typedef CALL_CONV int(__stdcall *CREATEFUNCPTR)(CI::ICiFbEncActivator**);
	CREATEFUNCPTR GetActivator = (CREATEFUNCPTR)::GetProcAddress(mHandle.get(), "createCiFBEncActivator");
	if (!GetActivator)
	{
		MessageBox(
			NULL,
			L"Не удалось получить из модуля CiFbEnc_x86.dll процедуру createCiFBEncActivator"
			 " - попробуйте посмотреть, что пишет tdump.",
			L"Загрузка модуля",
			MB_OK|MB_ICONERROR);
		return;
	}

	CI::ICiFbEncActivator* pActivator = NULL;
	GetActivator(&pActivator);
	if (!pActivator) { ShowMessage("ERROR GetActivator!"); return; }

	// Устанавливаем параметры подключения к БД
	pActivator->SetDBAccess("localhost:TESTDB", "SYSDBA", "masterkey");

	// Устанавливаем ключ активатору
	int res = pActivator->SetKey(&key, sizeof(key));
	if (Err_OK != res) { ShowMessage("ERROR SetKey!"); pActivator->Destroy(); return; }

	// Активируем доступ к ключу
	res = pActivator->Activate();
	if (Err_OK != res)
	{
		// Обрабатываем ошибку
		char errmsg[512] = {0};
		size_t esize = sizeof(errmsg);
		pActivator->GetFBStat(errmsg, esize);
		String sErrMsg = "ERROR Activate: " + String(errmsg);
		MessageBox(
			NULL,
			sErrMsg.w_str(),
			L"Активация доступа к ключу",
			MB_OK|MB_ICONERROR);
		pActivator->Destroy();
		return;
	}

	// Зашифровуем БД
	res = pActivator->Encrypt();
	if (Err_OK == res)
	{
		ShowMessage("DB Encrypted!");
	}
	else
	{
		// Обрабатываем ошибку
		char errmsg[512] = {0};
		size_t esize = sizeof(errmsg);
		pActivator->GetFBStat(errmsg, esize);
		String sErrMsg = "ERROR Encrypt: " + String(errmsg);
		MessageBox(
			NULL,
			sErrMsg.w_str(),
			L"Шифрование БД",
			MB_OK|MB_ICONERROR);
		pActivator->Destroy();
		return;
	}

	// Уничтожаем экземпляр активатора
	pActivator->Destroy();
	pActivator = NULL;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button4Click(TObject *Sender)
{
	// Загружаем модуль
	std::unique_ptr<HINSTANCE__, decltype(&::FreeLibrary)> mHandle(
		::LoadLibraryEx(L"C:\\TESTAPP\\CiFbEnc_x86.dll", 0, LOAD_WITH_ALTERED_SEARCH_PATH),
		&::FreeLibrary);

	if (!mHandle)
	{
		MessageBox(
			NULL,
			L"Модуль CiFbEnc_x86.dll не найден или рядом с ним нет fbclient.dll",
			L"Загрузка модуля",
			MB_OK|MB_ICONERROR);
		return;
	}

	// Получаем активатор
	typedef CALL_CONV int(__stdcall *CREATEFUNCPTR)(CI::ICiFbEncActivator**);
	CREATEFUNCPTR GetActivator = (CREATEFUNCPTR)::GetProcAddress(mHandle.get(), "createCiFBEncActivator");
	if (!GetActivator)
	{
		MessageBox(
			NULL,
			L"Не удалось получить из модуля CiFbEnc_x86.dll процедуру createCiFBEncActivator"
			 " - попробуйте посмотреть, что пишет tdump.",
			L"Загрузка модуля",
			MB_OK|MB_ICONERROR);
		return;
	}

	CI::ICiFbEncActivator* pActivator = NULL;
	GetActivator(&pActivator);
	if (!pActivator) { ShowMessage("ERROR GetActivator!"); return; }

	// Устанавливаем параметры подключения к БД
	pActivator->SetDBAccess("localhost:TESTDB", "SYSDBA", "masterkey");

	// Устанавливаем ключ активатору
	int res = pActivator->SetKey(&key, sizeof(key));
	if (Err_OK != res) { ShowMessage("ERROR SetKey!"); pActivator->Destroy(); return; }

	// Активируем доступ к ключу
	res = pActivator->Activate();
	if (Err_OK != res)
	{
		// Обрабатываем ошибку
		char errmsg[512] = {0};
		size_t esize = sizeof(errmsg);
		pActivator->GetFBStat(errmsg, esize);
		String sErrMsg = "ERROR Activate: " + String(errmsg);
		MessageBox(
			NULL,
			sErrMsg.w_str(),
			L"Активация доступа к ключу",
			MB_OK|MB_ICONERROR);
		pActivator->Destroy();
		return;
	}

	// Расшифровуем БД
	res = pActivator->Decrypt();
	if (Err_OK == res)
	{
		ShowMessage("DB Decrypted!");
	}
	else
	{
		// Обрабатываем ошибку
		char errmsg[512] = {0};
		size_t esize = sizeof(errmsg);
		pActivator->GetFBStat(errmsg, esize);
		String sErrMsg = "ERROR Decrypt: " + String(errmsg);
		MessageBox(
			NULL,
			sErrMsg.w_str(),
			L"Шифрование БД",
			MB_OK|MB_ICONERROR);
		pActivator->Destroy();
		return;
	}

	// Уничтожаем экземпляр активатора
	pActivator->Destroy();
	pActivator = NULL;
}
//---------------------------------------------------------------------------

