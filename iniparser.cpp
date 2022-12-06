#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cctype>

#include <algorithm>
#include <iostream>

#include "iniparser.h"

#if defined(_MSC_VER) && defined(_WIN32)
#pragma warning(disable: 4800)
#define strncasecmp _strnicmp
#define strcasecmp _stricmp
#endif

//
// MAKEDWORD_
//
static constexpr std::uint32_t MAKEDWORD_(std::uint16_t low, std::uint16_t high)
{
	return (static_cast<std::uint32_t>(low) | static_cast<std::uint32_t>(high) << 16);
}

//
// MAKEWORD_
//
static constexpr std::uint16_t MAKEWORD_(std::uint8_t low, std::uint8_t high)
{
	return (static_cast<std::uint16_t>(low) | static_cast<std::uint16_t>(high) << 8);
}

//
// StrCmpI_
//
static int StrCmpI_(const std::string &lhs, const std::string &rhs)
{
	std::size_t cch = std::max(lhs.size(), rhs.size());
	return strncasecmp(lhs.data(), rhs.data(), cch);
}

//
// StrNeqI
//
bool StrNeqI(const std::string &lhs, const std::string &rhs)
{
	return (StrCmpI_(lhs, rhs) != 0);
}

//
// StrEquI
//
bool StrEquI(const std::string &lhs, const std::string &rhs)
{
	return (StrCmpI_(lhs, rhs) == 0);
}

//
// StrLeqI
//
bool StrLeqI(const std::string &lhs, const std::string &rhs)
{
	return (StrCmpI_(lhs, rhs) <= 0);
}

//
// StrLssI
//
bool StrLssI(const std::string &lhs, const std::string &rhs)
{
	return (StrCmpI_(lhs, rhs) < 0);
}

//
// StrGeqI
//
bool StrGeqI(const std::string &lhs, const std::string &rhs)
{
	return (StrCmpI_(lhs, rhs) >= 0);
}

//
// StrGtrI
//
bool StrGtrI(const std::string &lhs, const std::string &rhs)
{
	return (StrCmpI_(lhs, rhs) > 0);
}

//
// stob_result_t
//
typedef enum {
	stob_result_none  = -1,
	stob_result_false =  0,
	stob_result_true  =  1,
	stob_result_max
} stob_result_t;

//
// __is_subseq_upper_false
//
inline static bool __is_subseq_upper_false(const char *psz)
{
	if (*psz++ == 'A')
		if (*psz++ == 'L')
			if (*psz++ == 'S')
				return (*psz == 'E');
	return false;
}

//
// __is_subseq_lower_false
//
inline static bool __is_subseq_lower_false(const char *psz)
{
	if (*psz++ == 'a')
		if (*psz++ == 'l')
			if (*psz++ == 's')
				return (*psz == 'e');
	return false;
}

//
// __is_subseq_upper_true
//
inline static bool __is_subseq_upper_true(const char *psz)
{
	if (*psz++ == 'R')
		if (*psz++ == 'U')
			return (*psz == 'E');
	return false;
}

//
// __is_subseq_lower_true
//
inline static bool __is_subseq_lower_true(const char *psz)
{
	if (*psz++ == 'r')
		if (*psz++ == 'u')
			return (*psz == 'e');
	return false;
}

//
// __is_subseq_upper_yes
//
inline static bool __is_subseq_upper_yes(const char *psz)
{
	if (*psz++ == 'E')
		return (*psz == 'S');
	return false;
}

//
// __is_subseq_lower_yes
//
inline static bool __is_subseq_lower_yes(const char *psz)
{
	if (*psz++ == 'e')
		return (*psz == 's');
	return false;
}

//
// __is_subseq_upper_off
//
inline static bool __is_subseq_upper_off(const char *psz)
{
	if (*psz++ == 'F')
		return (*psz == 'F');
	return false;
}

//
// __is_subseq_lower_off
//
inline static bool __is_subseq_lower_off(const char *psz)
{
	if (*psz++ == 'f')
		return (*psz == 'f');
	return false;
}

