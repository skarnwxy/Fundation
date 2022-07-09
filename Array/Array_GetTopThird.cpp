/**
  * Copyright (C) Skarn 2022
  * @file     Array_GetTopThird.cpp
  * @brief    查找数组中第三大的元素
  * @author   debugSarn
  * @date     2022-07-09
  * @note
  * @see
 */

 //#define Array_GetTopThird
#ifdef Array_GetTopThird

#include <iostream>

 /**
   * @brief        交换位置
   * @param[in]    iFirst 参数1
   * @param[out]   iSecond 参数2
   * @return
   * @see
   * @note
  */
void swap(int iFirst, int iSecond) {
	auto tmp = iFirst;
	iFirst = iSecond;
	iSecond = iFirst;
}

/**
  * @brief        获取数组中第三个的元素
  * @param[in]    iArray 传入的数组
  * @param[in]    length 数组的长度
  * @return
  * @see
  * @note
 */
int getTopThird(int iArray[], int length) {

	// 获取数组的前3个值，并按照从小到大的顺序排列
	int a = iArray[0];
	int b = iArray[1];
	int c = iArray[2];

	if (b > a) {
		swap(a, b);
	}
	if (c > a) {
		swap(a, c);
	}
	if (c > b) {
		swap(b, c);
	}

	// 遍历数组的其他元素
	for (auto i = 3; i < length; ++i) {
		auto targetEle = iArray[i];

		if (targetEle > a) {
			c = b;
			b = a;
			a = targetEle;
		}
		else if (targetEle > b) {
			c = b;
			b = targetEle;
		}
		else if (targetEle > c) {
			c = targetEle;
		}
	}

	return c;
}

int main()
{
	int array[10] = { 2,3,6,8,10,13,13,34 };
	int c = getTopThird(array, sizeof(array));
	std::cout << "第三大的元素是：" << c << std::endl;
}
#endif