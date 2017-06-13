{*
 * \file    ICiFbEncActivator.h
 * \brief   Интерфейс модуля активации.
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
 * \brief	  Показатели состояния шифрования БД.
 *
 * \author	Leonid Belyasnik (leonid.belyasnik@cipher.kiev.ua)
 * \date	  19/01/2017
 }
DBEncInf = record
	PageSize : NativeUInt;      ///< Размер страницы БД.
	PagesCount : NativeUInt;    ///< Количество страниц БД.
	ProcessedPage : NativeUInt; ///< Номер последней обработанной фоновым шифрованием страницы.
	isEncrypted : Boolean;      ///< Флаг, указывающий зашифрована = 1 или нет = 0 БД.
	inProcess : Boolean;        ///< Флаг, указывающий на состояние процесса шифрования. 0 - выполнено, 1 - выполняется.
end;
{*
 * \brief	Интерфейс активатора шифрования БД.
 *
 * \author	Leonid Belyasnik (leonid.belyasnik@cipher.kiev.ua)
 * \date	19/01/2017
 }
ICiFbEncActivator = class
  /// \brief Установка ключа шифрования БД.
  /// \param [in] pKey - байтовый массив, содержащий ключ шифрования БД.
  /// \param [in] nKeyLen - двоичная длина ключа шифрования БД.
  /// \return Возвращается код ошибки.
  function SetKey(const pKey : Pointer; nKeyLen : NativeUInt) : Integer; virtual; stdcall; abstract;
  /// \brief Установка параметров подключения к БД.
	/// \param [in] pDBAlias - наименование альяса доступа к БД или путь к ней.
	/// \param [in] pDBLogin - логин доступа к БД (только для доступа к данным в Embedded режиме можно указать NIL).
	/// \param [in] pDBPass - пароль доступа к БД (только для доступа к данным в Embedded режиме можно указать NIL).
	/// \return Возвращается код ошибки.
  function SetDBAccess(const pDBAlias : PAnsiChar; const pDBLogin : PAnsiChar; const pDBPass : PAnsiChar) : Integer; virtual; stdcall; abstract;
  /// \brief Инициализация активатора.
  /// \return Возвращается код ошибки.
  function Activate : Integer; virtual; stdcall; abstract;
  /// \brief Запуск зашифровывания БД.
  /// \return Возвращается код ошибки.
  function Encrypt : Integer; virtual; stdcall; abstract;
  /// \brief Запуск расшифровывания БД.
  /// \return Возвращается код ошибки.
  function Decrypt : Integer; virtual; stdcall; abstract;
  /// \brief Запрос информации о статусе шифрования БД.
  /// \param [out] inf - структура для заполнения информацией.
  /// \return Возвращается код ошибки.
  function GetState(var inf : DBEncInf) : Integer; virtual; stdcall; abstract;
  /// \brief Запрос информации о статусе БД обращением к сервису.
  /// \param [in, out] pBuf - буфер, куда будет записана информация.
  /// \param [in, out] nBuf - размер буфера и в результате выполнения размер записанной информации.
  /// \return Возвращается код ошибки.
  function GetStateSVC(pBuf : PAnsiChar; var nBuf : NativeUInt) : Integer; virtual; stdcall; abstract;
  /// \brief Запрос информации о статусе выполнения операций Firebird (если была ошибка, то в буфер будет записана строка описания её).
  /// \param [in, out] pBuf - буфер, куда будет записана информация.
  /// \param [in, out] nBuf - размер буфера и в результате выполнения размер записанной информации.
  /// \return Возвращается код ошибки.
  function GetFBStat(pBuf : PAnsiChar; var nBuf : NativeUInt) : Integer; virtual; stdcall; abstract;
  /// \brief Удаление объекта активации ключа шифрования БД.
  /// \remarks Подлежит обязательному вызову.
  procedure Destroy; virtual; stdcall; abstract;
end;

TActivatorFunction = function( out ppEncActivator : ICiFbEncActivator) : Integer; stdcall;

const
  {*
   * \brief Перечень используемых идентификаторов ошибок.
   }
  Err_OK = 0;                 ///< Успешное выполнение операции, ошибка отсутствует.
	Err_IncorrectCall = -1;     ///< Ошибка вызова.
	Err_IncorrectSize = -2;     ///< Неправильный размер.
	Err_CantAllocMemory = -3;   ///< Невозможно выделить память.
	Err_AlreadyInSameState = -4;///< Уже находится в запрашиваемом состоянии.
	Err_FirebirdState = -5;     ///< Firebird вернул ошибку.
	Err_IncorrectParam = -6;    ///< Некорректный параметр при вызове метода или функции.
	Err_NullPointer = -7;       ///< Передан пустой указатель.

implementation

end.
