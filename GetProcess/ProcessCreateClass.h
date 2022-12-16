/** 
  * Copyright (C) Skarn 2022
  * @file     ProcessCreateClass.h                                                       
  * @brief    进程创建类                                                    
  * @author   debugSkarn                                                     
  * @date     2022-12-14 
  * @note     
  * @see      
 */

#pragma once

#include <windows.h> 
#include <iostream>

//#include<unistd.h> //unix标准文件

using namespace std;

/**
  * @class  ProcessCreateClass
  * @brief 进程创建类
 */
class ProcessCreateClass {

public:
	ProcessCreateClass() {};
	~ProcessCreateClass() {};

	//////////////////////////////////CreateProcess////////////////////////////////////////
	void createProcessWithWindAPI() {
		//LPTSTR 与 wchar_t* 等价(Unicode环境下)  
		//wchar_t* TEST = (wchar_t*)L"H://software//install//WeChat//WeChat.exe";              //设置命令行 
		TCHAR TEST[] = TEXT("GetMinValue.exe");

		LPTSTR cWinDir = new TCHAR[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, cWinDir);
		
		STARTUPINFO si = { sizeof(si) };                       //startupinfo 结构体初始化
		si.dwFlags = STARTF_USESHOWWINDOW;                     // 指定wShowWindow成员有效
		si.wShowWindow = TRUE;                                 // 此成员设为TRUE的话则显示新建进程的主窗口，为FALSE的话则不显示

		//LPCWSTR strLpcwastr = L"E://Windows//System32//notepad.exe";
		//LPWSTR TEST = (LPWSTR)strLpcwastr;
		PROCESS_INFORMATION pi;                                //process_infomation
		BOOL bRet = ::CreateProcess(
			NULL,                                              // 指定可执行文件文件名,不在此指定可执行文件的文件名
			TEST,                                              // 命令行参数 
			NULL,                                              // 新进程安全属性,默认进程安全性
			NULL,                                              // 线程是否被继承,默认线程安全性
			FALSE,                                             // 是否继承父进程句柄,指定当前进程内的句柄不可以被子进程继承
			CREATE_NEW_CONSOLE,                                // 设置子进程的线程调度的优先级,为新进程创建一个新的控制台窗口
			NULL,                                              // 指向一个新进程的环境块，默认为父进程环境,使用本进程的环境变量
			NULL,                                              // 子进程的工作目录,使用本进程的驱动器和目录
			&si,                                               // 决定子进程的主窗体如何显示的STARTUPINFO结构体
			&pi);                                              // 接收子进程的识别信息的PROCESS_INFORMATION结构体

		// 如果不使用两个句柄，则立刻将它们关闭
		if (bRet){
			::CloseHandle(pi.hThread);                          //结束新创建主线程句柄
			::CloseHandle(pi.hProcess);                         //结束新创建主进程句柄

			cout << "新进程的进程ID号:" << pi.dwProcessId << endl;    //dwprocessid：新建进程ID
			cout << "新进程的主线程ID号：" << pi.dwThreadId << endl;   //dwthreadid: 新建主线程ID
		}
	}

	////////////////////////////////// fork 方法////////////////////////////////////////
	/** 
	  * @brief        创建单一子进程(Unix系统下实现)
	 */
	/*
	void createSingleProcessWithFork() {
		using namespace std;
		pid_t pid;
		cout << "parent have!" << endl;

		// note: 关于fork执行的操作
		// "main"进程将会创建一个子进程, 确切的讲是复制一个子进程, 也就是fork创建子进程, 是基于当前进程, 复制了其父进程的用户空间(也就是你所看见的上面的代码全部都会被复制).也就是, 此刻, 存在了两个并行的进程(这个很重要.并不是我们认为的父进程执行完毕了再执行子进程).更确切的讲, 下一个时刻, 父进程要执行上述代码中的8行以后的代码, 子进程也要执行8行以后的代码, 究竟谁先执行, 看谁抢到了CPU.
		pid = fork();      //执行fork的时候到底发生了什么?
		if (pid == -1)     //错误创建
		{
			perror("fork error");
			 _exit(1);
		}
		else if (pid == 0) //子进程
		{
			cout << "i am child,pid = " << getpid() << " my parent is:" << getppid() << endl;
		}
		else              //父进程
		{
			// sleep(1);
			cout << "i am parent,pid = " << getpid() << " my parent is:" << getppid() << endl;
		}
		cout << "both have!" << endl;
	}
	*/
};