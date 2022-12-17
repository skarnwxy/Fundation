/**
  * Copyright (C) Skarn 2022
  * @file     TreadMutexClass.h
  * @brief    多线程同步和互斥
  * @author   debugSkarn
  * @date     2022-12-16
  * @note
  * @see
 */

#include <iostream>
#include <windows.h>
#include <thread>
#include <vector>
#include <list>
#include <mutex>

using namespace std;

list<int> g_msgRecvQueue;    // 共享数据，用户写入和存储数据

/**
  * @class  TreadMutexClass
  * @brief 多线程同步和互斥,用于创建线程的实例函数
 */
class TreadMutexClass {
public:

	// 在list中写入数据
	void inMsgRecvQueue(){
		for (int i = 0; i < 10; ++i){
			my_mutex.lock();
			cout << "共享数据列表g_msgRecvQueue中插入一个元素" << i << endl;
			g_msgRecvQueue.push_back(i);
			my_mutex.unlock();
		}
	}

	// 从list中读取数据
	void outMsgRecvQueue() {
		for (int i = 0; i < 10; ++i){
			my_mutex.lock();
			if (!g_msgRecvQueue.empty()){
				int command = g_msgRecvQueue.front(); //返回第一个元素
				g_msgRecvQueue.pop_front();           //移除第一个元素但不返回
				cout << "从消息数据列表g_msgRecvQueue中取出一个数据" << command << endl;
				my_mutex.unlock();
			}
			else{
				my_mutex.unlock();//进行判断时要注意每种情况下都要有对应的unlock()
				cout << "outMsgRecvQueue执行，但是消息队列为空" << i << endl;
			}

		}
	}

private:
	mutex my_mutex;        // 互斥信号量
};