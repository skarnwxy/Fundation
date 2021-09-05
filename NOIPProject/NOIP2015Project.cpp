// NOIPProject.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
// 2021.1.10 Add NOIP2015年编程题

#define NOIP29

//--------------------------------------
#ifdef NOIP21

#include <iostream>
using namespace std;

int fun(int n, int fromPos, int toPos) {
	int t, tot;
	if (n == 0)
		return 0;
	for (t = 1; t <= 3; t++)
		if (t != fromPos && t != toPos)
			break;
	tot = 0;
	tot += fun(n - 1, fromPos, t);
	tot++;
	tot += fun(n - 1, t, toPos);
	return tot;
}

int main()
{
	int n;
	cin >> n;
	cout << fun(n, 1, 3) << endl;
}
#endif 

//--------------------------------------
#ifdef NOIP22
#include <iostream>
#include <string>

using namespace std;

int main() {
	int len, maxlen;
	string s, ss;
	maxlen = 0;
	do {
		cin >> ss;
		len = ss.length();
		if (ss[0] == '#')
			break;
		if (len > maxlen) {
			s = ss;
			maxlen = len;
		}
	} while (true);
	cout << s << endl;
	return 0;
}
#endif

//--------------------------------------
#ifdef NOIP23

#include <iostream>
using namespace std;

void fun(char *a, char *b) {
	a = b;
	(*a)++;
}

int main() {
	char c1, c2, *p1, *p2;
	c1 = 'A';
	c2 = 'a';
	p1 = &c1;
	p2 = &c2;
	fun(p1, p2);
	cout << c1 << c2 << endl;
	return 0;
}
#endif

//--------------------------------------
#ifdef NOIP24
#include <iostream>
using namespace std;

struct point {
	int x;
	int y;
};

int main() {
	struct EX {
		int a;
		int b;
		point c;
	} e;
	e.a = 1;
	e.b = 2;
	e.c.x = e.a + e.b;
	e.c.y = e.a * e.b;
	cout << e.c.x << ',' << e.c.y << endl;
	return 0;
}
#endif

//--------------------------------------
#ifdef NOIP27
// 题目内容：
// （双子序列最大和）给定一个长度为 n（3 ≤ n ≤ 1000）的整数序列，要求从中选出两个 连续子序列，使得这两个连续子序列的序列和之和最大，最终只需输出这个最大和。一 个连续子序列的序列和为该连续子序列中所有数之和。要求：每个连续子序列长度至少 为 1，且两个连续子序列之间至少间隔 1 个数

#include <iostream> 
using namespace std;
const int MAXN = 1000;
int n, i, ans, sum;
int x[MAXN];
int lmax[MAXN]; // lmax[i]为仅含 x[i]及 x[i]左侧整数的连续子序列的序列和中，最大的序列和 
int rmax[MAXN]; // rmax[i]为仅含 x[i]及 x[i]右侧整数的连续子序列的序列和中，最大的序列和 
int main() {
	// 步骤1：用数组表示整数序列，给整数序列赋值
	cin >> n;
	for (i = 0; i < n; i++) {
		cin >> x[i];
	}

	// 步骤2：
	lmax[0] = x[0];
	for (i = 1; i < n; i++) {
		if (lmax[i - 1] <= 0) {
			lmax[i] = x[i];
		}
		else {
			lmax[i] = lmax[i - 1] + x[i];
		}
	}

	for (i = 1; i < n; i++) {
		if (lmax[i] < lmax[i - 1]) {
			lmax[i] = lmax[i - 1];
		}
	}

	// (1);
	rmax[n-1] = x[n-1];

	// 步骤3：
	for (i = n - 2; i >= 0; i--) {
		if (rmax[i + 1] <= 0) {
			//(2);
			rmax[i] = x[i];
		}
		else {
			//(3);
			rmax[i] = rmax[i + 1] + x[i];
		}
	}

	for (i = n - 2; i >= 0; i--) {
		if (rmax[i] < rmax[i + 1]) {
			//(4);
			rmax[i] = rmax[i + 1];
		}
	}

	// 步骤3：
	ans = x[0] + x[2];
	for (i = 1; i < n - 1; i++) {
		//sum = (5);
		sum = lmax[i - 1] + rmax[i];
		//sum += rmax[i] + lmax[i-1];

		if (sum > ans) {
			ans = sum;
		}
	}
	cout << ans << endl;
	return 0;
}
#endif

