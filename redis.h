
#ifndef _REDIS_H_
#define _REDIS_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <string>
#include <stdio.h>

#include <hiredis/hiredis.h>
#include <climits>


class Redis
{
public:

    Redis(){}

    ~Redis()
    {
        this->_connect = NULL;
        this->_reply = NULL;
    }

    bool connect(std::string host, int port)
    {
        this->_connect = redisConnect(host.c_str(), port);
        if(this->_connect != NULL && this->_connect->err)
        {
            printf("connect error: %s\n", this->_connect->errstr);
            return 0;
        }
        return 1;
    }

    void select(std::string num){
        std::cout << "select()" << std::endl;
        this->_reply = (redisReply*)redisCommand(this->_connect, "SELECT %s", num.c_str());
    }

    std::string get(std::string key);

    void set(std::string key, std::string value);

    void zadd(std::string key, std::string score, std::string member);

    std::string zrange_all(std::string key, std::string max);

private:

    redisContext* _connect;
    redisReply* _reply;

};

#endif  //_REDIS_H_

