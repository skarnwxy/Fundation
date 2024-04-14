/**
  * Opencascade Skarn Copyright (C) 2022
  * @file     GetProcess.cpp
  * @brief    读取进程数
  * @author   debugSkarn
  * @date     2020.6.15
  * @note
  * @see
 */

//#define GetProcess

/////////////////////////////////////////////////////////////////////
#include <iostream>
#include <windows.h>
#include <psapi.h>
#include <tchar.h>

#include <stdio.h>
#include <stdlib.h> 
#include <winnt.h>

#include <fstream>
#include <string>
#include <io.h>

#include "tlhelp32.h"
#include "Psapi.h"

#include<ctime>
#pragma comment(lib,"psapi.lib")

#include <thread>
#include "ThreadCreateClass.h"
#include "ProcessCreateClass.h"
#include "TreadMutexClass.h"

#include <ATLComTime.h>

#ifndef __linux__
#include "windows.h"
#include <stdio.h> 

// 命令调取 CpuID
#define CpuNum "wmic cpu get DeviceID"
// 命令调取 Cpu 物理核数
#define CpuCoreNum "wmic cpu get NumberOfCores"
// 命令调取 Cpu 逻辑核数
#define CpuLogicalCoreNum "wmic cpu get NumberOfLogicalProcessors"

#else
#include "unistd.h"
#include "sys/sysinfo.h"
#endif

using namespace std;

//////////////////////////////////进程相关操作///////////////////////////////////
/**
  * @brief        通过进程ID获取进程句柄
 */
HANDLE GetProcessHandle(DWORD processID)
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);
	return hProcess;
}

/**
  * @brief        根据进程名称获取进程ID
 */
DWORD GetProcessIdFromName(const char *name)
{
	HANDLE hsnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hsnapshot == INVALID_HANDLE_VALUE)
	{
		printf("CreateToolhelp32Snapshot Error!\n");
		return 0;
	}

	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(PROCESSENTRY32);

	int flag = Process32First(hsnapshot, &pe);
	while (flag != 0)
	{
		char* currentName = (char*)pe.szExeFile;
		if (strcmp(currentName, name) == 0)
		{
			return pe.th32ProcessID;
		}
		flag = Process32Next(hsnapshot, &pe);
	}
	CloseHandle(hsnapshot);

	return 0;
}

/**
  * @brief        根据进程的ID获取进程的名称
 */
char* GetProcessNameFormId(DWORD processID) {

	// 根据进程的ID获取名称（TCHAR <——> wchar_t）
	TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
	if (NULL != hProcess)
	{
		HMODULE hMod;
		DWORD cbNeeded;
		if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
		{
			GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(TCHAR));
		}
	}
	CloseHandle(hProcess);

	// 将WCHAR转换为char
	//char* processname = (char*)szProcessName;
	WCHAR *wch = szProcessName;
	int size = WideCharToMultiByte(CP_ACP, 0, wch, -1, NULL, 0, NULL, NULL);
	char *processname = new char[size + 1];
	if (!WideCharToMultiByte(CP_ACP, 0, wch, -1, processname, size, NULL, NULL)) {
		return processname;
	}

	return processname;
}

//////////////////////////////////进程内存相关操作///////////////////////////////////
/** 
  * @brief        获取当前进程的内存
 */
void GetCurrentProcessMemoryInfo(void)
{
	HANDLE handle = GetCurrentProcess();
	PROCESS_MEMORY_COUNTERS pmc;
	GetProcessMemoryInfo(handle, &pmc, sizeof(pmc));

	cout << "WorkingSetSize: " << pmc.WorkingSetSize / (1024 * 1024) << " MB" << endl;
	cout << "PeakWorkingSetSize: " << pmc.PeakWorkingSetSize / (1024 * 1024) << " MB" << endl;
	cout << "PagefileUsage: " << pmc.PagefileUsage / (1024 * 1024) << " MB" << endl;
	cout << "PeakPagefileUsage: " << pmc.PeakPagefileUsage / (1024 * 1024) << " MB" << endl;
}

/** 
  * @brief        获取指定进程的内存
 */
