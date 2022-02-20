// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <sstream>
#include <iomanip>
#include <openssl/x509.h>
#include "3des.hpp"

// base64 编码
inline std::string base64Encode(const char* buffer, unsigned int length, bool line_feed = false)
{
    BIO* bmem = NULL;
    BIO* b64 = NULL;
    BUF_MEM* bptr;

    b64 = BIO_new(BIO_f_base64());
    if (!line_feed) {
        BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    }
    bmem = BIO_new(BIO_s_mem());
    b64 = BIO_push(b64, bmem);
    BIO_write(b64, buffer, length);
    BIO_flush(b64);
    BIO_get_mem_ptr(b64, &bptr);
    BIO_set_close(b64, BIO_NOCLOSE);

    std::string strResult(bptr->data, bptr->length);
    BIO_free_all(b64);

    return strResult;
}

inline std::string base64Decode(const char* input, int length, bool line_feed = false)
{
    BIO* b64 = NULL;
    BIO* bmem = NULL;
    char* buffer = (char*)malloc(length);
    memset(buffer, 0, length);
    b64 = BIO_new(BIO_f_base64());
    if (!line_feed) {
        BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    }
    bmem = BIO_new_mem_buf(input, length);
    bmem = BIO_push(b64, bmem);
    BIO_read(bmem, buffer, length);
    BIO_free_all(bmem);

    return buffer;
}

inline std::string ToHexString(const char* buf, unsigned int len, const char* pszTok = "0x")
{
    std::stringstream ss;
    for (unsigned int i = 0; i < len; ++i)
    {
        if (pszTok)
            ss << pszTok;
        ss << std::setw(2) << std::setfill('0') << std::hex << (int)(buf[i] & 0xff);

        if (pszTok)
            ss << " ";
    }

    return ss.str();
}

int main()
{
    std::string strPlainText = "12345678910", strCipherText;
    utils::crypto::tribledes_encrypt("kench_tt", "01234567", strPlainText, strCipherText, 1);
    std::cout << ToHexString(strCipherText.data(), strCipherText.length()) << std::endl;;
    std::cout << base64Encode(strCipherText.data(), strCipherText.length(), true) << std::endl;

    strCipherText.clear();

    utils::crypto::tribledes_encrypt("kench_tt", "01234567", strPlainText, strCipherText, 0);
    std::cout << ToHexString(strCipherText.data(), strCipherText.length()) << std::endl;
    std::cout << base64Encode(strCipherText.data(), strCipherText.length(), true) << std::endl;

    return 0;
}
