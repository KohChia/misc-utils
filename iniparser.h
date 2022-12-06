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
private:
	typedef typename std::add_lvalue_reference<decltype(StrLssI)>::type RefStrLssI;
	template<typename K, typename V> using ProfileBasicMap = std::map<K, V, RefStrLssI>;
private:
	static const char kSC = ';'; // semi-colon
	static const char kNS = '#'; // number sign
	static const char kLB = '['; // left square brackets
	static const char kRB = ']'; // right square brackets
	static const char kES = '='; // equal sign
	static const char kDQ = '"'; // double quotes
private:
	static auto prev_(const std::string &line, std::size_t pos, std::size_t min) -> std::size_t;
	static auto next_(const std::string &line, std::size_t pos, std::size_t max) -> std::size_t;
public:
	class ProfileSection
	{
		friend class ProfileParser;
	private:
		typedef ProfileBasicMap<std::string, std::string> KeyValueMap;
		typedef typename KeyValueMap::mapped_type mapped_type;
		typedef bool KeyValueHandler(const mapped_type &);
	private:
		bool readHelper_(const std::string &key, const std::function<KeyValueHandler> &handler) const;
		bool readString_(const std::string &key, std::string &result) const;
		bool readUInt64_(const std::string &key, std::uint64_t &result) const;
		bool readUInt32_(const std::string &key, std::uint32_t &result) const;
		bool readInt64_ (const std::string &key, std::int64_t &result) const;
		bool readInt32_ (const std::string &key, std::int32_t &result) const;
		bool readDouble_(const std::string &key, double &result) const;
		bool readFloat_ (const std::string &key, float &result) const;
		bool readBool_  (const std::string &key, bool &result) const;
	public:
		auto readString (const std::string &key, const std::string &def) const -> std::string;
		auto readString (const std::string &key, const char *def = nullptr) const -> std::string;
		auto readUInt64 (const std::string &key, std::uint64_t def = 0) const -> std::uint64_t;
		auto readUInt32 (const std::string &key, std::uint32_t def = 0) const -> std::uint32_t;
		auto readInt64  (const std::string &key, std::int64_t def = 0) const -> std::int64_t;
		auto readInt32  (const std::string &key, std::int32_t def = 0) const -> std::int32_t;
		auto readDouble (const std::string &key, double def = 0.0) const -> double;
		auto readFloat  (const std::string &key, float def = 0.0f) const -> float;
		auto readBool   (const std::string &key, bool def = false) const -> bool;
	public:
		auto writeString(const std::string &key, const std::string &value) -> bool;
		auto writeString(const std::string &key, const char *value) -> bool;
		auto writeUInt64(const std::string &key, std::uint64_t value) -> bool;
		auto writeUInt32(const std::string &key, std::uint32_t value) -> bool;
		auto writeInt64 (const std::string &key, std::int64_t value) -> bool;
		auto writeInt32 (const std::string &key, std::int32_t value) -> bool;
		auto writeDouble(const std::string &key, double value) -> bool;
		auto writeFloat (const std::string &key, float value) -> bool;
		auto writeBool  (const std::string &key, bool value) -> bool;
	public:
		template<typename... Args> inline auto emplace(Args&&... args) { return map_.emplace(std::forward<Args&&>(args)...); }
	public:
		inline auto empty() const { return map_.empty(); }
		inline auto size() const { return map_.size(); }
		inline auto cbegin() const { return map_.cbegin(); }
		inline auto cend() const { return map_.cend(); }
		inline auto begin() const { return map_.begin(); }
		inline auto end() const { return map_.end(); }
	public:
		ProfileSection();
	private:
		KeyValueMap map_;
	};
private:
	typedef ProfileBasicMap<std::string, std::shared_ptr<ProfileSection>> SectionMap;
	typedef typename SectionMap::mapped_type section_type;
	typedef bool SectionHandler(const section_type &);
	typedef std::set<std::string> AliasSet;
public:
	bool setAliasApp(const std::string &name, const std::string &alias);
	bool isAliasApp(const std::string &name) const;
private:
	bool readHelper_(const std::string &name, const std::string &key, const std::function<SectionHandler> &handler) const;
public:
	auto readString (const std::string &name, const std::string &key, const std::string &def) const -> std::string;
	auto readString (const std::string &name, const std::string &key, const char *def = nullptr) const -> std::string;
	auto readUInt64 (const std::string &name, const std::string &key, std::uint64_t def = 0) const -> std::uint64_t;
	auto readUInt32 (const std::string &name, const std::string &key, std::uint32_t def = 0) const -> std::uint32_t;
	auto readInt64  (const std::string &name, const std::string &key, std::int64_t def = 0) const -> std::int64_t;
	auto readInt32  (const std::string &name, const std::string &key, std::int32_t def = 0) const -> std::int32_t;
	auto readDouble (const std::string &name, const std::string &key, double def = 0.0) const -> double;
	auto readFloat  (const std::string &name, const std::string &key, float def = 0.0f) const -> float;
	auto readBool   (const std::string &name, const std::string &key, bool def = false) const -> bool;
public:
	auto writeString(const std::string &name, const std::string &key, const std::string &value) -> bool;
	auto writeString(const std::string &name, const std::string &key, const char *value) -> bool;
	auto writeUInt64(const std::string &name, const std::string &key, std::uint64_t value) -> bool;
	auto writeUInt32(const std::string &name, const std::string &key, std::uint32_t value) -> bool;
	auto writeInt64 (const std::string &name, const std::string &key, std::int64_t value) -> bool;
	auto writeInt32 (const std::string &name, const std::string &key, std::int32_t value) -> bool;
	auto writeDouble(const std::string &name, const std::string &key, double value) -> bool;
	auto writeFloat (const std::string &name, const std::string &key, float value) -> bool;
	auto writeBool  (const std::string &name, const std::string &key, bool value) -> bool;
private:
	void parse_(std::string &name, const std::string &line);
public:
	void save(const std::string &file) const;
	void open(const std::string &file);
	void save(std::ostream &os) const;
	void open(std::istream &is);
public:
	template<typename... Args> inline auto emplace(Args&&... args) { return data_.emplace(std::forward<Args&&>(args)...); }
public:
	inline auto empty() const { return data_.empty(); }
	inline auto size() const { return data_.size(); }
	inline auto cbegin() const { return data_.cbegin(); }
	inline auto cend() const { return data_.cend(); }
	inline auto begin() const { return data_.begin(); }
	inline auto end() const { return data_.end(); }
public:
	ProfileParser(const std::string &file);
	ProfileParser(std::istream &is);
	ProfileParser();
private:
	SectionMap data_;
	AliasSet alias_;
};