//
// stob_ - convert string to bool
//
// case FALSE/False/false/OFF/Off/off/NO/No/no/0 => stob_result_false (succeeded, represent for false)
// case TRUE/True/true/YES/Yes/yes/ON/On/on/1 => stob_result_true (succeeded, represent for true)
// case else => stob_result_none (failed)
//
inline static stob_result_t stob_(const char *psz, std::size_t cch)
{
	switch (cch) {
	case 5:
		switch (*psz++) {
		case 'F':
			// checking for "FALSE" (upper case)
			if (__is_subseq_upper_false(psz))
				return stob_result_false;
			// checking for "False" (title case)
		case 'f':
			// checking for "false" (lower case)
			if (__is_subseq_lower_false(psz))
				return stob_result_false;
			break;
		default:
			break;
		}
		break;

	case 4:
		switch (*psz++) {
		case 'T':
			// checking for "TRUE" (upper case)
			if (__is_subseq_upper_true(psz))
				return stob_result_true;
			// checking for "True" (title case)
		case 't':
			// checking for "true" (lower case)
			if (__is_subseq_lower_true(psz))
				return stob_result_true;
			break;
		default:
			break;
		}
		break;

	case 3:
		switch (*psz++) {
		case 'Y':
			// checking for "YES" (upper case)
			if (__is_subseq_upper_yes(psz))
				return stob_result_true;
			// checking for "Yes" (title case)
		case 'y':
			// checking for "yes" (lower case)
			if (__is_subseq_lower_yes(psz))
				return stob_result_true;
			break;
		case 'O':
			// checking for "OFF" (upper case)
			if (__is_subseq_upper_off(psz))
				return stob_result_false;
			// checking for "Off" (title case)
		case 'o':
			// checking for "off" (lower case)
			if (__is_subseq_lower_off(psz))
				return stob_result_false;
			break;
		default:
			break;
		}
		break;

	case 2:
		switch (*psz++) {
		case 'O':
			// checking for "ON" (upper case)
			if (*psz == 'N')
				return stob_result_true;
			// checking for "On" (title case)
		case 'o':
			// checking for "on" (lower case)
			if (*psz == 'n')
				return stob_result_true;
			break;
		case 'N':
			// checking for "NO" (upper case)
			if (*psz == 'O')
				return stob_result_false;
			// checking for "No" (title case)
		case 'n':
			// checking for "no" (lower case)
			if (*psz == 'o')
				return stob_result_false;
			break;
		default:
			break;
		}
		break;

	case 1:
		switch (*psz) {
			// checking for "1"
		case '1':
			return stob_result_true;
			// checking for "0"
		case '0':
			return stob_result_false;

		default:
			break;
		}

	default:
		break;
	}

	return stob_result_none;
}

//
// ProfileParser
//
auto ProfileParser::prev_(const std::string &line, std::size_t pos, std::size_t min) -> std::size_t
{
	std::size_t res;

	while ([&](std::size_t min) {
		auto ok = (pos > min);
		res = pos--;
		if (ok)
			ok = std::isspace(line.at(pos));
		return ok;
	}(min));

	return res;
}

auto ProfileParser::next_(const std::string &line, std::size_t pos, std::size_t max) -> std::size_t
{
	std::size_t res;

	while ([&](std::size_t max) {
		auto ok = (pos < max);
		res = pos++;
		if (ok)
			ok = std::isspace(line.at(res));
		return ok;
	}(max));

	return res;
}

bool ProfileParser::ProfileSection::readHelper_(const std::string &key, const std::function<KeyValueHandler> &handler) const
{
	bool res = false;
	const auto iter = map_.find(key);
	if (iter != map_.end())
	{
		std::string value(iter->second);
		if (!value.empty())
		{
			auto &front = value.front();
			auto &back = value.back();
			switch (MAKEDWORD_(front, back)) {
			case MAKEDWORD_(kDQ, kDQ):
				if (std::addressof(back) > std::addressof(front))
				{
					value.erase(value.begin());
					value.pop_back();
				}
			default:
				res = handler(value);
				break;
			}
		}
	}
	return res;
}

bool ProfileParser::ProfileSection::readString_(const std::string &key, std::string &result) const
{
	return readHelper_(key, [&result](const mapped_type &value) {
		result.assign(value);
		return !result.empty();
	});
}

bool ProfileParser::ProfileSection::readUInt64_(const std::string &key, std::uint64_t &result) const
{
	return readHelper_(key, [&result](const mapped_type &value) {
		std::size_t pos = 0;
		result = std::stoull(value, &pos);
		return (pos > 0);
	});
}

bool ProfileParser::ProfileSection::readUInt32_(const std::string &key, std::uint32_t &result) const
{
	return readHelper_(key, [&result](const mapped_type &value) {
		std::size_t pos = 0;
		result = std::stoul(value, &pos);
		return (pos > 0);
	});
}

