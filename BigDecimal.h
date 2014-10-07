#include <iostream>
#include <string>

#ifndef BIGDECIMAL_H_INCLUDED
#define BIGDECIMAL_H_INCLUDED

using namespace std;

struct BigDecimal
{
    int signum;
    int scale;
    int length;
    int blocks;
    int* mag;
    /*
    BigDecimal& operator+= (const BigDecimal& val)
    {
        //TODO
        signum += val.signum;
        return *this;
    };
    BigDecimal& operator-= (const BigDecimal& val)
    {
        //TODO
        signum -= val.signum;
        return *this;
    }
    BigDecimal& operator*= (const BigDecimal& val)
    {
        //TODO
        signum += val.signum;
        return *this;
    };
    BigDecimal& operator/= (const BigDecimal& val)
    {
        //TODO
        signum -= val.signum;
        return *this;
    }
    BigDecimal& operator^= (const BigDecimal& val)
    {
        //TODO
        signum -= val.signum;
        return *this;
    }
    */
    void operator- ()
    {
        signum = -signum;
    }
};

BigDecimal getBigDecimal(const int );
BigDecimal getBigDecimal(const long );
BigDecimal getBigDecimal(const long long );
BigDecimal getBigDecimal(const float );
BigDecimal getBigDecimal(const double );
BigDecimal getBigDecimal(const char*);
BigDecimal getBigDecimal(const string);

BigDecimal clone(const BigDecimal&);

void abs(const BigDecimal&, BigDecimal&);
void neg(const BigDecimal&, BigDecimal&);

void add(const BigDecimal&, const BigDecimal&, BigDecimal&);
void sub(const BigDecimal&, const BigDecimal&, BigDecimal&);
void mul(const BigDecimal&, const BigDecimal&, BigDecimal&);
BigDecimal& div(const BigDecimal&, const BigDecimal&);
BigDecimal& exp(const BigDecimal&);
BigDecimal& pow(const BigDecimal&, const BigDecimal&);
BigDecimal& ln(const BigDecimal&);
BigDecimal& log(const BigDecimal&, const BigDecimal&);

ostream& operator << (ostream& ,const BigDecimal& );

#endif // BIGDECIMAL_H_INCLUDED
