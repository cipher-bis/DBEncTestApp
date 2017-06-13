{*
 * \file    ICiFbEncActivator.h
 * \brief   ��������� ������ ���������.
 *
 * \author  Leonid Belyasnik (leonid.belyasnik@cipher.kiev.ua)
 * \date    19/01/2017
 *
 * \copyright Cipher BIS, LLC.
 }
unit CI.ICiFbEncActivator;

interface

type
{*
 * \brief	  ���������� ��������� ���������� ��.
 *
 * \author	Leonid Belyasnik (leonid.belyasnik@cipher.kiev.ua)
 * \date	  19/01/2017
 }
DBEncInf = record
	PageSize : NativeUInt;      ///< ������ �������� ��.
	PagesCount : NativeUInt;    ///< ���������� ������� ��.
	ProcessedPage : NativeUInt; ///< ����� ��������� ������������ ������� ����������� ��������.
	isEncrypted : Boolean;      ///< ����, ����������� ����������� = 1 ��� ��� = 0 ��.
	inProcess : Boolean;        ///< ����, ����������� �� ��������� �������� ����������. 0 - ���������, 1 - �����������.
end;
{*
 * \brief	��������� ���������� ���������� ��.
 *
 * \author	Leonid Belyasnik (leonid.belyasnik@cipher.kiev.ua)
 * \date	19/01/2017
 }
ICiFbEncActivator = class
  /// \brief ��������� ����� ���������� ��.
  /// \param [in] pKey - �������� ������, ���������� ���� ���������� ��.
  /// \param [in] nKeyLen - �������� ����� ����� ���������� ��.
  /// \return ������������ ��� ������.
  function SetKey(const pKey : Pointer; nKeyLen : NativeUInt) : Integer; virtual; stdcall; abstract;
  /// \brief ��������� ���������� ����������� � ��.
	/// \param [in] pDBAlias - ������������ ������ ������� � �� ��� ���� � ���.
	/// \param [in] pDBLogin - ����� ������� � �� (������ ��� ������� � ������ � Embedded ������ ����� ������� NIL).
	/// \param [in] pDBPass - ������ ������� � �� (������ ��� ������� � ������ � Embedded ������ ����� ������� NIL).
	/// \return ������������ ��� ������.
  function SetDBAccess(const pDBAlias : PAnsiChar; const pDBLogin : PAnsiChar; const pDBPass : PAnsiChar) : Integer; virtual; stdcall; abstract;
  /// \brief ������������� ����������.
  /// \return ������������ ��� ������.
  function Activate : Integer; virtual; stdcall; abstract;
  /// \brief ������ �������������� ��.
  /// \return ������������ ��� ������.
  function Encrypt : Integer; virtual; stdcall; abstract;
  /// \brief ������ ��������������� ��.
  /// \return ������������ ��� ������.
  function Decrypt : Integer; virtual; stdcall; abstract;
  /// \brief ������ ���������� � ������� ���������� ��.
  /// \param [out] inf - ��������� ��� ���������� �����������.
  /// \return ������������ ��� ������.
  function GetState(var inf : DBEncInf) : Integer; virtual; stdcall; abstract;
  /// \brief ������ ���������� � ������� �� ���������� � �������.
  /// \param [in, out] pBuf - �����, ���� ����� �������� ����������.
  /// \param [in, out] nBuf - ������ ������ � � ���������� ���������� ������ ���������� ����������.
  /// \return ������������ ��� ������.
  function GetStateSVC(pBuf : PAnsiChar; var nBuf : NativeUInt) : Integer; virtual; stdcall; abstract;
  /// \brief ������ ���������� � ������� ���������� �������� Firebird (���� ���� ������, �� � ����� ����� �������� ������ �������� �).
  /// \param [in, out] pBuf - �����, ���� ����� �������� ����������.
  /// \param [in, out] nBuf - ������ ������ � � ���������� ���������� ������ ���������� ����������.
  /// \return ������������ ��� ������.
  function GetFBStat(pBuf : PAnsiChar; var nBuf : NativeUInt) : Integer; virtual; stdcall; abstract;
  /// \brief �������� ������� ��������� ����� ���������� ��.
  /// \remarks �������� ������������� ������.
  procedure Destroy; virtual; stdcall; abstract;
end;

TActivatorFunction = function( out ppEncActivator : ICiFbEncActivator) : Integer; stdcall;

const
  {*
   * \brief �������� ������������ ��������������� ������.
   }
  Err_OK = 0;                 ///< �������� ���������� ��������, ������ �����������.
	Err_IncorrectCall = -1;     ///< ������ ������.
	Err_IncorrectSize = -2;     ///< ������������ ������.
	Err_CantAllocMemory = -3;   ///< ���������� �������� ������.
	Err_AlreadyInSameState = -4;///< ��� ��������� � ������������� ���������.
	Err_FirebirdState = -5;     ///< Firebird ������ ������.
	Err_IncorrectParam = -6;    ///< ������������ �������� ��� ������ ������ ��� �������.
	Err_NullPointer = -7;       ///< ������� ������ ���������.

implementation

end.
