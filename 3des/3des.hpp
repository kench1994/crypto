#pragma once
#include <string>
#include <vector>
#include <memory>
#include <openssl/evp.h>
#include <boost/scoped_array.hpp>

namespace utils
{
	namespace crypto
	{
		
		inline int tribledes_encrypt(const void* key, const void* iv,\
			const std::string& strPlainText, std::string& strCipherText)
		{
			//cipber buffer prepare
			//i don't know if i calc correct?
			unsigned int ciperBufferLen = (strPlainText.length() / 8 + 1) * 8;
			boost::scoped_array<unsigned char>ciperBuffer(new unsigned char[ciperBufferLen]);
			memset(ciperBuffer.get(), 0, ciperBufferLen);

			int ciperLength;
			int ciperLengthTmp;

			std::shared_ptr<EVP_CIPHER_CTX> ctx(EVP_CIPHER_CTX_new(), [](EVP_CIPHER_CTX* p) {
				EVP_CIPHER_CTX_cleanup(p);
			});
			EVP_CIPHER_CTX_init(ctx.get());
			EVP_CIPHER_CTX_set_padding(ctx.get(), 1);

			int ret = EVP_EncryptInit_ex(ctx.get(), EVP_des_ede3_cbc(), NULL,\
				static_cast<const unsigned char*>(key), static_cast<const unsigned char*>(iv));
			if (ret != 1)
			{
				return -1;
			}
			ret = EVP_EncryptUpdate(ctx.get(), ciperBuffer.get(), &ciperLength,\
				reinterpret_cast<const unsigned char*>(strPlainText.data()), strPlainText.length());
			if (ret != 1)
			{
				return -2;
			}
			ret = EVP_EncryptFinal_ex(ctx.get(), ciperBuffer.get() + ciperLength, &ciperLengthTmp);
			if (ret != 1)
			{
				return -3;
			}
			ciperLength = ciperLength + ciperLengthTmp;
			strCipherText.assign(reinterpret_cast<const char*>(ciperBuffer.get()), ciperLength);
			return 0;
		}
	}
}

