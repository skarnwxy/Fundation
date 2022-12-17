/** 
  * Copyright (C) Skarn 2022
  * @file     ThreadCreateClass.h                                                       
  * @brief    �̴߳�����                                                    
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
  * @brief ����WindowAPI�е��̺߳���
 */
class HelloeWindows {
public:
	static DWORD WINAPI myThreadFun(LPVOID lp);
};

DWORD WINAPI HelloeWindows::myThreadFun(LPVOID lp) {

	Sleep(3);
	std::cout << "ͨ��WindowsAPI�����߳�: " << "Hello Word! " << "�߳�ID��: " << std::this_thread::get_id() << std::endl;

	return 0;
}

/** 
  * @brief        ���������ĺ���
 */
void HelloWithoutParam() {
	std::cout << "ͨ�����������ĺ��������߳�: "<< "Hello, World! " << "�߳�ID��: " << std::this_thread::get_id() << std::endl;
}

/**
  * @brief        �������ĺ���
 */
void HelloWithParam(const char* what) {
	// ˯��5����ģ�����ݴ���
	std::this_thread::sleep_for(std::chrono::seconds(3));
	std::cout << "ͨ���������ĺ��������߳�: " << "Hello, " << what << "! " << "�߳�ID��: " << std::this_thread::get_id() << std::endl;
}

/**
  * @class  HelloFunctor
  * @brief �������� ���� �º���
 */
class HelloFunctor {
public:
	void operator()(const char* what) {
		std::this_thread::sleep_for(std::chrono::seconds(3));
		std::cout << "ͨ���������󣨷º����������̣߳�" << "Hello, " << what << "! " << "�߳�ID��: " << std::this_thread::get_id() << std::endl;
	}
};

/**
  * @class  HelloMember
  * @brief ��������ĳ�Ա����
 */
class HelloMember {
public:
	void ThreadFuntion() {
		std::this_thread::sleep_for(std::chrono::seconds(3));
		std::cout << "ͨ������ĳ�Ա���������߳�: " << "ThreadFuntion1! " << "�߳�ID��: " << std::this_thread::get_id() << std::endl;
	}
};

/**
  * @class  HelloWithConstructor
  * @brief ��������Ĺ��캯��
 */
class HelloWithConstructor {
public:
	HelloWithConstructor() {
		std::thread t(std::bind(&HelloWithConstructor::Entry, this, "World"));
		t.join();
	}

private:
	// �̺߳���
	void Entry(const char* what) {
		std::this_thread::sleep_for(std::chrono::seconds(3));
		std::cout << "ͨ������Ĺ��캯�������߳�: " << "Hello, " << what << "!" << "�߳�ID��: " << std::this_thread::get_id() << std::endl;
	}
};

/**
  * @class  ThreadCreateClass
  * @brief �̴߳�����
 */
class ThreadCreateClass {

public:
	ThreadCreateClass() {};
	~ThreadCreateClass() {};

	//////////////////////////////////Windows API////////////////////////////////////////
	void createThreadWithWindAPI() {
		HANDLE m_myCreateThread = CreateThread(NULL,//�̰߳�ȫ����
			0,//��ջ��С
			HelloeWindows::myThreadFun,//�̺߳���
			0,//�̲߳���
			0,//�̴߳�������
			NULL);//�߳�ID

		if (m_myCreateThread == INVALID_HANDLE_VALUE){
			return;
		}

		//�����߳����ȼ�
		SetThreadPriority(m_myCreateThread, THREAD_PRIORITY_LOWEST);
		ResumeThread(m_myCreateThread);

		//�ر��߳�
		CloseHandle(m_myCreateThread);
	}

	//////////////////////////////////C++ 11�еķ���////////////////////////////////////////
	/** 
	  * @brief        ͨ�����������ĺ��������߳�
	 */
	void createThreadWithoutParam() {
		// ����һ���̶߳���ע�⺯�� HelloWithoutParam ����������
		std::thread t(&HelloWithoutParam);

		// �ȴ��߳̽�����
		// �����̻߳�ûִ�У��꣩����������Ѿ������ˡ�
		t.join();
	}

	/**
	  * @brief        ͨ�����������ĺ��������߳�
	 */
	void createThreadWithParam() {
		// ����һ���̶߳���ע�⺯�� HelloWithParam ����������
		std::thread t(&HelloWithParam, "World");


		// �ȼ���ʹ�� bind��
		// std::thread t(std::bind(&Hello, "World"));

		// �ȴ��߳̽�����
		// �����̻߳�ûִ�У��꣩����������Ѿ������ˡ�
		t.join();
	}

	/**
	  * @brief        ͨ���������󴴽��߳�
	 */
	void createThreadWithFunctor() {
		HelloFunctor helloObject;
		
		// ��ʽһ��������������
		std::thread t1(helloObject, "World! ������������");
		t1.join();

		// ��ʽ������������������ͨ�� boost::ref �������á�
		// �û����뱣֤���߳����õĺ�������ӵ�г����̵߳������ڡ�
		// ��������ͨ�� join �̱߳�֤����һ�㡣 
		std::thread t2(std::ref(helloObject), "World! ��������������");
		t2.join();
	}

	/**
	  * @brief        ͨ����Ա���������߳�
	 */
	void createThreadWithMember() {
		HelloMember helloObject;

		std::thread t1(&HelloMember::ThreadFuntion, &helloObject);
		t1.join();
	}

	/**
	  * @brief        ͨ�����캯�������߳�
	 */
	void createThreadWithConstructor() {
		HelloWithConstructor helloObject;
	}

	/**
	  * @brief        ͨ�����캯�������߳�
	 */
	void createThreadWithLambda() {
		auto threadFunction = []() {
			std::cout << "ͨ��lambda���ʽ�����߳�: ";
			std::cout << "Lambda thread id " << std::this_thread::get_id() << std::endl; // ��ӡ�߳�id
		};

		std::thread t1(threadFunction);
		t1.join();
	}
};