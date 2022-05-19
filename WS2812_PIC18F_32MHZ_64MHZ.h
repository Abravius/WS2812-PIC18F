/* 
 * File:   WS2812_PIC18F_32MHZ_64MHZ.h
 * Author: rionque
 * Description : this software can be include to your project when you want to
 * use the programmable LED WS2812 and WS2813.
 * Created on 17. mai 2022, 12:43
 */

// /!\ THIS DRIVER ONLY WORKS WITH A OSCILLATOR OF 32MHZ OR 64MHZ AND A PIC 18Fxxxxxx !  /!\


//////////////////////////////FIRST CONFIGURATION///////////////////////////////

//PIC_32MHZ     ->  Driver can be used with 32MHZ
//PIC_64MHZ     ->  Driver can be used with 64MHZ
#define PIC_64MHZ

//What PIN is used from the C port
#define SELECTED_PORT_OUTPUT    5

//How many leds on the strip
#define LED_QTY                 24

//Register for the selected port (Look in the technical Data of your PIC !)
#byte PORTX =                   0x3FCC
#byte LATX =                    0x3FBC
#byte TRISX =                   0x3FC4


///////////////////////////////////FUNCTIONS////////////////////////////////////

//Array that contain the colour(RGB) of each LED
static int LEDArray[3][LED_QTY];
//0 -> RED
//1 -> GREEN
//2 -> BLUE

//Initialisation of the selected PIN from the selected Port
void Init_NeoPixel()
{
#asm
    //Clear all the register for the C port
    BCF PORTX, SELECTED_PORT_OUTPUT;
    BCF LATX, SELECTED_PORT_OUTPUT;
    BCF TRISX, SELECTED_PORT_OUTPUT;
#endasm
}


//Function used set a colour for a specific LED
//<_Led> : Selected LED
//<_R> :    Red setting
//<_G> :    Green setting
//<_B> :    Blue setting
void SetPixelRGB(int _Led, int _R, int _G, int _B)
{
    LEDArray[0][_Led] = _R;
    LEDArray[1][_Led] = _G;
    LEDArray[2][_Led] = _B;
}

//function used set a colour and the brightness for a specific LED
//<_Led> :  Selected LED
//<_R> :    Red setting
//<_G> :    Green setting
//<_B> :    Blue setting
//<_Br> :   Set the brightness for the selected LED
void SetPixelRGBB(int _Led, int _R, int _G, int _B, int _Br)
{
    int Lum = (int)((float)_Br * 0.392156863);
    LEDArray[0][_Led] = ((_R / 100.0) * Lum);
    LEDArray[1][_Led] = ((_G / 100.0) * Lum);
    LEDArray[2][_Led] = ((_B / 100.0) * Lum);
}

//function used for set all the same colour for all the LED
//<_R> :    Red setting
//<_G> :    Green setting
//<_B> :    Blue setting
void FillStrip(int _R, int _G, int _B)
{
    for(int i = 0; i < LED_QTY; i++)
    {
        LEDArray[0][i] = _R;
        LEDArray[1][i] = _G;
        LEDArray[2][i] = _B;
    }
}

//function used for turn off all the LED
void ClearStrip()
{
    for(int i = 0; i < LED_QTY; i++)
    {
        LEDArray[0][i] = 0;
        LEDArray[1][i] = 0;
        LEDArray[2][i] = 0;
    }
}

//Function used for determinate the brightness for all the LED
//Always use before showStrip() and after all another instructions of WS2812/3 !
//<_Brightness> :   Brightness setting for all the LED
void GeneralBrightnessSet(int _Brightness)
{
    //Only when lower than 255. When it's 255, it's 100% this is why this instruction
    if(_Brightness < 255)
    {
        float GenerealBrightness = ((float)_Brightness * 0.003921569);
    
        for(int i = 0; i < LED_QTY; i++)
        {
            LEDArray[1][i] = LEDArray[1][i] * GenerealBrightness;
            LEDArray[0][i] = LEDArray[0][i] * GenerealBrightness;
            LEDArray[2][i] = LEDArray[2][i] * GenerealBrightness;
        }
    }
}

