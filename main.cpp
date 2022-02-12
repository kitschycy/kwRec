#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "redis.h"

int main(){
    Redis *r = new Redis();
    if(!r->connect("127.0.0.1", 6379))
    {
        printf("connect error!\n");
        return 0;
    }
    
    // 录入部分
    // 英文存在数据库1，中文存在数据库2，倒排表存在数据库0
    buildIndex(r, "enDict.dat", "0");

    //r->zrange_all("z","10");
    //storeWords(r, "enDict.dat", "1");
    //storeWords(r, "cnDict.dat", "2");
    
    
    delete r;
    return 0;
}


