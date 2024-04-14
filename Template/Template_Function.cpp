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

using namespace std;

//////////////////////////////////////////////////////////////////////////
// 右值引用相关
class Test {
public:
	Test() : x(0) {
		std::cout << "构造函数 this = " << this << std::endl;
	}
	Test(int x) : x(x) {
		std::cout << "构造函数 this = " << this << std::endl;
	}
	Test(const Test& another) : x(another.x) {
		std::cout << "拷贝构造 this = " << this << " from " << &another << std::endl;
	}
	Test(const Test&& another) noexcept : x(another.x) {
		std::cout << "移动构造 this = " << this << " from " << &another << std::endl;
	}
	~Test() {
		std::cout << "析构函数 this = " << this << std::endl;
	}

	int x;
};

ostream& operator<<(ostream& out, const Test& t) {
	out << "&t = " << &t << ", x = " << t.x;
	return out;
}

class Aa {
public:
	// Note: 如果不加const则导致编译操作，
	//Aa(const Test& t) : t(t) { 
	//	std::cout << t << std::endl;
	//	std::cout << this->t << std::endl; 
	//}

	// Note: 采用右值引用
	// 当将亡值出现时，左值引用虽然起别名，但是这个对象已经析构，别名也一起失效。因此左值引用一定保证她的生命周期小于等于它被引用的生命周期
	// 右值引用也可以看作起名，只是它起名的对象是一个将亡值。然后延续这个将亡值的生命，直到这个的右值的生命也结束了。
	//Aa(const Test&& t) = delete; //这行就可以禁止将亡值来赋值，使编译时报错。
	Aa(Test&& t) : t(t) {
		std::cout << t << std::endl;
		std::cout << this->t << std::endl;
	}

	void foo() {
		std::cout << t << std::endl;
	}

private:
	const Test &t;
};

//////////////////////////////////////////////////////////////////////////
// 字节跳动面试题
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
template<typename T>
TestClass* Creator(T& t)
{
	return new TestClass(t);
}

// 方法2：模板函数形参不取地址
template<typename T>
TestClass* CreatorObject(T t)
{
	return new TestClass(t);
}

// 方法3：通过const修改作为常引用
template<typename T>
TestClass* CreatorConst(const T& t)
{
	return new TestClass(t);
}

// 方法4：模板函数形参，右值引用，可以修改右值
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
	TestClass* p2_2 = CreatorObject(str1 + str2);

	// 方法3：通过const修饰，左右常引用
	TestClass* p2_3 = CreatorConst(str1 + str2);

	// 方法4：右值引用，可以修改右值
	TestClass* p2_4 = CreatorRight(str1 + str2);

	// 方法4: 
	//TestClass* p2_5 = CreatorRight(str1);

	// 备注: 右值引用和左值引用
	int a = 10;
	int &b = a;        // right, a是左值，正常的左值引用

	//int &c = 4;        // error, 右值不能取地址
	const int& d = 4;  // right, 常引用

	int && e = 10;    // right, e是右值引用
	//int && f = a;     // error, f不可以指向一个左值

	delete p1;
	delete p2_1;
	delete p2_2;
	delete p2_3;
	delete p2_4;

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
	int &aa = ++i;
	std::cout << " ++i 是左值： " << aa << std::endl;

	//int &b = i++;   // 编译问题: 无法从int转换为int&，即无法对右值取地址

	int array[] = {0, 1, 3};
	int &cc = array[0];
	std::cout << " array[0] 是左值： " << cc << std::endl;

	int p = 0;
	//int &d = &p;  // 编译问题: 无法从int*转换为int&，即无法对右值取地址

	//////////////////////////////////////////////////////////////////////////
	// Note: 右值引用相关
	Aa aObject{ Test() };
	aObject.foo();

	return 0;
}