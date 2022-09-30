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
std::size_t ProfileParser::prev_(const std::string &line, std::size_t pos, std::size_t min)
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

std::size_t ProfileParser::next_(const std::string &line, std::size_t pos, std::size_t max)
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

void ProfileParser::parse_(std::string &sec, const std::string &line)
{
	std::size_t p2 = 0;
	std::size_t p1 = 0;

	if (!line.empty() && (line.at(p1) != kNS) && (line.at(p1) != kSC))
	{
		p1 = next_(line, p1, line.size());

		if (line.at(p1) != kLB)
		{
			if (!sec.empty() && ((p2 = line.find_first_of(kES, p1)) != std::string::npos))
			{
				auto iter = data_.find(sec);
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
				auto map(std::make_shared<KeyValueMap>(StrLssI));
				sec.assign(line, p1, p2 - p1);
				data_.emplace(sec, std::move(map));
			}
		}
	}
}

bool ProfileParser::setAliasApp(const std::string &sec, const std::string &alias)
{
	bool res = false;

	auto iter = data_.find(sec);
	if (iter != data_.end())
	{
		auto pair = data_.emplace(alias, iter->second);
		alias_.insert(alias);
		res = pair.second;
	}

	return res;
}

bool ProfileParser::readHelper_(const std::string &sec, const std::string &key, const std::function<FieldDataHandler> &handler) const
{
	bool res = false;

	const auto iter1 = data_.find(sec);
	if (iter1 != data_.end())
	{
		const auto pair = iter1->second;
		if (!pair->empty())
		{
			const auto iter2 = pair->find(key);
			if (iter2 != pair->end())
			{
				std::string value(iter2->second);
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
		}
	}

	return res;
}

auto ProfileParser::readString(const std::string &sec, const std::string &key, const std::string &def) const -> std::string
{
	std::string res;

	if (!readHelper_(sec, key, [&res](const std::string &value) {
		res.assign(value);
		return !res.empty();
	}) && !def.empty())
	{
		res.assign(def);
	}

	return res;
}

auto ProfileParser::readString(const std::string &sec, const std::string &key, const char *def) const -> std::string
{
	std::string res;

	if (!readHelper_(sec, key, [&res](const std::string &value) {
		res.assign(value);
		return !res.empty();
	}) && (def != nullptr))
	{
		res.assign(def);
	}

	return res;
}

auto ProfileParser::readUInt64(const std::string &sec, const std::string &key, std::uint64_t def) const -> std::uint64_t
{
	std::uint64_t res;

	if (!readHelper_(sec, key, [&res](const std::string &value) {
		std::size_t pos = 0;
		res = std::stoull(value, &pos);
		return (pos > 0);
	}))
	{
		res = def;
	}

	return res;
}

auto ProfileParser::readUInt32(const std::string &sec, const std::string &key, std::uint32_t def) const -> std::uint32_t
{
	std::uint32_t res;

	if (!readHelper_(sec, key, [&res](const std::string &value) {
		std::size_t pos = 0;
		res = std::stoul(value, &pos);
		return (pos > 0);
	}))
	{
		res = def;
	}

	return res;
}

auto ProfileParser::readInt64(const std::string &sec, const std::string &key, std::int64_t def) const -> std::int64_t
{
	std::int64_t res;

	if (!readHelper_(sec, key, [&res](const std::string &value) {
		std::size_t pos = 0;
		res = std::stoll(value, &pos);
		return (pos > 0);
	}))
	{
		res = def;
	}

	return res;
}

auto ProfileParser::readInt32(const std::string &sec, const std::string &key, std::int32_t def) const -> std::int32_t
{
	std::int32_t res;

	if (!readHelper_(sec, key, [&res](const std::string &value) {
		std::size_t pos = 0;
		res = std::stol(value, &pos);
		return (pos > 0);
	}))
	{
		res = def;
	}

	return res;
}

auto ProfileParser::readDouble(const std::string &sec, const std::string &key, double def) const -> double
{
	double res;

	if (!readHelper_(sec, key, [&res](const std::string &value) {
		std::size_t pos = 0;
		res = std::stod(value, &pos);
		return (pos > 0);
	}))
	{
		res = def;
	}

	return res;
}

auto ProfileParser::readFloat(const std::string &sec, const std::string &key, float def) const -> float
{
	float res;

	if (!readHelper_(sec, key, [&res](const std::string &value) {
		std::size_t pos = 0;
		res = std::stof(value, &pos);
		return (pos > 0);
	}))
	{
		res = def;
	}

	return res;
}

auto ProfileParser::readBool(const std::string &sec, const std::string &key, bool def) const -> bool
{
	bool res;

	if (!readHelper_(sec, key, [&res](const std::string &value) {
		stob_result_t result = stob_(value.data(), value.size());
		switch (result) {
		case stob_result_true:
		case stob_result_false:
			res = static_cast<bool>(result);
			return true;
		case stob_result_none:
			return false;
		default:
			return false;
		}
	}))
	{
		res = def;
	}

	return res;
}

auto ProfileParser::writeString(const std::string &sec, const std::string &key, const std::string &value) -> bool
{
	bool res = false;

	auto iter = data_.find(sec);
	if (iter == data_.end())
	{
		auto map(std::make_shared<KeyValueMap>(StrLssI));
		auto pair1 = data_.emplace(sec, std::move(map));
		if (pair1.second)
		{
			auto pair2 = pair1.first->second->emplace(key, value);
			res = pair2.second;
		}
	}
	else
	{
		auto pair2 = iter->second->emplace(key, value);
		res = pair2.second;
	}

	return res;
}

auto ProfileParser::writeString(const std::string &sec, const std::string &key, const char *value) -> bool
{
	std::string str;
	if (value != nullptr)
		str.assign(value);
	return writeString(
		sec,
		key,
		str);
}

auto ProfileParser::writeUInt64(const std::string &sec, const std::string &key, std::uint64_t value) -> bool
{
	auto &&str = std::to_string(value);
	return writeString(
		sec,
		key,
		str);
}

auto ProfileParser::writeUInt32(const std::string &sec, const std::string &key, std::uint32_t value) -> bool
{
	auto &&str = std::to_string(value);
	return writeString(
		sec,
		key,
		str);
}

auto ProfileParser::writeInt64(const std::string &sec, const std::string &key, std::int64_t value) -> bool
{
	auto &&str = std::to_string(value);
	return writeString(
		sec,
		key,
		str);
}

auto ProfileParser::writeInt32(const std::string &sec, const std::string &key, std::int32_t value) -> bool
{
	auto &&str = std::to_string(value);
	return writeString(
		sec,
		key,
		str);
}

auto ProfileParser::writeDouble(const std::string &sec, const std::string &key, double value) -> bool
{
	auto &&str = std::to_string(value);
	return writeString(
		sec,
		key,
		str);
}

auto ProfileParser::writeFloat(const std::string &sec, const std::string &key, float value) -> bool
{
	auto &&str = std::to_string(value);
	return writeString(
		sec,
		key,
		str);
}

auto ProfileParser::writeBool(const std::string &sec, const std::string &key, bool value) -> bool
{
	std::stringstream ss;
	ss >> std::boolalpha >> value;
	auto &&str = ss.str();
	return writeString(
		sec,
		key,
		str);
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
		if (alias_.find(item.first) != alias_.end())
		{
			continue;
		}

		auto iter1 = item.second->cbegin();
		auto iter2 = item.second->cend();

		os << '[';
		os << item.first;
		os << ']';
		os << std::endl;

		while (iter1 != iter2)
		{
			os << iter1->first;
			os << ' ';
			os << '=';
			os << ' ';
			os << iter1->second;
			os << std::endl;
			iter1++;
		}
	}
}

void ProfileParser::open(std::istream &is)
{
	std::string line;
	std::string sec;

	while (is.good())
	{
		std::getline(is, line);
		parse_(sec, line);
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