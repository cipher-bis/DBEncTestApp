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

// �� ����-�� � ��� ���� ���� 192 ����
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
	// ��������� ������
 	std::unique_ptr<HINSTANCE__, decltype(&::FreeLibrary)> mHandle(
		::LoadLibraryEx(L"C:\\TESTAPP\\CiFbEnc_x86.dll", 0, LOAD_WITH_ALTERED_SEARCH_PATH),
		&::FreeLibrary);

	if (!mHandle)
	{
		MessageBox(
			NULL,
			L"������ CiFbEnc_x86.dll �� ������ ��� ����� � ��� ��� fbclient.dll",
			L"�������� ������",
			MB_OK|MB_ICONERROR);
		return;
	}

	// �������� ���������
	typedef CALL_CONV int(__stdcall *CREATEFUNCPTR)(CI::ICiFbEncActivator**);
	CREATEFUNCPTR GetActivator = (CREATEFUNCPTR)::GetProcAddress(mHandle.get(), "createCiFBEncActivator");
	if (!GetActivator)
	{
		MessageBox(
			NULL,
			L"�� ������� �������� �� ������ CiFbEnc_x86.dll ��������� createCiFBEncActivator"
			 " - ���������� ����������, ��� ����� tdump.",
			L"�������� ������",
			MB_OK|MB_ICONERROR);
		return;
	}

	CI::ICiFbEncActivator* pActivator = NULL;
	GetActivator(&pActivator);
	if (!pActivator) { ShowMessage("ERROR GetActivator!"); return; }

	// ������������� ��������� ����������� � ��
	pActivator->SetDBAccess("localhost:TESTDB", "SYSDBA", "masterkey");

	// ������������� ���� ����������
	int res = pActivator->SetKey(&key, sizeof(key));
	if (Err_OK != res) { ShowMessage("ERROR SetKey!"); pActivator->Destroy(); return; }

	// ���������� ������ � �����
	res = pActivator->Activate();
	if (Err_OK != res)
	{
		// ������������ ������
		char errmsg[512] = {0};
		size_t esize = sizeof(errmsg);
		pActivator->GetFBStat(errmsg, esize);
		String sErrMsg = "ERROR Activate: " + String(errmsg);
		MessageBox(
			NULL,
			sErrMsg.w_str(),
			L"��������� ������� � �����",
			MB_OK|MB_ICONERROR);
		pActivator->Destroy();
		return;
	}

	// ������������ � ��
	try
	{
		FDConnection1->Connected = true;
		FDTable1->Active = true;
	}
	catch(EFDDBEngineException &e)
	{
		String sErrMsg = L"�� ������� ������������ � ��. " + e.Message;
        MessageBox(
			NULL,
			sErrMsg.w_str(),
			L"����������� � ��",
			MB_OK|MB_ICONERROR);
	}

	// ���������� ��������� ����������
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
	// ��������� ������
	std::unique_ptr<HINSTANCE__, decltype(&::FreeLibrary)> mHandle(
		::LoadLibraryEx(L"C:\\TESTAPP\\CiFbEnc_x86.dll", 0, LOAD_WITH_ALTERED_SEARCH_PATH),
		&::FreeLibrary);

	if (!mHandle)
	{
		MessageBox(
			NULL,
			L"������ CiFbEnc_x86.dll �� ������ ��� ����� � ��� ��� fbclient.dll",
			L"�������� ������",
			MB_OK|MB_ICONERROR);
		return;
	}

	// �������� ���������
	typedef CALL_CONV int(__stdcall *CREATEFUNCPTR)(CI::ICiFbEncActivator**);
	CREATEFUNCPTR GetActivator = (CREATEFUNCPTR)::GetProcAddress(mHandle.get(), "createCiFBEncActivator");
	if (!GetActivator)
	{
		MessageBox(
			NULL,
			L"�� ������� �������� �� ������ CiFbEnc_x86.dll ��������� createCiFBEncActivator"
			 " - ���������� ����������, ��� ����� tdump.",
			L"�������� ������",
			MB_OK|MB_ICONERROR);
		return;
	}

	CI::ICiFbEncActivator* pActivator = NULL;
	GetActivator(&pActivator);
	if (!pActivator) { ShowMessage("ERROR GetActivator!"); return; }

	// ������������� ��������� ����������� � ��
	pActivator->SetDBAccess("localhost:TESTDB", "SYSDBA", "masterkey");

	// ���������, ��� ��� ��������� � �� ������ Firebird
	char stat_buf[1024] = { 0 };
	size_t bufsize = sizeof(stat_buf);

	int res = pActivator->GetStateSVC(stat_buf, bufsize);
	String sStatMsg = (String)stat_buf;
	if (Err_OK == res)
	{
		MessageBox(NULL, sStatMsg.c_str(), L"������ ��", MB_OK|MB_ICONINFORMATION);
	}
	else
	{
		String sErrMsg = L"ERROR GetStateSVC: " + sStatMsg;
		MessageBox(
			NULL,
			sErrMsg.c_str(),
			L"������ ��",
			MB_OK|MB_ICONERROR);

	}

	// ���������� ��������� ����������
	pActivator->Destroy();
	pActivator = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button3Click(TObject *Sender)
{
	// ��������� ������
	std::unique_ptr<HINSTANCE__, decltype(&::FreeLibrary)> mHandle(
		::LoadLibraryEx(L"C:\\TESTAPP\\CiFbEnc_x86.dll", 0, LOAD_WITH_ALTERED_SEARCH_PATH),
		&::FreeLibrary);

	if (!mHandle)
	{
		MessageBox(
			NULL,
			L"������ CiFbEnc_x86.dll �� ������ ��� ����� � ��� ��� fbclient.dll",
			L"�������� ������",
			MB_OK|MB_ICONERROR);
		return;
	}

	// �������� ���������
	typedef CALL_CONV int(__stdcall *CREATEFUNCPTR)(CI::ICiFbEncActivator**);
	CREATEFUNCPTR GetActivator = (CREATEFUNCPTR)::GetProcAddress(mHandle.get(), "createCiFBEncActivator");
	if (!GetActivator)
	{
		MessageBox(
			NULL,
			L"�� ������� �������� �� ������ CiFbEnc_x86.dll ��������� createCiFBEncActivator"
			 " - ���������� ����������, ��� ����� tdump.",
			L"�������� ������",
			MB_OK|MB_ICONERROR);
		return;
	}

	CI::ICiFbEncActivator* pActivator = NULL;
	GetActivator(&pActivator);
	if (!pActivator) { ShowMessage("ERROR GetActivator!"); return; }

	// ������������� ��������� ����������� � ��
	pActivator->SetDBAccess("localhost:TESTDB", "SYSDBA", "masterkey");

	// ������������� ���� ����������
	int res = pActivator->SetKey(&key, sizeof(key));
	if (Err_OK != res) { ShowMessage("ERROR SetKey!"); pActivator->Destroy(); return; }

	// ���������� ������ � �����
	res = pActivator->Activate();
	if (Err_OK != res)
	{
		// ������������ ������
		char errmsg[512] = {0};
		size_t esize = sizeof(errmsg);
		pActivator->GetFBStat(errmsg, esize);
		String sErrMsg = "ERROR Activate: " + String(errmsg);
		MessageBox(
			NULL,
			sErrMsg.w_str(),
			L"��������� ������� � �����",
			MB_OK|MB_ICONERROR);
		pActivator->Destroy();
		return;
	}

	// ����������� ��
	res = pActivator->Encrypt();
	if (Err_OK == res)
	{
		ShowMessage("DB Encrypted!");
	}
	else
	{
		// ������������ ������
		char errmsg[512] = {0};
		size_t esize = sizeof(errmsg);
		pActivator->GetFBStat(errmsg, esize);
		String sErrMsg = "ERROR Encrypt: " + String(errmsg);
		MessageBox(
			NULL,
			sErrMsg.w_str(),
			L"���������� ��",
			MB_OK|MB_ICONERROR);
		pActivator->Destroy();
		return;
	}

	// ���������� ��������� ����������
	pActivator->Destroy();
	pActivator = NULL;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button4Click(TObject *Sender)
{
	// ��������� ������
	std::unique_ptr<HINSTANCE__, decltype(&::FreeLibrary)> mHandle(
		::LoadLibraryEx(L"C:\\TESTAPP\\CiFbEnc_x86.dll", 0, LOAD_WITH_ALTERED_SEARCH_PATH),
		&::FreeLibrary);

	if (!mHandle)
	{
		MessageBox(
			NULL,
			L"������ CiFbEnc_x86.dll �� ������ ��� ����� � ��� ��� fbclient.dll",
			L"�������� ������",
			MB_OK|MB_ICONERROR);
		return;
	}

	// �������� ���������
	typedef CALL_CONV int(__stdcall *CREATEFUNCPTR)(CI::ICiFbEncActivator**);
	CREATEFUNCPTR GetActivator = (CREATEFUNCPTR)::GetProcAddress(mHandle.get(), "createCiFBEncActivator");
	if (!GetActivator)
	{
		MessageBox(
			NULL,
			L"�� ������� �������� �� ������ CiFbEnc_x86.dll ��������� createCiFBEncActivator"
			 " - ���������� ����������, ��� ����� tdump.",
			L"�������� ������",
			MB_OK|MB_ICONERROR);
		return;
	}

	CI::ICiFbEncActivator* pActivator = NULL;
	GetActivator(&pActivator);
	if (!pActivator) { ShowMessage("ERROR GetActivator!"); return; }

	// ������������� ��������� ����������� � ��
	pActivator->SetDBAccess("localhost:TESTDB", "SYSDBA", "masterkey");

	// ������������� ���� ����������
	int res = pActivator->SetKey(&key, sizeof(key));
	if (Err_OK != res) { ShowMessage("ERROR SetKey!"); pActivator->Destroy(); return; }

	// ���������� ������ � �����
	res = pActivator->Activate();
	if (Err_OK != res)
	{
		// ������������ ������
		char errmsg[512] = {0};
		size_t esize = sizeof(errmsg);
		pActivator->GetFBStat(errmsg, esize);
		String sErrMsg = "ERROR Activate: " + String(errmsg);
		MessageBox(
			NULL,
			sErrMsg.w_str(),
			L"��������� ������� � �����",
			MB_OK|MB_ICONERROR);
		pActivator->Destroy();
		return;
	}

	// ������������ ��
	res = pActivator->Decrypt();
	if (Err_OK == res)
	{
		ShowMessage("DB Decrypted!");
	}
	else
	{
		// ������������ ������
		char errmsg[512] = {0};
		size_t esize = sizeof(errmsg);
		pActivator->GetFBStat(errmsg, esize);
		String sErrMsg = "ERROR Decrypt: " + String(errmsg);
		MessageBox(
			NULL,
			sErrMsg.w_str(),
			L"���������� ��",
			MB_OK|MB_ICONERROR);
		pActivator->Destroy();
		return;
	}

	// ���������� ��������� ����������
	pActivator->Destroy();
	pActivator = NULL;
}
//---------------------------------------------------------------------------

