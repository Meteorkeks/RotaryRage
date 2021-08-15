#include "Fixture.h"
#include "bouncer.h"


// See https://www.codeproject.com/Articles/1040972/Using-GoogleTest-and-GoogleMock-frameworks-for-emb


int digitalRead(int line, int *value)
{
    return GetMock<GpioMock>().ReadGpio(line, value);
}

class BouncerUnitTest : public TestFixture
{
public:
    ...
};


// TODO: access protected members in test code https://stackoverflow.com/a/1725107/10045897


TEST_F(BouncerUnitTest, PusheButtonDownTest)
{
    // Time steps
    EXPECT_CALL(GetMock<OscillatorMock>(), millis()).Times(5).WillOnce(Return(0)).WillOnce(Return(33)).WillOnce(Return(66)).WillOnce(Return(100)).WillOnce(Return(133));
    EXPECT_CALL(GetMock<GpioMock>(), ReadGpio(0, _)).Times(5).WillOnce(Return(0)).WillOnce(Return(1)).WillOnce(Return(0)).WillOnce(Return(1)).WillOnce(Return(1));

    uint_8_t pin_number = 5;
    uint_16_t debounce_timeout = 100;
    int timeout = 100;
    debouncer = Bounce(
        pin_number,
        debounce_timeout
    )
    EXPECT_EQ(debouncer->update(), false)
    EXPECT_EQ(debouncer->)
    EXPECT_EQ(debouncer.update(), true)
    EXPECT_EQ(debouncer.update(), false)
    EXPECT_EQ(debouncer.update(), false)
    EXPECT_EQ(debouncer.update(), false)
}