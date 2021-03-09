#include "./include/criterion/criterion.h"
#include "main.h" 

// main.h doesnt have access to function declaration thats why its failing
Test(sample,test){
    cr_assert(getLastMultiple(58) == 40);
    cr_assert(getLastMultiple(29) == 20);
    cr_assert(getLastMultiple(640) == 640);
}

Test(colorRect,colorRect_test){
    
}
