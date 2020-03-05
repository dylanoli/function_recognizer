#include<cmath>
#include<iostream>
using namespace std;

float f(char * str, float x);
int main()
{
    char str[] = "x+3";
    cout<<"Resultado: "<<f(str,3);
}

float f(char * str, float x)
{
    
    return x*2;
}