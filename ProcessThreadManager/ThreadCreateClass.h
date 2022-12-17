/** 
  * Copyright (C) Skarn 2022
  * @file     ThreadCreateClass.h                                                       
  * @brief    线程创建类                                                    
  * @author   debugSkarn                                                     
  * @date     2022-12-13 
  * @note     
  * @see      
 */

#pragma once

#include <iostream>
#include <thread>
#include <processthreadsapi.h>

/**
  * @class  HelloeWindows
  * @brief 构造WindowAPI中的线程函数
 */
class HelloeWindows {
public:
	static DWORD WINAPI myThreadFun(LPVOID lp);
};

DWORD WINAPI HelloeWindows::myThreadFun(LPVOID lp) {

	Sleep(3);
	std::cout << "通过WindowsAPI创建线程: " << "Hello Word! " << "线程ID是: " << std::this_thread::get_id() << std::endl;

	return 0;
}

/** 
  * @brief        不带参数的函数
 */
void HelloWithoutParam() {
	std::cout << "通过不带参数的函数创建线程: "<< "Hello, World! " << "线程ID是: " << std::this_thread::get_id() << std::endl;
}

/**
  * @brief        带参数的函数
 */
void HelloWithParam(const char* what) {
	// 睡眠5秒以模拟数据处理。
	std::this_thread::sleep_for(std::chrono::seconds(3));
	std::cout << "通过带参数的函数创建线程: " << "Hello, " << what << "! " << "线程ID是: " << std::this_thread::get_id() << std::endl;
}

/**
  * @class  HelloFunctor
  * @brief 函数对象 —— 仿函数
 */
class HelloFunctor {
public:
	void operator()(const char* what) {
		std::this_thread::sleep_for(std::chrono::seconds(3));
		std::cout << "通过函数对象（仿函数）创建线程：" << "Hello, " << what << "! " << "线程ID是: " << std::this_thread::get_id() << std::endl;
	}
};

/**
  * @class  HelloMember
  * @brief 创建对象的成员函数
 */
class HelloMember {
public:
	void ThreadFuntion() {
		std::this_thread::sleep_for(std::chrono::seconds(3));
		std::cout << "通过对象的成员函数创建线程: " << "ThreadFuntion1! " << "线程ID是: " << std::this_thread::get_id() << std::endl;
	}
};

/**
  * @class  HelloWithConstructor
  * @brief 创建对象的构造函数
 */
class HelloWithConstructor {
public:
	HelloWithConstructor() {
		std::thread t(std::bind(&HelloWithConstructor::Entry, this, "World"));
		t.join();
	}

private:
	// 线程函数
	void Entry(const char* what) {
		std::this_thread::sleep_for(std::chrono::seconds(3));
		std::cout << "通过对象的构造函数创建线程: " << "Hello, " << what << "!" << "线程ID是: " << std::this_thread::get_id() << std::endl;
	}
};

/**
  * @class  ThreadCreateClass
  * @brief 线程创建类
 */
class ThreadCreateClass {

public:
	ThreadCreateClass() {};
	~ThreadCreateClass() {};

	//////////////////////////////////Windows API////////////////////////////////////////
	void createThreadWithWindAPI() {
		HANDLE m_myCreateThread = CreateThread(NULL,//线程安全属性
			0,//堆栈大小
			HelloeWindows::myThreadFun,//线程函数
			0,//线程参数
			0,//线程创建属性
			NULL);//线程ID

		if (m_myCreateThread == INVALID_HANDLE_VALUE){
			return;
		}

		//设置线程优先级
		SetThreadPriority(m_myCreateThread, THREAD_PRIORITY_LOWEST);
		ResumeThread(m_myCreateThread);

		//关闭线程
		CloseHandle(m_myCreateThread);
	}

	//////////////////////////////////C++ 11中的方法////////////////////////////////////////
	/** 
	  * @brief        通过不带参数的函数创建线程
	 */
	void createThreadWithoutParam() {
		// 创建一个线程对象，注意函数 HelloWithoutParam 将立即运行
		std::thread t(&HelloWithoutParam);

		// 等待线程结束。
		// 否则线程还没执行（完），主程序就已经结束了。
		t.join();
	}

	/**
	  * @brief        通过不带参数的函数创建线程
	 */
	void createThreadWithParam() {
		// 创建一个线程对象，注意函数 HelloWithParam 将立即运行
		std::thread t(&HelloWithParam, "World");


		// 等价于使用 bind：
		// std::thread t(std::bind(&Hello, "World"));

		// 等待线程结束。
		// 否则线程还没执行（完），主程序就已经结束了。
		t.join();
	}

	/**
	  * @brief        通过函数对象创建线程
	 */
	void createThreadWithFunctor() {
		HelloFunctor helloObject;
		
		// 方式一：拷贝函数对象。
		std::thread t1(helloObject, "World! 拷贝函数对象");
		t1.join();

		// 方式二：不拷贝函数对象，通过 boost::ref 传入引用。
		// 用户必须保证被线程引用的函数对象，拥有超出线程的生命期。
		// 比如这里通过 join 线程保证了这一点。 
		std::thread t2(std::ref(helloObject), "World! 不拷贝函数对象");
		t2.join();
	}

	/**
	  * @brief        通过成员函数创建线程
	 */
	void createThreadWithMember() {
		HelloMember helloObject;

		std::thread t1(&HelloMember::ThreadFuntion, &helloObject);
		t1.join();
	}

	/**
	  * @brief        通过构造函数创建线程
	 */
	void createThreadWithConstructor() {
		HelloWithConstructor helloObject;
	}

	/**
	  * @brief        通过构造函数创建线程
	 */
	void createThreadWithLambda() {
		auto threadFunction = []() {
			std::cout << "通过lambda表达式创建线程: ";
			std::cout << "Lambda thread id " << std::this_thread::get_id() << std::endl; // 打印线程id
		};

		std::thread t1(threadFunction);
		t1.join();
	}
};