bool ProfileParser::ProfileSection::readInt64_(const std::string &key, std::int64_t &result) const
{
	return readHelper_(key, [&result](const mapped_type &value) {
		std::size_t pos = 0;
		result = std::stoll(value, &pos);
		return (pos > 0);
	});
}

bool ProfileParser::ProfileSection::readInt32_(const std::string &key, std::int32_t &result) const
{
	return readHelper_(key, [&result](const mapped_type &value) {
		std::size_t pos = 0;
		result = std::stol(value, &pos);
		return (pos > 0);
	});
}

bool ProfileParser::ProfileSection::readDouble_(const std::string &key, double &result) const
{
	return readHelper_(key, [&result](const mapped_type &value) {
		std::size_t pos = 0;
		result = std::stod(value, &pos);
		return (pos > 0);
	});
}

bool ProfileParser::ProfileSection::readFloat_(const std::string &key, float &result) const
{
	return readHelper_(key, [&result](const mapped_type &value) {
		std::size_t pos = 0;
		result = std::stof(value, &pos);
		return (pos > 0);
	});
}

bool ProfileParser::ProfileSection::readBool_(const std::string &key, bool &result) const
{
	return readHelper_(key, [&result](const mapped_type &value) {
		switch (stob_(value.data(), value.size())) {
		case stob_result_true:
			result = true;
			return true;
		case stob_result_false:
			result = false;
			return true;
		default:
			return false;
		}
	});
}

auto ProfileParser::ProfileSection::readString(const std::string &key, const std::string &def) const -> std::string
{
	std::string result;
	if (!readString_(key, result) && !def.empty())
	{
		result.assign(def);
	}
	return result;
}

auto ProfileParser::ProfileSection::readString(const std::string &key, const char *def) const -> std::string
{
	std::string result;
	if (!readString_(key, result) && (def != nullptr))
	{
		result.assign(def);
	}
	return result;
}

auto ProfileParser::ProfileSection::readUInt64(const std::string &key, std::uint64_t def) const -> std::uint64_t
{
	std::uint64_t result;
	if (!readUInt64_(key, result))
	{
		result = def;
	}
	return result;
}

auto ProfileParser::ProfileSection::readUInt32(const std::string &key, std::uint32_t def) const -> std::uint32_t
{
	std::uint32_t result;
	if (!readUInt32_(key, result))
	{
		result = def;
	}
	return result;
}

auto ProfileParser::ProfileSection::readInt64(const std::string &key, std::int64_t def) const -> std::int64_t
{
	std::int64_t result;
	if (!readInt64_(key, result))
	{
		result = def;
	}
	return result;
}

auto ProfileParser::ProfileSection::readInt32(const std::string &key, std::int32_t def) const -> std::int32_t
{
	std::int32_t result;
	if (!readInt32_(key, result))
	{
		result = def;
	}
	return result;
}

auto ProfileParser::ProfileSection::readDouble(const std::string &key, double def) const -> double
{
	double result;
	if (!readDouble_(key, result))
	{
		result = def;
	}
	return result;
}

auto ProfileParser::ProfileSection::readFloat(const std::string &key, float def) const -> float
{
	float result;
	if (!readFloat_(key, result))
	{
		result = def;
	}
	return result;
}

auto ProfileParser::ProfileSection::readBool(const std::string &key, bool def) const -> bool
{
	bool result;
	if (!readBool_(key, result))
	{
		result = def;
	}
	return result;
}

auto ProfileParser::ProfileSection::writeString(const std::string &key, const std::string &value) -> bool
{
	auto pair = map_.emplace(key, value);
	return pair.second;
}

auto ProfileParser::ProfileSection::writeString(const std::string &key, const char *value) -> bool
{
	std::string str;
	if (value != nullptr)
		str.assign(value);
	return writeString(
		key,
		str);
}

auto ProfileParser::ProfileSection::writeUInt64(const std::string &key, std::uint64_t value) -> bool
{
	auto &&str = std::to_string(value);
	return writeString(
		key,
		str);
}

auto ProfileParser::ProfileSection::writeUInt32(const std::string &key, std::uint32_t value) -> bool
{
	auto &&str = std::to_string(value);
	return writeString(
		key,
		str);
}

auto ProfileParser::ProfileSection::writeInt64(const std::string &key, std::int64_t value) -> bool
{
	auto &&str = std::to_string(value);
	return writeString(
		key,
		str);
}

auto ProfileParser::ProfileSection::writeInt32(const std::string &key, std::int32_t value) -> bool
{
	auto &&str = std::to_string(value);
	return writeString(
		key,
		str);
}

