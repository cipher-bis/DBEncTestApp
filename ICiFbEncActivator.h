/**
 * \file    ICiFbEncActivator.h
 * \brief   ��������� ������ ���������.
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

/// \brief �������� ������������ ��������������� ������.
enum CIErrors
{
	Err_OK = 0,							///< �������� ���������� ��������, ������ �����������.
	Err_IncorrectCall = -1,				///< ������ ������.
	Err_IncorrectSize = -2,				///< ������������ ������.
	Err_CantAllocMemory = -3,			///< ���������� �������� ������.
	Err_AlreadyInSameState = -4,		///< ��� ��������� � ������������� ���������.
	Err_FirebirdState = -5,				///< Firebird ������ ������.
	Err_IncorrectParam = -6,			///< ������������ �������� ��� ������ ������ ��� �������.
	Err_NullPointer = -7				///< ������� ������ ���������.
};

namespace CI {
#pragma pack(push,1)
	/**
	* \brief	���������� ��������� ���������� ��.
	*
	* \author	Leonid Belyasnik (leonid.belyasnik@cipher.kiev.ua)
	* \date	19/01/2017
	*/
	struct DBEncInf
	{
		size_t PageSize;			///< ������ �������� ��.
		size_t PagesCount;			///< ���������� ������� ��.
		size_t ProcessedPage;		///< ����� ��������� ������������ ������� ����������� ��������.
		bool isEncrypted;			///< ����, ����������� ����������� = 1 ��� ��� = 0 ��.
		bool inProcess;				///< ����, ����������� �� ��������� �������� ����������. 0 - ���������, 1 - �����������.
	};
#pragma pack(pop)
	/**
	* \brief	��������� ���������� ���������� ��.
	*
	* \author	Leonid Belyasnik (leonid.belyasnik@cipher.kiev.ua)
	* \date	19/01/2017
	*/
	class CI_LIB_EXP NO_VTBL ICiFbEncActivator
	{
	public:
		/// \brief ��������� ����� ���������� ��.
		/// \param [in] pKey - �������� ������, ���������� ���� ���������� ��.
		/// \param [in] cbitKeyLen - �������� ����� ����� ���������� ��.
		/// \return ������������ ��� ������.
		virtual int32_t CALL_CONV SetKey (const void *pKey, size_t nKeyLen) = 0;
		/// \brief ��������� ���������� ����������� � ��.
		/// \param [in] pDBAlias - ������������ ������ ������� � �� ��� ���� � ���.
		/// \param [in] pDBLogin - ����� ������� � �� (������ ��� ������� � ������ � Embedded ������ ����� ������� NULL).
		/// \param [in] pDBPass - ������ ������� � �� (������ ��� ������� � ������ � Embedded ������ ����� ������� NULL).
		/// \return ������������ ��� ������.
		virtual int32_t CALL_CONV SetDBAccess(const char* pDBAlias, const char* pDBLogin, const char* pDBPass) = 0;
		/// \brief ������������� ����������.
		/// \return ������������ ��� ������.
		virtual int32_t CALL_CONV Activate(void) = 0;
		/// \brief ������ �������������� ��.
		/// \return ������������ ��� ������.
		virtual int32_t CALL_CONV Encrypt(void) = 0;
		/// \brief ������ ��������������� ��.
		/// \return ������������ ��� ������.
		virtual int32_t CALL_CONV Decrypt(void) = 0;
		/// \brief ������ ���������� � ������� ���������� ��.
		/// \param [out] inf - ��������� ��� ���������� �����������.
		/// \return ������������ ��� ������.
		virtual int32_t CALL_CONV GetState(DBEncInf& inf) = 0;
		/// \brief ������ ���������� � ������� �� ���������� � �������.
		/// \param [in, out] buf - �����, ���� ����� �������� ����������.
		/// \param [in, out] buf_size - ������ ������ � � ���������� ���������� ������ ���������� ����������.
		/// \return ������������ ��� ������.
		virtual int32_t CALL_CONV GetStateSVC(char* pBuf, size_t& nBuf) = 0;
		/// \brief ������ ���������� � ������� ���������� �������� Firebird (���� ���� ������, �� � ����� ����� �������� ������ �������� �).
		/// \param [in, out] buf - �����, ���� ����� �������� ����������.
		/// \param [in, out] buf_size - ������ ������ � � ���������� ���������� ������ ���������� ����������.
		/// \return ������������ ��� ������.
		virtual int32_t CALL_CONV GetFBStat(char* pBuf, size_t& nBuf) = 0;
		/// \brief �������� ������� ��������� ����� ���������� ��.
		/// \remarks �������� ������������� ������.
		virtual void CALL_CONV Destroy(void) = 0;
	};
}

#endif