/**
  * Opencascade Skarn Copyright (C) 2022
  * @file     Operator_Compare.cpp
  * @brief    运算符比较
  * @author   debugSarn
  * @date     2022-03-30
  * @note
  * @see
 */


#include <iostream>

int main()
{
    std::cout << "Hello World!\n";

	// 情况1： ++i和i++的区别

	// ++i后，i为1， ++i为1 ，即++i后，i和++i均为1(增加1)
	std::cout << "-----------------++i-------------------" << std::endl;
	int i = 0;

	std::cout << " i is: " << i << std::endl;
	std::cout << " ++i is: " << ++i << std::endl;
	std::cout << " i is: " << i << std::endl;

	// i++后，i为1， i++为0，即i++后，i为1，i++为0
	std::cout << "-----------------i++-------------------" << std::endl;

	i = 0;

	std::cout << " i is: " << i << std::endl;
	std::cout << " i++ is: " << i++ << std::endl;
	std::cout << " i is: " << i << std::endl;
	
	// 总结: i++和++i后，i都为1，但++i为1，i++为0

	// 情况2：
	std::cout << "++i循环10次: " << std::endl;
	i = 0;
	for (auto j = 0; j < 10; ++j) {
		i = (++i);
	}
	std::cout << " 循环10次后，i的值为：" << i << std::endl;

	std::cout << "i++循环10次: " << std::endl;
	i = 0;
	for (auto j = 0; j < 10; ++j) {
		i = (i++);
	}
	std::cout << " 循环10次后，i的值为：" << i << std::endl;

}

int fun1(int i) {
	auto a = 0;
	a = i;
	i = i + 1;

	std::cout << "a就相当于i++" << "a的值是：" << a << std::endl;
	return a;
}

int fun2(int i) {
	auto a = 0;
	i = i + 1;
	a = 1;

	std::cout << "a就相当于i++" << "a的值是：" << a << std::endl;

	return a;
}