# WS2812_PIC18F_32MHZ_64MHZ
Driver allowing to use WS2812 and WS2813 LED with some 18F PIC (CCS C)

This driver allowing you to use the LED WS2812 and WS2813 with some 18Fxxxx PIC !

But, this driver is only usable with a 32MHz or 64MHz oscillator !

Before using it, you need to place in the first "#define" one of this two value :
-> "PIC_64MHZ" :  You're PIC works with a frequency of 64MHz.
-> "PIC_32MHZ" :  You're PIC works with a frequency of 32MHz.

Then, you need to find the right registers for your selected port (TRISx, PORTx, LATx). After you've find it, place their value into "PORTX", "LATX" and "TRISX" located at the beginning of the driver's code.

After that, place your PIN number in "SELECTED_PORT_OUTPUT" (normally from 0 to 7)

Now, all you have to do is to use the mutiple functions of the driver.

Enjoy :)



PS : For now, it is possible to operate only one strip.
