// 2020.10.21 Add 获取最小值方法

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
int main()
{
	// 步骤1：求和
	vector<int>vecSumIndex;
	int index = 0;
	for (int i = 1; i <= 70; ++i) {
		for (int j = 1; j <= 6; ++j) {
			for (int ii = 1; ii <= 77; ++ii) {
				for (int jj = 1; jj <= 1269; ++jj) {
					int sum = 145 * i + 1781 * j + 132 * ii + 8 * jj;
					if (sum == 10150) {
						++index;
						std::cout << "情况: " << index;
						std::cout << " " << i << ", ";
						std::cout << " " << j << ", ";
						std::cout << " " << ii << ", ";
						std::cout << " " << jj << ", "; 
						int sumIndex = i + j + ii + jj;
						std::cout << " "<< sumIndex;
						vecSumIndex.push_back(sumIndex);
						std::cout << std::endl;
					}
				}
			}
		}
	}
	// 步骤2: 找最小值
	// 方法1：
	std::cout << std::endl;
	auto smallest = std::min_element(std::begin(vecSumIndex), std::end(vecSumIndex));
	std::cout << "Min element is " << *smallest << " at position " << std::distance(std::begin(vecSumIndex), smallest+1) << std::endl;
	// 方法2： 
	//for (int i = vecSumIndex.size() - 1; i > 0; --i)
	//{
	//	if (vecSumIndex[i] < vecSumIndex[i - 1])
	//	{
	//		int temp = vecSumIndex[i - 1];
	//		vecSumIndex[i - 1] = vecSumIndex[vecSumIndex.size() - 1];
	//		vecSumIndex[vecSumIndex.size() - 1] = temp;
	//		vecSumIndex.pop_back();
	//		continue;
	//	}
	//	if (vecSumIndex[i] > vecSumIndex[i - 1])
	//	{
	//		int num = vecSumIndex.size();
	//		for (int j = 0; j < num - i; ++j)
	//		{
	//			vecSumIndex.pop_back();
	//		}
	//	}
	//}
	//for (int i = vecSumIndex.size() - 1; i >= 0; --i)
	//{
	//	std::cout << "Min：" << vecSumIndex[i] << std::endl;
	//}

	int i = 0;

	//int a = (++i) + (++i) + (++i);

	int a1 = ++i;
	int a2 = ++i;
	int a3 = ++i;


	int j = 0;
	int b = (j++) + (j++) + (j++);

	int b1 = j++;
	int b2 = j++;
	int b3 = j++;

	//std::cout << a << std::endl;
	std::cout << a1 << std::endl;
	std::cout << a2 << std::endl;
	std::cout << a3 << std::endl;

	std::cout << b << std::endl;
	std::cout << b1 << std::endl;
	std::cout << b2 << std::endl;
	std::cout << b3 << std::endl;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