void GetAssignProcessMemoryInfo(DWORD processID) {
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);

	PROCESS_MEMORY_COUNTERS pmc;
	if (NULL != hProcess) {
		GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc));
	}

	cout << "WorkingSetSize: " << pmc.WorkingSetSize / (1024 * 1024) << " MB" << endl;
	cout << "PeakWorkingSetSize: " << pmc.PeakWorkingSetSize / (1024 * 1024) << " MB" << endl;
	cout << "PagefileUsage: " << pmc.PagefileUsage / (1024 * 1024) << " MB" << endl;
	cout << "PeakPagefileUsage: " << pmc.PeakPagefileUsage / (1024 * 1024) << " MB" << endl;

	CloseHandle(hProcess);
}

//////////////////////////////////进程打开和关闭的相关操作///////////////////////////////////
/** 
  * @brief        获取当前exe程序的token
 */
BOOL GetTokenByName(HANDLE &hToken, LPSTR lpName)
{
	if (!lpName)
	{
		return FALSE;
	}

	HANDLE	hProcessSnap = NULL;
	BOOL	bRet = FALSE;
	PROCESSENTRY32 pe32 = { 0 };
	HANDLE hProcess = NULL;

	// 获取系统中正在运行的进程信息
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	pe32.dwSize = sizeof(PROCESSENTRY32);

	if (Process32First(hProcessSnap, &pe32))
	{
		do
		{
			// 寻找指定进程的Token
			WCHAR *wch = (WCHAR*)pe32.szExeFile;
			int size = WideCharToMultiByte(CP_ACP, 0, wch, -1, NULL, 0, NULL, NULL);
			char *ch = new char[size + 1];
			if (!WideCharToMultiByte(CP_ACP, 0, wch, -1, ch, size, NULL, NULL)) {
				return false;
			}

			if (_strcmpi(ch, lpName/*_strupr(cPe32), _strupr(lpName)*/) == 0)
			{
				hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pe32.th32ProcessID);
				bRet = OpenProcessToken(hProcess, TOKEN_ALL_ACCESS, &hToken);
				CloseHandle(hProcess);
				CloseHandle(hProcessSnap);
				return (bRet);
			}
		} while (Process32Next(hProcessSnap, &pe32));
	}

	CloseHandle(hProcessSnap);
	return (bRet);
}

/**
  * @brief        打开程序
 */
BOOL StartProcess(LPCSTR lpProcName)
{
	string CurSubProcessDir(lpProcName);
	//设置子进程当前的目录
	CurSubProcessDir = CurSubProcessDir.substr(0, CurSubProcessDir.rfind('\\'));

	if (!lpProcName)
	{
		return FALSE;
	}

	HANDLE hToken = NULL;
	LPCSTR procName = "notepad.exe";
	LPSTR proName = (LPSTR)procName;
	if (!GetTokenByName(hToken, proName))
	{
		//WriteToLog("Get Wrong Token ! ");
		return FALSE;
	}

	STARTUPINFO         stStartup;
	PROCESS_INFORMATION stProcInfo;

	ZeroMemory(&stStartup, sizeof(STARTUPINFO));
	stStartup.cb = sizeof(STARTUPINFO);
	LPCSTR clpDestop = "winsta0\\default";
	stStartup.lpDesktop = (LPWSTR)clpDestop;
	stStartup.dwFlags |= STARTF_USESHOWWINDOW;
	stStartup.wShowWindow = SW_SHOWNORMAL;

	// 以当前用户的身份打开程序，就会有界面了
	BOOL bResult = CreateProcessAsUser(hToken, (LPWSTR)lpProcName, NULL, NULL, NULL,FALSE, NORMAL_PRIORITY_CLASS, NULL, (LPWSTR)CurSubProcessDir.c_str(), &stStartup, &stProcInfo);

	CloseHandle(hToken);

	if (bResult)
	{
		OutputDebugString(TEXT("CreateProcessAsUser ok! \r\n"));
		//WriteToLog("CreateProcessAsUser ok! ");
		printf("CreateProcess Successed");
	}
	else
	{
		char info[80];
		sprintf_s(info, "CreateProcess failed. error code = %d", GetLastError());
		//WriteToLog(info);
		printf( "CreateProcess failed (%d)\n", GetLastError() );
	}

	return bResult;
}

/** 
  * @brief        启动程序(新建进程)
 */
