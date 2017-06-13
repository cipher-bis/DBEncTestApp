/**
 * \file    ICiFbEncActivator.h
 * \brief   Интерфейс модуля активации.
 *
 * \author  Leonid Belyasnik (leonid.belyasnik@cipher.kiev.ua)
 * \date    19/01/2017
 *
 * \copyright Cipher BIS, LLC.
 */
#ifndef _ICiFbEncActivator_H_
#define _ICiFbEncActivator_H_

#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__)) && !defined(__MINGW32__)
	#define CALL_CONV   __stdcall
	#define CI_LIB_EXP __declspec(dllexport)
	#define NO_VTBL __declspec(novtable)
#else
	#define CALL_CONV
	#define CI_LIB_EXP
	#define NO_VTBL
#endif

#define CI_LIB_API extern "C" CI_LIB_EXP

/// \brief Перечень используемых идентификаторов ошибок.
enum CIErrors
{
	Err_OK = 0,							///< Успешное выполнение операции, ошибка отсутствует.
	Err_IncorrectCall = -1,				///< Ошибка вызова.
	Err_IncorrectSize = -2,				///< Неправильный размер.
	Err_CantAllocMemory = -3,			///< Невозможно выделить память.
	Err_AlreadyInSameState = -4,		///< Уже находится в запрашиваемом состоянии.
	Err_FirebirdState = -5,				///< Firebird вернул ошибку.
	Err_IncorrectParam = -6,			///< Некорректный параметр при вызове метода или функции.
	Err_NullPointer = -7				///< Передан пустой указатель.
};

namespace CI {
#pragma pack(push,1)
	/**
	* \brief	Показатели состояния шифрования БД.
	*
	* \author	Leonid Belyasnik (leonid.belyasnik@cipher.kiev.ua)
	* \date	19/01/2017
	*/
	struct DBEncInf
	{
		size_t PageSize;			///< Размер страницы БД.
		size_t PagesCount;			///< Количество страниц БД.
		size_t ProcessedPage;		///< Номер последней обработанной фоновым шифрованием страницы.
		bool isEncrypted;			///< Флаг, указывающий зашифрована = 1 или нет = 0 БД.
		bool inProcess;				///< Флаг, указывающий на состояние процесса шифрования. 0 - выполнено, 1 - выполняется.
	};
#pragma pack(pop)
	/**
	* \brief	Интерфейс активатора шифрования БД.
	*
	* \author	Leonid Belyasnik (leonid.belyasnik@cipher.kiev.ua)
	* \date	19/01/2017
	*/
	class CI_LIB_EXP NO_VTBL ICiFbEncActivator
	{
	public:
		/// \brief Установка ключа шифрования БД.
		/// \param [in] pKey - байтовый массив, содержащий ключ шифрования БД.
		/// \param [in] cbitKeyLen - двоичная длина ключа шифрования БД.
		/// \return Возвращается код ошибки.
		virtual int32_t CALL_CONV SetKey (const void *pKey, size_t nKeyLen) = 0;
		/// \brief Установка параметров подключения к БД.
		/// \param [in] pDBAlias - наименование альяса доступа к БД или путь к ней.
		/// \param [in] pDBLogin - логин доступа к БД (только для доступа к данным в Embedded режиме можно указать NULL).
		/// \param [in] pDBPass - пароль доступа к БД (только для доступа к данным в Embedded режиме можно указать NULL).
		/// \return Возвращается код ошибки.
		virtual int32_t CALL_CONV SetDBAccess(const char* pDBAlias, const char* pDBLogin, const char* pDBPass) = 0;
		/// \brief Инициализация активатора.
		/// \return Возвращается код ошибки.
		virtual int32_t CALL_CONV Activate(void) = 0;
		/// \brief Запуск зашифровывания БД.
		/// \return Возвращается код ошибки.
		virtual int32_t CALL_CONV Encrypt(void) = 0;
		/// \brief Запуск расшифровывания БД.
		/// \return Возвращается код ошибки.
		virtual int32_t CALL_CONV Decrypt(void) = 0;
		/// \brief Запрос информации о статусе шифрования БД.
		/// \param [out] inf - структура для заполнения информацией.
		/// \return Возвращается код ошибки.
		virtual int32_t CALL_CONV GetState(DBEncInf& inf) = 0;
		/// \brief Запрос информации о статусе БД обращением к сервису.
		/// \param [in, out] buf - буфер, куда будет записана информация.
		/// \param [in, out] buf_size - размер буфера и в результате выполнения размер записанной информации.
		/// \return Возвращается код ошибки.
		virtual int32_t CALL_CONV GetStateSVC(char* pBuf, size_t& nBuf) = 0;
		/// \brief Запрос информации о статусе выполнения операций Firebird (если была ошибка, то в буфер будет записана строка описания её).
		/// \param [in, out] buf - буфер, куда будет записана информация.
		/// \param [in, out] buf_size - размер буфера и в результате выполнения размер записанной информации.
		/// \return Возвращается код ошибки.
		virtual int32_t CALL_CONV GetFBStat(char* pBuf, size_t& nBuf) = 0;
		/// \brief Удаление объекта активации ключа шифрования БД.
		/// \remarks Подлежит обязательному вызову.
		virtual void CALL_CONV Destroy(void) = 0;
	};
}

#endif