auto ProfileParser::ProfileSection::writeDouble(const std::string &key, double value) -> bool
{
	auto &&str = std::to_string(value);
	return writeString(
		key,
		str);
}

auto ProfileParser::ProfileSection::writeFloat(const std::string &key, float value) -> bool
{
	auto &&str = std::to_string(value);
	return writeString(
		key,
		str);
}

auto ProfileParser::ProfileSection::writeBool(const std::string &key, bool value) -> bool
{
	std::stringstream ss;
	ss >> std::boolalpha >> value;
	auto &&str = ss.str();
	return writeString(
		key,
		str);
}

ProfileParser::ProfileSection::ProfileSection()
	: map_(StrLssI)
{
}

bool ProfileParser::setAliasApp(const std::string &name, const std::string &alias)
{
	bool res = false;
	auto iter = data_.find(name);
	if (iter != data_.end())
	{
		auto pair = data_.emplace(alias, iter->second);
		alias_.insert(alias);
		res = pair.second;
	}
	return res;
}

bool ProfileParser::isAliasApp(const std::string &name) const
{
	auto iter = alias_.find(name);
	return (iter != alias_.end());
}

bool ProfileParser::readHelper_(const std::string &name, const std::string &key, const std::function<SectionHandler> &handler) const
{
	bool res = false;

	const auto iter = data_.find(name);
	if (iter != data_.end())
	{
		const auto section = iter->second;
		if (!section->empty())
		{
			res = handler(section);
		}
	}

	return res;
}

auto ProfileParser::readString(const std::string &name, const std::string &key, const std::string &def) const -> std::string
{
	std::string result;
	if (!readHelper_(name, key, [&](const section_type &section) {
		return section->readString_(key, result);
	}) && !def.empty())
	{
		result.assign(def);
	}
	return result;
}

auto ProfileParser::readString(const std::string &name, const std::string &key, const char *def) const -> std::string
{
	std::string result;
	if (!readHelper_(name, key, [&](const section_type &section) {
		return section->readString_(key, result);
	}) && (def != nullptr))
	{
		result.assign(def);
	}
	return result;
}

auto ProfileParser::readUInt64(const std::string &name, const std::string &key, std::uint64_t def) const -> std::uint64_t
{
	std::uint64_t result;
	if (!readHelper_(name, key, [&](const section_type &section) {
		return section->readUInt64_(key, result);
	}))
	{
		result = def;
	}
	return result;
}

auto ProfileParser::readUInt32(const std::string &name, const std::string &key, std::uint32_t def) const -> std::uint32_t
{
	std::uint32_t result;
	if (!readHelper_(name, key, [&](const section_type &section) {
		return section->readUInt32_(key, result);
	}))
	{
		result = def;
	}
	return result;
}

auto ProfileParser::readInt64(const std::string &name, const std::string &key, std::int64_t def) const -> std::int64_t
{
	std::int64_t result;
	if (!readHelper_(name, key, [&](const section_type &section) {
		return section->readInt64_(key, result);
	}))
	{
		result = def;
	}
	return result;
}

auto ProfileParser::readInt32(const std::string &name, const std::string &key, std::int32_t def) const -> std::int32_t
{
	std::int32_t result;
	if (!readHelper_(name, key, [&](const section_type &section) {
		return section->readInt32_(key, result);
	}))
	{
		result = def;
	}
	return result;
}

auto ProfileParser::readDouble(const std::string &name, const std::string &key, double def) const -> double
{
	double result;
	if (!readHelper_(name, key, [&](const section_type &section) {
		return section->readDouble_(key, result);
	}))
	{
		result = def;
	}
	return result;
}

auto ProfileParser::readFloat(const std::string &name, const std::string &key, float def) const -> float
{
	float result;
	if (!readHelper_(name, key, [&](const section_type &section) {
		return section->readFloat_(key, result);
	}))
	{
		result = def;
	}
	return result;
}

auto ProfileParser::readBool(const std::string &name, const std::string &key, bool def) const -> bool
{
	bool result;
	if (!readHelper_(name, key, [&](const section_type &section) {
		return section->readBool_(key, result);
	}))
	{
		result = def;
	}
	return result;
}

auto ProfileParser::writeString(const std::string &name, const std::string &key, const std::string &value) -> bool
{
	bool res = false;

	auto iter = data_.find(name);
	if (iter == data_.end())
	{
		auto section = std::make_shared<ProfileSection>();
		auto pair = data_.emplace(name, section);
		if (pair.second)
		{
			res = pair.first->second->writeString(key, value);
		}
	}
	else
	{
		res = iter->second->writeString(key, value);
	}

	return res;
}