//--------------------------------------
#ifdef NOIP28
// 题目内容：
// （最短路径问题）无向连通图 G 有 n 个结点，依次编号为 0,1,2,...,(n-1)。用邻接矩阵的 形式给出每条边的边长，要求输出以结点 0 为起点出发，到各结点的最短路径长度。 使用 Dijkstra 算法解决该问题：利用 dist 数组记录当前各结点与起点的已找到的最 短路径长度；每次从未扩展的结点中选取 dist 值最小的结点 v 进行扩展，更新与 v 相邻 的结点的 dist 值；不断进行上述操作直至所有结点均被扩展，此时 dist 数据中记录的值 即为各结点与起点的最短路径长度。（第五空 2 分，其余 3 分）

#include <iostream> 
using namespace std;
const int MAXV = 100;
int n, i, j, v; int w[MAXV][MAXV]; // 邻接矩阵，记录边长 // 其中 w[i][j]为连接结点 i 和结点 j 的无向边长度，若无边则为-1 
int dist[MAXV]; int used[MAXV]; // 记录结点是否已扩展（0：未扩展；1：已扩展） 
int main()
{
	// 步骤1:
	cin >> n;
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			cin >> w[i][j];
		}
	}

	// 步骤2：
	dist[0] = 0;
	for (i = 1; i < n; i++) {
		dist[i] = -1;
	}
	for (i = 0; i < n; i++) {
		used[i] = 0;
	}

	// 步骤3:
	while (true) {
		// (1);
		v = -1;

		for (i = 0; i < n; i++) {
			if (used[i] != 1 && dist[i] != -1 && (v == -1 || dist[i]<dist[v]/*(2)*/)) {
				// (3);
				v = i;
			}
		}

		if (v == -1) {
			break;
		}
		
		// (4);
		used[v] = 1;

		for (i = 0; i < n; i++) {
			if (w[v][i] != -1 && (dist[i] == -1 || dist[v]+w[v][i] < dist[i]/*(5)*/)) {
				dist[i] = dist[v] + w[v][i];
			}
		}
	}
	for (i = 0; i < n; i++) {
		cout << dist[i] << endl;
	}

	return 0;
}
#endif

//--------------------------------------
#ifdef NOIP30

#include <iostream>
using namespace std;

int main() {
	int sum = 0;

	for (int i = 1; i <= 5; ++i) {
		for (int j = i; j <= 5; ++j) {
			if (j%i == 0) {
				continue;
			}
			sum += j;
			if (j>=i*i) {
				break;
			}
		}
	}

	cout << sum << endl;
	return 0;
}

#endif

//--------------------------------------
#ifdef NOIP31

#include <iostream>
using namespace std;

int main() {
	int a, cnt = 0;
	cin >> a;
	while (a>0)
	{
		if (a%10 == 5) {
			cnt++;
		}

		a /= 10;
	}
	cout << cnt << endl;

	return 0;
}

#endif

//--------------------------------------
#ifdef NOIP32

#include <iostream>
#include <vector>
using namespace std;

int main() {
	vector<int> a[10];

	for (int i = 1; i <= 100; ++i) {
		for (int j = 1; j <= 9; ++j) {
			if (i%j == 0) {
				a[j].push_back(i);
			}
		}
	}

	cout << "a[6][a[6].size() - 1]: " << a[6][a[6].size() - 1] << endl;

	cout << "a[9].size(): " << a[9].size() << endl;

	cout << "a[9][20]: " << a[9][20] << endl;

	//cout << "a.size()" << a.size() << endl;

	cout << "a[10].size(): " << a[10].size() << endl;

}

#endif

//--------------------------------------
#ifdef NOIP33

#include <iostream>
#include <vector>
using namespace std;

int main() {
	// 步骤1：
	int m;
	int n;

	cin >> m;
	cin >> n;
	if (m < 0 || m>10) {
		return 0;
	}
	if (n < 0 || n>16) {
		return 0;
	}

	// 步骤2：
	const int p = 10;
	const int q = 16;
	vector<vector<char>>v_int(p,vector<char>(q));

	for (int i = 65; i <= 74; ++i) {
		for(int j=0; j<= 15; ++j) {
			v_int[i-65][j] = i + j;
			//cout << v_int[i-65][j];
		}
		//cout << endl;
	}

	// 步骤3：
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j) {
			cout << v_int[i][j];
		}
		cout << endl;
	}

	return 0;
}

#endif

//--------------------------------------
#ifdef NOIP34

#include <iostream>
#include <math.h>
using namespace std;

#define value  0.01