//Function used for set colour and the brightness for some selected LED
//<_LedMin> :   Start LED (need to be lower than _LedMax)
//<_LedMax> :   Stop LED (Need to be lower than the quantity of LED)
//<_R> :        Red setting
//<_G> :        Green setting
//<_B> :        Blue setting
//<_Br> :       Brightness setting for all the LED
void SetMultiplePixelRGBB(int _LedMin, int _LedMax, int _R, int _G, int _B, int _Br)
{
    float LUM = ((float)_Br * 0.003921569);
    for(int i = _LedMin; i <= _LedMax; i++)
    {
        LEDArray[0][i] = _R * LUM;
        LEDArray[1][i] = _G * LUM;
        LEDArray[2][i] = _B * LUM;
    }
}

//Function used for set colour for some selected LED
//<_LedMin> :   Start LED (need to be lower than _LedMax)
//<_LedMax> :   Stop LED (Need to be lower than the quantity of LED)
//<_R> :        Red setting
//<_G> :        Green setting
//<_B> :        Blue setting
//<_Br> :       Brightness setting for all the LED
void SetMultiplePixelRGB(int _LedMin, int _LedMax, int _R, int _G, int _B)
{
    for(int i = _LedMin; i <= _LedMax; i++)
    {
        LEDArray[0][i] = _R;
        LEDArray[1][i] = _G;
        LEDArray[2][i] = _B;
    }
}

//Function used to turn on LED with the specific color defined earlier
#inline
void showStrip()
{
    int LP0 = 0, LP1 = 0, LP2 = 0;
    for(int i = 0; i < LED_QTY; i++)
    {
        int _G = LEDArray[1][i];
        int _R = LEDArray[0][i];
        int _B = LEDArray[2][i];

//IN ASSEMBLER FOR SPEED
#asm
#ifdef PIC_32MHZ
///////////////////////////////////////////////////////FOR THE GREEN
        SUB0: 
        BTFSC LP0, 3;
        GOTO SUB1;
        INCFSZ LP0, 1;

        BSF PORTX,SELECTED_PORT_OUTPUT;
        NOP;
        BTFSS _G, 7;
        BCF PORTX,SELECTED_PORT_OUTPUT;
        BCF PORTX,SELECTED_PORT_OUTPUT;
        RLCF _G;
        GOTO SUB0;
    
///////////////////////////////////////////////////////FOR THE RED
        SUB1:
        BTFSC LP1, 3;
        GOTO SUB2;
        INCFSZ LP1, 1;

        BSF PORTX,SELECTED_PORT_OUTPUT;
        NOP;
        BTFSS _R, 7;
        BCF PORTX,SELECTED_PORT_OUTPUT;
        BCF PORTX,SELECTED_PORT_OUTPUT;
        RLCF _R;
        GOTO SUB1;

////////////////////////////////////////////////////////FOR THE BLUE
        SUB2:
        BTFSC LP2, 3;
        GOTO SUB3;
        INCFSZ LP2, 1;

        BSF PORTX,SELECTED_PORT_OUTPUT;
        NOP;
        BTFSS _B, 7;
        BCF PORTX,SELECTED_PORT_OUTPUT;
        BCF PORTX,SELECTED_PORT_OUTPUT;
        RLCF _B;
        GOTO SUB2;
    
//////////////////////////////END OF THE SEND CYCLE/////////////////////////////
        SUB3:
        CLRF LP0;
        CLRF LP1;
        CLRF LP2;
#endif
#ifdef PIC_64MHZ
///////////////////////////////////////////////////////CONCERNE LA COULEUR VERTE
        SUB0: 
        BTFSC LP0, 3;
        GOTO SUB1;
        INCFSZ LP0, 1;

        BSF PORTX,SELECTED_PORT_OUTPUT;
        NOP;
        NOP;
        NOP;
        NOP;
        BTFSS _G, 7; //Condition permettant de d�terminer quand la PIN doit �tre FALSE
        BCF PORTX,SELECTED_PORT_OUTPUT;
        NOP;
        NOP;
        NOP;
        BCF PORTX,SELECTED_PORT_OUTPUT;
        RLCF _G;
        GOTO SUB0;
    
///////////////////////////////////////////////////////CONCERNE LA COULEUR ROUGE
        SUB1:
        BTFSC LP1, 3;
        GOTO SUB2;
        INCFSZ LP1, 1;

        BSF PORTX,SELECTED_PORT_OUTPUT;
        NOP;
        NOP;
        NOP;
        NOP;
        BTFSS _R, 7; //Condition permettant de d�terminer quand la PIN doit �tre FALSE
        BCF PORTX,SELECTED_PORT_OUTPUT;
        NOP;
        NOP;
        NOP;
        BCF PORTX,SELECTED_PORT_OUTPUT;
        RLCF _R;
        GOTO SUB1;

////////////////////////////////////////////////////////CONCERNE LA COULEUR BLEU
        SUB2:
        BTFSC LP2, 3;
        GOTO SUB3;
        INCFSZ LP2, 1;

        BSF PORTX,SELECTED_PORT_OUTPUT;
        NOP;
        NOP;
        NOP;
        NOP;
        BTFSS _B, 7; //Condition permettant de d�terminer quand la PIN doit �tre FALSE
        BCF PORTX,SELECTED_PORT_OUTPUT;
        NOP;
        NOP;
        NOP;
        BCF PORTX,SELECTED_PORT_OUTPUT;
        RLCF _B;
        GOTO SUB2;
    
//////////////////////////////FIN DU CYCLE D'ENVOI//////////////////////////////
        SUB3:
        CLRF LP0;
        CLRF LP1;
        CLRF LP2;
#endif
#endasm
    }
    
    //Waiting >=50uS because it's the end of the instruction, view the datasheet
    //of the LED for more informations
    delay_us(500);
}

