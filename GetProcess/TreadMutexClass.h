/**
  * Copyright (C) Skarn 2022
  * @file     TreadMutexClass.h
  * @brief    ���߳�ͬ���ͻ���
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

list<int> g_msgRecvQueue;    // �������ݣ��û�д��ʹ洢����

/**
  * @class  TreadMutexClass
  * @brief ���߳�ͬ���ͻ���,���ڴ����̵߳�ʵ������
 */
class TreadMutexClass {
public:

	// ��list��д������
	void inMsgRecvQueue(){
		for (int i = 0; i < 10; ++i){
			my_mutex.lock();
			cout << "���������б�g_msgRecvQueue�в���һ��Ԫ��" << i << endl;
			g_msgRecvQueue.push_back(i);
			my_mutex.unlock();
		}
	}

	// ��list�ж�ȡ����
	void outMsgRecvQueue() {
		for (int i = 0; i < 10; ++i){
			my_mutex.lock();
			if (!g_msgRecvQueue.empty()){
				int command = g_msgRecvQueue.front(); //���ص�һ��Ԫ��
				g_msgRecvQueue.pop_front();           //�Ƴ���һ��Ԫ�ص�������
				cout << "����Ϣ�����б�g_msgRecvQueue��ȡ��һ������" << command << endl;
				my_mutex.unlock();
			}
			else{
				my_mutex.unlock();//�����ж�ʱҪע��ÿ������¶�Ҫ�ж�Ӧ��unlock()
				cout << "outMsgRecvQueueִ�У�������Ϣ����Ϊ��" << i << endl;
			}

		}
	}

private:
	mutex my_mutex;        // �����ź���
};