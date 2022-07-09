/**
  * Copyright (C) Skarn 2022
  * @file     Array_DeleteRepeatElement.cpp
  * @brief    ɾ�������е��ظ�Ԫ�� ���� ɾ��������Ϊ0��Ԫ��
  * @author   debugSarn
  * @date     2022-07-09
  * @note
  * @see
 */

#define Array_DeleteRepeatElement
#ifdef Array_DeleteRepeatElement

#include <iostream>
using namespace std;

/**
  * @brief        ɾ�������е�0Ԫ��
  * @param[in]    arr ���������
  * @param[in]    n ����ĳ���
  * @return       ����������ĳ���
  * @see
  * @note         �������飬����������Ϊ0��Ԫ��ʱ���������Ԫ����ǰ�ƶ���ֱ�����һ��Ԫ��
 */
int deleteZeroElementInArray1(int arr[], int n) {
	int i = 0;
	while (i < n) {
		if (arr[i] == 0) {
			for (int j = i + 1, k = i; j < n; j++, k++) {
				arr[k] = arr[j];
			}
			n--;
		}
		else {
			i++;
		}
	}

	return n;
}

/**
  * @brief        ɾ�������е�0Ԫ��
  * @param[in]    arr ���������
  * @param[in]    n ����ĳ���
  * @return       ����������ĳ���
  * @see
  * @note         �������飬��¼Ԫ��ǰ0Ԫ�صĸ��������ж��ٸ�0Ԫ������ǰ�ƶ�����λ
 */
int deleteZeroElementInArray2(int arr[], int n) {

	int size = 0;
	for (int i = 0; i < n; i++) {
		if (arr[i] == 0) {
			size++;
		}
		else {
			arr[i - size] = arr[i];
		}
	}

	return n - size;
}

int main() {
	//int a;
	//cin >> a;
	//cout << "Hello World!" << endl;

	// ����1: 
	std::cout << "����1: " << std::endl;
	int array1[] = { 0, 1,2,3,0,10,0,-2,0 };
	int n1 = deleteZeroElementInArray1(array1, 9);
	for (auto i = 0; i < n1; i++) {
		std::cout << array1[i] << ",";
	}
	std::cout << std::endl;

	// ����2��
	std::cout << "����2: " << std::endl;
	int array2[] = { 0, 1,2,3,0,10,0,-2,0 };
	int n2 = deleteZeroElementInArray2(array2, 9);
	for (auto i = 0; i < n2; i++) {
		std::cout << array2[i] << ",";
	}
	std::cout << std::endl;
}
#endif