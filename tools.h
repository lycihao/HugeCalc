#ifndef TOOLS_H_INCLUDED
#define TOOLS_H_INCLUDED

int lengthof(const int);
int lengthof(const long);
int lengthof(const long long);
long lengthof(const char*);

int isNumber(const char*);
int isInteger(const char*);

char* stripHeadZero(const char*, int*);
char* stripTrailingZero(const char*, int*);

#endif // TOOLS_H_INCLUDED
