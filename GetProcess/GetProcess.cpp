/**
  * Opencascade Skarn Copyright (C) 2022
  * @file     GetProcess.cpp
  * @brief    读取进程数
  * @author   debugSarn
  * @date     2020.6.15
  * @note
  * @see
 */

/////////////////////////////////////////////////////////////////////
#include <iostream>
#include <windows.h>
#include <psapi.h>
#include <tchar.h>

#include <stdio.h>
#include <stdlib.h> 


#include "tlhelp32.h"

#include<ctime>
#pragma comment(lib,"psapi.lib")
using namespace std;

/////////////////////////////////////////////////////////////////////
// (1):进程相关操作
// 1-1:获取当前进程的内存
void GetMemoryInfo(void)
{
	HANDLE handle = GetCurrentProcess();
	PROCESS_MEMORY_COUNTERS pmc;
	GetProcessMemoryInfo(handle, &pmc, sizeof(pmc));

	cout << "WorkingSetSize: " << pmc.WorkingSetSize / (1024 * 1024) << " MB" << endl;
	cout << "PeakWorkingSetSize: " << pmc.PeakWorkingSetSize / (1024 * 1024) << " MB" << endl;
	cout << "PagefileUsage: " << pmc.PagefileUsage / (1024 * 1024) << " MB" << endl;
	cout << "PeakPagefileUsage: " << pmc.PeakPagefileUsage / (1024 * 1024) << " MB" << endl;
	cout << endl;
}

// 1-2:获取指定进程的内存
void GetAssignMemoryInfo(DWORD processID) {
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);

	PROCESS_MEMORY_COUNTERS pmc;
	if (NULL != hProcess) {
		GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc));
	}

	cout << "WorkingSetSize: " << pmc.WorkingSetSize / (1024 * 1024) << " MB" << endl;
	cout << "PeakWorkingSetSize: " << pmc.PeakWorkingSetSize / (1024 * 1024) << " MB" << endl;
	cout << "PagefileUsage: " << pmc.PagefileUsage / (1024 * 1024) << " MB" << endl;
	cout << "PeakPagefileUsage: " << pmc.PeakPagefileUsage / (1024 * 1024) << " MB" << endl;
	cout << endl;

	CloseHandle(hProcess);
}
// 1-3:通过进程ID获取进程句柄
HANDLE GetProcessHandle(DWORD processID)
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);
	return hProcess;
}

/////////////////////////////////////////////////////////////////////
// 进程的打开和关闭
// 2-1: 获取当前exe程序的token
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
			WCHAR *wch = pe32.szExeFile;
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

// 2-2: 打开程序
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
	LPCSTR procName = "EXPLORER.EXE";
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

void StartProcessWay() {
	TCHAR commandLine[] = TEXT("H://software//install//WeChat//WeChat.exe");
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

		printf("进程ID：%d\n", pi.dwProcessId);
		printf("线程ID：%d\n", pi.dwThreadId);
		cout << "Open target process success" << endl;
	}
	else {
		cout << "Open target process failed" << endl;
	}
}

// 2-3:关闭程序
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
			WCHAR *wch = stProcEntry32.szExeFile;
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


/////////////////////////////////////////////////////////////////////
// ②：获取系统进程的名称和ID
void PrintProcessNameAndID(DWORD processID)
{
	TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);

	if (NULL != hProcess)
	{
		HMODULE hMod;
		DWORD cbNeeded;
		if (EnumProcessModules(hProcess, &hMod, sizeof(hMod),&cbNeeded))
		{
			GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(TCHAR));
		}
	}

	if (wcscmp(szProcessName, TEXT("GetProcess.exe")) == 0) {
		cout << "ProcessName And ID: " << endl;
		_tprintf(TEXT("%s  (PID: %u)\n"), szProcessName, processID);
		cout << endl;

		cout << "Process Memory: " << endl;
		GetAssignMemoryInfo(processID);
		cout << endl;
	}

	CloseHandle(hProcess);
}

// ③：对特定进程进行监控
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

// 对特定进程进行监控
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
	DWORD dwBytesRead;
	DWORD dwBytesWritten;
	DWORD dwBuff;
	DWORD dwBPCnt = 0;
	DWORD dwSSCnt = 0;
	DWORD value = 0;
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

					febx = Regs.Esp;
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


// 主函数
int main(int argc, char* argv)
{
	time_t begin = clock();
	
	////////////////////////////////////////////////////////////////////
	// ①：获取当前进程的内存使用情况
	cout << "-------------------------------" << endl;
	cout << "当前进程内存的使用情况：" << endl;
	GetMemoryInfo();
	/*cout << "回收所有可回收的内存" << endl;
	EmptyWorkingSet(GetCurrentProcess());
	GetMemoryInfo();
	cout << "开始动态分配内存" << endl;
	char* buf[5];
	for (int i = 0; i < sizeof(buf) / sizeof(char*); i++)
	{
		buf[i] = new char[102400];
		GetMemoryInfo();
	}
	cout << "开始释放内存" << endl;
	for (int i = 0; i < sizeof(buf) / sizeof(char*); i++)
	{
		delete buf[i];
		buf[i] = NULL;
		GetMemoryInfo();
	}
	cout << "回收所有可回收的内存" << endl;
	EmptyWorkingSet(GetCurrentProcess());
	GetMemoryInfo();
	cout << endl;*/

	////////////////////////////////////////////////////////////////////
	// ②：获取特定进程的内存使用情况
	cout << "-------------------------------" << endl;
	cout << "指定的GetProcess进程内存的使用情况：" << endl;

	DWORD aProcesses[1024], cbNeeded, cProcesses;
	unsigned int i;
	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
	{
		return 1;
	}
	cProcesses = cbNeeded / sizeof(DWORD);

	for (i = 0; i < cProcesses; i++)
	{
		if (aProcesses[i] != 0)
		{
			PrintProcessNameAndID(aProcesses[i]);
		}
	}

	////////////////////////////////////////////////////////////////////
	// ③：获取特定进程的内存使用情况
	// 先获取所有的进程，然后通过进程的名称监控内存的变化
	//DetectmMemory();
	//LPCWSTR pszExe = TEXT("HlpViewer.exe");
	//WatchMemory(0x4168fc, 4, pszExe);

	////////////////////////////////////////////////////////////////////
	// ④：打开和关闭外部程序
	//StartProcess("EXPLORER.EXE");
	//KillProgram("WeChat.exe");
	StartProcessWay();

	time_t end = clock();
	double ret = double(end - begin) / CLOCKS_PER_SEC;
	cout << "This Process Runtime is:" << ret/60. << " min" << endl;
	cout << endl;

	return 0;
}