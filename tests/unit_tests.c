#include <stdio.h>
#include <strings.h>

unsigned int count=0, expected=0;
unsigned int lenstr(const char*);

unsigned int test_puts()
{
    count += lenstr("Test Case1"); expected += 10;
    count += lenstr("Test Case   2"); expected += 13;
    count += lenstr("TestCase\n"); expected += 9;
    
    if (expected != count) 
        return 1;
    else
        return 0;
}

int main()
{
    
    printf("------------------------------------------\n");
    printf("        ATAK Kernel Test Suite            \n"); 
    printf("------------------------------------------\n");
    printf("------------------------------------------\n");
    printf("        1 ==> Fail, 0 ==> Pass            \n");
    printf("------------------------------------------\n");
    printf("Strlen Test 1: %d                         \n", test_puts());
    printf("------------------------------------------\n");
    printf("------------------------------------------\n");
    return 0;
}


