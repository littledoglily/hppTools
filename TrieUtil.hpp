/*Date:     2017/12/08 00:01:11
 *Author:   Jianlin LI
 */
#ifndef TRIEUTIL_
#define TRIEUTIL_

#if(__cplusplus == 201103L)
#include <unordered_map>
#include <unordered_set>
#else
#include <tr1/unordered_map>
#include <tr1/unordered_set>
#endif

#include "StringUtil.hpp"
#include "LocalVector.hpp"

namespace TrieUtil {
using std::vector;
using std::pair;
#if(__cplusplus == 201103L)
using std::unordered_map;
#else
using std::tr1::unordered_map;
#endif
using limonp::LocalVector;
typedef uint32_t TrieKey;
typedef struct DictValue {
	DictValue():value(0){}
	DictValue(double value):value(value){}
	double value;
}DictValue; //DictValue
typedef struct Dag {
	//size_t offset;
	StringUtil::WordRuneStr runeStr;
	//vector<pair<size_t, DictValue*> > nexts;
	LocalVector<pair<size_t, DictValue*> > nexts;
}Dag; //Dag
class TrieNode {
 public:
	 TrieNode():next_(NULL),pvalue(NULL){}
 public:
	 typedef unordered_map<TrieKey, TrieNode*> NextChild;
	 NextChild* next_;
	 DictValue* pvalue;
}; //TrieNode

class Trie {
 public:
	 Trie():Myroot_(new (std::nothrow)TrieNode){}
	 ~Trie() {Destroy(Myroot_);}
	 bool Insert(const char* words, size_t len, double value) {
		 if (!words || len <=0) {
			 fprintf(stdout, "the words[%s] is NULL or the len[%lu] is zero\n",words, len);
			 return false;
		 }
		 LocalVector<StringUtil::WordRuneStr> runeStrArray;
		 if (!DecodeWordRunesStrLite(words, len, runeStrArray)) {
			 fprintf(stdout, "the words[%s] decodeRune failed!\n", words);
			 return false;
		 }
		 TrieNode* root_ = Myroot_;
		 TrieNode::NextChild* childNode;
		 LocalVector<StringUtil::WordRuneStr>::const_iterator wordIter;
		 for (wordIter = runeStrArray.begin(); wordIter != runeStrArray.end(); wordIter++) {
			 if (!root_->next_) {
				 root_->next_ = new (std::nothrow)TrieNode::NextChild;
				 if(!root_->next_){
					 fprintf(stdout, "alloc memory failed!\n");
					 return false;
				 }
			 }
			 childNode = root_->next_;
			 if (childNode->end() == childNode->find(wordIter->rune)) {
				 TrieNode* node = new (std::nothrow)TrieNode;
				 if (!node) {
					 fprintf(stdout, "alloc Trienode failed\n");
					 return false;
				 }
				 childNode->insert(pair<TrieKey, TrieNode*>(wordIter->rune, node));
			 }
			 root_ = (*childNode)[wordIter->rune];
		 }
		 root_->pvalue = new (std::nothrow)DictValue(value);
		 return true;
	 }
	 bool Delete(const char* words, size_t len) {
		 if (NULL == words || 0 >= len) {
			 fprintf(stdout, "the words[%s] is null or len is zero!\n", words);
			 return false;
		 }
		 LocalVector<StringUtil::WordRuneStr> runeStrArray;
		 if (!DecodeWordRunesStrLite(words, len, runeStrArray)) {
			 fprintf(stdout, "the words[%s] decodeRune failed!\n", words);
			 return false;
		 }
		 TrieNode* root = Myroot_;
		 TrieNode::NextChild::const_iterator child;
		 LocalVector<StringUtil::WordRuneStr>::const_iterator wordIter;
		 for (wordIter = runeStrArray.begin(); wordIter != runeStrArray.end(); wordIter++) {
			 if (NULL == root || NULL == root->next_) {
				 return false;
			 }
			 child = root->next_->find(wordIter->rune);
			 if (root->next_->end() == child) {
				 return false;
			 }
			 root = child->second;
		 }
		 if (NULL != root->pvalue) {
			 delete root->pvalue;
			 root->pvalue = NULL;
		 }
		 return true;
	 }
	 /*
	 bool Find(const char* words, size_t len, vector<pair<std::string, double> >& result) {
		 if (NULL == words || 0 >= len) {
			 fprintf(stdout, "the words[%s] is null or len is zero!\n", words);
			 return false;
		 }
		 vector<Dag> dag;
		 vector<StringUtil::WordRuneStr> runeStrArray;
		 if (!DecodeWordRunesStrLite(words, len, runeStrArray)) {
			 fprintf(stdout, "the words[%s] decodeRune failed!\n", words);
			 return false;
		 }
		 if (!Find(runeStrArray.begin(), runeStrArray.end(), dag)) {
			 fprintf(stdout, "find key words failed!\n");
			 return false;
		 }
		 size_t sbegin = 0;
		 size_t send = 0;
		 for (size_t i = 0; i < dag.size(); i++) {
			 vector<pair<size_t, DictValue*> >::const_iterator nextIter;
			 for (nextIter = dag[i].nexts.begin(); nextIter != dag[i].nexts.end(); nextIter++) {
				 if (NULL != nextIter->second) {
					 sbegin = dag[i].runeStr.charoffset;
					 send = runeStrArray[nextIter->first].charoffset + runeStrArray[nextIter->first].len;
					 result.push_back(pair<std::string, double>(std::string(words + sbegin, send - sbegin), nextIter->second->value));
				 }
			 }
		 }
		 return true;
	 }
	 */
	 bool Find(LocalVector<StringUtil::WordRuneStr>::const_iterator runeStrArrayBegin,
			 LocalVector<StringUtil::WordRuneStr>::const_iterator runeStrArrayEnd,
			 LocalVector<pair<std::string, double> >& result) {
		 vector<Dag> dag;
		 if (!Find(runeStrArrayBegin, runeStrArrayEnd, dag)) {
			 fprintf(stdout, "find key words failed!\n");
			 return false;
		 }
		 /*
			size_t sbegin = 0;
			size_t send = 0;
			for (size_t i = 0; i < dag.size(); i++) {
			vector<pair<size_t, DictValue*> >::const_iterator nextIter;
			for (nextIter = dag[i].nexts.begin(); nextIter != dag[i].nexts.end(); nextIter++) {
			if (NULL != nextIter->second) {
			sbegin = dag[i].runeStr.charoffset;
			send = runeStrArray[nextIter->first].charoffset + runeStrArray[nextIter->first].len;
			result.push_back(pair<std::string, double>(std::string(words + sbegin, send - sbegin), nextIter->second->value));
			}
			}
			}
		  */
		 return true;
	 }
	 bool Find(LocalVector<StringUtil::WordRuneStr>::const_iterator runeStrArrayBegin,
			 LocalVector<StringUtil::WordRuneStr>::const_iterator runeStrArrayEnd,
			 vector<Dag>& match) {
		 match.resize(runeStrArrayEnd - runeStrArrayBegin);
		 TrieNode::NextChild::iterator childIter;
		 TrieNode* rootCopy = NULL;
		 for (size_t i = 0; i < size_t(runeStrArrayEnd - runeStrArrayBegin); i++) {
			 match[i].runeStr = *(runeStrArrayBegin + i);
			 childIter = Myroot_->next_->find(match[i].runeStr.rune);
			 if (Myroot_->next_->end() == childIter) {
				 rootCopy = NULL;
			 } else {
				 rootCopy = childIter->second;
			 }
			 if (!rootCopy) {
				 match[i].nexts.push_back(pair<size_t, DictValue*>(i, (DictValue*)NULL));
			 } else {
				 match[i].nexts.push_back(pair<size_t, DictValue*>(i, rootCopy->pvalue));
			 }
			 for (size_t j = i + 1; j < size_t(runeStrArrayEnd - runeStrArrayBegin); j++) {
				 if (!rootCopy || !rootCopy->next_) {
					 break;
				 }
				 childIter = rootCopy->next_->find((runeStrArrayBegin + j)->rune);
				 if (childIter == rootCopy->next_->end())
					 break;
				 if (childIter->second->pvalue) {
					 match[i].nexts.push_back(pair<size_t, DictValue*>(j, childIter->second->pvalue));
				 }
				 rootCopy = childIter->second;
			 }
		 }
		 return true;
	 }
	 void Destroy(TrieNode* root) {
		 if (!root) return;
		 TrieNode::NextChild::iterator child;
		 if (root->next_) {
			 for (child = root->next_->begin(); child != root->next_->end(); child++) {
				 Destroy(child->second);
			 }
		 }
		 if (root->next_) {
			 delete root->next_;
			 root->next_ = NULL;
		 }
		 if (root->pvalue) {
			 delete root->pvalue;
			 root->pvalue = NULL;
		 }
		 if (root) {
			 delete root;
			 root = NULL;
		 }
	 }
	 const TrieNode* GetRoot() const {
		 return Myroot_;
	 }
 private:
	 TrieNode* Myroot_;
}; //Trie
} //TrieUtil

#endif
