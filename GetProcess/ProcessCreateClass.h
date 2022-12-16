/** 
  * Copyright (C) Skarn 2022
  * @file     ProcessCreateClass.h                                                       
  * @brief    ���̴�����                                                    
  * @author   debugSkarn                                                     
  * @date     2022-12-14 
  * @note     
  * @see      
 */

#pragma once

#include <windows.h> 
#include <iostream>

//#include<unistd.h> //unix��׼�ļ�

using namespace std;

/**
  * @class  ProcessCreateClass
  * @brief ���̴�����
 */
class ProcessCreateClass {

public:
	ProcessCreateClass() {};
	~ProcessCreateClass() {};

	//////////////////////////////////CreateProcess////////////////////////////////////////
	void createProcessWithWindAPI() {
		//LPTSTR �� wchar_t* �ȼ�(Unicode������)  
		//wchar_t* TEST = (wchar_t*)L"H://software//install//WeChat//WeChat.exe";              //���������� 
		TCHAR TEST[] = TEXT("GetMinValue.exe");

		LPTSTR cWinDir = new TCHAR[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, cWinDir);
		
		STARTUPINFO si = { sizeof(si) };                       //startupinfo �ṹ���ʼ��
		si.dwFlags = STARTF_USESHOWWINDOW;                     // ָ��wShowWindow��Ա��Ч
		si.wShowWindow = TRUE;                                 // �˳�Ա��ΪTRUE�Ļ�����ʾ�½����̵������ڣ�ΪFALSE�Ļ�����ʾ

		//LPCWSTR strLpcwastr = L"E://Windows//System32//notepad.exe";
		//LPWSTR TEST = (LPWSTR)strLpcwastr;
		PROCESS_INFORMATION pi;                                //process_infomation
		BOOL bRet = ::CreateProcess(
			NULL,                                              // ָ����ִ���ļ��ļ���,���ڴ�ָ����ִ���ļ����ļ���
			TEST,                                              // �����в��� 
			NULL,                                              // �½��̰�ȫ����,Ĭ�Ͻ��̰�ȫ��
			NULL,                                              // �߳��Ƿ񱻼̳�,Ĭ���̰߳�ȫ��
			FALSE,                                             // �Ƿ�̳и����̾��,ָ����ǰ�����ڵľ�������Ա��ӽ��̼̳�
			CREATE_NEW_CONSOLE,                                // �����ӽ��̵��̵߳��ȵ����ȼ�,Ϊ�½��̴���һ���µĿ���̨����
			NULL,                                              // ָ��һ���½��̵Ļ����飬Ĭ��Ϊ�����̻���,ʹ�ñ����̵Ļ�������
			NULL,                                              // �ӽ��̵Ĺ���Ŀ¼,ʹ�ñ����̵���������Ŀ¼
			&si,                                               // �����ӽ��̵������������ʾ��STARTUPINFO�ṹ��
			&pi);                                              // �����ӽ��̵�ʶ����Ϣ��PROCESS_INFORMATION�ṹ��

		// �����ʹ����������������̽����ǹر�
		if (bRet){
			::CloseHandle(pi.hThread);                          //�����´������߳̾��
			::CloseHandle(pi.hProcess);                         //�����´��������̾��

			cout << "�½��̵Ľ���ID��:" << pi.dwProcessId << endl;    //dwprocessid���½�����ID
			cout << "�½��̵����߳�ID�ţ�" << pi.dwThreadId << endl;   //dwthreadid: �½����߳�ID
		}
	}

	////////////////////////////////// fork ����////////////////////////////////////////
	/** 
	  * @brief        ������һ�ӽ���(Unixϵͳ��ʵ��)
	 */
	/*
	void createSingleProcessWithFork() {
		using namespace std;
		pid_t pid;
		cout << "parent have!" << endl;

		// note: ����forkִ�еĲ���
		// "main"���̽��ᴴ��һ���ӽ���, ȷ�еĽ��Ǹ���һ���ӽ���, Ҳ����fork�����ӽ���, �ǻ��ڵ�ǰ����, �������丸���̵��û��ռ�(Ҳ������������������Ĵ���ȫ�����ᱻ����).Ҳ����, �˿�, �������������еĽ���(�������Ҫ.������������Ϊ�ĸ�����ִ���������ִ���ӽ���).��ȷ�еĽ�, ��һ��ʱ��, ������Ҫִ�����������е�8���Ժ�Ĵ���, �ӽ���ҲҪִ��8���Ժ�Ĵ���, ����˭��ִ��, ��˭������CPU.
		pid = fork();      //ִ��fork��ʱ�򵽵׷�����ʲô?
		if (pid == -1)     //���󴴽�
		{
			perror("fork error");
			 _exit(1);
		}
		else if (pid == 0) //�ӽ���
		{
			cout << "i am child,pid = " << getpid() << " my parent is:" << getppid() << endl;
		}
		else              //������
		{
			// sleep(1);
			cout << "i am parent,pid = " << getpid() << " my parent is:" << getppid() << endl;
		}
		cout << "both have!" << endl;
	}
	*/
};