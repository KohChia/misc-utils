#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <functional>
#include <memory>
#include <map>
#include <set>

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

	typedef bool FieldDataHandler(const std::string &);

private:
	static std::size_t prev_(const std::string &line, std::size_t pos, std::size_t min);
	static std::size_t next_(const std::string &line, std::size_t pos, std::size_t max);

private:
	static const char kSC = ';'; // semi-colon
	static const char kNS = '#'; // number sign
	static const char kLB = '['; // left square brackets
	static const char kRB = ']'; // right square brackets
	static const char kES = '='; // equal sign
	static const char kDQ = '"'; // double quotes

private:
	std::map<std::string, KeyValueMapPtr, RefStrLssI> data_;
	std::set<std::string> alias_;

private:
	void parse_(std::string &sec, const std::string &line);

private:
	bool readHelper_(const std::string &sec, const std::string &key, const std::function<FieldDataHandler> &handler) const;

public:
	bool setAliasApp(const std::string &sec, const std::string &alias);

public:
	auto readString (const std::string &sec, const std::string &key, const std::string &def) const -> std::string;
	auto readString (const std::string &sec, const std::string &key, const char *def = nullptr) const -> std::string;
	auto readUInt64 (const std::string &sec, const std::string &key, std::uint64_t def = 0) const -> std::uint64_t;
	auto readUInt32 (const std::string &sec, const std::string &key, std::uint32_t def = 0) const -> std::uint32_t;
	auto readInt64  (const std::string &sec, const std::string &key, std::int64_t def = 0) const -> std::int64_t;
	auto readInt32  (const std::string &sec, const std::string &key, std::int32_t def = 0) const -> std::int32_t;
	auto readDouble (const std::string &sec, const std::string &key, double def = 0.0) const -> double;
	auto readFloat  (const std::string &sec, const std::string &key, float def = 0.0f) const -> float;
	auto readBool   (const std::string &sec, const std::string &key, bool def = false) const -> bool;

public:
	auto writeString(const std::string &sec, const std::string &key, const std::string &value) -> bool;
	auto writeString(const std::string &sec, const std::string &key, const char *value) -> bool;
	auto writeUInt64(const std::string &sec, const std::string &key, std::uint64_t value) -> bool;
	auto writeUInt32(const std::string &sec, const std::string &key, std::uint32_t value) -> bool;
	auto writeInt64 (const std::string &sec, const std::string &key, std::int64_t value) -> bool;
	auto writeInt32 (const std::string &sec, const std::string &key, std::int32_t value) -> bool;
	auto writeDouble(const std::string &sec, const std::string &key, double value) -> bool;
	auto writeFloat (const std::string &sec, const std::string &key, float value) -> bool;
	auto writeBool  (const std::string &sec, const std::string &key, bool value) -> bool;

public:
	void save(const std::string &file) const;
	void open(const std::string &file);
	void save(std::ostream &os) const;
	void open(std::istream &is);

public:
	ProfileParser(const std::string &file);
	ProfileParser(std::istream &is);
	ProfileParser();
};