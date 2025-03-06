#include "alphadisp.h"
#include <ShiftRegister74HC595.h>

#define CHAROFFSET ' '



#define SIG_A   (~0x01)<<1
#define SIG_B   (~0x02)<<1
#define SIG_C   (~0x04)<<1
#define SIG_D   (~0x08)<<1
#define SIG_E   (~0x10)<<1
#define SIG_F   (~0x20)<<1
#define SIG_G1  (~0x40)<<1

#define SIG_G2  (~0x01)<<1
#define SIG_H   (~0x02)<<1
#define SIG_J   (~0x04)<<1
#define SIG_K   (~0x08)<<1
#define SIG_L   (~0x10)<<1
#define SIG_M   (~0x20)<<1
#define SIG_N   (~0x40)<<1

#define EMPTY   (~0x00)<<1

SemaphoreHandle_t mtx_datalock;
static const uint16_t alphanumeric_segs[96][2]{
	// nmlkjihgfedcba
	{EMPTY,EMPTY}, // ' ' (space)
	{EMPTY,EMPTY}, // '!'
	{EMPTY,EMPTY}, // '"'
 	{EMPTY,EMPTY}, // '#'
	{EMPTY,EMPTY}, // '$'
	{EMPTY,EMPTY}, // '%'
	{EMPTY,EMPTY}, // '&'
	{EMPTY,SIG_J}, // '''
	{EMPTY,EMPTY}, // '('
	{EMPTY,EMPTY}, // ')'
	{EMPTY,EMPTY}, // '*'
	{SIG_G1,SIG_G2 & SIG_J & SIG_M}, // '+'
	{EMPTY,EMPTY}, // ','
	{SIG_G1,SIG_G2}, // '-'
	{EMPTY,EMPTY}, // '.'
	{EMPTY,SIG_K & SIG_N}, // '/'
	{SIG_A & SIG_B & SIG_C & SIG_D & SIG_E & SIG_F,EMPTY}, // '0'
	{EMPTY,SIG_J & SIG_M}, // '1'
	{SIG_A & SIG_B & SIG_E & SIG_D & SIG_G1,SIG_G2}, // '2'
	{SIG_A & SIG_B & SIG_C & SIG_D,SIG_G2}, // '3'
	{SIG_B & SIG_C & SIG_F & SIG_G1,SIG_G2}, // '4'
	{SIG_A & SIG_C & SIG_D & SIG_F & SIG_G1,SIG_G2}, // '5'
	{SIG_A & SIG_C & SIG_D & SIG_E & SIG_F & SIG_G1,SIG_G2}, // '6'
	{SIG_A,SIG_K & SIG_M}, // '7'
	{SIG_A & SIG_B & SIG_C & SIG_D & SIG_E & SIG_F & SIG_G1,SIG_G2}, // '8'
	{SIG_A & SIG_B & SIG_C & SIG_D & SIG_F & SIG_G1,SIG_G2}, // '9'
	{EMPTY,EMPTY}, // ':'
	{EMPTY,EMPTY}, // ';'
	{EMPTY,SIG_K & SIG_L}, // '<'
	{EMPTY,EMPTY}, // '='
	{EMPTY,SIG_H & SIG_N}, // '>'
    {EMPTY,EMPTY}, // '?'
	{EMPTY,EMPTY}, // '@'
	{SIG_A & SIG_B & SIG_C & SIG_E & SIG_F & SIG_G1,SIG_G2}, // 'A'
	{SIG_E & SIG_F & SIG_G1,SIG_H & SIG_N}, // 'B'
	{SIG_D & SIG_E & SIG_F & SIG_A,EMPTY}, // 'C'
	{SIG_E & SIG_F,SIG_H & SIG_N}, // 'D'
	{SIG_A & SIG_D & SIG_E & SIG_F & SIG_G1,SIG_G2}, // 'E'
	{SIG_A & SIG_E & SIG_F & SIG_G1,SIG_G2}, // 'F'
	{SIG_A & SIG_D & SIG_E & SIG_F &  SIG_C,SIG_G2}, // 'G'
	{SIG_B & SIG_C & SIG_E & SIG_F & SIG_G1,SIG_G2}, // 'H'
	{SIG_A & SIG_D,SIG_J & SIG_M}, // 'I'
	{SIG_A & SIG_B & SIG_C & SIG_D & SIG_E,EMPTY}, // 'J'
	{SIG_E & SIG_F & SIG_G1,SIG_K & SIG_L}, // 'K'
	{SIG_D & SIG_E & SIG_F,EMPTY}, // 'L'
	{SIG_B & SIG_C & SIG_E & SIG_F,SIG_H & SIG_K}, // 'M'
	{SIG_B & SIG_C & SIG_E & SIG_F,SIG_H & SIG_L}, // 'N'
	{SIG_A & SIG_B & SIG_C & SIG_D & SIG_E & SIG_F,EMPTY}, // 'O'
	{SIG_A & SIG_B & SIG_E & SIG_F & SIG_G1,SIG_G2}, // 'P'
	{SIG_A & SIG_B & SIG_C & SIG_D & SIG_E & SIG_F,SIG_L}, // 'Q'
	{SIG_A & SIG_B & SIG_E & SIG_F & SIG_G1,SIG_G2}, // 'R'
	{SIG_A & SIG_C & SIG_D & SIG_F & SIG_G1,SIG_G2}, // 'S'
	{SIG_A,SIG_J & SIG_M}, // 'T'
	{SIG_B & SIG_C & SIG_D & SIG_E & SIG_F,EMPTY}, // 'U'
	{SIG_B & SIG_C,SIG_H & SIG_L}, // 'V'
	{SIG_B & SIG_C & SIG_E & SIG_F,SIG_L & SIG_N}, // 'W'
	{EMPTY,SIG_H & SIG_K & SIG_L & SIG_N}, // 'X'
	{EMPTY,SIG_H & SIG_K & SIG_M}, // 'Y'
	{SIG_A & SIG_D & SIG_G1,SIG_G2 & SIG_K & SIG_N}, // 'Z'
	{EMPTY,EMPTY}, // '['
	{EMPTY,SIG_H & SIG_L}, // '\'
	{EMPTY,EMPTY}, // ']'
    {EMPTY,EMPTY}, // '^'
	{SIG_D,EMPTY}, // '_'
	{EMPTY,EMPTY}, // '`'
	{SIG_A & SIG_B & SIG_C & SIG_E & SIG_F & SIG_G1,SIG_G2}, // 'a'
	{SIG_E & SIG_F & SIG_G1,SIG_H & SIG_N}, // 'b'
	{SIG_D & SIG_E & SIG_F & SIG_A,EMPTY}, // 'c'
	{SIG_E & SIG_F,SIG_H & SIG_N}, // 'd'
	{SIG_A & SIG_D & SIG_E & SIG_F & SIG_G1,SIG_G2}, // 'e'
	{SIG_A & SIG_E & SIG_F & SIG_G1,SIG_G2}, // 'f'
	{SIG_A & SIG_D & SIG_E & SIG_F &  SIG_C,SIG_G2}, // 'g'
	{SIG_B & SIG_C & SIG_E & SIG_F & SIG_G1,SIG_G2}, // 'h'
	{SIG_A & SIG_D,SIG_J & SIG_M}, // 'i'
	{SIG_A & SIG_B & SIG_C & SIG_D & SIG_E,EMPTY}, // 'j'
	{SIG_E & SIG_F & SIG_G1,SIG_K & SIG_L}, // 'k'
	{SIG_D & SIG_E & SIG_F,EMPTY}, // 'l'
	{SIG_B & SIG_C & SIG_E & SIG_F,SIG_H & SIG_K}, // 'm'
	{SIG_B & SIG_C & SIG_E & SIG_F,SIG_H & SIG_L}, // n'
	{SIG_A & SIG_B & SIG_C & SIG_D & SIG_E & SIG_F,EMPTY}, // 'o'
	{SIG_A & SIG_B & SIG_E & SIG_F & SIG_G1,SIG_G2}, // 'p'
	{SIG_A & SIG_B & SIG_C & SIG_D & SIG_E & SIG_F,SIG_L}, // 'q'
	{SIG_A & SIG_B & SIG_E & SIG_F & SIG_G1,SIG_G2}, // 'r'
	{SIG_A & SIG_C & SIG_D & SIG_F & SIG_G1,SIG_G2}, // 's'
	{SIG_A,SIG_J & SIG_M}, // 't'
	{SIG_B & SIG_C & SIG_D & SIG_E & SIG_F,EMPTY}, // 'u'
	{SIG_B & SIG_C,SIG_H & SIG_L}, // 'v'
	{SIG_B & SIG_C & SIG_E & SIG_F,SIG_L & SIG_N}, // 'w'
	{EMPTY,SIG_H & SIG_K & SIG_L & SIG_N}, // 'x'
	{EMPTY,SIG_H & SIG_K & SIG_M}, // 'y'
	{SIG_A & SIG_D & SIG_G1,SIG_G2 & SIG_K & SIG_N}, // 'z'
	{EMPTY,EMPTY}, // '{'
	{EMPTY,SIG_J & SIG_M}, // '|'
	{EMPTY,EMPTY}, // '}'
	{EMPTY,EMPTY}, // '~'
	{EMPTY,EMPTY}, // Unknown character (DEL or RUBOUT)
};