void StartProcessWay() {
	TCHAR commandLine[] = TEXT("H://software//install//WeChat//WeChat.exe");
	//TCHAR commandLine[] = TEXT("notepad.exe");
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	bool bRet = CreateProcess(
		NULL,
		commandLine,
		NULL,
		NULL,
		FALSE,
		CREATE_NO_WINDOW,
		NULL,
		NULL,
		&si,
		&pi);
	int error = GetLastError();
	if (bRet)
	{
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);

		cout << "新建的进程是: WeChat.exe" << endl;
		printf("进程ID：%d\n", pi.dwProcessId);
		printf("线程ID：%d\n", pi.dwThreadId);
		cout << "Open target process success" << endl;
	}
	else {
		cout << "Open target process failed" << endl;
	}
}

/**
  * @brief        关闭程序
 */
int KillProgram(LPCSTR lpProcName)
{
	const char    *strFile = NULL;
	HANDLE         hProcessSnap = NULL;
	PROCESSENTRY32 stProcEntry32 = { 0 };

	if (!lpProcName || !lpProcName[0])
	{
		return 1; // 参数错误
	}

	strFile = strrchr((const char*)lpProcName, '\\');
	if (0 != strFile)
	{
		lpProcName = strFile + 1;
	}
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == (HANDLE)-1)
	{
		return 3;  // 调用失败
	}

	//用来给stProcEntry32空间赋值0
	memset(&stProcEntry32, 0, sizeof(stProcEntry32));
	stProcEntry32.dwSize = sizeof(PROCESSENTRY32);

	if (Process32First(hProcessSnap, &stProcEntry32))
	{
		do
		{
			// 将WCHAR转换为char
			WCHAR *wch = (WCHAR*)stProcEntry32.szExeFile;
			int size = WideCharToMultiByte(CP_ACP, 0, wch, -1, NULL, 0, NULL, NULL);     
			char *ch = new char[size + 1];     
			if (!WideCharToMultiByte(CP_ACP, 0, wch, -1, ch, size, NULL, NULL)){ 
				return false; 
			}

			if (_strcmpi(ch, lpProcName) == 0 ||
				_strcmpi(ch, "WerFault.exe") == 0)  //Windows 操作系统将错误处理从崩溃进程的上下文移至新服务，即 Windows 错误报告(WER)
			{
				HANDLE hTerminate = GetProcessHandle(stProcEntry32.th32ProcessID);
				// 终止进程
				TerminateProcess(hTerminate, 999);
				CloseHandle(hTerminate);
				Sleep(10);
			}
		} while (Process32Next(hProcessSnap, &stProcEntry32));
	}
	CloseHandle(hProcessSnap);

	return 0;   //操作正常
}

//////////////////////////////////进程监控的相关操作///////////////////////////////////
/** 
  * @brief        对特定进程进行监控
 */
void DetectmMemory()
{
	for (;;)
	{
		char buffer[20];
		 
		//执行内存检测脚本
		FILE *pf = _popen("sh_detect_memory.sh", "r");
		if (pf != NULL) {

			//buffer：enPAC进程占用的内存(KB)
			//fread 将pf指向的内容，读sizeof(buffer)个字节，读一次，读到buffer中
			fread(buffer, sizeof(buffer), 1, pf);

			//used:将buffer转成整型，单位为MB
			short int used = atoi(buffer) / 1024;
			_pclose(pf);

			//判断enPAC占用的内存，超过一定比例，则将memory_flag=false
			//同时break，让dfs（）进行return，结束此公式的search
			short int total_mem = 100;
			if (100*used / total_mem > 70)
			{
				bool memory_flag = false;
				cout << "detect memory over the size  given" << endl;
				break;
			}
		}
		else {
			cout << "未能检测到GetProcess进程所占内存" << endl;
			_pclose(pf);
		}

		//每隔5秒，对enPAC占用内存进行查询
		Sleep(1);
	}
}

//detect_mem_thread = thread(&Product_Automata::detect_memory, this);  //创建一个子线程进行内存监控

/** 
  * @brief        对特定进程进行监控
 */