/*

//   /!\ THIS DRIVER WORKS ONLY WITH A 64MHZ PIC FREQUENCY AND A PIC 18F /!\

//What PIN is used from the C port
#define SELECTED_PORTC_OUTPUT 5

//How many leds on the strip
#define LED_QTY 24

#byte PORTC =   0xF82
#byte LATC =    0xF8B
#byte TRISC =   0xF94


//Array that contain the colour of each LED
static int LEDArray[3][LED_QTY];

//Initialisation of the selected PIN from the C port
void Init_NeoPixel()
{
#asm
    //Clear all the register for the C port
    BCF PORTC, SELECTED_PORTC_OUTPUT;
    BCF LATC, SELECTED_PORTC_OUTPUT;
    BCF TRISC, SELECTED_PORTC_OUTPUT;
#endasm
}


//Function used set a colour for a specific LED
//<_Led> : Selected LED
//<_R> :    Red setting
//<_G> :    Green setting
//<_B> :    Blue setting
void SetPixelRGB(int _Led, int _R, int _G, int _B)
{
    LEDArray[0][_Led] = _R;
    LEDArray[1][_Led] = _G;
    LEDArray[2][_Led] = _B;
}

//function used set a colour and the brightness for a specific LED
//<_Led> :  Selected LED
//<_R> :    Red setting
//<_G> :    Green setting
//<_B> :    Blue setting
//<_Br> :   Set the brightness for the selected LED
void SetPixelRGBB(int _Led, int _R, int _G, int _B, int _Br)
{
    int Lum = _Br * 0.392156863;
    LEDArray[0][_Led] = (_R / 100) * Lum;
    LEDArray[1][_Led] = (_G / 100) * Lum;
    LEDArray[2][_Led] = (_B / 100) * Lum;
}

//function used for set all the same colour for all the LED
//<_R> :    Red setting
//<_G> :    Green setting
//<_B> :    Blue setting
void FillStrip(int _R, int _G, int _B)
{
    for(int i = 0; i < LED_QTY; i++)
    {
        LEDArray[0][i] = _R;
        LEDArray[1][i] = _G;
        LEDArray[2][i] = _B;
    }
}

//function used for turn off all the LED
void ClearStrip()
{
    for(int i = 0; i < LED_QTY; i++)
    {
        LEDArray[0][i] = 0;
        LEDArray[1][i] = 0;
        LEDArray[2][i] = 0;
    }
}

//Function used for determinate the brightness for all the LED
//<_Brightness> :   Brightness setting for all the LED
void BrightnessSet(int _Brightness)
{
    int LUM = _Brightness * 0.392156863;
    for(int i = 0; i < LED_QTY; i++)
    {
        LEDArray[0][i] = (LEDArray[i] / 100) * LUM;
        LEDArray[1][i] = (LEDArray[i] / 100) * LUM;
        LEDArray[2][i] = (LEDArray[i] / 100) * LUM;
    }
}

//Function used for set colour for some selected LED
//<_LedMin> :   Start LED
//<_LedMax> :   Stop LED
//<_R> :        Red setting
//<_G> :        Green setting
//<_B> :        Blue setting
//<_Br> :       Brightness setting for all the LED
void SetManyLED(int _LedMin, int _LedMax, int _R, int _G, int _B, int _Br)
{
    //THERE IS A PROBLEM, NOT THE NEST METHOD
    int LUM = _Br * 0.392156863;
    if(_LedMax == 0)
        for(int i = _LedMin; i < LED_QTY - _LedMax; i++)
        {
            LEDArray[0][i] = (_R / 100) * LUM;
            LEDArray[1][i] = (_G / 100) * LUM;
            LEDArray[2][i] = (_B / 100) * LUM;
        }
    else
        for(int i = _LedMin; i < _LedMax - _LedMax; i++)
        {
            LEDArray[0][i] = (_R / 100) * LUM;
            LEDArray[1][i] = (_G / 100) * LUM;
            LEDArray[2][i] = (_B / 100) * LUM;
        }
}

//Function used to turn on LED with the specific color defined earlier
#inline
void showStrip()
{
    int LP0 = 0, LP1 = 0, LP2 = 0;
    
    for(int i = 0; i < LED_QTY; i++)
    {
        int _G = LEDArray[1][i];
        int _R = LEDArray[0][i];
        int _B = LEDArray[2][i];

//PARTIE EN ASSEMBLEUR POUR GARANTIR LA VITESSE DE TRAITEMENT EN NANOSECONDE
#asm
///////////////////////////////////////////////////////CONCERNE LA COULEUR VERTE
        SUB0: 
        BTFSC LP0, 3;
        GOTO SUB1;
        INCFSZ LP0, 1;

        BSF PORTC,SELECTED_PORTC_OUTPUT;
        NOP;
        NOP;
        NOP;
        NOP;
        BTFSS _G, 7; //Condition permettant de d�terminer quand la PIN doit �tre FALSE
        BCF PORTC,SELECTED_PORTC_OUTPUT;
        NOP;
        NOP;
        NOP;
        BCF PORTC,SELECTED_PORTC_OUTPUT;
        RLCF _G;
        GOTO SUB0;
    
///////////////////////////////////////////////////////CONCERNE LA COULEUR ROUGE
        SUB1:
        BTFSC LP1, 3;
        GOTO SUB2;
        INCFSZ LP1, 1;

        BSF PORTC,SELECTED_PORTC_OUTPUT;
        NOP;
        NOP;
        NOP;
        NOP;
        BTFSS _R, 7; //Condition permettant de d�terminer quand la PIN doit �tre FALSE
        BCF PORTC,SELECTED_PORTC_OUTPUT;
        NOP;
        NOP;
        NOP;
        BCF PORTC,SELECTED_PORTC_OUTPUT;
        RLCF _R;
        GOTO SUB1;

////////////////////////////////////////////////////////CONCERNE LA COULEUR BLEU
        SUB2:
        BTFSC LP2, 3;
        GOTO SUB3;
        INCFSZ LP2, 1;

        BSF PORTC,SELECTED_PORTC_OUTPUT;
        NOP;
        NOP;
        NOP;
        NOP;
        BTFSS _B, 7; //Condition permettant de d�terminer quand la PIN doit �tre FALSE
        BCF PORTC,SELECTED_PORTC_OUTPUT;
        NOP;
        NOP;
        NOP;
        BCF PORTC,SELECTED_PORTC_OUTPUT;
        RLCF _B;
        GOTO SUB2;
    
//////////////////////////////FIN DU CYCLE D'ENVOI//////////////////////////////
        SUB3:
        CLRF LP0;
        CLRF LP1;
        CLRF LP2;
#endasm
    }
}
*/