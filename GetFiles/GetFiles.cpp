/**
  * Opencascade Skarn Copyright (C) 2022
  * @file     GetFiles.cpp
  * @brief    获取文件夹的中的文件
  * @author   debugSkarn
  * @date     2022.12.17
  * @note
  * @see
 */

#pragma once

#include <iostream>
#include <io.h>
#include <string>
#include <vector>

#include<filesystem>    ///< 需要设置语言标准（C++17）
#include <fstream>
using namespace std;
using namespace std::filesystem;

//////////////////////////////////////////////////////////////////////////
/** 
  * @brief        所有指定目录下的所有文件
  * @see          
  * @note          
 */
void getAllFiles(string path, vector <string> & files)
{
	long hFile = 0;
	struct _finddata_t fileinfo;
	string pathp;
	if ((hFile = _findfirst(pathp.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					getAllFiles(pathp.assign(path).append("/").append(fileinfo.name), files);
				}
			}
			else
			{
				string filestr = fileinfo.name;
				files.push_back(filestr);
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

/** 
  * @brief        获取指定目录下制定格式的文件
  * @see          
  * @note          
 */
void getFiles(string path, vector<string>& files, string fileType)
{
	// 文件句柄
	long long hFile = 0;
	// 文件信息
	struct _finddata_t fileinfo;

	string p;

	if ((hFile = _findfirst(p.assign(path).append("\\*" + fileType).c_str(), &fileinfo)) != -1) {
		do {
			// 保存文件的全路径
			files.push_back(p.assign(path).append("\\").append(fileinfo.name));

		} while (_findnext(hFile, &fileinfo) == 0);  //寻找下一个，成功返回0，否则-1

		_findclose(hFile);
	}
}

////////////////////////////////C++ 17中的filesystem//////////////////////////////////////////
/** 
  * @brief        检查一个路径是否是目录
  * @see          
  * @note          
 */
bool checkIsDir(const string& dir) {
	if (!exists(dir)) {
		cout << dir << " not exists. Please check." << endl;
		return false;
	}
	directory_entry entry(dir);
	if (entry.is_directory())
		return true;
	return false;
}

/** 
  * @brief        搜索一个目录下所有的图像文件，以 指定后缀 结尾的文件
  * @see          
  * @note          
 */
void getAllFileNames(const string dir, const vector<string> suffixs, vector<string>& files) {
	// 首先检查目录是否为空，以及是否是目录
	if (!checkIsDir(dir)) return;
	// 递归遍历所有的文件
	directory_iterator iters(dir);
	for (auto &iter : iters) {
		// 路径的拼接
		string file_path(dir);
		file_path += "/"; 
		file_path += iter.path().filename().string();
		
		// 查看是否是目录，如果是目录则循环递归
		if (checkIsDir(file_path)) {
			getAllFileNames(file_path, suffixs, files);
		}
		//不是目录则检查后缀是否是指定类型
		else { 
			// 获取文件的后缀名
			string extension = iter.path().extension().string(); 
			
			// 可以扩展成你自己想要的文件类型来进行筛选, 比如加上.gif .bmp之类的
			for (auto s : suffixs) {
				if (extension == s) {
					files.push_back(file_path);
					break;
				}
			}

		}
	}
}

int main(int argc, char** argv)
{
	//////////////////////////////////////////////////////////////////////////
	//string strFileName = "G:\\Code\\branch_Fundation\\GetFiles";
	string strFileName = "F:\\avic\\日常工作\\居家办公\\2022年11月居家办公_SolidWorksAndInventor大装配数据构建\\开发一部居家办公成果_王小垚_2022年11月29日-12月2日\\特征模型\\Inventor模型\\CubeMode";

	vector <string> files;
	getFiles(strFileName, files, ".ipt");

	//////////////////////////////////////////////////////////////////////////
	// 准备数据（dir: 目录；suffixs: 指定的文件后缀）
	string dir = argv[1];
	vector<string> suffixs;
	try
	{
		dir = argv[1];
		for (int i = 2; i < argc; i++) {
			suffixs.push_back(argv[i]);
		}
	}
	catch (const std::exception& e)
	{
		cout << e.what() << endl;
		return EXIT_FAILURE;
	}

	// 获取所有的文件
	vector<string> fileNames;
	getAllFileNames(dir, suffixs, fileNames);

	// 存储或输出文件
	ofstream fout(dir + "/fileNames.txt");
	for (auto item : fileNames) {
		cout << item << endl;
		fout << item << endl;
	}
	fout.close();
	cout << endl;

	// 输出结果
	string suff = "";
	for (auto item : suffixs) {
		suff += "  *";
		suff += item;
		suff += " ";
	}
	cout << "There are " << fileNames.size() << suff << argv[1] << endl;

}

