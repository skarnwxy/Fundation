/**
  * Copyright (C) Skarn 2022
  * @file     Array_CoordSort.cpp
  * @brief    �����㷨���
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
   * @brief �����࣬������ά����(X, Y ,Z)
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
	// ��ֵ��������(������Ĭ��ʵ�֣�ֵ����)
	void operator=(Coord iCoord) {
		X = iCoord.getX();
		Y = iCoord.getY();
		Z = iCoord.getZ();
	}

	// ����ֵ���
	int sum() {
		return X + Y + Z;
	}

	// ��ȡ����
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
  * @brief        ������ά����֮�͵Ĵ�С����
  * @param[in]    iArray ��������
  * @param[in]    length ��������鳤��
  * @return
  * @see
  * @note         ����ð������
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

///< �洢��ά������������ֵ(���Զ���)
const int MaxSize = 50;

///< ��ں���
int main() {
	//////////////////////////////////////////////////////////////////////////
	std::cout << "------------------------------------------------" << std::endl;
	// ���������
	Coord array3(1, 2, 3);
	Coord array1(7, 8, 9);
	Coord array2(4, 5, 6);
	Coord array0(10, 11, 13);
	Coord array4(4, 5, 18);
	Coord array5(4, 18, 6);

	// ��������
	Coord array[MaxSize];
	array[0] = array0;
	array[1] = array1;
	array[2] = array2;
	array[3] = array3;
	array[4] = array4;
	array[5] = array5;

	// ����ǰ�Ľ��
	std::cout << "����ǰ�Ľ����" << std::endl;
	for (auto ii = 0; ii < 6; ++ii) {
		Coord coord = array[ii];
		std::cout << "(" << coord.getX() << ", ";
		std::cout << coord.getY() << ", ";
		std::cout << coord.getZ() << ")";
		std::cout << std::endl;
	}

	// ð������
	bubbleSortCoord(array, 6);

	// ���������
	std::cout << "�����Ľ����" << std::endl;
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