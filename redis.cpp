#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "redis.h"

using std::cout;
using std::string;
using std::ifstream;
using std::endl;
using std::cerr;



ifstream ifs;
string word, frequency;

int storeWords(Redis *r, string filename, string DB){
    //首先打开文件，按行读入，r->set(“单词”，“词频”)
    r->select(DB);
    ifs.open(filename, ios::in);
    if(!ifs.is_open()){
        cerr << "Could not open the file - '"
             << filename << "'" << endl;
        return EXIT_FAILURE;
    }

    string line;
    while(getline(ifs, line)){
        std::istringstream is(line);
        is >> word >> frequency;         
        cout << "SET " << word << " --> " << frequency << endl;
        r->set(word, frequency);
    }
    ifs.close();
    return 0;
}

void buildIndex(Redis *r, string filename, string DB){
    // 倒排索引表 结构 字母key -- 词频score -- 单词value
    // 数据结构选择：
    r->select("0");
    ifs.open(filename, ios::in);
    if(!ifs.is_open()){
        cerr << "Could not open the file - '"
             << filename << "'" << endl;
    }
    string line;
    while(getline(ifs, line)){
        std::istringstream is(line);
        is >> word >> frequency;
        for(auto ch : word){
            string s(1, ch);
            r->zadd(s, frequency, word);
            //cout << "ZADD " << s << " " << frequency << " " << word << endl;
        }
    }
    ifs.close();
}

std::string Redis::get(std::string key)
{
    this->_reply = (redisReply*)redisCommand(this->_connect, "GET %s", key.c_str());
    std::string str = this->_reply->str;
    freeReplyObject(this->_reply);
    return str;
}

void Redis::set(std::string key, std::string value)
{
    redisCommand(this->_connect, "SET %s %s", key.c_str(), value.c_str());
}

// 存储，字母，单词，词频
void Redis::zadd(std::string key, std::string score, std::string member){
    
    //std::string str = "SELECT " + std::to_string(num);
    //this->_reply = (redisReply*)redisCommand(this->_connect, "SELECT %s", num.c_str());
    //std::cout << "select " << num << std::endl;
    redisCommand(this->_connect, "ZADD %s %s %s", key.c_str(), score.c_str(), member.c_str());
}

std::string Redis::zrange_all(std::string key, std::string max){
    this->_reply = (redisReply*)redisCommand(this->_connect, "ZRANGE %s 0 %s", key.c_str()
            , max.c_str());
    std::string str; 
    if (this->_reply->type == REDIS_REPLY_ARRAY) {
        for (int j = 0; j < this->_reply->elements; j++) {
            //printf("%u) %s\n", j, this->_reply->element[j]->str);
            str = str + "/" + this->_reply->element[j]->str;
        }
    }
    freeReplyObject(this->_reply);
    return str;
}
