#include "BigDecimal.h"
#include "tools.h"              //外置工具
#include "MyExcepction.h"       //异常头文件
#include <iomanip>              //设置格式化输出
#include <iostream>
//#include <string>
#include <memory.h>
#include <sstream>

using namespace std;

const int MAXSCALE = 1000000;
const int UNITNUM = 1000000000;
const int SUBNUM = 999999999;

//整数赋值
BigDecimal getBigDecimal(const int val)
{
    BigDecimal rs;
    rs.scale = 0;
    rs.length = lengthof(val);
    if(val == 0)
    {
        rs.signum = 0;
        rs.blocks = 1;
        rs.mag = new int[0];
        return rs;
    }
    rs.signum = val > 0 ? 1 : -1;
    rs.blocks = 1;
    rs.mag = new int[1];
    rs.mag[0] = val * rs.signum;
    return rs;
}

//长整数赋值
BigDecimal getBigDecimal(const long val)
{
    BigDecimal rs;
    rs.scale = 0;
    rs.length = lengthof(val);
    if(val == 0)
    {
        rs.signum = 0;
        rs.blocks = 1;
        rs.mag = new int[0];
        return rs;
    }
    rs.signum = val > 0 ? 1 : -1;
    rs.blocks = 1;
    rs.mag = new int[1];
    rs.mag[0] = val * rs.signum;
    return rs;
}

//长长整数赋值
BigDecimal getBigDecimal(const long long val)
{
    BigDecimal rs;
    rs.length = lengthof(val);
    rs.scale = 0;
    if(rs.length <= 9) rs.blocks = 1;
    else rs.blocks = 2;
    if(val == 0)
    {
        rs.mag = new int [0];
        rs.signum = 0;
        rs.blocks = 0;
    }
    else
    {
        rs.signum = val > 0 ? 1 : -1;
        rs.mag = new int [rs.blocks];
        rs.mag[rs.blocks - 1] = rs.signum * val / 1000000000;
        rs.mag[0] = rs.signum * val % 1000000000;
    }
    return rs;
}

//单精度浮点数赋值
BigDecimal getBigDecimal(const float val)
{
    stringstream sstream;
    sstream << val;
    string str = sstream.str();
    return getBigDecimal(str.c_str());
}

//双精度浮点数赋值
BigDecimal getBigDecimal(const double val)
{
    stringstream sstream;
    sstream << setprecision(16) << val;
    string str = sstream.str();
    return getBigDecimal(str.c_str());
}

long long parseExp(const char* val, int offset, int len)
{
    long long ex = 0;
    bool isNeg = false;
    if(val[offset] == '-')
    {
        isNeg = true;
        ++offset;
        --len;
    }
    else if(val[offset] == '+')
    {
        ++offset;
        --len;
    }
    for(int i = offset; i < offset + len; i++)
    {
        if(val[i] >= '0' && val[i] <= '9')
        {
            ex = ex * 10 + val[i] - '0';
            continue;
        }
        else
        {
            MyExcepction excep(FormatError);
            excep.setErrorInfo("Invalid character!");
            throw excep;
        }
    }
    if(!len)
    {
        MyExcepction excep(FormatError);
        excep.setErrorInfo("No Number");
        throw excep;
    }
    if(len > 9)
    {
        MyExcepction excep(OverflowError);
        excep.setErrorInfo("Exp Overflow!");
        throw excep;
    }
    if(isNeg) ex = -ex;
    return ex;
}

int* parseInt(const char* val, int len)
{
    if(!len) return new int[0];
    int blocks = len / 9;
    int firstBlocks = len % 9;
    if(firstBlocks) ++blocks;
    else firstBlocks = 9;
    int* mag = new int[blocks];
    memset(mag,0,blocks*sizeof(int));
    for(int i = 0; i < firstBlocks; i++) mag[blocks - 1] = mag[blocks - 1]*10 + val[i] - '0';
    for(int i = 0; i < blocks - 1; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            mag[blocks - i - 2] = mag[blocks - i - 2]*10 + val[firstBlocks+i*9+j] - '0';
        }
    }
    return mag;
}

