// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#ifndef SICRYPT_STDAFX_H_INCLUDED
#define SICRYPT_STDAFX_H_INCLUDED


#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

#ifndef WINVER				// Allow use of features specific to Windows 95 and Windows NT 4 or later.
#define WINVER _WIN32_WINNT_WIN7		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif

#include <afx.h>
#include <afxwin.h>         // MFC core and standard components

#pragma warning (push)
#pragma warning (disable: 4267)
#pragma warning (disable: 4231)

#define CRYPTOPP_MAINTAIN_BACKWARDS_COMPATIBILITY
#include <crypto5/secblock.h>
#include <crypto5/aes.h>
#include <crypto5/sha.h>
#include <crypto5/hmac.h>
#include <crypto5/modes.h>
#include <crypto5/osrng.h>
#include <crypto5/filters.h>



/** Helper function that adds the data to the buffer */
extern void AES_PHM_AppendToBuffer(CryptoPP::SecByteBlock& sbb, unsigned int& nBufSize, unsigned int& nTotalLen, byte const* pbData, unsigned int nDataLen);

/** Helper function that generates Initialization Vector from the seed */
extern void AES_PHM_GenerateIvFromSeed(byte const* pbIvSeed, byte* pbIv);

/** Magic numbers used by AES_PHM_Encryption and AES_PHM_Decryption */
enum
{
    MINIMUM_CIPHERTEXT_LENGTH = 46,
    IV_SEED_SIZE = 8,
    IV_SIZE = CryptoPP::AESEncryption::BLOCKSIZE,
};

#pragma warning (pop)

#endif //SICRYPT_STDAFX_H_INCLUDED