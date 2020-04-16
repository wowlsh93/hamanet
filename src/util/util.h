//
// Created by hama on 20. 4. 14..
//

#ifndef NAVI_UTIL_H
#define NAVI_UTIL_H


#include <iostream>
#include <vector>
#include <string>
#include <cstdarg>
#include <cstring>
#include <memory>
#include <stdio.h>
#include <stdarg.h>

#define LOGGING(info)  \
    std::cout << "[INFO]: " << info <<  std::endl;



#define DISALLOW_COPY_AND_ASSIGN_AND_MOVE(classname) \
  classname(const classname&) = delete; \
  classname& operator=(const classname&) = delete; \
  classname(const classname&&) = delete; \
  classname& operator=(const classname&&) = delete;



std::string string_format(const char *fmt, ...)
{
    va_list args;
    va_start ( args, fmt );

    int size = 256;
    std::string result;
    int c;

    while(1)
    {
        result.resize(size);
        c = vsnprintf ( (char*)(result.data()), size, fmt, args );
        if(c!=-1) break;

        size <<=1;
    }
    result.resize(c);
    va_end ( args );

    return result;
}

#endif //NAVI_UTIL_H
