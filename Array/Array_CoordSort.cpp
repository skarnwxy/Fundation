/**
  * Copyright (C) Skarn 2022
  * @file     Array_CoordSort.cpp
  * @brief    排序算法相关
  * @author   debugSarn
  * @date     2022-07-09
  * @note
  * @see
 */

 //#define Array_CoordSort
#ifdef Array_CoordSort

#include <iostream>

 /**
   * @File  Coord
   * @brief 坐标类，管理三维坐标(X, Y ,Z)
  */
class Coord {

public:
	Coord() {
		X = 0; Y = 0; Z = 0;
	}
	Coord(int iX, int iY, int iZ) {
		X = iX; Y = iY; Z = iZ;
	}
	~Coord() {

	}
	// 赋值运算重载(定义类默认实现，值拷贝)
	void operator=(Coord iCoord) {
		X = iCoord.getX();
		Y = iCoord.getY();
		Z = iCoord.getZ();
	}

	// 坐标值求和
	int sum() {
		return X + Y + Z;
	}

	// 获取坐标
	int getX() {
		return X;
	}
	int getY() {
		return Y;
	}
	int getZ() {
		return Z;
	}
private:
	int X;
	int Y;
	int Z;
};

/**
  * @brief        按照三维坐标之和的大小排序
  * @param[in]    iArray 传入数组
  * @param[in]    length 传入的数组长度
  * @return
  * @see
  * @note         采用冒泡排序
 */
void bubbleSortCoord(Coord iArray[], int length) {
	for (auto i = 0; i < length - 1; ++i) {
		for (auto j = 0; j < length - 1 - i; ++j) {
			Coord coordPre = iArray[j];
			int addValuePre = coordPre.sum();

			Coord coordNext = iArray[j + 1];
			int addValueNext = coordNext.sum();

			if (addValuePre > addValueNext) {
				Coord tmp = iArray[j];
				iArray[j] = iArray[j + 1];
				iArray[j + 1] = tmp;
			}
		}
	}
}

///< 存储三维坐标数组的最大值(可自定义)
const int MaxSize = 50;

///< 入口函数
int main() {
	//////////////////////////////////////////////////////////////////////////
	std::cout << "------------------------------------------------" << std::endl;
	// 构造类对象
	Coord array3(1, 2, 3);
	Coord array1(7, 8, 9);
	Coord array2(4, 5, 6);
	Coord array0(10, 11, 13);
	Coord array4(4, 5, 18);
	Coord array5(4, 18, 6);

	// 构造数组
	Coord array[MaxSize];
	array[0] = array0;
	array[1] = array1;
	array[2] = array2;
	array[3] = array3;
	array[4] = array4;
	array[5] = array5;

	// 排序前的结果
	std::cout << "排序前的结果：" << std::endl;
	for (auto ii = 0; ii < 6; ++ii) {
		Coord coord = array[ii];
		std::cout << "(" << coord.getX() << ", ";
		std::cout << coord.getY() << ", ";
		std::cout << coord.getZ() << ")";
		std::cout << std::endl;
	}

	// 冒泡排序
	bubbleSortCoord(array, 6);

	// 输出排序结果
	std::cout << "排序后的结果：" << std::endl;
	for (auto ii = 0; ii < 6; ++ii) {
		Coord coord = array[ii];
		std::cout << "(" << coord.getX() << ", ";
		std::cout << coord.getY() << ", ";
		std::cout << coord.getZ() << ")";
		std::cout << std::endl;
	}

	system("pause ");
}
#endif