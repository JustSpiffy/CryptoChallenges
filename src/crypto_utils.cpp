/*
 * crypto_utils.cpp
 *
 *  Created on: Aug 20, 2014
 *      Author: constantin
 */

#include <crypto_util.h>
#include <vector>

namespace CryptoUtils {

namespace {
static const std::string base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";

void binaryToBase64(const unsigned char input[3], unsigned char output[4])
{
    output[0] = input[0] >> 2;
    output[1] = (input[0] & 0x03) << 4 | input[1] >> 4;
    output[2] = (input[1] & 0x0F) << 2 | input[2] >> 6;
    output[3] = input[2] & 0x3F;
}

void base64ToBinary(const unsigned char input[4], unsigned char output[3]) {
    output[0] = input[0] << 2 | input[1] >> 4;
    output[1] = ((input[1] & 0x0F) << 4) | (input[2] >> 2);
    output[2] = ((input[2] & 0x03) << 6) | (input[3] & 0x3F);
}

unsigned char from_base64_char(const char & curChar) {
    if(curChar >= 'A' && curChar <= 'Z')
        return curChar - 'A';
    else if(curChar >= 'a' && curChar <= 'z')
        return curChar - 'a' + 26;
    else if(curChar >= '0' && curChar <= '9')
        return curChar - '0' + 52;
    else if(curChar == '+')
        return 62;
    else if(curChar == '/')
        return 63;
    else
        return 64;
}

}

std::string encodeToBase64(unsigned char * input_array, int length)
{
    std::string result = "";
    if(input_array && length > 0)
    {
        unsigned char cur_triplet[3];
        unsigned char output[4];
        int cur_index = 0;
        while(cur_index+2 < length)
        {
            cur_triplet[0] = input_array[cur_index++];
            cur_triplet[1] = input_array[cur_index++];
            cur_triplet[2] = input_array[cur_index++];

            binaryToBase64(cur_triplet, output);

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

            binaryToBase64(cur_triplet,output);

            result += base64_chars[output[0]];
            result += base64_chars[output[1]];
            result += "==";
        }
        else if(bytes_left == 2)
        {
            cur_triplet[0] = input_array[cur_index++];
            cur_triplet[1] = input_array[cur_index++];
            cur_triplet[2] = 0;

            binaryToBase64(cur_triplet,output);
            result += base64_chars[output[0]];
            result += base64_chars[output[1]];
            result += base64_chars[output[2]];

            result += '=';
        }
    }

    return result;
}

void decodeBase64(const std::string & base64Str, unsigned char* & outputBuff, unsigned int & bufSize) {
    if(base64Str.length() % 4 != 0) //base64 is always in bytes of 4
        return;

    const unsigned int stringLength = base64Str.length(); //WE know string length won't change no need to recalculate
    unsigned int numQuartets = stringLength / 4;
    if(numQuartets == 0) {
        outputBuff = 0;
        bufSize = 0;
        return;
    }
    bufSize = numQuartets * 3;
    if(base64Str[stringLength - 1] == '=')
        --bufSize;
    if(base64Str[stringLength - 2] == '=')
        --bufSize;

    outputBuff = new unsigned char[bufSize];

    unsigned char cur_quartet[4];
    unsigned int output_index;
    unsigned int cur_index = 0;
    while(output_index+2 < bufSize) {
        cur_quartet[0] = from_base64_char((char)base64Str[cur_index++]);
        cur_quartet[1] = from_base64_char((char)base64Str[cur_index++]);
        cur_quartet[2] = from_base64_char((char)base64Str[cur_index++]);
        cur_quartet[3] = from_base64_char((char)base64Str[cur_index++]);

        base64ToBinary(cur_quartet, &(outputBuff[output_index]));
        output_index+=3;
    }

    unsigned int bytes_left = bufSize - output_index;
    if(bytes_left > 0) {
        cur_quartet[0] = from_base64_char((char)base64Str[cur_index++]);
        cur_quartet[1] = from_base64_char((char)base64Str[cur_index++]);
        cur_quartet[2] = from_base64_char((char)base64Str[cur_index++]);
        cur_quartet[3] = from_base64_char((char)base64Str[cur_index++]);

        unsigned char output[3];
        base64ToBinary(cur_quartet, output);
        if(bytes_left == 1) {
            outputBuff[output_index++] = output[0];
        } else if(bytes_left == 2) {
            outputBuff[output_index++] = output[0];
            outputBuff[output_index++] = output[1];
        }
    }
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

char int2char(unsigned char input)
{
    if(input >= 0 && input <= 9)
        return '0' + input;
    if(input >= 10 && input <= 15)
        return 'a' + input - 10;
    return '0';
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

        std::string outputStr = encodeToBase64(dataArray.data(), dataArray.size());
        //       std::cout << outputStr << std::endl;
        unsigned char * outBuff;
        unsigned int size;
        decodeBase64(outputStr, outBuff, size);
        std::string newString;
        for(unsigned int i = 0; i < size; ++i) {
            newString.push_back(int2char(outBuff[i] >> 4));
            newString.push_back(int2char(outBuff[i] & 0x0F));
        }

        if(newString.compare(input_string) != 0) {
            std::cout << "Strings don't match!" << std::endl;
            std::cout << input_string << std::endl;
            std::cout << newString << std::endl;
        }
    }

}

CryptoUtil::~CryptoUtil() {
    std::cout << "Goodbye world" << std::endl;
}

} /* namespace CryptoUtils */
