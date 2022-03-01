#pragma once
#include <string>
#include <vector>
#include <memory>
#include <openssl/evp.h>
#include "utils/unique_array.hpp"
namespace utils
{
	namespace crypto
	{
		/**
		 * @brief 
		 * Todo: enum padding mode & custom evp_cipber
		 */
		inline int tribledes_cbc_encrypt(const std::string& strSercetKey, const std::string& strInitialVector,\
			const std::string& strPlainText, std::string& strCipherText, int nPadding = 1)
		{
			//cipber buffer prepare
			//but i don't know if my algorithm correct?
			//好像是nopadding的时候余0不补位,余其他直接补一个字节
			//其他padding时都补1位
			unsigned int ciperBufferLen = 0;
			ciperBufferLen = ((!nPadding && 0 == (strPlainText.length() % 8) ? 0 : 1) + strPlainText.length() / 8) * 8;
			utils::unique_array<unsigned char>ciperBuffer(new unsigned char[ciperBufferLen]);
			memset(ciperBuffer.get(), 0, ciperBufferLen);
			memcpy(ciperBuffer.get(), strPlainText.data(), strPlainText.length());

			//prepare key & iv
			unsigned char key[EVP_MAX_KEY_LENGTH]{ 0 };
			unsigned char iv[EVP_MAX_KEY_LENGTH]{ 0 };
			strcpy_s(reinterpret_cast<char*>(key), strSercetKey.length() + 1, strSercetKey.c_str());
			strcpy_s(reinterpret_cast<char*>(iv), strInitialVector.length() + 1, strInitialVector.c_str());

			if (nPadding)
			{
				//pkcs5 padding
				//加密前：数据字节长度对8取余，余数为m
				//若m > 0, 则补足8 - m个字节，字节数值为8 - m
				//即差几个字节就补几个字节，字节数值即为补充的字节数
				//若为0则补充8个字节的8
				unsigned int m = strPlainText.length() % 8;
				if (m)
				{
					char ch = 8 - m;
					unsigned int cnt = static_cast<unsigned int>(ch);
					for(unsigned int i = 0; i < cnt; ++i)
						memcpy(ciperBuffer.get() + strPlainText.length() + i, &ch, 1);
				}
				else
				{
					char ch = 8;
					for (unsigned int i = 0; i < 8; ++i)
						memcpy(ciperBuffer.get() + strPlainText.length() + i, &ch, 1);
				}
			}

			std::shared_ptr<EVP_CIPHER_CTX> ctx(EVP_CIPHER_CTX_new(), [](EVP_CIPHER_CTX* p) {
				EVP_CIPHER_CTX_free(p);
			});

			//calc
			int ret = 1;
			int ciperLength = 0, ciperLengthTmp = 0;
			if (1 == (ret = EVP_EncryptInit_ex(ctx.get(), EVP_des_ede3_cbc(), NULL, key, iv)) && \
				/*user shuould set padding byself & disable openssl default padding here*/
				1 == (ret = EVP_CIPHER_CTX_set_padding(ctx.get(), 0)) && \
				1 == (ret = EVP_EncryptUpdate(ctx.get(), ciperBuffer.get(), &ciperLength, ciperBuffer.get(), ciperBufferLen)) &&\
				1 == (ret = EVP_EncryptFinal_ex(ctx.get(), ciperBuffer.get() + ciperLength, &ciperLengthTmp)))
			{
				ciperLength = ciperLength + ciperLengthTmp;
				strCipherText.assign(reinterpret_cast<const char*>(ciperBuffer.get()), ciperLength);
				return 0;
			}
			return ret;
		}


		//TODO:padding
		inline int tribledes_cbc_decrypt(const std::string& strSercetKey, const std::string& strInitialVector,\
			const std::string& strCipherText, std::string& strPlainText)
		{
			unsigned char key[EVP_MAX_KEY_LENGTH]{ 0 };
			unsigned char iv[EVP_MAX_IV_LENGTH]{ 0 };
			strcpy_s(reinterpret_cast<char*>(key), strSercetKey.length() + 1, strSercetKey.c_str());
			strcpy_s(reinterpret_cast<char*>(iv), strInitialVector.length() + 1, strInitialVector.c_str());

			//我没有去算要准备多的空间,但是应该不会超出密文大小
			unsigned int uCipherTextLen = strCipherText.length();
			utils::unique_array<unsigned char>plainBuffer(new unsigned char[uCipherTextLen + 1]);
			memset(plainBuffer.get(), 0, uCipherTextLen + 1);

			//明文长度
			int nPlainTextLen = 0, nPlainTextLenTmp = 0;
			std::shared_ptr<EVP_CIPHER_CTX> ctx(EVP_CIPHER_CTX_new(), [](EVP_CIPHER_CTX* p) {
				EVP_CIPHER_CTX_free(p);
			});

			int ret = 1;
			
			//std::string strTemp = strCipherText.substr(0, strCipherText.length() - 5);
			const unsigned char* pszDtsToCrypto = reinterpret_cast<const unsigned char*>(strCipherText.data());
			if (1 == (ret = EVP_DecryptInit_ex(ctx.get(), EVP_des_ede3_cbc(), NULL, key, iv)) && \
				/*user shuould set padding byself & disable openssl default padding here*/
				1 == (ret = EVP_CIPHER_CTX_set_padding(ctx.get(), 1)) && 
				1 == (ret = EVP_DecryptUpdate(ctx.get(), plainBuffer.get(), &nPlainTextLen, pszDtsToCrypto, uCipherTextLen)))
			{
				EVP_DecryptFinal_ex(ctx.get(), plainBuffer.get() + nPlainTextLen, &nPlainTextLenTmp);
				auto* pszDtsResultBufferPtr = reinterpret_cast<char*>(plainBuffer.get());
				strPlainText.assign(pszDtsResultBufferPtr, nPlainTextLen + nPlainTextLenTmp);
				return 0;
			}
			return ret;
		}
	}
}