int main() {
	int fun = 200;
	
	// 步骤1：输入存钱年数和房子增长率
	int n;
	double k;
	cin >> n >> k;

	if (n<10 || n>50) {
		return 0;
	}
	if (k<1 || k>20) {
		return 0;
	}

	// 步骤2:
	int yearFlag = 0;
	double sumRoomValue2 = fun;
	double sumSaveValue = 0;
	double increase = (1 + k / 100);
	for (int i = 1; i <= 30; ++i) {
		// 复利模式
		//double powValue = pow(increase, i);
		//double sumRoomValue1 = fun * powValue;
		// 单利模式
		//sumRoomValue2 = fun * (1 + i*(k / 100));

		sumSaveValue += n;
		if (sumSaveValue >= sumRoomValue2 /*fabs(sumRoomValue - sumSaveValue) < value*/) {
			yearFlag = i;
			cout << yearFlag << endl;
			return 0;
		}
		else {
			sumRoomValue2 *= increase;
		}
	}
	cout << "NO" << endl;

	// 步骤3：
	//if (yearFlag >= 1 && yearFlag <= 30) {
	//	cout << yearFlag << endl;
	//}
	//if (yearFlag == 31) {
	//	cout << "NO" << endl;
	//}

	return 0;
}

#endif

//--------------------------------------
#ifdef NOIP35

#include <iostream>
using namespace std;

int main() {
	// 步骤1：输入学生人数
	int numOfStudent;
	cin >> numOfStudent;
	if (numOfStudent<3 || numOfStudent>40) {
		return 0;
	}

	// 步骤2：输入学生成绩
	char* nameOfStudent[40];          // 学生姓名
	int chineseGradeOfStudent[40];    // 学生语文成绩
	int mathGradeOfStudent[40];       // 学生数学成绩 
	int englishGradeOfStudent[40];    // 学生英文成绩 
	int geograhpyGradeOfStudent[40];  // 学生地理成绩 

	for (int i = 0; i < numOfStudent; ++i) {
		cin >> nameOfStudent[i];
		cin >> chineseGradeOfStudent[i];
		cin >> mathGradeOfStudent[i];
		cin >> englishGradeOfStudent[i];
		cin >> geograhpyGradeOfStudent[i];
		cout << endl;
	}

	// 步骤3：计算学生总分
	int sumOfGrade[40];
	for (int i = 0; i < numOfStudent; ++i) {
		sumOfGrade[i] = chineseGradeOfStudent[i] + mathGradeOfStudent[i] + englishGradeOfStudent[i] + geograhpyGradeOfStudent[i];
	}

	// 步骤4：求总分前三的学生
	int gradeOfTemp = 0;
	char* nameTemp = new char[10];
	for (int i = 0; i < numOfStudent; ++i) {
		for (int j = 0; j < numOfStudent-i; ++j) {
			if (sumOfGrade[j]>sumOfGrade[j+1]) {
				gradeOfTemp = sumOfGrade[j];
				sumOfGrade[j] = sumOfGrade[j + 1];
				sumOfGrade[j + 1] = gradeOfTemp;

				nameTemp = nameOfStudent[j];
				nameOfStudent[j] = nameOfStudent[j + 1];
				nameOfStudent[j + 1] = nameTemp;
			}
		}
	}

	// 步骤5：输出前三名
	for (int i = 0; i < 3; ++i) {
		cout << nameOfStudent[i] << endl;
	}

	return 0;
}

#endif

//--------------------------------------
#ifdef NOIP29
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <climits>
#include <cstring>
#include <string>
#include <algorithm>
#include <vector>
#include <deque>
#include <list>
#include <utility>
#include <set>
#include <map>
#include <stack>
#include <queue>
#include <bitset>
#include <iterator>
#define INT_MAX 0x7fffffff
#define INT_MIN 0x80000000
using namespace std;

struct stu
{
	string name;
	int m;
	int c;
	int e;
	int g;
	int sum;
}a[1005];

int cmp(stu a, stu b)
{
	if (a.sum == b.sum) {
		return a.name < b.name;
	}
	return a.sum > b.sum;
}

int main()
{
	int n;
	while (cin >> n) {
		for (int i = 0; i < n; i++) {
			cin >> a[i].name >> a[i].m >> a[i].c >> a[i].e >> a[i].g;
			a[i].sum += a[i].m + a[i].c + a[i].e + a[i].g;
		}
		sort(a, a + n, cmp);
		for (int i = 0; i < 3; i++) {
			cout << a[i].name << endl;
		}
	}

	return 0;
}
#endif


// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
