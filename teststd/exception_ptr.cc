
#include <iostream>
#include <exception>

int test_foo()
{
}   

int main(int argc, char const *argv[])
{
  return 0
     || test_foo()
  ;
}
