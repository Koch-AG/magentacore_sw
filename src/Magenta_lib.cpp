#include "Magenta_lib.h"
#include "font.h"
#include "STK8321.h"
#include "alphadisp.h"

#include <Wire.h>
#include <LiteLED.h>
#include <ESP32Encoder.h>
#include <ESP32Servo.h>
#include <NonBlockingRtttl.h>

#define LED_TYPE          LED_STRIP_WS2812
#define LED_TYPE_IS_RGBW  0   // if the LED is an RGBW type, change the 0 to 1
#define LED_GPIO          1   // change this number to be the GPIO pin connected to the LED
#define LED_BRIGHT        100  // sets how bright the LED is. O is off; 255 is burn your eyeballs out (not recommended)

#define PCF_ADRESS  0x38

#define BUTTON_C      0x01
#define BUTTON_U      0x02
#define BUTTON_R      0x04
#define BUTTON_D      0x08
#define BUTTON_L      0x10
#define BUTTON_RE_M   0x20
#define BUTTON_RE_L   0x40
#define BUTTON_RE_R   0x80

#define BTN_THRESHOLD 2

#define SERVO1PIN 16
#define SERVO2PIN 15

#define BUZZER_PIN 48
const char * BeepHigh = "BeepHigh:d=4,o=5,b=100:2a5";
const char * BeepLow = "BeepLow: d=4,o=5,b=100:2a4";
const char * HauntedHouse = "FurElise:d=4,o=5,b=60:32e6,32d#6,32e6,32d#6,32e6,32b5,32d6,32c6,32a5,16p,32c5,32e5,32a5,32b5,16p,32e5,32gs5,32b5,32c6,16p,32e5,32e6,32d#6,32e6,32d#6,32e6,32b5,32d6,32c6,32a5,16p,32c5,32e5,32a5,32b5,16p,32e5,32c6,32b5,32a5,8e5,8c5,8a4,8b4,8p,8e4,8gs4,8b4,8c5,8p,8e4,8e5,8d#5,8e5,8d#5,8e5,8b4,8d5,8c5,8a4,16p,8c4,8e4,8a4,8b4,16p,8e4,8c5,8b4,8a4,8e5,8c5,8a4,8b4,8p,8e4,8e5,8d#5,8e5,8d#5,8e5,8b4,8d5,8c5,8a4,16p,8c4,8e4,8a4,8b4,16p,8e4,8c5,8b4,8a4";

LiteLED magentacoreLed( LED_TYPE, LED_TYPE_IS_RGBW );    // create the LiteLED object; we're calling it "myLED"

ESP32Encoder encoder1;
ESP32Encoder encoder2;
ESP32Encoder encoder3;

STK8321 stk8321;

Servo servo1;
Servo servo2;
ESP32PWM pwm;

AlphaDispDriver magentacoreAlphaDisp;

static byte data[8] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

int LED_num = 0;

int minUs = 1000;
int maxUs = 2000;


MagentaCore::MagentaCore() {
 
}

void buzzerPlayTask(void * param) {

    for(;;) {
        if(rtttl::isPlaying()) {
            rtttl::play();
        }
        vTaskDelay(10/portTICK_PERIOD_MS);
    }
}

bool MagentaCore::isConnected() {
    Wire.beginTransmission(PCF_ADRESS);
    return ( Wire.endTransmission() == 0);
}

int MagentaCore::pcf_read() {
    int pcfBuffer;
    if (! isConnected()) {
        return false;
    }
    //printf("connected: %i\n", isConnected());

    Wire.requestFrom(PCF_ADRESS, 1);

    while (Wire.available() == 0);
    // Wire.endTransmission();
    pcfBuffer = Wire.read();
    // printf("pcfBuffer: %i\n", pcfBuffer);

    return pcfBuffer;
}

int MagentaCore::read_io() { 
    // printf("readIO aufgerufen\n");
    char pcfState;
    // microseconds_now = micros();
    pcfState = pcf_read();
    updateRotaryEncoderPoti();
    // printf("pcfState: %i\n", pcfState);

    button_1 = button_2 = button_3 = button_Center = button_Up = button_Right = button_Down = button_Left = 0x00;

    if (pcfState != 255) {
        getButtonPress(pcfState);
    }

    getSensor();

    // printf("readIO fertig\n");
    return pcfState;
}

