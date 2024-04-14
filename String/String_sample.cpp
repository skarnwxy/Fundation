/**
  * Opencascade Skarn Copyright (C) 2022
  * @file     String_simple.cpp
  * @brief    string相关
  * @author   debugSarn
  * @date     2022-07-01
  * @note
  * @see
 */


#include <iostream>
#include <string>

int main() {

	// 字符串的长度和大小
	std::string str = "abcabc ";

	auto lengthValue = str.length();   // 长度
	auto sizeValue = str.size();       // 大小

	auto sizeOfValue = sizeof(str);      // sizeOf

	std::cout << "string length is: " << lengthValue << std::endl;
	std::cout << "string size is: " << sizeValue << std::endl;
	std::cout << "string sizeof is: " << sizeOfValue << std::endl;

	// sizeof
	char a[] = "aaaaa";
	auto sizeOf = sizeof(a);
	std::cout << "char sizeOf is: " << sizeOf << std::endl;

}