ShiftRegister74HC595<2> sr(41, 40, 18);

char AlphaDispDriver::getData(uint8_t pos) {
    return data[pos];
}
bool AlphaDispDriver::getColon() {
    return colon;
}
bool AlphaDispDriver::getDot() {
    return dot;
}

void updateAlphaDispTask(void* param) {
    AlphaDispDriver* instance = static_cast<AlphaDispDriver*>(param);
    uint16_t test = 0b00010000000110; // '1' for Testing
    char x = instance->getData(3);
    uint8_t step = 0;
    char value = ' ';
    sr.setAllHigh();
    uint8_t srdata[] = {0xFD, 0xFE};
    for(;;) {       
        // printf("Test");
        srdata[0] = 0xFF;
        srdata[1] = 0x01; //BColon On
        
        switch(step) {
            case 0:            
                value = instance->getData(0);
                // printf("Pos0: %c\n", value);
                srdata[0] = ~(0x01<<0);
                srdata[1] = 0x01 | (alphanumeric_segs[value-CHAROFFSET][0]);
                step = 1;
                break;
            case 1:
                srdata[0] = ~(0x01<<1);
                srdata[1] = 0x01 | (alphanumeric_segs[value-CHAROFFSET][1]);
                step = 2;
                break;
            case 2:                
                value = instance->getData(1);
                // printf("Pos1: %c\n", value);
                srdata[0] = ~(0x01<<2);
                srdata[1] = 0x01 | (alphanumeric_segs[value-CHAROFFSET][0]);
                step = 3;
                break;
            case 3:
                step = 4;
                srdata[0] = ~(0x01<<3);
                srdata[1] = 0x01 | (alphanumeric_segs[value-CHAROFFSET][1]);
                break;
            case 4:
                value = instance->getData(2);
                // printf("Pos2: %c\n", value);
                srdata[0] = ~(0x01<<4);
                srdata[1] = 0x01 | (alphanumeric_segs[value-CHAROFFSET][0]);
                step = 5;
                break;
            case 5:
                srdata[0] = ~(0x01<<5);
                srdata[1] = 0x01 | (alphanumeric_segs[value-CHAROFFSET][1]);
                step = 6;
                break;
            case 6:
                value = instance->getData(3);
                // printf("Pos3: %c\n", value);
                srdata[0] = ~(0x01<<6);
                srdata[1] = 0x01 | (alphanumeric_segs[value-CHAROFFSET][0]);
                step = 7;
                break;
            case 7:
                srdata[0] = ~(0x01<<7);
                srdata[1] = 0x01 | (alphanumeric_segs[value-CHAROFFSET][1]);
                step = 8;
                break;
            case 8:
                value = instance->getColon();
                value <<= 1;
                value |= instance->getDot();
                srdata[0] = ~(0x00);
                srdata[1] = 0x00 | ((~value)<<1);
                step = 0;
                break;
        }
        sr.setAll(srdata);
        vTaskDelay(1/portTICK_PERIOD_MS);
    }
}

void AlphaDispDriver::begin(void) {
    sr.setAllHigh();
    printf("Init Alphadisp");
    mtx_datalock = xSemaphoreCreateMutex();
    xTaskCreate(updateAlphaDispTask, "alphaDispTask", 2*2048, this, 3, NULL);
}

AlphaDispDriver::AlphaDispDriver() {
    data[0] = CHAROFFSET;
    data[1] = CHAROFFSET;
    data[2] = CHAROFFSET;
    data[3] = CHAROFFSET;
    colon = false;
    dot = false;
}

void AlphaDispDriver::writeChar(uint8_t pos, char c) {
    xSemaphoreTake(mtx_datalock, portMAX_DELAY);
    data[pos] = c;
    xSemaphoreGive(mtx_datalock);
}