//字符数组赋值
BigDecimal getBigDecimal(const char* val)
{
    int len = lengthof(val);
    bool isNeg = false;
    int index = 0;
    if(len <= 0)
    {
        MyExcepction excep(FormatError);
        excep.setErrorInfo("No Number");
        throw excep;
    }
    if(val[index] == '-')
    {
        isNeg = true;
        ++index;
        --len;
    }
    else if(val[index] == '+')
    {
        ++index;
        --len;
    }
    int prec = 0;
    int scl = 0;
    long long exp = 0;
    char c;
    bool dot = false;
    char* coeff = new char[len + 9];
    int idx = 0;
    for(; len > 0; index++,len--)
    {
        c = val[index];
        if(c == '0')
        {
            if(prec == 0)
            {
                prec = 1;
                coeff[idx] = c;
            }
            else if(idx)
            {
                coeff[idx++] = c;
                ++prec;
            }
            if(dot) ++scl;
        }
        else if(c > '0' && c <= '9')
        {
            if(prec != 1 || idx!= 0) ++prec;
            coeff[idx++] = c;
            if(dot) ++scl;
        }
        else if(c == '.')
        {
            if(dot)
            {
                MyExcepction excep(FormatError);
                excep.setErrorInfo("More than one dot.");
                throw excep;
            }
            dot = true;
        }
        else if(c == 'e' || c == 'E')
        {
            try
            {
                exp = parseExp(val,++index,--len);
            }
            catch(MyExcepction ex)
            {
                throw ex;
            }
            if((int) exp != exp)
            {
                MyExcepction excep(OverflowError);
                excep.setErrorInfo("Exp overflow");
                throw excep;
            }
            break;
        }
        else
        {
            MyExcepction excep(FormatError);
            excep.setErrorInfo("Invalid character");
            throw excep;
        }
    }
    coeff[idx] = '\0';
    if(prec == 0)
    {
        MyExcepction excep(FormatError);
        excep.setErrorInfo("No Number");
        throw excep;
    }
    if(exp != 0)
    {
        long adj = scl - exp;
        if((int) adj != adj)
        {
            MyExcepction excep(OverflowError);
            excep.setErrorInfo("Number Overflow.");
            throw excep;
        }
        scl = (int) adj;
    }
    if(idx == 0)
    {
        BigDecimal rs = {0,0,1,0,new int[0]};
        return rs;
    }
    else
    {
        coeff = stripTrailingZero(coeff,&idx);
        scl -= prec - idx;
        int adj = scl % 9;
        if(adj > 0)
        {
            adj = 9 - adj;
            scl += adj;
        }
        if(adj < 0)
        {
            adj = -adj;
            scl += adj;
        }
        for(int i = 0; i < adj; i++) coeff[idx++] = '0';
        coeff[idx] = '\0';
        prec = idx;
    }
    int blocks = prec / 9;
    if(prec % 9) ++blocks;
    if(isNeg)
    {
        BigDecimal rs;
        rs.signum = -1;
        rs.scale = scl;
        rs.length = prec;
        rs.blocks = blocks;
        rs.mag = parseInt(coeff,prec);
        return rs;
    }
    else
    {
        BigDecimal rs;
        rs.signum = 1;
        rs.scale = scl;
        rs.length = prec;
        rs.blocks = blocks;
        rs.mag = parseInt(coeff,prec);
        return rs;
    }
}

//字符串赋值
BigDecimal getBigDecimal(const string val)
{
    return getBigDecimal(val.c_str());
}

//复制
BigDecimal clone(const BigDecimal& value)
{
    BigDecimal rs;
    rs.signum = value.signum;
    rs.scale = value.scale;
    rs.length = value.length;
    rs.blocks = value.blocks;
    rs.mag = new int[rs.blocks];
    memcpy(rs.mag,value.mag,sizeof(int)*rs.blocks);
    return rs;
}

