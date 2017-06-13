program apptest;

uses
  Vcl.Forms,
  Main in 'Main.pas' {Form2},
  CI.ICiFbEncActivator in 'CI.ICiFbEncActivator.pas';

{$R *.res}

begin
  Application.Initialize;
  Application.MainFormOnTaskbar := True;
  Application.CreateForm(TForm2, Form2);
  Application.Run;
end.
