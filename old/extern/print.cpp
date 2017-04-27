#include <iostream>
#include <stdio.h>
void print (int x)
{
  std::cout<<x<<"\n";
}

void print (float x)
{
  std::cout<<x<<"\n";
}

void print (char * cstr)
{
  printf("%s\n",cstr);
}

