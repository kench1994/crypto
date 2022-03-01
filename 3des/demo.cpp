// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "3des.hpp"
#include "utils/str_helper.hpp"


int main()
{
    std::string strPlainText = "123456", strCipherText;
    for(const char& ch : {'7', '8', '9', '0'})
    {
        strPlainText += ch;
        std::cout << "plainTex:" << strPlainText << std::endl;
        std::cout << "pkcs5 padding" << std::endl;
        utils::crypto::tribledes_cbc_encrypt("kench_tt", "01234567", strPlainText, strCipherText, 1);
        std::cout << utils::ToHexString(strCipherText.data(), strCipherText.length()) << std::endl;
        std::string strBase64Encode = utils::base64Encode(strCipherText.data(), strCipherText.length(), true);
        std::cout << strBase64Encode << std::endl;

        strPlainText.clear();
        std::string strBase64Decode = utils::base64Decode(strBase64Encode.data(), strBase64Encode.length());
        if(strCipherText == strBase64Decode)
            utils::crypto::tribledes_cbc_decrypt("kench_tt", "01234567", strBase64Decode, strPlainText);

        

        strCipherText.clear();

        std::cout << "zero padding" << std::endl;
        utils::crypto::tribledes_cbc_encrypt("kench_tt", "01234567", strPlainText, strCipherText, 0);
        std::cout << utils::ToHexString(strCipherText.data(), strCipherText.length()) << std::endl;
        std::cout << utils::base64Encode(strCipherText.data(), strCipherText.length(), true) << std::endl;

        std::cout << "-------------" << std::endl;
    }
    return 0;
}