int WatchMemory(DWORD address, BYTE len, LPCWSTR pszExe)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&si, sizeof(pi));
	si.cb = sizeof(STARTUPINFO);
	CreateProcess(pszExe, NULL, NULL, NULL, TRUE, DEBUG_PROCESS | NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);

	DEBUG_EVENT DebugEv;                   // debugging event information 
	DWORD dwContinueStatus = DBG_CONTINUE; // exception continuation 
	CONTEXT Regs;
	Regs.ContextFlags = CONTEXT_FULL | CONTEXT_DEBUG_REGISTERS;

	int DR0E = 1;
	int LEBPME = 0x100;
	int RW = 0x30000;
	int WATCHLEN = 0;
	//char *szNTDll = "ntdll";
	//char *szNTCont = "NtContinue";
	LPCWSTR szNTDll = TEXT("ntdll");
	LPCSTR szNTCont = "NtContinue";

	DWORD NTContAddr = 0;
	HMODULE ntdll;
	DWORD febx = 0;
	//DWORD dwBytesRead;
	//DWORD dwBytesWritten;
	DWORD dwBuff;
	DWORD dwBPCnt = 0;
	DWORD dwSSCnt = 0;
	DWORD value = 0;

	SIZE_T dwBytesRead;
	SIZE_T dwBytesWritten;

	ResumeThread(pi.hThread);
	switch (len)
	{
	case 2:
		WATCHLEN = 1 << 18;
		break;
	case 4:
		WATCHLEN = (1 << 19) + (1 << 18);
		break;
	default:
		WATCHLEN = 0;
		break;
	}
	for (;;)
	{
		// Wait for debugging event to occur 
		WaitForDebugEvent(&DebugEv, INFINITE);
		dwContinueStatus = DBG_EXCEPTION_NOT_HANDLED;
		if (DebugEv.dwDebugEventCode == EXCEPTION_DEBUG_EVENT)
		{
			if (DebugEv.u.Exception.ExceptionRecord.ExceptionCode == EXCEPTION_BREAKPOINT)
			{
				dwContinueStatus = DBG_CONTINUE;
				dwBPCnt++;
				if (dwBPCnt == 1)
				{
					GetThreadContext(pi.hThread, &Regs);
					Regs.Dr7 = LEBPME + DR0E;
					ntdll = GetModuleHandle(szNTDll);
					if (ntdll == NULL) //这里的处理是因为第一次中断是由ntdll引起的，为什么要这么做，MS没给解释
						return 0;
					NTContAddr = (DWORD)GetProcAddress(ntdll, szNTCont);
					Regs.Dr0 = NTContAddr;
					SetThreadContext(pi.hThread, &Regs);
				}
			}
			else if (DebugEv.u.Exception.ExceptionRecord.ExceptionCode == EXCEPTION_SINGLE_STEP)
			{
				dwContinueStatus = DBG_CONTINUE;
				dwSSCnt++;
				if (dwSSCnt == 1)
				{
					GetThreadContext(pi.hThread, &Regs);

					//febx = Regs.Esp;
					febx += 4; //这里加4的是为了获得CONTEXT的地址，原理可以参考BugSlayer专栏，MSDN上有。
					ReadProcessMemory(pi.hProcess, (void*)febx, &dwBuff, 4, &dwBytesRead);
					ReadProcessMemory(pi.hProcess, (void*)dwBuff, (void*)&Regs, sizeof(CONTEXT), &dwBytesRead);
					Regs.Dr0 = address;             //关联要观测的地址
					Regs.Dr7 = RW + LEBPME + DR0E; //设置断点发生的条件。
					WriteProcessMemory(pi.hProcess, (void*)dwBuff, (void*)&Regs, sizeof(CONTEXT), &dwBytesWritten);
					SetThreadContext(pi.hThread, &Regs);

				}
				else
				{
					/*
						//如果想要清除断点的话可以使用下面注释的代码
					     GetThreadContext(pi.hThread,&Regs);
					     Regs.Dr0 = 0;
					     Regs.Dr7 = 0;
					     SetThreadContext(pi.hThread,&Regs);
					*/
					//获取要检测内存的内容并且打印出来。
					BOOL ret = ReadProcessMemory(pi.hProcess, (void*)address, &value, len, &dwBytesRead);
					printf("ret = %d/t%d/n", ret, value);

				}
			}
		}
		else if (DebugEv.dwDebugEventCode == EXIT_PROCESS_DEBUG_EVENT)
		{
			return 0;
		}
		ContinueDebugEvent(DebugEv.dwProcessId, DebugEv.dwThreadId, dwContinueStatus);
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////
/** 
  * @brief        获取CPU的逻辑核数
  * @param[out]   oNumOfLogicalCores 获取的逻辑核数
  * @see          
  * @note          
 */
void retriveCPULogicalCores(int &oNumOfLogicalCores) {
#ifndef __linux__
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);

	oNumOfLogicalCores = sysInfo.dwNumberOfProcessors;
	cout << "CPU逻辑核数是: " << oNumOfLogicalCores << endl;
#else
	int numConfCores = sysconf(_SC_NPROCS_CONF);
	cout << "CPU逻辑核数是(当前系统所有的CPU核数): " << numConfCores << endl;

	int numOnlnCores = sysconf(_SC_NPROCS_ONLN);
	cout << "CPU逻辑核数是(当前系统用户可以使用的CPU核数): " << numConfCores << endl;

#endif
}

