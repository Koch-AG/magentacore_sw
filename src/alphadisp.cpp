#include "alphadisp.h"
#include <ShiftRegister74HC595.h>

#define CHAROFFSET ' '



#define SIG_A   ((uint16_t)((~0x01)<<1))
#define SIG_B   ((uint16_t)((~0x02)<<1))
#define SIG_C   ((uint16_t)((~0x04)<<1))
#define SIG_D   ((uint16_t)((~0x08)<<1))
#define SIG_E   ((uint16_t)((~0x10)<<1))
#define SIG_F   ((uint16_t)((~0x20)<<1))
#define SIG_G1  ((uint16_t)((~0x40)<<1))

#define SIG_G2  ((uint16_t)((~0x01)<<1))
#define SIG_H   ((uint16_t)((~0x02)<<1))
#define SIG_J   ((uint16_t)((~0x04)<<1))
#define SIG_K   ((uint16_t)((~0x08)<<1))
#define SIG_L   ((uint16_t)((~0x10)<<1))
#define SIG_M   ((uint16_t)((~0x20)<<1))
#define SIG_N   ((uint16_t)((~0x40)<<1))

#define EMPTY   ((uint16_t)(((~0x00)<<1)))

SemaphoreHandle_t mtx_datalock;
static const uint16_t alphanumeric_segs[96][2]{
	// nmlkjihgfedcba
	{(uint16_t)EMPTY,(uint16_t)EMPTY}, // ' ' (space)
	{(uint16_t)(SIG_A & SIG_B & SIG_C & SIG_D & SIG_E & SIG_F & SIG_G1),(uint16_t)(SIG_G2 & SIG_H & SIG_J & SIG_K & SIG_L & SIG_M & SIG_N)}, // '!'
	{(uint16_t)EMPTY,(uint16_t)EMPTY}, // '"'
 	{(uint16_t)EMPTY,(uint16_t)EMPTY}, // '#'
	{(uint16_t)EMPTY,(uint16_t)EMPTY}, // '$'
	{(uint16_t)EMPTY,(uint16_t)EMPTY}, // '%'
	{(uint16_t)EMPTY,(uint16_t)EMPTY}, // '&'
	{(uint16_t)EMPTY,(uint16_t)SIG_J}, // '''
	{(uint16_t)EMPTY,(uint16_t)EMPTY}, // '('
	{(uint16_t)EMPTY,(uint16_t)EMPTY}, // ')'
	{(uint16_t)EMPTY,(uint16_t)EMPTY}, // '*'
	{(uint16_t)SIG_G1,(uint16_t)(SIG_G2 & SIG_J & SIG_M)}, // '+'
	{(uint16_t)EMPTY,(uint16_t)EMPTY}, // ','
	{(uint16_t)SIG_G1,(uint16_t)SIG_G2}, // '-'
	{(uint16_t)EMPTY,(uint16_t)EMPTY}, // '.'
	{(uint16_t)EMPTY,(uint16_t)(SIG_K & SIG_N)}, // '/'
	{(uint16_t)(SIG_A & SIG_B & SIG_C & SIG_D & SIG_E & SIG_F),(uint16_t)EMPTY}, // '0'
	{(uint16_t)EMPTY,(uint16_t)(SIG_J & SIG_M)}, // '1'
	{(uint16_t)(SIG_A & SIG_B & SIG_E & SIG_D & SIG_G1),(uint16_t)SIG_G2}, // '2'
	{(uint16_t)(SIG_A & SIG_B & SIG_C & SIG_D),(uint16_t)SIG_G2}, // '3'
	{(uint16_t)(SIG_B & SIG_C & SIG_F & SIG_G1),(uint16_t)SIG_G2}, // '4'
	{(uint16_t)(SIG_A & SIG_C & SIG_D & SIG_F & SIG_G1),(uint16_t)SIG_G2}, // '5'
	{(uint16_t)(SIG_A & SIG_C & SIG_D & SIG_E & SIG_F & SIG_G1),(uint16_t)SIG_G2}, // '6'
	{(uint16_t)SIG_A,(uint16_t)(SIG_K & SIG_M)}, // '7'
	{(uint16_t)(SIG_A & SIG_B & SIG_C & SIG_D & SIG_E & SIG_F & SIG_G1),(uint16_t)SIG_G2}, // '8'
	{(uint16_t)(SIG_A & SIG_B & SIG_C & SIG_D & SIG_F & SIG_G1),(uint16_t)SIG_G2}, // '9'
	{(uint16_t)EMPTY,(uint16_t)EMPTY}, // ':'
	{(uint16_t)EMPTY,(uint16_t)EMPTY}, // ';'
	{(uint16_t)EMPTY,(uint16_t)(SIG_K & SIG_L)}, // '<'
	{(uint16_t)EMPTY,(uint16_t)EMPTY}, // '='
	{(uint16_t)EMPTY,(uint16_t)(SIG_H & SIG_N)}, // '>'
    {(uint16_t)EMPTY,(uint16_t)EMPTY}, // '?'
	{(uint16_t)EMPTY,(uint16_t)EMPTY}, // '@'
	{(uint16_t)(SIG_A & SIG_B & SIG_C & SIG_E & SIG_F & SIG_G1),(uint16_t)SIG_G2}, // 'A'
	{(uint16_t)(SIG_A & SIG_D & SIG_E & SIG_F & SIG_G1),(uint16_t)(SIG_K & SIG_L)}, // 'B'
	{(uint16_t)(SIG_D & SIG_E & SIG_F & SIG_A),(uint16_t)EMPTY}, // 'C'
	{(uint16_t)(SIG_E & SIG_F),(uint16_t)(SIG_H & SIG_N)}, // 'D'
	{(uint16_t)(SIG_A & SIG_D & SIG_E & SIG_F & SIG_G1),(uint16_t)SIG_G2}, // 'E'
	{(uint16_t)(SIG_A & SIG_E & SIG_F & SIG_G1),(uint16_t)SIG_G2}, // 'F'
	{(uint16_t)(SIG_A & SIG_D & SIG_E & SIG_F & SIG_C),(uint16_t)SIG_G2}, // 'G'
	{(uint16_t)(SIG_B & SIG_C & SIG_E & SIG_F & SIG_G1),(uint16_t)SIG_G2}, // 'H'
	{(uint16_t)(SIG_A & SIG_D),(uint16_t)(SIG_J & SIG_M)}, // 'I'
	{(uint16_t)(SIG_A & SIG_B & SIG_C & SIG_D & SIG_E),(uint16_t)EMPTY}, // 'J'
	{(uint16_t)(SIG_E & SIG_F & SIG_G1),(uint16_t)(SIG_K & SIG_L)}, // 'K'
	{(uint16_t)(SIG_D & SIG_E & SIG_F),(uint16_t)EMPTY}, // 'L'
	{(uint16_t)(SIG_B & SIG_C & SIG_E & SIG_F),(uint16_t)(SIG_H & SIG_K)}, // 'M'
	{(uint16_t)(SIG_B & SIG_C & SIG_E & SIG_F),(uint16_t)(SIG_H & SIG_L)}, // 'N'
	{(uint16_t)(SIG_A & SIG_B & SIG_C & SIG_D & SIG_E & SIG_F),(uint16_t)EMPTY}, // 'O'
	{(uint16_t)(SIG_A & SIG_B & SIG_E & SIG_F & SIG_G1),(uint16_t)SIG_G2}, // 'P'
	{(uint16_t)(SIG_A & SIG_B & SIG_C & SIG_D & SIG_E & SIG_F),(uint16_t)SIG_L}, // 'Q'
	{(uint16_t)(SIG_A & SIG_B & SIG_E & SIG_F & SIG_G1),(uint16_t)(SIG_G2 & SIG_L)}, // 'R'
	{(uint16_t)(SIG_A & SIG_C & SIG_D & SIG_F & SIG_G1),(uint16_t)SIG_G2}, // 'S'
	{(uint16_t)SIG_A,(uint16_t)(SIG_J & SIG_M)}, // 'T'
	{(uint16_t)(SIG_B & SIG_C & SIG_D & SIG_E & SIG_F),(uint16_t)EMPTY}, // 'U'
	{(uint16_t)(SIG_B & SIG_C),(uint16_t)(SIG_H & SIG_L)}, // 'V'
	{(uint16_t)(SIG_B & SIG_C & SIG_E & SIG_F),(uint16_t)(SIG_L & SIG_N)}, // 'W'
	{(uint16_t)EMPTY,(uint16_t)(SIG_H & SIG_K & SIG_L & SIG_N)}, // 'X'
	{(uint16_t)EMPTY,(uint16_t)(SIG_H & SIG_K & SIG_M)}, // 'Y'
	{(uint16_t)(SIG_A & SIG_D & SIG_G1),(uint16_t)(SIG_G2 & SIG_K & SIG_N)}, // 'Z'
	{(uint16_t)EMPTY,(uint16_t)EMPTY}, // '['
	{(uint16_t)EMPTY,(uint16_t)(SIG_H & SIG_L)}, // '\'
	{(uint16_t)EMPTY,(uint16_t)EMPTY}, // ']'
    {(uint16_t)EMPTY,(uint16_t)EMPTY}, // '^'
	{(uint16_t)SIG_D,(uint16_t)EMPTY}, // '_'
	{(uint16_t)EMPTY,(uint16_t)EMPTY}, // '`'
	{(uint16_t)(SIG_A & SIG_B & SIG_C & SIG_E & SIG_F & SIG_G1),(uint16_t)SIG_G2}, // 'a'
	{(uint16_t)(SIG_A & SIG_D & SIG_E & SIG_F & SIG_G1),(uint16_t)(SIG_K & SIG_L)}, // 'b'
	{(uint16_t)(SIG_D & SIG_E & SIG_F & SIG_A),(uint16_t)EMPTY}, // 'c'
	{(uint16_t)(SIG_E & SIG_F),(uint16_t)(SIG_H & SIG_N)}, // 'd'
	{(uint16_t)(SIG_A & SIG_D & SIG_E & SIG_F & SIG_G1),(uint16_t)SIG_G2}, // 'e'
	{(uint16_t)(SIG_A & SIG_E & SIG_F & SIG_G1),(uint16_t)SIG_G2}, // 'f'
	{(uint16_t)(SIG_A & SIG_D & SIG_E & SIG_F & SIG_C),(uint16_t)SIG_G2}, // 'g'
	{(uint16_t)(SIG_B & SIG_C & SIG_E & SIG_F & SIG_G1),(uint16_t)SIG_G2}, // 'h'
	{(uint16_t)(SIG_A & SIG_D),(uint16_t)(SIG_J & SIG_M)}, // 'i'
	{(uint16_t)(SIG_A & SIG_B & SIG_C & SIG_D & SIG_E),(uint16_t)EMPTY}, // 'j'
	{(uint16_t)(SIG_E & SIG_F & SIG_G1),(uint16_t)(SIG_K & SIG_L)}, // 'k'
	{(uint16_t)(SIG_D & SIG_E & SIG_F),(uint16_t)EMPTY}, // 'l'
	{(uint16_t)(SIG_B & SIG_C & SIG_E & SIG_F),(uint16_t)(SIG_H & SIG_K)}, // 'm'
	{(uint16_t)(SIG_B & SIG_C & SIG_E & SIG_F),(uint16_t)(SIG_H & SIG_L)}, // n'
	{(uint16_t)(SIG_A & SIG_B & SIG_C & SIG_D & SIG_E & SIG_F),(uint16_t)EMPTY}, // 'o'
	{(uint16_t)(SIG_A & SIG_B & SIG_E & SIG_F & SIG_G1),(uint16_t)SIG_G2}, // 'p'
	{(uint16_t)(SIG_A & SIG_B & SIG_C & SIG_D & SIG_E & SIG_F),(uint16_t)SIG_L}, // 'q'
	{(uint16_t)(SIG_A & SIG_B & SIG_E & SIG_F & SIG_G1),(uint16_t)(SIG_G2 & SIG_L)}, // 'r'
	{(uint16_t)(SIG_A & SIG_C & SIG_D & SIG_F & SIG_G1),(uint16_t)SIG_G2}, // 's'
	{(uint16_t)SIG_A,(uint16_t)(SIG_J & SIG_M)}, // 't'
	{(uint16_t)(SIG_B & SIG_C & SIG_D & SIG_E & SIG_F),(uint16_t)EMPTY}, // 'u'
	{(uint16_t)(SIG_B & SIG_C),(uint16_t)(SIG_H & SIG_L)}, // 'v'
	{(uint16_t)(SIG_B & SIG_C & SIG_E & SIG_F),(uint16_t)(SIG_L & SIG_N)}, // 'w'
	{(uint16_t)EMPTY,(uint16_t)(SIG_H & SIG_K & SIG_L & SIG_N)}, // 'x'
	{(uint16_t)EMPTY,(uint16_t)(SIG_H & SIG_K & SIG_M)}, // 'y'
	{(uint16_t)(SIG_A & SIG_D & SIG_G1),(uint16_t)(SIG_G2 & SIG_K & SIG_N)}, // 'z'
	{(uint16_t)EMPTY,(uint16_t)EMPTY}, // '{'
	{(uint16_t)EMPTY,(uint16_t)(SIG_J & SIG_M)}, // '|'
	{(uint16_t)EMPTY,(uint16_t)EMPTY}, // '}'
	{(uint16_t)EMPTY,(uint16_t)EMPTY}, // '~'
	{(uint16_t)EMPTY,(uint16_t)EMPTY}, // Unknown character (DEL or RUBOUT)
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
