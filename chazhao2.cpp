// chazhao2.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <Windows.h>
#include <fstream>
#include <iterator>
#include <string>

#include "mysql.h"
using namespace std;


void find(char* lpPath, std::vector<const std::string> &filelist)
{
	char szFind[1024];
	WIN32_FIND_DATAA FindFileData;

	strcpy(szFind, lpPath);
	strcat(szFind, "\\*.*");

	HANDLE hFind = ::FindFirstFileA(szFind, &FindFileData);

	if(INVALID_HANDLE_VALUE == hFind) return;
	
	while(true)
	{
		if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if(FindFileData.cFileName[0] != '.')
			{
				char szFile[1024];
				strcpy(szFile, lpPath);
				strcat(szFile, "\\");
				strcat(szFile, (char*)(FindFileData.cFileName));
				find(szFile, filelist);

			}
		}
		else
		{
			char szFile2[1024];
			strcpy(szFile2, lpPath);
			strcat(szFile2, "\\");
			strcat(szFile2, FindFileData.cFileName);
			filelist.push_back(szFile2);
		}
		if(!FindNextFileA(hFind, &FindFileData)) break;
	}
	FindClose(hFind);
}

string UTF8ToGB(const char* str)
{
     string result;
     WCHAR *strSrc;
     LPSTR szRes;

     //获得临时变量的大小
     int i = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
     strSrc = new WCHAR[i+1];
     MultiByteToWideChar(CP_UTF8, 0, str, -1, strSrc, i);

     //获得临时变量的大小
     i = WideCharToMultiByte(CP_ACP, 0, strSrc, -1, NULL, 0, NULL, NULL);
     szRes = new CHAR[i+1];
     WideCharToMultiByte(CP_ACP, 0, strSrc, -1, szRes, i, NULL, NULL);

     result = szRes;
     delete []strSrc;
     delete []szRes;

     return result;
}


int _tmain(int argc, _TCHAR* argv[])
{
	std::vector<const std::string> filelist;   //定义一个存放结果文件名称的链表

	//遍历一次结果的所有文件，获取文件名列表
	find("D:\\website\\doc", filelist);

	std::string lujing = "econpdf.txt";
	std::ofstream outfile;
	outfile.open(lujing, std::ios::out | std::ios::app);
	char buffer[512000];
	int xuhao = 0;

	/**MYSQL* mydbdata = mysql_init((MYSQL*)0);
	bool lianjiemysql = false;
	//连接数据库
	if(mysql_real_connect(mydbdata, "localhost", "root", "**", "sufe", 3306, "", 0))
	{
		lianjiemysql = true;
		char sql[256] = {0};
		strcpy_s(sql, "set names gb2312");
		mysql_query(mydbdata, sql);
	}
	*/
	std::vector<string> zifuchuan;
	std::vector<int> fanhuizhi;
	zifuchuan.resize(40);
	fanhuizhi.resize(40);
	zifuchuan[0] = "***";
	zifuchuan[1] = "***";
	zifuchuan[2] = "***";
	zifuchuan[3] = "***";
	

	std::vector<string> mingancis(4);
	mingancis[0] = "**";
	mingancis[1] = "***";
	mingancis[2] = "****";
	mingancis[3] = "*****";
	int i;
	for(i = 0; i < filelist.size(); i++)
	{
		//读取txt文件
		std::fstream infile;
		infile.open(filelist[i], std::ios::in);
		char s[12];             //设定12位对于存储32位int值足够  
		itoa(i,s,10);            //itoa函数亦可以实现，但是属于C中函数，在C++中推荐用流的方法  
		std::string xx = s;  
		int geshu = 0;
		while(!infile.eof())
		{
			infile.getline(buffer, 512000, '\n');
			if(strcmp(buffer, "") == 0)
			{
				geshu += 1;
			}
			if(geshu > 100)
			{
				break;
			}
			std::string ss;
			ss = buffer;
			//ss = UTF8ToGB(buffer);
			

			for(int j = 0; j < zifuchuan.size(); j++)
			{
				fanhuizhi[j] = ss.find(zifuchuan[j]);
			}
			int geshu = 0;
			std::string pipeiguanjianci;
			for(int j = 0; j < 20; j++)
			{
				int j2, j3;
				j2 = j * 2;
				j3 = j * 2 + 1;
				if(fanhuizhi[j2] > 0 || fanhuizhi[j3] > 0)
				{
					geshu += 1;
					if(fanhuizhi[j2] > 0)
					{
						pipeiguanjianci.append(zifuchuan[j2]);
					}
					if(fanhuizhi[j3] > 0)
					{
						pipeiguanjianci.append(zifuchuan[j3]);
					}
				}
			}
			bool minganci = false;
			for(int j = 0; j < mingancis.size(); j++)
			{
				fanhuizhi[j] = ss.find(mingancis[j]);
				if(fanhuizhi[j] > 0)
				{
					pipeiguanjianci.append(mingancis[j]);
					minganci = true;
				}
			}
			if(geshu > 1 || minganci == true/** || ssint2 > 0 || ssint3 > 0 || ssint4 > 0 || ssint5 > 0 || ssint6 > 0*/)
			{
				/**char querystr[1000] = "select title, id from paper where pcontent like '%";
				string ss7 = filelist[i];
				int dataint;
				dataint = filelist[i].find("data");
				string subfilestr = filelist[i].substr(dataint, filelist[i].length() - dataint - 4);
				strcat(querystr, subfilestr.c_str());
				strcat(querystr, "%'");
				int ret = mysql_query(mydbdata, querystr);
				if(!ret)
				{
					MYSQL_RES *res = mysql_store_result(mydbdata);
					if(res->row_count != 0)
					{
						MYSQL_ROW row;
						//取出结果集
						row = mysql_fetch_row(res);
						if(row)
						{
							outfile << xuhao << ":" << filelist[i] << std::endl;
							outfile << xuhao << ", " << row[0] << std::endl;
							outfile << xuhao << ":<a href='http://econ.shufe.edu.cn/se/content/" << row[1] << ">http://econ.shufe.edu.cn/se/content/" << row[1] << "</a>" << std::endl;
							outfile << xuhao << pipeiguanjianci.c_str() << std::endl << std::endl;
						}
					}
				}*/
				outfile << xuhao << ": " << filelist[i] << std::endl;
				outfile << xuhao << pipeiguanjianci.c_str() << std::endl << std::endl;
				//找到数据库中对应的标题
				//获得文章的id
				xuhao = xuhao + 1;
				break;
			}
		}
		infile.close();
	}
	//关闭数据库
	//mysql_close(mydbdata);

	outfile.close();

	return 0;
}

