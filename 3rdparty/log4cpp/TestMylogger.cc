///
/// @file    TestMylogger.cc
/// @author  lemon(haohb13@gmail.com)
/// @date    2022-01-10 11:20:02
///

#include "Mylogger.hpp"
#include <iostream>
#include <string>
using std::cout;
using std::endl;
using std::string;


void test0() 
{
	Mylogger::getInstance()->debug("this is a debug msg");
	Mylogger::getInstance()->info("this is a info msg");
	Mylogger::getInstance()->warn("this is a warn msg");
	Mylogger::getInstance()->error("this is a error msg");
} 

void test1()
{
	//希望日志记录中有文件的名字, 函数的名字，行号
	cout << __LINE__ << endl;
	cout << __FILE__ << endl;
	cout << __func__ << endl;
	//接下来做字符串的拼接操作即可
	//std::string s1,s2; 
	//s1 + s2;
	//s1.append(s2)
}

void test2() 
{
	//定位信息 =》 TestMylogger.cc/test2/47
	Mylogger::getInstance()->debug(addprefix("this is a debug msg"));
	Mylogger::getInstance()->info(addprefix("this is a info msg"));
	Mylogger::getInstance()->warn(addprefix("this is a warn msg"));
	Mylogger::getInstance()->error(addprefix("this is a error msg"));

	int number = 1;
	const char * pstr = "wangdao";
	Mylogger::getInstance()->debug(addprefix("this is a debug msg, %d, %s"), number, pstr);
} 

void test3()
{
	int number = 1;
	logInfo("The log is %d info message", number);	//或者   
	logError("The log is error message");
	logWarn("The log is warn message");
	logDebug("The log is debug message");
}


int main(void)
{
	/* test0(); */
	/* test1(); */
	/* test2(); */
	test3();
	return 0;
}