/** 
  * @brief        获取 Cpu 信息
 */
void getCpuInformation(const char *command)
{
	// 获取windows命令回执
	//FILE *winCommand = popen(command, "r");
	//char buf[100] = {};

	//if (!winCommand)
	//{
	//	perror("popen");
	//	exit(EXIT_FAILURE);
	//}
	//else
	//{
	//	// 输出命令回执
	//	while (fgets(buf, sizeof(buf) - 1, winCommand) != 0)
	//	{
	//		printf("%s", buf);
	//		memset(buf, 0, sizeof(buf));
	//	}

	//	pclose(winCommand);
	//}
}

/**
  * @brief        获取CPU的物理核数
  * @param[out]   oNumOfCores 获取的物理核数
  * @see
  * @note
 */
void retrieveCPUCores(int &oNumOfCores)
{
	oNumOfCores = 0;
	
	// 获取逻辑处理器信息
	PSYSTEM_LOGICAL_PROCESSOR_INFORMATION buffer = NULL;
	DWORD returnLength = 0;
	while (true){
		DWORD ro = GetLogicalProcessorInformation(buffer, &returnLength);
		if (ro != FALSE){
			break;
		}
		
		if (GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
			return;
		}

		if (buffer) {
			free(buffer);
		}
		buffer = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION)malloc(returnLength);
	}

	// 遍历处理器获取其中的物理核数信息
	PSYSTEM_LOGICAL_PROCESSOR_INFORMATION ptr = buffer;
	DWORD byteOffest = 0;
	while (byteOffest + sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION) <= returnLength){
		if (ptr->Relationship == RelationProcessorCore) {
			++oNumOfCores;
		}
		byteOffest += sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
		++ptr;
	}
	free(buffer);
}

/** 
  * @brief        多线程文件锁定
  * @see          
  * @note          
 */
void multiThreadLockFile() {
	/*
	C++实现文件锁定的方法：
		1.	使用std::fstream打开文件，并使用std::ios::app模式打开文件以进行追加写入。
		2.	使用std::unique_lock创建一个独占锁对象，以确保在写入文件时不会被其他线程或进程干扰。
		3.	在写入文件之前，使用std::lock_guard获取锁对象，以确保在写入文件时不会被其他线程或进程干扰。
		4.	写入文件后，使用std::unlock_guard释放锁对象。
	*/

	std::string filename = "example.txt";
	std::string account = "123456";
	std::ofstream ofs(filename, std::ios::app);
	std::mutex mtx;
	std::unique_lock<std::mutex> lck(mtx);
	ofs << "Account: " << account << std::endl;
	std::lock_guard<std::mutex> guard(mtx);
	ofs << "Write something to file." << std::endl;
}

/** 
  * @brief        通过LockFileEx方法锁定文件
  * @see          
  * @note          
 */
