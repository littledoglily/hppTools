/*Date:		2017/12/08 00:01:11
 *Author:	Jianlin LI
 */
#ifndef CONFIG_
#define CONFIG_
#include <map>
#include <vector>
#include <cstring>
#include <fstream>
#include <iostream>
#include "StringUtil.hpp"

namespace multiTools {
using std::map;
using std::string;
using std::vector;
using std::ifstream;

class Config {
public:
	explicit Config(const char* path) {
		LoadConfFile(path);
	}
	~Config() {
		MyConfig_.clear();
	}
	string GetOption(const string& opt, const string& defaultValue) const {
		map<string, string>::const_iterator iter = MyConfig_.find(opt);
		if (MyConfig_.end() != iter) {
			return iter->second;
		}
		return defaultValue;
	}
	int GetOption(const string& opt, int defaultValue) const {
		string val = GetOption(opt, "");
		if ("" == val) { 
			return defaultValue;
		}
		return atoi(val.c_str());
	}
	const char* operator [] (const char* key) const {
		if (!key) {
			return NULL;
		}
		map<string, string>::const_iterator it = MyConfig_.find(key);
		if (MyConfig_.end() != it) {
			return it->second.c_str();
		}
		return NULL;

	}
	bool IsEmpty() {
		return MyConfig_.empty();
	}
	void LoadConfFile(const char* path) {
		string line;
		vector<string> conVec;
		ifstream fileHandle(path);
		if (!fileHandle.is_open()) {
			fprintf(stdout, "file cant open\n");
			return;
		}
		while (!fileHandle.eof()) {
			std::getline(fileHandle, line);
			if (line.length() <= 0) 
				continue;
			if (StringUtil::StartWith(line, "#") || StringUtil::StartWith(line, "//"))
				continue;
			conVec.clear();
			StringUtil::Split(line.c_str(), "=", conVec);
			if (conVec.size() != 2 ) {
				fprintf(stdout, "read config file %s failed!\n", line.c_str());
				continue;
			}
			string key = StringUtil::Strip(conVec[0]);
			string value = StringUtil::Strip(conVec[1]);
			if (!MyConfig_.insert(make_pair(key, value)).second) {
				fprintf(stdout, "key[%s] already exits!\n", key.c_str());
				continue;
			}
		}
		fileHandle.close();
	}
private:
	map<string, string> MyConfig_;
}; //Config

} // multiTools


#endif //Config
