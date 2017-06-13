unit Main;

interface

uses
  Winapi.Windows, Winapi.Messages, System.SysUtils, System.Variants, System.Classes, Vcl.Graphics,
  Vcl.Controls, Vcl.Forms, Vcl.Dialogs, Vcl.StdCtrls, Vcl.ExtCtrls,
  FireDAC.Phys.FBDef, FireDAC.Stan.Intf, FireDAC.Stan.Option,
  FireDAC.Stan.Error, FireDAC.UI.Intf, FireDAC.Phys.Intf, FireDAC.Stan.Def,
  FireDAC.Stan.Pool, FireDAC.Stan.Async, FireDAC.Phys, FireDAC.Phys.FB,
  FireDAC.Stan.Param, FireDAC.DatS, FireDAC.DApt.Intf, FireDAC.DApt,
  FireDAC.VCLUI.Wait, Data.DB, Vcl.DBCtrls, Vcl.Grids, Vcl.DBGrids,
  FireDAC.Comp.UI, FireDAC.Comp.DataSet, FireDAC.Comp.Client,
  FireDAC.Phys.IBBase, CI.ICiFbEncActivator;

type
  TForm2 = class(TForm)
    Panel1: TPanel;
    Button3: TButton;
    Button4: TButton;
    Button5: TButton;
    Button2: TButton;
    Button1: TButton;
    FDPhysFBDriverLink1: TFDPhysFBDriverLink;
    FDConnection1: TFDConnection;
    FDTransaction1: TFDTransaction;
    FDTable1: TFDTable;
    DataSource1: TDataSource;
    FDGUIxWaitCursor1: TFDGUIxWaitCursor;
    DBGrid1: TDBGrid;
    DBNavigator1: TDBNavigator;
    procedure Button1Click(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure Button5Click(Sender: TObject);
    procedure Button3Click(Sender: TObject);
    procedure Button4Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form2: TForm2;

const
// От куда-то у нас есть ключ 192 бита
key : array [0..23] of Byte =
(
  	$06,$DE,$81,$A1,$30,$55,$1A,$C9,
	  $9C,$A3,$42,$A9,$B6,$0F,$54,$F0,
	  $B6,$F9,$70,$18,$85,$04,$83,$BF
);

implementation

{$R *.dfm}

procedure TForm2.Button1Click(Sender: TObject);
var
pActivator : ICiFbEncActivator;
res : Integer;
CreateActivator: TActivatorFunction;
mHandle : HINST;
errmsg : array[0..511] of AnsiChar;
bufsize : NativeUInt;
begin
  // Загружаем модуль
	mHandle := LoadLibraryEx(PChar('C:\TESTAPP\CiFbEnc_x86.dll'), 0, LOAD_WITH_ALTERED_SEARCH_PATH);
  if mHandle = 0 then
  begin
     MessageBox(
        Application.Handle,
        'Модуль CiFbEnc_x86.dll не найден или рядом с ним нет fbclient.dll',
        'Загрузка модуля',
        MB_OK OR MB_ICONERROR);
      Exit;
  end;

  // Получаем активатор
  CreateActivator := GetProcAddress(mHandle, 'createCiFBEncActivator');
  if not Assigned(CreateActivator) then
  begin
     MessageBox(
        Application.Handle,
        'Не удалось получить из модуля CiFbEnc_x86.dll процедуру createCiFBEncActivator' +
        ' - попробуйте посмотреть, что пишет tdump.',
        'Загрузка модуля',
        MB_OK OR MB_ICONERROR);
		  Exit;
  end;
  pActivator := nil;
  res := CreateActivator(pActivator);
  if not Assigned(pActivator) then begin ShowMessage('ERROR CreateActivator!'); Exit; end;

  // Устанавливаем параметры подключения к БД
  res := pActivator.SetDBAccess('localhost:TESTDB', 'SYSDBA', 'masterkey');

  // Устанавливаем ключ активатору
  res := pActivator.SetKey(@key, Length(key));
  if Err_OK <> res then begin ShowMessage('ERROR SetKey!'); pActivator.Destroy; Exit; end;

  // Активируем доступ к ключу
	res := pActivator.Activate;
	if Err_OK <> res then
	begin
		bufsize := SizeOf(errmsg);
    ZeroMemory(@errmsg, bufsize);
		pActivator.GetFBStat(errmsg, bufsize);
		MessageBox(
			Application.Handle,
			PChar('ERROR Activate: ' + String(errmsg)),
			'Активация доступа к ключу',
			MB_OK OR MB_ICONERROR);

		pActivator.Destroy;
		Exit;
	end;

  // Подключаемся к БД
  try
    FDConnection1.Connected := True;
	  FDTable1.Active := True;
  except
    on E: EFDDBEngineException do begin
       MessageBox(
        Application.Handle,
        PChar('Не удалось подключиться к БД. ' + String(E.Message)),
        'Подключение к БД',
        MB_OK OR MB_ICONERROR);
    end;
  end;

  // Уничтожаем экземпляр активатора
  pActivator.Destroy;
  pActivator := nil;

  FreeLibrary(mHandle);
end;

procedure TForm2.Button2Click(Sender: TObject);
begin
  FDConnection1.Connected := False;
  FDTable1.Active := False;
end;

procedure TForm2.Button3Click(Sender: TObject);
var
pActivator : ICiFbEncActivator;
res : Integer;
CreateActivator: TActivatorFunction;
mHandle : HINST;
errmsg : array[0..511] of AnsiChar;
bufsize : NativeUInt;
begin
  // Загружаем модуль
	mHandle := LoadLibraryEx(PChar('C:\TESTAPP\CiFbEnc_x86.dll'), 0, LOAD_WITH_ALTERED_SEARCH_PATH);
  if mHandle = 0 then
  begin
     MessageBox(
        Application.Handle,
        'Модуль CiFbEnc_x86.dll не найден или рядом с ним нет fbclient.dll',
        'Загрузка модуля',
        MB_OK OR MB_ICONERROR);
      Exit;
  end;

  // Получаем активатор
  CreateActivator := GetProcAddress(mHandle, 'createCiFBEncActivator');
  if not Assigned(CreateActivator) then
  begin
     MessageBox(
        Application.Handle,
        'Не удалось получить из модуля CiFbEnc_x86.dll процедуру createCiFBEncActivator' +
        ' - попробуйте посмотреть, что пишет tdump.',
        'Загрузка модуля',
        MB_OK OR MB_ICONERROR);
		  Exit;
  end;
  pActivator := nil;
  res := CreateActivator(pActivator);
  if not Assigned(pActivator) then begin ShowMessage('ERROR CreateActivator!'); Exit; end;

  // Устанавливаем параметры подключения к БД
  res := pActivator.SetDBAccess('localhost:TESTDB', 'SYSDBA', 'masterkey');

  // Устанавливаем ключ активатору
  res := pActivator.SetKey(@key, Length(key));
  if Err_OK <> res then begin ShowMessage('ERROR SetKey!'); pActivator.Destroy; Exit; end;

  // Активируем доступ к ключу
	res := pActivator.Activate;
	if Err_OK <> res then
	begin
		bufsize := SizeOf(errmsg);
    ZeroMemory(@errmsg, bufsize);
		pActivator.GetFBStat(errmsg, bufsize);
		MessageBox(
			Application.Handle,
			PChar('ERROR Activate: ' + String(errmsg)),
			'Активация доступа к ключу',
			MB_OK OR MB_ICONERROR);

		pActivator.Destroy;
		Exit;
	end;

  // Зашифровуем БД
  res := pActivator.Encrypt;
  if Err_OK = res then
  begin
    ShowMessage('DB Encrypted!');
  end
  else
	begin
		bufsize := SizeOf(errmsg);
    ZeroMemory(@errmsg, bufsize);
		pActivator.GetFBStat(errmsg, bufsize);
		MessageBox(
			Application.Handle,
			PChar('ERROR Encrypt: ' + String(errmsg)),
			'Шифрованиеи БД',
			MB_OK OR MB_ICONERROR);

		pActivator.Destroy;
		Exit;
	end;

  // Уничтожаем экземпляр активатора
  pActivator.Destroy;
  pActivator := nil;

  FreeLibrary(mHandle);
end;

procedure TForm2.Button4Click(Sender: TObject);
var
pActivator : ICiFbEncActivator;
res : Integer;
CreateActivator: TActivatorFunction;
mHandle : HINST;
errmsg : array[0..511] of AnsiChar;
bufsize : NativeUInt;
begin
  // Загружаем модуль
	mHandle := LoadLibraryEx(PChar('C:\TESTAPP\CiFbEnc_x86.dll'), 0, LOAD_WITH_ALTERED_SEARCH_PATH);
  if mHandle = 0 then
  begin
     MessageBox(
        Application.Handle,
        'Модуль CiFbEnc_x86.dll не найден или рядом с ним нет fbclient.dll',
        'Загрузка модуля',
        MB_OK OR MB_ICONERROR);
      Exit;
  end;

  // Получаем активатор
  CreateActivator := GetProcAddress(mHandle, 'createCiFBEncActivator');
  if not Assigned(CreateActivator) then
  begin
     MessageBox(
        Application.Handle,
        'Не удалось получить из модуля CiFbEnc_x86.dll процедуру createCiFBEncActivator' +
        ' - попробуйте посмотреть, что пишет tdump.',
        'Загрузка модуля',
        MB_OK OR MB_ICONERROR);
		  Exit;
  end;
  pActivator := nil;
  res := CreateActivator(pActivator);
  if not Assigned(pActivator) then begin ShowMessage('ERROR CreateActivator!'); Exit; end;

  // Устанавливаем параметры подключения к БД
  res := pActivator.SetDBAccess('localhost:TESTDB', 'SYSDBA', 'masterkey');

  // Устанавливаем ключ активатору
  res := pActivator.SetKey(@key, Length(key));
  if Err_OK <> res then begin ShowMessage('ERROR SetKey!'); pActivator.Destroy; Exit; end;

  // Активируем доступ к ключу
	res := pActivator.Activate;
	if Err_OK <> res then
	begin
		bufsize := SizeOf(errmsg);
    ZeroMemory(@errmsg, bufsize);
		pActivator.GetFBStat(errmsg, bufsize);
		MessageBox(
			Application.Handle,
			PChar('ERROR Activate: ' + String(errmsg)),
			'Активация доступа к ключу',
			MB_OK OR MB_ICONERROR);

		pActivator.Destroy;
		Exit;
	end;

  // Расшифровуем БД
  res := pActivator.Decrypt;
  if Err_OK = res then
  begin
    ShowMessage('DB Decrypted!');
  end
  else
	begin
		bufsize := SizeOf(errmsg);
    ZeroMemory(@errmsg, bufsize);
		pActivator.GetFBStat(errmsg, bufsize);
		MessageBox(
			Application.Handle,
			PChar('ERROR Decrypt: ' + String(errmsg)),
			'Шифрование БД',
			MB_OK OR MB_ICONERROR);

		pActivator.Destroy;
		Exit;
	end;

  // Уничтожаем экземпляр активатора
  pActivator.Destroy;
  pActivator := nil;

  FreeLibrary(mHandle);
end;

procedure TForm2.Button5Click(Sender: TObject);
var
pActivator : ICiFbEncActivator;
res : Integer;
CreateActivator: TActivatorFunction;
mHandle : HINST;
stat_buf : array[0..1023] of AnsiChar;
bufsize : NativeUInt;
begin
  // Загружаем модуль
	mHandle := LoadLibraryEx(PChar('C:\TESTAPP\CiFbEnc_x86.dll'), 0, LOAD_WITH_ALTERED_SEARCH_PATH);
  if mHandle = 0 then
  begin
     MessageBox(
        Application.Handle,
        'Модуль CiFbEnc_x86.dll не найден или рядом с ним нет fbclient.dll',
        'Загрузка модуля',
        MB_OK OR MB_ICONERROR);
      Exit;
  end;

  // Получаем активатор
  CreateActivator := GetProcAddress(mHandle, 'createCiFBEncActivator');
  if not Assigned(CreateActivator) then
  begin
     MessageBox(
        Application.Handle,
        'Не удалось получить из модуля CiFbEnc_x86.dll процедуру createCiFBEncActivator' +
        ' - попробуйте посмотреть, что пишет tdump.',
        'Загрузка модуля',
        MB_OK OR MB_ICONERROR);
		  Exit;
  end;
  pActivator := nil;
  res := CreateActivator(pActivator);
  if not Assigned(pActivator) then begin ShowMessage('ERROR CreateActivator!'); Exit; end;

  // Устанавливаем параметры подключения к БД
  res := pActivator.SetDBAccess('localhost:TESTDB', 'SYSDBA', 'masterkey');

  // Посмотрим, что нам расскажет о БД сервис Firebird
  bufsize := SizeOf(stat_buf);
  ZeroMemory(@stat_buf, bufsize);
  res := pActivator.GetStateSVC(stat_buf, bufsize);

	if Err_OK = res then
  begin
       MessageBox(Application.Handle, PChar(String(stat_buf)),
       'Статус БД', MB_OK OR MB_ICONINFORMATION);
       end
  else
  begin
       MessageBox(Application.Handle, PChar(String(stat_buf)),
       'Ошибка', MB_OK OR MB_ICONERROR);
  end;

  // Уничтожаем экземпляр активатора
  pActivator.Destroy;
  pActivator := nil;

  FreeLibrary(mHandle);
end;

end.