//取绝对值
void abs(const BigDecimal& value, BigDecimal& rs)
{
    rs.signum = value.signum*value.signum;
    rs.blocks = value.blocks;
    rs.length = value.length;
    rs.scale = value.scale;
    rs.mag = new int[value.blocks];
    memcpy(rs.mag,value.mag,value.blocks*sizeof(int));
}

//取相反数
void neg(const BigDecimal& value, BigDecimal& rs)
{
    rs.signum = 0 - value.signum;
    rs.blocks = value.blocks;
    rs.length = value.length;
    rs.scale = value.scale;
    rs.mag = new int[value.blocks];
    memcpy(rs.mag,value.mag,value.blocks*sizeof(int));
}

//加法运算
/// result = value1 + value2
void add(const BigDecimal&value1, const BigDecimal& value2, BigDecimal& rs)
{
    if(value1.signum == 0)
    {
        rs = clone(value2);
        return;
    }
    else if(value2.signum == 0)
    {
        rs = clone(value1);
        return;
    }
    int scale = value1.scale > value2.scale ? value1.scale : value2.scale;
    int adj1 = scale - value1.scale;
    int adj2 = scale - value2.scale;
    int block1 = value1.blocks + adj1 / 9 ;
    int block2 = value2.blocks + adj2 / 9 ;
    int blocks = block1 > block2 ? block1 : block2;
    ++blocks;
    int* mag = new int[blocks];
    int blockadj = adj1 / 9;
    int index = 0;
    memset(mag,0,sizeof(int)*blocks);
    for(int i = blockadj; i < value1.blocks + blockadj; i++)
    {
        mag[i] = value1.mag[i];
    }
    blockadj = adj2 / 9;
    if(value1.signum == value2.signum)
    {
        for(int i = 0; i < value2.blocks; i++)
        {
            mag[i + blockadj] += value2.mag[i];
            if(mag[i+blockadj] >= UNITNUM)
            {
                mag[i+blockadj] -= UNITNUM;
                ++mag[i+blockadj+1];
            }
        }
        index = blockadj + value2.blocks;
        while(mag[index] >= UNITNUM)
        {
            mag[index] -= UNITNUM;
            ++mag[++index];
        }
        rs.signum = value1.signum;
    }
    else
    {
        for(int i = 0; i < value2.blocks; i++)
        {
            mag[i+blockadj] -= value2.mag[i];
            if(mag[i+blockadj] < 0)
            {
                mag[i+blockadj] += UNITNUM;
                mag[i+blockadj + 1] -= 1;
            }
        }
        index = blockadj + value2.blocks;
        while(index < blocks - 1)
        {
            if(mag[index] < 0)
            {
                mag[index] += UNITNUM;
                mag[++index] -= 1;
            }
            else break;
        }
        if(mag[blocks - 1] == -1)
        {
            mag[blocks - 1] = 0;
            mag[0] = UNITNUM - mag[0];
            for(int i = 1; i < blocks - 1; i++)
            {
                mag[i] = SUBNUM - mag[i];
            }
            for(int i = 0; i < blocks - 1; i++)
            {
                if(mag[i] >= UNITNUM)
                {
                    mag[i+1] += 1;
                    mag[i] -= UNITNUM;
                    continue;
                }
                else break;
            }
            rs.signum = -value1.signum;
        }
        else rs.signum = value1.signum;
    }
    while(1)
    {
        if(blocks == 0) {
            rs.signum = 0;
            rs.blocks = 0;
            rs.length = 1;
            rs.mag = new int[0];
            rs.scale = 0;
            return;
        }
        if(*mag == 0)
        {
            ++mag;
            --blocks;
            scale -= 9;
        }
        else break;
    }
    for(int i = blocks; i >= 0; i--)
    {
        if(i == 0) {
            rs.signum = 0;
            rs.blocks = 0;
            rs.length = 1;
            rs.mag = new int[0];
            rs.scale = 0;
            return;
        }
        if(mag[i - 1] == 0) continue;
        else
        {
            rs.mag = new int[i];
            rs.blocks = i;
            rs.scale = scale;
            memcpy(rs.mag, mag, rs.blocks*sizeof(int));
            rs.length = (i - 1)*9 + lengthof(mag[i - 1]);
            return;
        }
    }
    return;
}

