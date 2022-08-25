#include <stdio.h>
#include <tchar.h>

#include <iostream>
#include <string>

#include <sdkddkver.h>
#include <ntstatus.h>
#define WIN32_LEAN_AND_MEAN
#define WIN32_NO_STATUS
#define NOMINMAX
#include <windows.h>

#include "iniparser.h"

int _tmain(int argc, const _TCHAR *argv[])
{
	const std::string sec1("default");
	const std::string sec2("config");

	ProfileParser parser("config.ini");

	//
	// 设置 default 的别名为 config，读取 config 等于读取 default。
	//
	parser.setAliasApp(sec1, sec2);

	auto s1 = parser.readString(sec2, "string1");
	std::cout << "[" << s1 << "]" << std::endl;
	auto s2 = parser.readString(sec2, "string2");
	std::cout << "[" << s2 << "]" << std::endl;

	//
	// FALSE/False/false/OFF/Off/off/NO/No/no/0 => false
	// TRUE/True/true/YES/Yes/yes/ON/On/on/1 => true
	//
	auto b1 = parser.readBool(sec2, "bool1");
	std::cout << "[" << std::boolalpha << b1 << "]" << std::endl;
	auto b2 = parser.readBool(sec2, "bool2");
	std::cout << "[" << std::boolalpha << b2 << "]" << std::endl;
	auto b3 = parser.readBool(sec2, "bool3");
	std::cout << "[" << std::boolalpha << b3 << "]" << std::endl;
	auto b4 = parser.readBool(sec2, "bool4");
	std::cout << "[" << std::boolalpha << b4 << "]" << std::endl;
	auto b5 = parser.readBool(sec2, "bool5");
	std::cout << "[" << std::boolalpha << b5 << "]" << std::endl;
	auto b6 = parser.readBool(sec2, "bool6");
	std::cout << "[" << std::boolalpha << b6 << "]" << std::endl;

	auto u1 = parser.readUInt64(sec2, "uint64");
	std::cout << "[" << u1 << "]" << std::endl;
	auto i1 = parser.readInt64(sec2, "int64");
	std::cout << "[" << i1 << "]" << std::endl;

	auto u2 = parser.readUInt32(sec2, "uint32");
	std::cout << "[" << u2 << "]" << std::endl;
	auto i2 = parser.readInt32(sec2, "int32");
	std::cout << "[" << i2 << "]" << std::endl;

	auto d1 = parser.readDouble(sec2, "double");
	std::cout << "[" << d1 << "]" << std::endl;
	auto f1 = parser.readFloat(sec2, "float");
	std::cout << "[" << f1 << "]" << std::endl;

	return 0;
}