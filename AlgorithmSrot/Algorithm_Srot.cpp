/**
  * Copyright (C) Skarn 2022
  * @file     Algorithm_sort.cpp
  * @brief    排序算法相关
  * @author   debugSarn
  * @date     2022-07-01
  * @note
  * @see
 */

#include <iostream>

/**
  * @File  Coord
  * @brief 坐标类，管理三维坐标(X, Y ,Z)
 */
class Coord {
public:
	Coord(){
		X = 0;
		Y = 0;
		Z = 0;
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

//////////////////////////////////////////////////////////////////////////
/**
  * @brief        简单数组冒泡排序
  * @param[in]    iArray 传入数组
  * @param[in]    length 传入的数组长度
  * @return
  * @see
  * @note         采用冒泡排序
  *               从第一个元素开始，与相邻的后一个元素比较大小，若本身比较大则换位，把大的换到后面，一次循环比较完最后一个元素为止，然后再次重复循环，直到所有元素都按从小到大排序为止。
 */
void bubbleSort(int arr[], int length)
{
	// 思路1: 
	//循环n-1轮
	for (auto i = 0; i < length-1; i++)
	{
		//每轮把最大的1个放到了最后面
		for (auto j = 0; j < length - 1- i; j++)
		{
			if (arr[j] > arr[j + 1])
			{
				auto temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
		}
		for (int k = 0; k < length; ++k) {
			std::cout << arr[k] << ", ";
		}
		std::cout << std::endl;
	}

	// 思路2：
	//for (auto i = 0; i < length-1; i++)
	//{
	//	// j从前往后循环
	//	for (auto j = length-2; j >= i ; j--)
	//	{
	//		// 每轮把最大的1个放到了最后面
	//		if (arr[j] > arr[j + 1])
	//		{
	//			auto temp = arr[j];
	//			arr[j] = arr[j + 1];
	//			arr[j + 1] = temp;
	//		}
	//	}
	//	for (int k = 0; k < length; ++k) {
	//		std::cout << arr[k] << ", ";
	//	}
	//	std::cout << std::endl;
	//}

}

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

//////////////////////////////////////////////////////////////////////////
int partion(int arr[], int left, int right)
{
	auto i = left;
	auto K = arr[right];   // 存放主元素
	for (auto j = left; j < right; j++)
	{
		if (arr[j] < K)
		{  //交换值
			auto temp = arr[i];
			arr[i] = arr[j];
			arr[j] = temp;
			i++;
		}
	}
	arr[right] = arr[i];
	arr[i] = K;
	return i;//把主元的下标返回
}

/**
  * @brief        简单数组快速排序
  * @param[in]    iArray 传入数组
  * @param[in]    length 传入的数组长度
  * @return
  * @see
  * @note         采用快速排序
 */
void quickSort(int arr[], int left, int right)
{
	if (left < right)
	{
		auto center = partion(arr, left, right);

		for (int k = 0; k <= left + right; ++k) {
			std::cout << arr[k] << ", ";
		}
		std::cout << std::endl;

		quickSort(arr, left, center - 1);//左半部分
		quickSort(arr, center + 1, right);//右半部分
	}
}

//////////////////////////////////////////////////////////////////////////
/**
  * @brief        简单数组选择排序
  * @param[in]    iArray 传入数组
  * @param[in]    length 传入的数组长度
  * @return
  * @see
  * @note         采用选择排序（跟全部的数作比较）
  *               首先，找到数组中最小的那个元素，其次，将它和数组的第一个元素交换位置(如果第一个元素就是最小元素那么它就和自己交换)。其次，在剩下的元素中找到最小的元素，将它与数组的第二个元素交换位置。如此往复，直到将整个数组排序。这种方法我们称之为选择排序
 */
void selectSort(int arr[], int length)
{
	for (auto i = 0; i < length-1; i++){
		for (auto j = i + 1; j < length; j++){
			if (arr[i] > arr[j]){
				auto temp = arr[i];
				arr[i] = arr[j];
				arr[j] = temp;
			}
		}

		for (int k = 0; k < length; ++k) {
			std::cout << arr[k] << ", ";
		}
		std::cout << std::endl;
	}
}

/**
  * @brief        简单数组插入排序
  * @param[in]    iArray 传入数组
  * @param[in]    length 传入的数组长度
  * @return
  * @see
  * @note         采用选择排序（跟前面的数作比较）
  *               插入排序就是在一个有序的序列中，插入一个数，这个序列照样有序。第一个数默认有序，这个序列的第二个数和第一个比较，若是第二个比第一个小就交换位置，那么前两个就是有序的了。然后循环序列中的第三个数，第三个数首先和第二个比较，若是小于第二个，交换位置，大于则不换位置。再和第一个比较，若比第-个小，和第一个交换位置，若比第一个大，不变。一直这样循环下去，直至最后一个。
 */
void insertSort(int arr[], int length) {
	int j;
	for (int i = 1; i < length; i++) {
		int tmp = arr[i];
		for (j = i; j > 0 && tmp < arr[j - 1]; j--) {
			arr[j] = arr[j - 1];
		}
		arr[j] = tmp;

		for (int k = 0; k < length; ++k) {
			std::cout << arr[k] << ", ";
		}
		std::cout << std::endl;
	}
}

///< 存储三维坐标数组的最大值(可自定义)
const int MaxSize = 50;

///< 入口函数
int main() {
	//////////////////////////////////////////////////////////////////////////
	std::cout << "------------------------------------------------" << std::endl;
	int length = 10;
	int arr1[10] = { 2,6,3,5,10,7,9,8,1,6 };
	std::cout << "排序前: " << std::endl;
	for (int i = 0; i < length; i++){
		std::cout << arr1[i] << ", ";
	}
	std::cout << std::endl;

	std::cout << "冒泡排序排序后: " << std::endl;
	bubbleSort(arr1, length);
	for (int i = 0; i < length; i++){
		std::cout << arr1[i] << ", ";
	}
	std::cout << std::endl;

	std::cout << "快速排序排序后: " << std::endl;
	int arr2[10] = { 2,6,3,5,10,7,9,8,1,6 };
	//quickSort(arr2, 0, length-1);
	for (int i = 0; i < length; i++) {
		std::cout << arr2[i] << ", ";
	}
	std::cout << std::endl;

	std::cout << "选择排序排序后: " << std::endl;
	int arr3[10] = { 2,6,3,5,10,7,9,8,1,6 };
	selectSort(arr3, length);
	for (int i = 0; i < length; i++) {
		std::cout << arr3[i] << ", ";
	}
	std::cout << std::endl;

	std::cout << "插入排序排序后: " << std::endl;
	int arr4[10] = { 2,6,3,5,10,7,9,8,1,6 };
	insertSort(arr4, length);
	for (int i = 0; i < length; i++) {
		std::cout << arr4[i] << ", ";
	}
	std::cout << std::endl;

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