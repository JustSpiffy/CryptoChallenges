/*
 * crypto_util.h
 *
 *  Created on: Aug 20, 2014
 *      Author: constantin
 */

#include <iostream>
#include <array>

#ifndef CRYPTO_UTIL_H_
#define CRYPTO_UTIL_H_

namespace CryptoUtils {

//unsigned char[4] binaryToBase64(unsigned char[3] input);

std::array<unsigned char, 4> binaryToBase64(const std::array<unsigned char, 3> & input);

class CryptoUtil {
public:
	CryptoUtil();
	virtual ~CryptoUtil();


};

} /* namespace CryptoUtils */

#endif /* CRYPTO_UTIL_H_ */
