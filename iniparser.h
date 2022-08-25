#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <functional>
#include <memory>
#include <map>

//
// string comparison
//
bool StrNeqI(const std::string &lhs, const std::string &rhs);
bool StrEquI(const std::string &lhs, const std::string &rhs);
bool StrLeqI(const std::string &lhs, const std::string &rhs);
bool StrLssI(const std::string &lhs, const std::string &rhs);
bool StrGeqI(const std::string &lhs, const std::string &rhs);
bool StrGtrI(const std::string &lhs, const std::string &rhs);

//
// ProfileParser
//
class ProfileParser
{
	typedef std::add_lvalue_reference<decltype(StrLssI)>::type RefStrLssI;
	typedef std::map<std::string, std::string, RefStrLssI> KeyValueMap;
	typedef std::shared_ptr<KeyValueMap> KeyValueMapPtr;

	typedef bool FieldReaderCallback(const std::string &);

private:
	static std::size_t prev_(const std::string &line, std::size_t pos);
	static std::size_t next_(const std::string &line, std::size_t pos);

private:
	std::map<std::string, KeyValueMapPtr, RefStrLssI> data_;

private:
	void parseLine_(std::string &app, const std::string &line);

private:
	void parse_(std::istringstream &iss);
	void parse_(std::ifstream &ifs);

private:
	bool readHelper_(const std::string &app, const std::string &key, const std::function<FieldReaderCallback> &fn) const;

public:
	auto readString (const std::string &app, const std::string &key, const std::string &def) const-> std::string;
	auto readString (const std::string &app, const std::string &key, const char *def = nullptr) const -> std::string;
	auto readUInt64 (const std::string &app, const std::string &key, std::uint64_t def = 0) const -> std::uint64_t;
	auto readUInt32 (const std::string &app, const std::string &key, std::uint32_t def = 0) const -> std::uint32_t;
	auto readInt64  (const std::string &app, const std::string &key, std::int64_t def = 0) const -> std::int64_t;
	auto readInt32  (const std::string &app, const std::string &key, std::int32_t def = 0) const -> std::int32_t;
	auto readDouble (const std::string &app, const std::string &key, double def = 0.0) const -> double;
	auto readFloat  (const std::string &app, const std::string &key, float def = 0.0f) const -> float;
	auto readBool   (const std::string &app, const std::string &key, bool def = false) const -> bool;

public:
	bool setAliasApp(const std::string &app, const std::string &old);

public:
	ProfileParser(const std::string &file);
	ProfileParser();
};