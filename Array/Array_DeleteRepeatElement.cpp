/**
  * Copyright (C) Skarn 2022
  * @file     Array_DeleteRepeatElement.cpp
  * @brief    删除数组中的重复元素 —— 删除数组中为0的元素
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
  * @brief        删除数组中的0元素
  * @param[in]    arr 传入的数组
  * @param[in]    n 数组的长度
  * @return       返回性数组的长度
  * @see
  * @note         遍历数组，当遇到数组为0的元素时，将后面的元素向前移动，直到最后一个元素
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
  * @brief        删除数组中的0元素
  * @param[in]    arr 传入的数组
  * @param[in]    n 数组的长度
  * @return       返回性数组的长度
  * @see
  * @note         遍历数组，记录元素前0元素的个数，当有多少个0元素则向前移动多少位
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

	// 方法1: 
	std::cout << "方法1: " << std::endl;
	int array1[] = { 0, 1,2,3,0,10,0,-2,0 };
	int n1 = deleteZeroElementInArray1(array1, 9);
	for (auto i = 0; i < n1; i++) {
		std::cout << array1[i] << ",";
	}
	std::cout << std::endl;

	// 方法2：
	std::cout << "方法2: " << std::endl;
	int array2[] = { 0, 1,2,3,0,10,0,-2,0 };
	int n2 = deleteZeroElementInArray2(array2, 9);
	for (auto i = 0; i < n2; i++) {
		std::cout << array2[i] << ",";
	}
	std::cout << std::endl;
}
#endif