/**
  * Copyright (C) Skarn 2022
  * @file     Array_GetTopThird.cpp
  * @brief    ���������е������Ԫ��
  * @author   debugSarn
  * @date     2022-07-09
  * @note
  * @see
 */

 //#define Array_GetTopThird
#ifdef Array_GetTopThird

#include <iostream>

 /**
   * @brief        ����λ��
   * @param[in]    iFirst ����1
   * @param[out]   iSecond ����2
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
  * @brief        ��ȡ�����е�������Ԫ��
  * @param[in]    iArray ���������
  * @param[in]    length ����ĳ���
  * @return
  * @see
  * @note
 */
int getTopThird(int iArray[], int length) {

	// ��ȡ�����ǰ3��ֵ�������մ�С�����˳������
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

	// �������������Ԫ��
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
	std::cout << "�������Ԫ���ǣ�" << c << std::endl;
}
#endif