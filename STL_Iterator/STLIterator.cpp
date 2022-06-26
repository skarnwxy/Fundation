/**
  * Opencascade Skarn Copyright (C) 2022
  * @file     STLIterator.cpp
  * @brief    STL迭代器
  * @author   debugSarn
  * @date     2022-03-30
  * @note
  * @see
 */


#include <iostream>
#include <vector>

int main()
{
    std::cout << "Hello World!\n";
	vector<int>intVect(5);

	vector<int>::iterator out = intVect.begin();

	std::cout << "Vect: ";
	vector <int>::iterator it = intVect.begin();
	while (it != intVect.end())
	{
		std::cout << *it++ << std::endl;
	}
	std::endl;

}

