#include "mico.h"
#include "sensor/DHT11/DHT11.h"
#include "dht11_operation.h"

void IOT_DHT11_Rst(mico_gpio_t pin)
{
    IOT_DHT11_IO_OUT(pin);   //SET OUTPUT
    IOT_DHT11_DATA_Clr(pin); //GPIOA.0=0
    Delay_ms(20);        //Pull down Least 18ms
    IOT_DHT11_DATA_Set(pin); //GPIOA.0=1
    Delay_us(30);        //Pull up 20~40us
}

uint8_t IOT_DHT11_Check(mico_gpio_t pin)
{
    uint8_t retry = 0;
    IOT_DHT11_IO_IN(pin);                 //SET INPUT
    while (IOT_DHT11_DQ_IN(pin) && retry < 100) //DHT11 Pull down 40~80us
    {
        retry++;
        Delay_us(1);
    }

    if (retry >= 100)
        return 1;
    else
        retry = 0;

    while (!IOT_DHT11_DQ_IN(pin) && retry < 100) //DHT11 Pull up 40~80us
    {
        retry++;
        Delay_us(1);
    }

    if (retry >= 100)
        return 1; //chack error

    return 0;
}

uint8_t IOT_DHT11_Read_Bit(mico_gpio_t pin)
{
    uint8_t retry = 0;
    while (IOT_DHT11_DQ_IN(pin) && retry < 100) //wait become Low level
    {
        retry++;
        Delay_us(1);
    }

    retry = 0;
    while (!IOT_DHT11_DQ_IN(pin) && retry < 100) //wait become High level
    {
        retry++;
        Delay_us(1);
    }

    Delay_us(40); //wait 40us

    if (IOT_DHT11_DQ_IN(pin))
        return 1;
    else
        return 0;
}

uint8_t IOT_DHT11_Read_Byte(mico_gpio_t pin)
{
    uint8_t i, dat;
    dat = 0;
    for (i = 0; i < 8; i++)
    {
        dat <<= 1;
        dat |= IOT_DHT11_Read_Bit(pin);
    }

    return dat;
}

uint8_t IOT_DHT11_Read_Data(mico_gpio_t pin, uint8_t *temperature, uint8_t *humidity)
{
    uint8_t buf[5];
    uint8_t i;
    IOT_DHT11_Rst(pin);
    if (IOT_DHT11_Check(pin) == 0)
    {
        for (i = 0; i < 5; i++)
        {
            buf[i] = IOT_DHT11_Read_Byte(pin);
        }
        if ((buf[0] + buf[1] + buf[2] + buf[3]) == buf[4])
        {
            *humidity = buf[0];
            *temperature = buf[2];
        }
    }
    else
    {
        return 1;
    }

    return 0;
}

uint8_t IOT_DHT11_Init(mico_gpio_t pin)
{
    IOT_DHT11_IO_OUT(pin);

    IOT_DHT11_Rst(pin);
    return IOT_DHT11_Check(pin);
}