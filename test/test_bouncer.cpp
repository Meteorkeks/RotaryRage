#include "../include/fff/fff.h"
#include "../include/bouncer.h"
#include <gtest/gtest.h>
#include <iostream>


// See https://www.codeproject.com/Articles/1040972/Using-GoogleTest-and-GoogleMock-frameworks-for-emb
// https://interrupt.memfault.com/blog/unit-testing-basics

DEFINE_FFF_GLOBALS;

FAKE_VALUE_FUNC(int, digitalRead, int);
FAKE_VALUE_FUNC(unsigned long, millis);


TEST(BouncerTest, return_rising_edge){
    int buttonPinReadVals[6] = {0, 1, 1, 0, 1, 1};
    unsigned long millisTimeStamps[6] = {0, 33, 66, 100, 133, 166};



    SET_RETURN_SEQ(digitalRead, buttonPinReadVals, 6);
    SET_RETURN_SEQ(millis, millisTimeStamps, 6);

    Bounce bouncer(0, 100);

    ASSERT_FALSE(bouncer.update());
    ASSERT_TRUE(bouncer.update());
    ASSERT_FALSE(bouncer.update());
    ASSERT_FALSE(bouncer.update());
    ASSERT_FALSE(bouncer.update());
    ASSERT_FALSE(bouncer.update());
}