void MagentaCore::init() {
    Wire.begin(4, 5, 100000);

    magentacoreLed.begin( LED_GPIO, 64 );         // initialze the myLED object. Here we have 1 LED attached to the LED_GPIO pin
    magentacoreLed.brightness( LED_BRIGHT, 1 );

    encoder1.attachHalfQuad (11, 10);
    encoder1.setCount (0);
    encoder1.resumeCount();
    encoder2.attachHalfQuad (8, 7);
    encoder2.setCount (0);
    encoder2.resumeCount();
    encoder3.attachHalfQuad (13, 14);
    encoder3.setCount (0);
    encoder3.resumeCount();

    ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);

    pinMode(SERVO1PIN, OUTPUT);
    pinMode(SERVO2PIN, OUTPUT);

    servo1.setPeriodHertz(50);      // Standard 50hz servo
    servo1.attach(SERVO1PIN, minUs, maxUs);
    servo2.setPeriodHertz(50);      // Standard 50hz servo
    servo2.attach(SERVO2PIN, minUs, maxUs);


    magentacoreAlphaDisp.begin();

    setColor(0xB8, 0, 0xF8, 0x20, 0, 0x50);

    configPoti();

    xTaskCreate(buzzerPlayTask,"buzzerPlayTask",2048*2,NULL,10,NULL );      /* Used to pass out the created task's handle. */
}

void MagentaCore::clear() {
    for(int i=0;i<8;i++) {
        data[i]=0;
    }
    writeDataToLED();
}

void MagentaCore::write (byte dataRow1, byte dataRow2, byte dataRow3, byte dataRow4,
                         byte dataRow5, byte dataRow6, byte dataRow7, byte dataRow8) {
    // printf("write aufgerufen\n");
    data[0] = dataRow1;
    // printf("Data_0: %i\n", data[0]);
    data[1] = dataRow2;
    // printf("Data_1: %i\n", data[1]);
    data[2] = dataRow3;
    // printf("Data_2: %i\n", data[2]);
    data[3] = dataRow4;
    // printf("Data_3: %i\n", data[3]);
    data[4] = dataRow5;
    // printf("Data_4: %i\n", data[4]);
    data[5] = dataRow6;
    // printf("Data_5: %i\n", data[5]);
    data[6] = dataRow7;
    // printf("Data_6: %i\n", data[6]);
    data[7] = dataRow8;
    // printf("Data_7: %i\n", data[7]);
    
    writeDataToLED();
}

void MagentaCore::write_array(byte matrixData[]) {
    for(int i = 0; i < 8; i++) {
        data[i] = matrixData[i];
    }
}

void MagentaCore::write_char(char character) {
    char buffer[8];                                            /* buffer to store fron from flash */
    clear();                                                  /* clear matrix screen */

    for(int i = 0; i < 8; i++) {                                            /* read font from flash */
        buffer[i] = font[character][i];
    
    }

    for (int i = 0; i < 8; i++) {                     /* write font from buffer to matrix display */
        for (int j = 0; j < 8; j++) {    
            if (buffer[i] & (0x01 << j))
                data[i] |= (0x80 >> j) << 1;
        }
    }
    
    writeDataToLED();
}

void  MagentaCore::writeDataToLED() {
    // printf("WriteDataToLED aufgerufen\n");
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            // printf("x: %i, y: %i\n", x, y);
            LED_num = (y * 8) + x;
            if (data[y] & (0x80 >> x)) { 
                // printf("LED: %i\n", LED_num);
                magentacoreLed.setPixel(LED_num, basecolor);
            } else {
                magentacoreLed.setPixel(LED_num, backgroundcolor);
            }
        }
    }

    magentacoreLed.show();
}

void MagentaCore::setColor(byte r, byte g, byte b, byte backgroundColor_r, byte backgroundColor_g, byte backgroundColor_b) {
    basecolor = (r << 16) + (g << 8)+ b;
    backgroundcolor = (backgroundColor_r << 16) + (backgroundColor_g << 8) + backgroundColor_b;
}

void MagentaCore::configPoti(int max, int min, int faktor) {
    if((max || min) != 0){
        maxPoti = max;
        minPoti = min;

        if (max < min) {
            maxPoti = min;
            minPoti = max;
        }
    }

    stepSize = faktor;
}

void MagentaCore::sample(int pattern) {
    switch(pattern) {
        
        case 0:
            matrix.setColor(0x80,0x30,0x00,0x00,0x00,0x20);
            for (int i = 0; i < 8; i++)
                data[i] = ((i % 2) == 0) ? 0b10101010 : 0b01010101; 

            break;
        case 1:
            matrix.setColor(0x80,0x30,0x00,0x00,0x00,0x20);
            for (int i = 0; i < 8; i++){
                data[i] = ((i % 2) == 0) ? 0b01010101 : 0b10101010;
            } 

            break;

        // case 2:
        //     matrix.setColor(0x0,0x00,0x80,0x60,0x30,0x00);
        //     for (int i = 0; i < 64; i++){
        //         data[i];
        //     }

        //     break;

        default: {
            break;
        }
    }   

    writeDataToLED(); 
}

