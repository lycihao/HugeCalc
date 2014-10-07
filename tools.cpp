#include "tools.h"
#include <memory.h>

int lengthof(const int x)
{
    int val = x;
    if(val == 0) return 1;
    int len = 0;
    while(val != 0)
    {
        ++len;
        val /= 10;
    }
    return len;
}

int lengthof(const long x)
{
    long val = x;
    if(val == 0) return 1;
    int len = 0;
    while(val != 0)
    {
        ++len;
        val /= 10;
    }
    return len;
}


int lengthof(const long long x)
{
    long long val = x;
    if(val == 0) return 1;
    int len = 0;
    while(val != 0)
    {
        ++len;
        val /= 10;
    }
    return len;
}

long lengthof(const char* val)
{
    long len = -1;
    while(val[++len]);
    return len;
}

int isInteger(const char* val)
{
    if(!lengthof(val)) return 0;
    int index = 0;
    if(val[index] == '-' || val[index] == '+') ++index;
    if(!val[index]) return 0;
    while(val[index])
    {
        if(val[index] < '0' || val[index] > '9') return 0;
        ++index;
    }
    return 1;
}

int isNumber(const char* val)
{
    if(!lengthof(val)) return 0;
    int index = 0;
    if(val[index] == '-' || val[index] == '+') ++index;
    if(!val[index]) return 0;
    bool dot = false;
    while(val[index])
    {
        if(val[index] >= '0' && val[index] <= '9')
        {
            ++index;
            continue;
        }
        else if(val[index] == '.')
        {
            if(!dot) dot = true;
            else return 0;
        }
    }
    return 1;
}

char* stripHeadZero(const char* val, int* len)
{
    while(val[0] != '\0' && val[0] == '0')
    {
        ++val;
        --(*len);
    }
    char* rs = new char[*len];
    memcpy(rs,val,*len*sizeof(char));
    return rs;
}

char* stripTrailingZero(const char* val, int* len)
{
    if(*len < 1) return new char[0];
    while(*len)
    {
        if(val[*len - 1] != '0') break;
        --(*len);
    }
    if(*len == 0) return new char[0];
    char* rs = new char[*len];
    memcpy(rs,val,*len*sizeof(char));
    return rs;
}
