#include <iostream>
#include "BigDecimal.h"
#include "MyExcepction.h"

using namespace std;

int main()
{
    //BigDecimal bd;
    //cout << bd << endl;

    string ch1, ch2 ;
    BigDecimal a,b,c;
    while(cin >> ch1 >> ch2)
    {
        try
        {
            a = getBigDecimal(ch1);
            b = getBigDecimal(ch2);
            mul(a,b,c);
            cout << c << endl;
        }
        catch(MyExcepction ex)
        {
            cout << ex.getError() << endl;
            cout << ex.getErrorInfo() << endl;
        }
    }

    return 0;
}
