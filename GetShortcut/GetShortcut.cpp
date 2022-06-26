/** 
  * Opencascade Skarn Copyright (C) 2022
  * @file     GetShortcut.h                                                       
  * @brief    Windows平台全局捕获键盘消息                                                    
  * @author   debugSarn                                                     
  * @date     2022-03-30  
  * @note     
  * @see      
 */

#include<iostream>
#include<windows.h>

#define KEY_Down(vKey) ((GetAsyncKeyState(vKey) & 0x8000) ? 1:0)
using namespace std;

int main() {
	while (1) {
		if (KEY_Down(VK_SHIFT) && KEY_Down('A')) {
			cout << " Shift + A";
			Sleep(150);
		}

		if (KEY_Down(VK_SHIFT) && KEY_Down('B')) {
			cout << " Shift + B";
			Sleep(150);
		}
	}
	return 0;
}