void deleteFileLockFileExt()
{
	// 1. 打开文件 —— 获取文件句柄
	// 方法1: 通过文件缓冲区获取文件句柄对象
	/*
	ofstream file("example.txt");
	// 获取文件流的缓冲区对象
	std::streambuf* buf = file.rdbuf();
	// 调用缓冲区对象的 fd() 成员函数获取文件描述符
	int fd = buf->fd();
	// 检索与指定的文件说明符关联的操作系统文件句柄
	HANDLE handle = (HANDLE)_get_osfhandle(fd);  // 包含io.h头文件
	*/

	// 方法2: 通过CreateFile方法获取文件句柄对象
	// const char* ——> wchar_t(LPCWSTR)
	//const char* str = "G:\\Code\\branch_Fundation\\GetProcess\\example.txt";
	//int len = lstrlenA(str) + 1;
	//wchar_t* filePath = new wchar_t[len];
	//lstrcpyW(filePath, str);

	const char* str = "example.txt";
	int len = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	wchar_t* filePath = new wchar_t[len];
	MultiByteToWideChar(CP_UTF8, 0, str, -1, filePath, len);

	HANDLE handle = CreateFile(
		filePath,
		// Just normal reading
		FILE_GENERIC_READ,
		// Share all, do not lock the file
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	if (handle == INVALID_HANDLE_VALUE) {
		// 获取错误
		DWORD erM = 0;
		LPVOID lpMsgBuf;
		CString theErr;
		erM = GetLastError();
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			erM,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf,
			0, NULL);

		char* pChar = static_cast<char*>(lpMsgBuf); // 将LPVOID转换为char*指针
		CString str(pChar); // 使用CString的构造函数将char*指针转换为CString对象
		theErr.Format(_T("%s"), lpMsgBuf);//theErr显示为“拒绝访问”

		std::cout << "Failed to open file." << std::endl;
		return;
	}

	// 2. 锁定文件
	OVERLAPPED overlapped = { 0 };
	overlapped.Offset = 0;
	overlapped.OffsetHigh = 0;
	overlapped.hEvent = NULL;
	LockFileEx(handle, LOCKFILE_EXCLUSIVE_LOCK, 0, MAXDWORD, MAXDWORD, &overlapped);

	// 3. 在文件中写入数据
	//file << "Hello, world!" << endl;

	// 4. 解锁文件
	UnlockFileEx(handle, 0, MAXDWORD, MAXDWORD, &overlapped);

	// 5. 关闭文件
	//file.close();
	CloseHandle(handle);
	delete[] filePath;

	return ;
}

/** 
  * @brief        通过LockFile方法锁定文件
  * @see          
  * @note          
 */
void deleteFileLockFile() {
	// 要删除的文件路径
	const char* str = "G:\\example.txt";
	int len = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	wchar_t* filePath = new wchar_t[len];
	MultiByteToWideChar(CP_UTF8, 0, str, -1, filePath, len);

	// 获取文件句柄
	//HANDLE hFile = CreateFile(filePath, GENERIC_ALL, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	HANDLE hFile = CreateFile(
		filePath,
		// Just normal reading
		FILE_GENERIC_READ,
		// Share all, do not lock the file
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	if (hFile == INVALID_HANDLE_VALUE) {
		// 获取错误
		DWORD erM = 0;
		LPVOID lpMsgBuf;
		CString theErr;
		erM = GetLastError();
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			erM,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf,
			0, NULL);
		theErr.Format(_T("%s"), lpMsgBuf);//theErr显示为“拒绝访问”

		std::cout << "Failed to open file." << std::endl;
		return ;
	}

	// 锁定文件
	bool result = LockFile(hFile, 0, 0, MAXDWORD, MAXDWORD);
	if (result)
	{
		std::cout << "File locked." << std::endl;

		// 删除文件 —— 执行相关操作
		if (DeleteFile(filePath)) {
			std::cout << "File deleted." << std::endl;
		}
		else {
			std::cout << "Failed to delete file." << std::endl;
		}

		// 解锁文件
		UnlockFile(hFile, 0, 0, MAXDWORD, MAXDWORD);
		std::cout << "File unlocked." << std::endl;
	}
	else {
		std::cout << "Failed to lock file." << std::endl;
	}

	// 删除文件 —— 执行相关操作
	if (DeleteFile(filePath)) {
		std::cout << "File deleted." << std::endl;
	}
	else {
		std::cout << "Failed to delete file." << std::endl;
	}

	// 关闭文件句柄
	CloseHandle(hFile);

	// 删除文件 —— 执行相关操作
	if (DeleteFile(filePath)) {
		std::cout << "File deleted." << std::endl;
	}
	else {
		std::cout << "Failed to delete file." << std::endl;
	}
}

