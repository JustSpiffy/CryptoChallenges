/*
 * crypto_utils.cpp
 *
 *  Created on: Aug 20, 2014
 *      Author: constantin
 */

#include <crypto_util.h>
#include <vector>

namespace CryptoUtils {

static const std::string base64_chars =
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";

std::array<unsigned char, 4> binaryToBase64(const std::array<unsigned char, 3> & input)
{
	std::array<unsigned char, 4> out = {0,0,0,0};

	out[0] = input[0] >> 2;
	out[1] = (input[0] & 0x03) << 4 | input[1] >> 4;
	out[2] = (input[1] & 0x0F) << 2 | input[2] >> 6;
	out[3] = input[2] & 0x3F;

	return out;
}

std::string encodeToBase64(unsigned char * input_array, int length)
{
	std::string result = "";
	if(input_array && length > 0)
	{
		std::array<unsigned char, 3> cur_triplet;
		std::array<unsigned char, 4> output;
		int cur_index = 0;
		while(cur_index+2 < length)
		{
			cur_triplet[0] = input_array[cur_index++];
			cur_triplet[1] = input_array[cur_index++];
			cur_triplet[2] = input_array[cur_index++];

			output = binaryToBase64(cur_triplet);

			result += base64_chars[output[0]];
			result += base64_chars[output[1]];
			result += base64_chars[output[2]];
			result += base64_chars[output[3]];
		}

		int bytes_left = length - cur_index;
		if(bytes_left == 1) //Quicker if we don't loop - just handle both possible situations
		{
			cur_triplet[0] = input_array[cur_index++];
			cur_triplet[1] = 0;
			cur_triplet[2] = 0;

			output = binaryToBase64(cur_triplet);

			result += base64_chars[output[0]];
			result += base64_chars[output[1]];
			result += "==";
		}
		else if(bytes_left == 2)
		{
			cur_triplet[0] = input_array[cur_index++];
			cur_triplet[1] = input_array[cur_index++];
			cur_triplet[2] = 0;

			output = binaryToBase64(cur_triplet);
			result += base64_chars[output[0]];
			result += base64_chars[output[1]];
			result += base64_chars[output[2]];

			result += '=';
		}
	}

	return result;
}


unsigned char char2int(char input)
{
  if(input >= '0' && input <= '9')
    return input - '0';
  if(input >= 'A' && input <= 'F')
    return input - 'A' + 10;
  if(input >= 'a' && input <= 'f')
    return input - 'a' + 10;
}

CryptoUtil::CryptoUtil() {
	std::cout << "Hello world" << std::endl;

	std::string input_string1 = "49";//206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
	std::string input_string2 = "4927";//206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
	std::string input_string3 = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";

	std::string input_string = input_string3;
	if(input_string.length() % 2 != 0)
		std::cout << "String wrong length: " << input_string.length() << std::endl;
	else
	{
		std::vector<unsigned char> dataArray;
		for(unsigned int i = 0; i < input_string.length(); i+=2)
		{
			unsigned char byte = char2int(input_string[i]) << 4 | char2int(input_string[i+1]);
			dataArray.emplace_back(byte);
		}

		std::cout << encodeToBase64(dataArray.data(), dataArray.size()) << std::endl;
	}

}

CryptoUtil::~CryptoUtil() {
	std::cout << "Goodbye world" << std::endl;
}

} /* namespace CryptoUtils */