auto ProfileParser::writeString(const std::string &name, const std::string &key, const char *value) -> bool
{
	std::string str;
	if (value != nullptr)
		str.assign(value);
	return writeString(
		name,
		key,
		str);
}

auto ProfileParser::writeUInt64(const std::string &name, const std::string &key, std::uint64_t value) -> bool
{
	auto &&str = std::to_string(value);
	return writeString(
		name,
		key,
		str);
}

auto ProfileParser::writeUInt32(const std::string &name, const std::string &key, std::uint32_t value) -> bool
{
	auto &&str = std::to_string(value);
	return writeString(
		name,
		key,
		str);
}

auto ProfileParser::writeInt64(const std::string &name, const std::string &key, std::int64_t value) -> bool
{
	auto &&str = std::to_string(value);
	return writeString(
		name,
		key,
		str);
}

auto ProfileParser::writeInt32(const std::string &name, const std::string &key, std::int32_t value) -> bool
{
	auto &&str = std::to_string(value);
	return writeString(
		name,
		key,
		str);
}

auto ProfileParser::writeDouble(const std::string &name, const std::string &key, double value) -> bool
{
	auto &&str = std::to_string(value);
	return writeString(
		name,
		key,
		str);
}

auto ProfileParser::writeFloat(const std::string &name, const std::string &key, float value) -> bool
{
	auto &&str = std::to_string(value);
	return writeString(
		name,
		key,
		str);
}

auto ProfileParser::writeBool(const std::string &name, const std::string &key, bool value) -> bool
{
	std::stringstream ss;
	ss >> std::boolalpha >> value;
	auto &&str = ss.str();
	return writeString(
		name,
		key,
		str);
}

void ProfileParser::parse_(std::string &name, const std::string &line)
{
	std::size_t p2 = 0;
	std::size_t p1 = 0;

	if (!line.empty() && (line.at(p1) != kNS) && (line.at(p1) != kSC))
	{
		p1 = next_(line, p1, line.size());

		if (line.at(p1) != kLB)
		{
			if (!name.empty() && ((p2 = line.find_first_of(kES, p1)) != std::string::npos))
			{
				auto iter = data_.find(name);
				if (iter != data_.end())
				{
					auto k2 = prev_(line, p2++, p1);
					auto k1 = next_(line, p1, k2);

					if (k2 > k1)
					{
						auto v2 = prev_(line, line.size(), p2);
						auto v1 = next_(line, p2, v2);

						auto ks = line.substr(k1, k2 - k1);
						auto vs = line.substr(v1, v2 - v1);

						iter->second->emplace(ks, vs);
					}
				}
			}
		}
		else
		{
			if ((p2 = line.find(kRB, ++p1)) != std::string::npos)
			{
				auto section = std::make_shared<ProfileSection>();
				name = line.substr(p1, p2 - p1);
				data_.emplace(name, section);
			}
		}
	}
}

void ProfileParser::save(const std::string &file) const
{
	std::ofstream ofs_(file);
	if (ofs_.is_open())
	{
		save(ofs_);
		ofs_.close();
	}
}

void ProfileParser::open(const std::string &file)
{
	std::ifstream ifs_(file);
	if (ifs_.is_open())
	{
		data_.clear();
		open(ifs_);
		ifs_.close();
	}
}

void ProfileParser::save(std::ostream &os) const
{
	for (const auto item : data_)
	{
		auto pos = item.second->cbegin();
		auto end = item.second->cend();

		if (isAliasApp(item.first))
		{
			continue;
		}

		os << '[';
		os << item.first;
		os << ']';
		os << std::endl;

		while (pos != end)
		{
			os << pos->first;
			os << ' ';
			os << '=';
			os << ' ';
			os << pos->second;
			os << std::endl;
			pos++;
		}
	}
}

void ProfileParser::open(std::istream &is)
{
	std::string line;
	std::string name;

	while (is.good())
	{
		std::getline(is, line);
		parse_(name, line);
	}
}

ProfileParser::ProfileParser(const std::string &file)
	: ProfileParser()
{
	std::ifstream ifs_(file);
	if (ifs_.is_open())
	{
		open(ifs_);
		ifs_.close();
	}
}

ProfileParser::ProfileParser(std::istream &is)
	: ProfileParser()
{
	open(is);
}

ProfileParser::ProfileParser()
	: data_(StrLssI)
{
}