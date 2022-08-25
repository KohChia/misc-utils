#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cctype>

#include <algorithm>

#include "iniparser.h"

#if defined(_MSC_VER) && defined(_WIN32)
#pragma warning(disable: 4800)
#define strncasecmp _strnicmp
#define strcasecmp _stricmp
#endif

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

//
// 循环：将索引移到上一字符，判断此索引字符是否空格。如果不是，则返回前一字符索引。
//
std::size_t ProfileParser::prev_(const std::string &line, std::size_t pos)
{
	std::size_t res;

	do {
		res = pos--;
	} while (std::isspace(line.at(pos)));

	return res;
}

//
// 循环：将索引移到下一字符，判断此索引字符是否空格。如果不是，则返回当前字符索引。
//
std::size_t ProfileParser::next_(const std::string &line, std::size_t pos)
{
	do {
		pos++;
	} while (std::isspace(line.at(pos)));

	return pos;
}

void ProfileParser::parseLine_(std::string &app, const std::string &line)
{
	static const char kLB_ = '['; // left square brackets
	static const char kRB_ = ']'; // right square brackets
	static const char kNS_ = '#'; // number sign
	static const char kSC_ = ';'; // semi-colon
	static const char kES_ = '='; // equal sign
	static const char kQM_ = '"'; // quotation mark

	std::size_t k2_ = 0;
	std::size_t k1_ = 0;

	std::size_t v2_;
	std::size_t v1_;

	if (!line.empty() && (line.at(k1_) != kNS_) && (line.at(k1_) != kSC_))
	{
		if (line.at(k1_) != kLB_)
		{
			if (!app.empty() && (k2_ = line.find_first_of(kES_, k1_)) != std::string::npos)
			{
				v2_ = prev_(line, line.size());
				v1_ = next_(line, k2_);
				k2_ = prev_(line, k2_);

				if ((line.at(--v2_) == kQM_) && (line.at(v1_) == kQM_))
				{
					++v1_;
				}
				else
				{
					++v2_;
				}

				const auto &it_ = data_.find(app);
				if (it_ != data_.end())
				{
					std::string key_(line, k1_, k2_ - k1_);
					std::string val_(line, v1_, v2_ - v1_);
					it_->second->emplace(key_, val_);
				}
			}
		}
		else
		{
			if ((k2_ = line.find(kRB_, ++k1_)) != std::string::npos)
			{
				auto map_(std::make_shared<KeyValueMap>(StrLssI));
				app.assign(line, k1_, k2_ - k1_);
				data_.emplace(app, std::move(map_));
			}
		}
	}
}

void ProfileParser::parse_(std::istringstream &iss)
{
	std::string line;
	std::string app;

	while (iss.good())
	{
		std::getline(iss, line);
		parseLine_(app, line);
	}
}

void ProfileParser::parse_(std::ifstream &ifs)
{
	std::string line;
	std::string app;

	while (ifs.good())
	{
		std::getline(ifs, line);
		parseLine_(app, line);
	}
}

bool ProfileParser::readHelper_(const std::string &app, const std::string &key, const std::function<FieldReaderCallback> &fn) const
{
	bool res = false;

	const auto &itr1 = data_.find(app);
	if (itr1 != data_.end())
	{
		const auto &map = itr1->second;
		if (!map->empty())
		{
			const auto &itr2 = map->find(key);
			if (itr2 != map->end())
			{
				const auto &str = itr2->second;
				if (!str.empty())
				{
					res = fn(str);
				}
			}
		}
	}

	return res;
}

auto ProfileParser::readString(const std::string &app, const std::string &key, const std::string &def) const -> std::string
{
	std::string res;

	if (!readHelper_(app, key, [&res](const std::string &str) {
		res.assign(str);
		return !res.empty();
	}) && !def.empty())
	{
		res.assign(def);
	}

	return res;
}

auto ProfileParser::readString(const std::string &app, const std::string &key, const char *def) const -> std::string
{
	std::string res;

	if (!readHelper_(app, key, [&res](const std::string &str) {
		res.assign(str);
		return !res.empty();
	}) && (def != nullptr))
	{
		res.assign(def);
	}

	return res;
}

auto ProfileParser::readUInt64(const std::string &app, const std::string &key, std::uint64_t def) const -> std::uint64_t
{
	std::uint64_t res;

	if (!readHelper_(app, key, [&res](const std::string &str) {
		std::size_t pos = 0;
		res = std::stoull(str, &pos);
		return (pos > 0);
	}))
	{
		res = def;
	}

	return res;
}

auto ProfileParser::readUInt32(const std::string &app, const std::string &key, std::uint32_t def) const -> std::uint32_t
{
	std::uint32_t res;

	if (!readHelper_(app, key, [&res](const std::string &str) {
		std::size_t pos = 0;
		res = std::stoul(str, &pos);
		return (pos > 0);
	}))
	{
		res = def;
	}

	return res;
}

auto ProfileParser::readInt64(const std::string &app, const std::string &key, std::int64_t def) const -> std::int64_t
{
	std::int64_t res;

	if (!readHelper_(app, key, [&res](const std::string &str) {
		std::size_t pos = 0;
		res = std::stoll(str, &pos);
		return (pos > 0);
	}))
	{
		res = def;
	}

	return res;
}

auto ProfileParser::readInt32(const std::string &app, const std::string &key, std::int32_t def) const -> std::int32_t
{
	std::int32_t res;

	if (!readHelper_(app, key, [&res](const std::string &str) {
		std::size_t pos = 0;
		res = std::stol(str, &pos);
		return (pos > 0);
	}))
	{
		res = def;
	}

	return res;
}

auto ProfileParser::readDouble(const std::string &app, const std::string &key, double def) const -> double
{
	double res;

	if (!readHelper_(app, key, [&res](const std::string &str) {
		std::size_t pos = 0;
		res = std::stod(str, &pos);
		return (pos > 0);
	}))
	{
		res = def;
	}

	return res;
}

auto ProfileParser::readFloat(const std::string &app, const std::string &key, float def) const -> float
{
	float res;

	if (!readHelper_(app, key, [&res](const std::string &str) {
		std::size_t pos = 0;
		res = std::stof(str, &pos);
		return (pos > 0);
	}))
	{
		res = def;
	}

	return res;
}

auto ProfileParser::readBool(const std::string &app, const std::string &key, bool def) const -> bool
{
	bool res;

	if (!readHelper_(app, key, [&res](const std::string &str) {
		stob_result_t val = stob_(str.data(), str.size());
		switch (val) {
		case stob_result_true:
		case stob_result_false:
			res = static_cast<bool>(val);
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

bool ProfileParser::setAliasApp(const std::string &app, const std::string &old)
{
	bool res = false;

	const auto &itr = data_.find(app);
	if (itr != data_.end())
	{
		const auto &pair = data_.emplace(old, itr->second);
		res = pair.second;
	}

	return res;
}

ProfileParser::ProfileParser(const std::string &file)
	: ProfileParser()
{
	std::ifstream ifs_(file);
	if (ifs_.is_open())
	{
		parse_(ifs_);
		ifs_.close();
	}
}

ProfileParser::ProfileParser()
	: data_(StrLssI)
{
}