//减法运算
/// result = value1 - value2
void sub(const BigDecimal& value1, const BigDecimal& value2, BigDecimal& rs)
{
    BigDecimal result;
    neg(value2, result);
    add(value1, result, rs);
    return;
}

//乘法运算
/// result = value1 * value2
void mul(const BigDecimal& value1, const BigDecimal& value2, BigDecimal& rs)
{
    rs.scale = 0;
    rs.signum = 0;
    rs.blocks = 0;
    rs.length = 1;
    rs.mag = new int[0];
    if(value1.signum == 0 || value2.signum == 0) return;
    int blocks = value1.blocks + value2.blocks;
    int* mag = new int[blocks];
    memset(mag, 0, sizeof(int) * blocks);
    int scale = value1.scale + value2.scale;
    int signum = value1.signum * value2.signum;
    long long mtemp;
    for(int i = 0; i < value1.blocks; i++)
    {
        for(int j = 0; j < value2.blocks; j++)
        {
            mtemp = (long long)value1.mag[i] * value2.mag[j];
            mag[i+j] += mtemp % UNITNUM;
            mag[i+j+1] += mtemp / UNITNUM;
        }
    }
    if(mag[blocks - 1] == 0) --blocks;
    if(blocks > 1112)
    {
        mag = mag + (blocks - 1112);
        scale -= 9*(blocks - 1112);
        blocks = 1112;
    }
    while(1)
    {
        if(blocks == 0) return;
        if(*mag == 0)
        {
            ++mag;
            --blocks;
            scale -= 9;
        }
        else break;
    }
    rs.blocks = blocks;
    rs.length = blocks*9 + lengthof(mag[blocks - 1]);
    rs.scale = scale;
    rs.mag = mag;
    rs.signum = signum;
    return;
}

//除法运算
/// result = value1 / value2
BigDecimal& div(const BigDecimal& value1, const BigDecimal& value2)
{
    //TODO
    BigDecimal rs;
    return rs;
}

//自然对数的幂运算
/// result = Exp[value]
BigDecimal& exp(const BigDecimal& value)
{
    //TODO
    BigDecimal rs;
    return rs;
}

//指数运算
BigDecimal& pow(const BigDecimal& value1, const BigDecimal& value2)
{
    //TODO
    BigDecimal rs;
    return rs;
}

//自然对数运算
BigDecimal& ln(const BigDecimal& value)
{
    //TODO
    BigDecimal rs;
    return rs;
}

//对数运算
BigDecimal& log(const BigDecimal& value1, const BigDecimal& value2)
{
    //TODO
    BigDecimal rs;
    return rs;
}

//重载输出流
ostream& operator << (ostream& os,const BigDecimal& val)
{
    if (val.signum == 0)
    {
        os << 0;
        return os;
    }
    stringstream sstream;
    sstream << val.mag[val.blocks - 1];
    for(int i = val.blocks - 2; i >= 0; --i) sstream << setfill('0') << setw(9) << val.mag[i];
    string str = sstream.str();
    if(val.scale == val.length)
    {
        str.insert(0,"0.");
    }
    else if(val.scale > 0 && val.length > val.scale)
    {
        str.insert(val.length - val.scale,".");
    }
    int dot = str.find('.');
    int zero = str.length();
    while(1)
    {
        if(str.at(zero - 1) == '0')
        {
            --zero;
            continue;
        }
        break;
    }
    if(dot == zero - 1) str = str.erase(dot);
    else if(dot != -1)str = str.erase(zero);
    if(val.scale > val.length || val.scale < 0)
    {
        str.append("e");
        if(val.scale > 0) str.append("-");
        else str.append("+");
        int v = val.scale > 0 ? val.scale : -val.scale;
        int len = str.length();
        while(v)
        {
            str.insert(len,1,(char)(v%10 + '0'));
            v = v / 10;
        }
    }
    if(val.signum == -1) str.insert(0,"-");
    os << str;
    return os;
}
