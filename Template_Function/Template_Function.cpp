/**
  * Opencascade Skarn Copyright (C) 2022
  * @file     Template_Function.cpp
  * @brief    模板函数、右值和左值
  * @author   debugSarn
  * @date     2022-06-30
  * @note
  * @see
 */


#include <string>
#include <iostream>
#include <vector>

class TestClass
{
public:
	TestClass() = delete;
	explicit TestClass(const std::string& str) : data(str)
	{
		std::cout << "TestClass(const std::string& str)" << std::endl;
	}
	explicit TestClass(std::string&& str) : data(str)
	{
		std::cout << "TestClass(std::string&& str)" << std::endl;
	}
	~TestClass()
	{
		std::cout << "~TestClass()" << std::endl;
	}
private:
	std::string data;
};

// Note: 考点右值和左值，右值引用

// 方法2：模板函数形参不取地址
template<typename T>
TestClass* Creator(T t)
{
	return new TestClass(t);
}

// 方法3：模板函数形参，右值引用，可以修改右值
template<typename T>
TestClass* CreatorRight(T &&t)
{
	return new TestClass(t);
}

int main(int argc, char **argv)
{
	//////////////////////////////////////////////////////////////////////////
	// Note1: 模板函数，左值右值
	std::string str1 = "hello";
	std::string str2 = " world";
	TestClass* p1 = Creator(str1);
	
	// 问题: 解决以下编译问题 
	//TestClass* p2 = Creator(str1 + str2);

	// 方法1: 重新构造str3，作为左值
	std::string str3 = str1 + str2;
	TestClass* p2_1 = Creator(str3);

	// 方法2：模板函数中参数不取地址（str1 + str2是右值，不能取地址）
	TestClass* p2_2 = Creator(str1 + str2);

	// 方法3：右值引用，可以修改右值
	TestClass* p2_3 = CreatorRight(str1 + str2);

	delete p1;
	delete p2_1;
	delete p2_2;
	delete p2_3;

	//////////////////////////////////////////////////////////////////////////
	// Note2： 容器左值右值
	std::string strHello("hello ");
	std::vector<std::string> vec;
	vec.reserve(10);

	std::cout << "=========================" << std::endl;
	vec.push_back(strHello);    // 用str1在vec数组拷贝构造一个对象

	// 问题: push_back通过右值引用，可以对右值进行修改
	/*
		先用"123"调用普通构造函数，构造出一个临时对象，然后用临时对象拷贝构造vec数组内的元素（这里匹配右值拷贝构造）
		这条语句执行完，会析构"123"构造的临时对象，此时什么也没做，因为该临时对象的m_data已经为nullptr了
	*/
	vec.push_back("123");

	std::cout << "Vector中第一个元素时: " << vec[0] << std::endl;
	std::cout << "Vector中第二个元素时: " << vec[1] << std::endl;
	std::cout << "=========================" << std::endl;

	//////////////////////////////////////////////////////////////////////////
	// ++i i++ array[0] &p
	// ++i 左值
	auto i = 0;
	int &a = ++i;
	std::cout << " ++i 是左值： " << a << std::endl;

	//int &b = i++;   // 编译问题: 无法从int转换为int&，即无法对右值取地址

	int array[] = {0, 1, 3};
	int &c = array[0];
	std::cout << " array[0] 是左值： " << c << std::endl;

	int p = 0;
	//int &d = &p;  // 编译问题: 无法从int*转换为int&，即无法对右值取地址


	return 0;
}