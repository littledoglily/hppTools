/*Date:		2017/12/08 00:01:11
 *Author:	Jianlin LI
 */
#ifndef STRINGUTIL_
#define STRINGUTIL_
#include <cstdio>
#include <cstring>
#include <cstdint>
#include <sstream>
#include <vector>
#include "LocalVector.hpp"

namespace StringUtil {

/*字符串分割
 */
inline bool Split(const char* sstring, const char* spliter, std::vector<std::string>& result) {
	if (!sstring)
		return false;
	const char *pos = NULL;
	const char* sbegin = sstring;
	while ((pos = strstr(sbegin, spliter)) != NULL) {
		if (pos - sbegin > 0)
			result.push_back(std::string(sbegin, pos - sbegin));
		sbegin = pos + strlen(spliter);
	}
	std::string lastItem(sbegin);
	if (lastItem.size() > 0)
		result.push_back(lastItem);
	return true;
}
/*字符串拼接
 */
inline std::string Join(const std::vector<std::string>& part, const std::string& spliter){
	std::stringstream strJoin;
	for (size_t index = 0; index < part.size(); index++) {
		if (index != part.size() -1)
			strJoin<<part[index]<<spliter;
		else
			strJoin<<part[index];
	}
	return strJoin.str();
}
/*去掉字符串前面指定字符
 */
inline std::string StripHead(const std::string& sstring, const char* characters=" \t\f\v\n\r")
{
	if (!characters)
		return sstring;
	if (sstring.size() == 0)
		return std::string("");
	std::size_t pos = sstring.find_first_not_of(characters);
	if (pos != std::string::npos){
		return sstring.substr(pos);
	}
	return std::string("");
}
inline std::string StripTail(const std::string& sstring, const char* characters=" \t\f\v\n\r")
{
	if (!characters)
		return sstring;
	if (sstring.size() == 0)
		return std::string("");
	std::size_t pos = sstring.find_last_not_of(characters);
	if (pos != std::string::npos){
		return sstring.substr(0, pos+1);
	}
	return std::string("");
}
inline std::string Strip(const std::string& sstring, const char* character=" \t\f\v\n\r")
{
	std::string stripHeadStr = StripHead(sstring, character);
	return StripTail(stripHeadStr, character);
}
/*判断一个字符是否以指定字符开头
 */
inline bool StartWith(const std::string& sstring, const std::string& prefix)
{
	if (prefix.length() > sstring.length())
		return false;
	return 0 == sstring.compare(0, prefix.length(), prefix);
}
/*判断一个字符是否以指定字符结尾
 */
inline bool EndWith(const std::string& sstring, const std::string& suffix)
{
	if (suffix.length() > sstring.length())
		return false;
	return 0 == sstring.compare(sstring.length() - suffix.length(), suffix.length(), suffix);
}
/*转换成固定格式的时间
 */
inline void GetTime(const std::string& format, std::string&  timeStr) {
  time_t timeNow;
  time(&timeNow);
  timeStr.resize(64);
  size_t len = strftime((char*)timeStr.c_str(), timeStr.size(), format.c_str(), localtime(&timeNow));
  timeStr.resize(len);
}
/*拼接路径
 */
inline std::string PathJoin(const std::string& path1, const std::string& path2) {
  if(EndWith(path1, "/")) {
    return path1 + path2;
  }
  return path1 + "/" + path2;
}
/*-----------------------------------------------------------------------------------*/
typedef struct WordRuneStrLite {
	uint32_t rune;
	uint32_t len;
	WordRuneStrLite():rune(0), len(0) {}
	WordRuneStrLite(uint32_t r, uint32_t l):rune(r), len(l){}
}WordRuneStrLite; //WordRuneStrLite
typedef struct WordRuneStr {
	uint32_t rune;
	uint32_t charoffset;
	uint32_t len;
	uint32_t unicode_offset;
	uint32_t unicode_len;
	WordRuneStr():rune(0), charoffset(0), len(0){
	}
	WordRuneStr(uint32_t r, uint32_t c, uint32_t l):
		rune(r), charoffset(c), len(l){
		}
	WordRuneStr(uint32_t r, uint32_t c, uint32_t l, uint32_t u_o, uint32_t u_l):
		rune(r), charoffset(c), len(l), unicode_offset(u_o), unicode_len(u_l) {
		}
}WordRune; //WordRuneStr
/*把一个汉字转成对应的unicode编码和长度
 */
inline WordRuneStrLite DecodeWordRuneStrLite(const char* str, size_t len) {
	WordRuneStrLite rsl(0,0);
	if (NULL == str || 0 >= len) {
		return rsl;
	}
	if (!(str[0] & 0x80)) {
		rsl.rune = (uint8_t)(str[0]) & 0x7f;
		rsl.len = 1;
	} else if ((uint8_t)(str[0]) <= 0xdf && 1 < len) {
		rsl.rune = (uint8_t)(str[0]) & 0x1f;
		rsl.rune <<= 6;
		rsl.rune |= (uint8_t)(str[1]) & 0x3f;
		rsl.len = 2;
	} else if ((uint8_t)(str[0]) <= 0xef && 2 < len) {
		rsl.rune = (uint8_t)(str[0]) & 0x0f;
		rsl.rune <<= 6;
		rsl.rune |= (uint8_t)(str[1]) & 0x3f;
		rsl.rune <<= 6;
		rsl.rune |= (uint8_t)(str[2]) & 0x3f;
		rsl.len = 3;
	} else if ((uint8_t)(str[0]) <= 0xf7 && 3 < len) {
		rsl.rune = (uint8_t)(str[0]) & 0x07;
		rsl.rune <<= 6;
		rsl.rune |= (uint8_t)(str[1]) & 0x3f;
		rsl.rune <<= 6;
		rsl.rune |= (uint8_t)(str[2]) & 0x3f;
		rsl.rune <<= 6;
		rsl.rune |= (uint8_t)(str[3]) & 0x3f;
		rsl.len = 4;
	} else {
		rsl.rune = 0;
		rsl.len = 0;
	}
	return rsl;
}

inline bool DecodeWordRunesStrLite(const char* str, size_t len, limonp::LocalVector<WordRuneStr>& RuneStrArray) {
	RuneStrArray.clear();
	RuneStrArray.reserve(len / 2);
	size_t wordIndex = 0;
	for (size_t i = 0; i < len; ) {
		WordRuneStrLite rs = DecodeWordRuneStrLite(str + i, len - i);
		if (rs.rune == 0 || rs.len == 0) {
			RuneStrArray.clear();
			return false;
		}
		WordRuneStr tempRune(rs.rune, i, rs.len, wordIndex, 1);
		RuneStrArray.push_back(tempRune);
		i += rs.len;
		wordIndex += 1;
	}
	return true;
}
/*一个utf8编码汉字的长度
 */
inline int WordLen(const char* word, size_t len){
	if(!word || len <= 0)
		return -1;
	if (((unsigned char)word[0] & 0x80) == 0)
		return 1;
	else if (((unsigned char)word[0] >= 0xc0) && ((unsigned char)word[0] <= 0xdf))
		return 2;
	else if (((unsigned char)word[0] >= 0xe0) && ((unsigned char)word[0] <= 0xef))
		return 3;
	else if (((unsigned char)word[0] >= 0xf0) && ((unsigned char)word[0] <= 0xf7))
		return 4;
	else if (((unsigned char)word[0] >= 0xf8) && ((unsigned char)word[0] <= 0xfb))
		return 5;
	else if (((unsigned char)word[0] >= 0xfc) && ((unsigned char)word[0] <= 0xfd))
		return 6;
	else
		return -2;
}
/*一个中文汉字串的中文、符号的个数
 */
//inline size_t wordslen(const char* words, const size_t wordsLength){
//}
/*一个中文汉字(utf8编码)unicode编码
 */
inline uint32_t ToUnicode(const char* word, size_t len)
{
	int wordRealLen = WordLen(word, len);
	if (wordRealLen <= 0) {
		fprintf(stdout, "get wordLen failed!\n");
		return 0;
	}
	uint32_t hashcode = 0;
	switch (wordRealLen) {
		case 1:{
			hashcode |= ((unsigned char)word[0] & 0x7f);
			break;
		}
		case 2:{
			hashcode |= ((((unsigned char)word[0] & 0x1f) << 3) | (((unsigned char)word[1] >> 3) & 0x7));
			hashcode <<= 3;
			hashcode |= ((unsigned char)word[1] & 0x7);
			break;	   
		}
		case 3:{
			hashcode |= ((((unsigned char)word[0] & 0xf) << 4) | (((unsigned char)word[1] >> 2) & 0xf));
			hashcode <<= 8;
			hashcode |= ((((unsigned char)word[1] & 0x3) << 6) | ((unsigned char)word[2] & 0x3f));
			break;
		}
		case 4:{
			hashcode |= ((unsigned char)word[0] & 0x07);
			hashcode <<= 6;
			hashcode |= ((unsigned char)word[1] & 0x3f);
			hashcode <<= 6;
			hashcode |= ((unsigned char)word[2] & 0x3f);
			hashcode <<= 6;
			hashcode |= ((unsigned char)word[3] & 0x3f);
			break;
		}
		default:
			   break;
	}
	return hashcode;
}

} // namespace StringUtil
#endif	//StringUtil