// 主函数
int main(int argc, char* argv)
{
	SYSTEMTIME sysStartTime = { 0 };
	GetLocalTime(&sysStartTime);

	////////////////////////////////////获取进程//////////////////////////////////////

	////////////////////////////////////////////////////////////////////
	// ①：获取当前进程的内存使用情况
	cout << "-------------------------------" << endl;
	const char* currentName = "GetProcess.exe";
	cout << "当前进程名称是：" << currentName << endl;
	DWORD currentId = GetProcessIdFromName(currentName);
	cout << "当前进程ID是：" << currentId << endl;
	
	cout << "当前进程内存的使用情况：" << endl;
	GetCurrentProcessMemoryInfo();
	/*cout << "回收所有可回收的内存" << endl;
	EmptyWorkingSet(GetCurrentProcess());
	GetCurrentProcessMemoryInfo();
	cout << "开始动态分配内存" << endl;
	char* buf[5];
	for (int i = 0; i < sizeof(buf) / sizeof(char*); i++)
	{
		buf[i] = new char[102400];
		GetCurrentProcessMemoryInfo();
	}
	cout << "开始释放内存" << endl;
	for (int i = 0; i < sizeof(buf) / sizeof(char*); i++)
	{
		delete buf[i];
		buf[i] = NULL;
		GetCurrentProcessMemoryInfo();
	}
	cout << "回收所有可回收的内存" << endl;
	EmptyWorkingSet(GetCurrentProcess());
	GetCurrentProcessMemoryInfo();
	cout << endl;*/
	cout << endl;

	////////////////////////////////////////////////////////////////////
	// ②：获取特定进程的内存使用情况
	cout << "-------------------------------" << endl;
	// 获取所有的进程
	DWORD aProcesses[1024], cbNeeded, cProcesses;
	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
{
		return 1;
	}
	cProcesses = cbNeeded / sizeof(DWORD);

	// 遍历所有进程找到指定的进程
	bool isExis = false;
	for (int i = 0; i < cProcesses; i++)
	{
		if (aProcesses[i] == 0)
		{
			continue;
		}

		//PrintProcessNameAndID(aProcesses[i]);
		DWORD processID = aProcesses[i];

		char* processName = GetProcessNameFormId(processID);
		if (strcmp(processName, "WeChat.exe") == 0) {
			//cout << "ProcessName And ID: " << endl;
			//_tprintf(TEXT("%s  (PID: %u)\n"), processname, processID);
			//cout << endl;
			cout << "指定进程名称是：" << processName << endl;
			cout << "指定进程ID是：" << processID << endl;

			cout << "指定的WeChat进程内存的使用情况：" << endl;
			GetAssignProcessMemoryInfo(processID);
			
			delete[] processName; processName = nullptr;

			isExis = true;
			break;
		}
	}
	if (!isExis) {
		cout << "指定的进程不存在或者指定的进程的已终止！" << endl;
	}
	cout << endl;

	////////////////////////////////////////////////////////////////////
	// ③：获取特定进程的内存使用情况
	//cout << "-------------------------------" << endl;
	//cout << "获取特定进程的内存使用情况: " << endl;
	// 先获取所有的进程，然后通过进程的名称监控内存的变化
	//DetectmMemory();
	//LPCWSTR pszExe = TEXT("HlpViewer.exe");
	//WatchMemory(0x4168fc, 4, pszExe);

	////////////////////////////////////////////////////////////////////
	// ④：打开和关闭外部程序
	cout << "-------------------------------" << endl;
	cout << "打开和关闭外部程序: " << endl;
	
	time_t begin = clock();
	//StartProcess("notepad.exe");   // 打开进程
	//KillProgram("WeChat.exe");     // 关闭进程
	StartProcessWay();               // 新建进程
	time_t end = clock();

	double ret = double(end - begin) / CLOCKS_PER_SEC;
	cout << "This Process Runtime is:" << ret << " s" << endl;
	cout << endl;

	////////////////////////////////////获取CPU的核数//////////////////////////////////////
	cout << "-------------------------------" << endl;
	cout << "获取CPU核数: " << endl;
	// (1): CPU逻辑核数(线程数)
	// Note: CPU的核数分为逻辑核数和物理核数，逻辑核数即线程数，指的是CPU同时可以运行的线程的数量. 
	//       当在并发编程中，创建线程的个数应该小于或者等于逻辑核数.
	//       为什么不使用物理核数而使用逻辑核数作为标准？——因此物理核数是固定的，而逻辑核数通过msconfig指令可以修改，因此不同的机器即使物理内核相同但是逻辑核数可能不同，因此不能简单的根据物理核数的二倍作为并发编程中创建线程的上线
	int numOfCores = 0;
	retriveCPULogicalCores(numOfCores);

	// 具体核数要数ID个数
	getCpuInformation(CpuNum);
	// 具体物理核数直接给出
	getCpuInformation(CpuCoreNum);
	// 具体逻辑核数直接给出
	getCpuInformation(CpuLogicalCoreNum);

	// CPU逻辑核数(线程数)
	numOfCores = std::thread::hardware_concurrency();
	cout << "CPU逻辑核数是: " << numOfCores << endl;

	// (2): CPU物理核数
	retrieveCPUCores(numOfCores);
	cout << "CPU物理核数是: " << numOfCores << endl;
	cout << endl;

	////////////////////////////////////创建进程//////////////////////////////////////
	cout << "-------------------------------" << endl;
	cout << "创建新的进程: " << endl;
	ProcessCreateClass processObject;
	processObject.createProcessWithWindAPI();

	cout << endl;

	////////////////////////////////////创建线程//////////////////////////////////////
	cout << "-------------------------------" << endl;
	cout << "创建新线程: " << endl;
	std::cout << "Main thread id " << std::this_thread::get_id() << std::endl;
	ThreadCreateClass threadObject;
	threadObject.createThreadWithWindAPI();

	threadObject.createThreadWithoutParam();
	threadObject.createThreadWithParam();
	threadObject.createThreadWithFunctor();
	threadObject.createThreadWithMember();
	threadObject.createThreadWithConstructor();
	threadObject.createThreadWithLambda();

	cout << endl;

	//////////////////////////////////////////////////////////////////////////
	cout << "-------------------------------" << endl;
	cout << "多进程/线程同步和互斥: " << endl;
	TreadMutexClass myobja;
	thread myOutMsgObj(&TreadMutexClass::outMsgRecvQueue, &myobja);//要注意第二个参数是引用才能保证线程中使用的是同一个对象
	thread myInnMsgObj(&TreadMutexClass::inMsgRecvQueue, &myobja);
	myOutMsgObj.join();
	myInnMsgObj.join();

	/////////////////////////////////////////////////////////////////////////
	cout << "-------------------------------" << endl;
	cout << "获取时间间隔：" << endl;

	for (int i = 0; i <= 1000000; ++i) {
		int a = 0;
	}

	//Sleep(100);
	SYSTEMTIME sysEndTime = {0};
	GetLocalTime(&sysEndTime);
	
	char strStartTime[64] = {0};
	sprintf_s(strStartTime, "%d-%02d-%02d %02d:%02d:%02d", sysStartTime.wYear, sysStartTime.wMonth, sysStartTime.wDay, sysStartTime.wHour, sysStartTime.wMinute, sysStartTime.wSecond);
	cout << "开始时间：" << strStartTime << endl;

	char strEndTime[64] = { 0 };
	sprintf_s(strEndTime, "%d-%02d-%02d %02d:%02d:%02d", sysEndTime.wYear, sysEndTime.wMonth, sysEndTime.wDay, sysEndTime.wHour, sysEndTime.wMinute, sysEndTime.wSecond);
	cout << "结束时间：" << strEndTime << endl;

	COleDateTime cStartTime(sysStartTime);
	COleDateTime cEndTime(sysEndTime);
	COleDateTimeSpan cCastTimeSpan = cEndTime - cStartTime;
	int nCastTime = cCastTimeSpan.GetTotalSeconds();
	cout << "时间间隔是：" << nCastTime << "s" << endl;

	////////////////////////////////////锁定文件//////////////////////////////////////
	//multiThreadLockFile();
	
	deleteFileLockFileExt();

	deleteFileLockFile();

	return 0;
}