int MagentaCore::getButtonPress(char pcfState) {
    // printf("getButtonPress aufgerufen\n");
    if((pcfState & BUTTON_RE_L) == 0) {
        button_1 = 0x01;
        return button_1;
    }
    else if((pcfState & BUTTON_RE_M) == 0) {
        button_2 = 0x01;
        return button_2;
    }
    else if((pcfState & BUTTON_RE_R) == 0) {
        button_3 = 0x01;
        return button_3;
    }
    else if((pcfState & BUTTON_U) == 0) {
        button_Up = 0x01;
        return button_Up;
    }
    else if((pcfState & BUTTON_D) == 0) {
        button_Down = 0x01;
        return button_Down;
    }
    else if((pcfState & BUTTON_L) == 0) {
        button_Left  = 0x01;
        return button_Left;
    }
    else if((pcfState & BUTTON_R) == 0) {
        button_Right = 0x01;
        return button_Right;
    }
    else if((pcfState & BUTTON_C) == 0) {
        button_Center = 0x01;
        return button_Center;
    }
    return 0;
    // printf("getButtonPress fertig\n");
}

void MagentaCore::updateRotaryEncoderPoti() {
    int encoderCounter1 = encoder1.getCount();
    int encoderCounter2 = encoder2.getCount();
    int encoderCounter3 = encoder3.getCount();
    
    encoder1.clearCount();
    encoder2.clearCount();
    encoder3.clearCount();

    potentiometer_1 = potentiometer_1 + (encoderCounter1 * stepSize);
    // printf("%i\n", potentiometer_0);

    potentiometer_2 = potentiometer_2 + (encoderCounter2 * stepSize);
    // printf("%i\n", potentiometer_1);

    potentiometer_3 = potentiometer_3 + (encoderCounter3 * stepSize);
    // printf("%i\n", potentiometer_2);

    if((maxPoti || minPoti) != 0) {
        if(potentiometer_1 >= maxPoti) {
            potentiometer_1 = maxPoti;
        }
        if(potentiometer_1 <= minPoti){
            potentiometer_1 = minPoti;
        }

        if(potentiometer_2 >= maxPoti) {
            potentiometer_2 = maxPoti;
        }
        if(potentiometer_2 <= minPoti){
            potentiometer_2 = minPoti;
        }

        if(potentiometer_3 >= maxPoti) {
            potentiometer_3 = maxPoti;
        }
        if(potentiometer_3 <= minPoti){
            potentiometer_3 = minPoti;
        }
    }
    
}

void MagentaCore::getSensor() {
    float x,y,z;
    getSensorData(&x,&y,&z);
    accelerometer_x = -y;
    accelerometer_y = x;
    accelerometer_z = z;
    // printf("X:%f - Y:-%f - Z:%f\n", x,y,z);
}

void MagentaCore::getSensorData(float *dataOut_x, float *dataOut_y, float *dataOut_z)
{
    *dataOut_x = 0;
    *dataOut_y = 0;
    *dataOut_z = 0;
    stk8321.STK8321_Getregister_data(dataOut_x, dataOut_y, dataOut_z);
}

void MagentaCore::progressbar(byte value, byte line) {
    data[line] = ((0xFF00 >> value) & 0xFF);

    writeDataToLED();
}

void MagentaCore::playBuzzer(int tone) {
    switch(tone){
        case 0:
            rtttl::begin(BUZZER_PIN, BeepHigh);
            
            break;
        case 1:
            rtttl::begin(BUZZER_PIN, BeepLow);
            
            break;
        case 2:
            rtttl::begin(BUZZER_PIN, HauntedHouse);
            
            break;
    }
}

void MagentaCore::setServo1(int grad) {
    servo1.write(grad);
    // printf("Grad: %i\n", grad);
}

void MagentaCore::setServo2(int grad) {
    servo2.write(grad);
}

void MagentaCore::setSegmentAnzeige(char character1, char character2, char character3, char character4) {
    magentacoreAlphaDisp.writeChar(0, character1);
    magentacoreAlphaDisp.writeChar(1, character2);
    magentacoreAlphaDisp.writeChar(2, character3);
    magentacoreAlphaDisp.writeChar(3, character4);

    // printf("Zeichen1:%c",zeichen1);
    // printf("Zeichen2:%c",zeichen2);
    // printf("Zeichen3:%c",zeichen3);
    // printf("Zeichen4:%c\n",zeichen4);
}

void MagentaCore::clearSegmentAnzeige() {
    magentacoreAlphaDisp.writeChar(0, ' ');
    magentacoreAlphaDisp.writeChar(1, ' ');
    magentacoreAlphaDisp.writeChar(2, ' ');
    magentacoreAlphaDisp.writeChar(3, ' ');
}

MagentaCore magentaobj;
MagentaCore matrix = magentaobj;