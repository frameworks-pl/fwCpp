#include "stdafx.h"
#include "fwcrypttest.h"

#include "fwaesencdec.h"
#include "fwsha256.h"
#include <src/fw/core/fwdatetime.h>
#include <src/fw/core/fwapputils.h>
#include <src/fw/core/fwtextconv.h>

#ifdef _DEBUG
//#define new DEBUG_NEW
#endif

namespace test {

bool
SICryptTest::run() {

  //testing AES PHM algorithm
  if (!AESPHMTest())
    return false;

  if (!SHA256Test())
    return false;


  return true;

}; //run


bool
SICryptTest::AESPHMTest() {

  int iMaxString = 128;
  for (int i=0; i < 20; i++) {

    //generating random length of the string and password
    int str_len = (rand() % (iMaxString-1)) + 1;
    int pass_len = (rand() %29) + 1;

    //generate random string and password
    CString input = fw::core::AppUtils::getRandomString(str_len);
    CString pass = fw::core::AppUtils::getRandomString(pass_len);

    //every five test do the empty password test
    if ((i % 5) == 0)
      pass = _T("");

    fw::crypt::AESEncDec aes;
    std::string s_pass = fw::core::TextConv::Unicode2UTF8(pass);
    aes.init((BYTE*)(LPCSTR)s_pass.c_str(), (int)s_pass.size());

    std::string s_input;
    s_input = fw::core::TextConv::Unicode2UTF8(input);
    fw::core::ByteBuffer encrypted_str = aes.encrypt((BYTE*)s_input.c_str(), (int)s_input.size());

    
    fw::core::ByteBuffer decrypted_str = aes.decrypt(encrypted_str.getBuffer(), encrypted_str.getLength());
    std::string s_out((const char*)decrypted_str.getBuffer(), decrypted_str.getLength());
    CString output;
    output = fw::core::TextConv::UTF82Unicode(s_out.c_str());

    if (input != output) {
      TRACE(_T("AES PHM Test failed!\ninput=%s\noutput=%s\n"),input, output);
      return false;
    }

  }


  return true;

}; //AESPHMTest


bool
SICryptTest::SHA256Test() {

  fw::crypt::SHA256 s1;
  BYTE* pHash = new BYTE[fw::crypt::SHA256::getHashSize()];
  memset(pHash, 65, fw::crypt::SHA256::getHashSize());
  s1.init(pHash, fw::crypt::SHA256::getHashSize());
  delete[] pHash;

  fw::crypt::SHA256 s2(s1);

  //copy constructor should give as two exactly the same hashes!
  for (int i=0; i < fw::crypt::SHA256::getHashSize(); i++) {
    if ((BYTE)*(s1.getBuffer()+i) != (BYTE)*(s2.getBuffer()+i)) {
      TRACE(_T("SHA256 Test failed (copy constructor)!\n"));
      return false;
    }
  }

  //cheking if the empty hash works
  fw::crypt::SHA256 empty_sha256;
  empty_sha256.create(NULL, 0);

  //empty buffer
  fw::core::ByteBuffer byte_buffer;

  if (!empty_sha256.verify(byte_buffer)) {
    TRACE(_T("SHA256 Test failed (empty password verification failed)!\n"));
    return false;
  }

  return true;

}; //SHA256Test

}; //namespace