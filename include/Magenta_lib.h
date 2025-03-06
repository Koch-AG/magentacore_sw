#pragma once
#include <Arduino.h>

class MagentaCore{
    public:
        MagentaCore();

            // @brief
            int pcf_read();

            // @brief When read_io() is called, pcf_read() is called and written to pcfState. 
            //RotaryEncoder_Poti() and getSensor() are also called. 
            //button_1, button_2 and button_3 are set to 0 and if pcfState does not have the value 255 then getButtonPress(pcfState) is called.
            //@return pcfState
            int read_io();

        // @brief Initializes the content once at the beginning.
        void init();

        // @brief Clears data from the LED matrix.
        void clear();

        // @brief Writes the individual data columns to a specific array position.
        // @param byte1 Sets the 8 LEDs in the first column
        // @param byte2 Sets the 8 LEDs in the second column
        // @param byte3 Sets the 8 LEDs in the third column
        // @param byte4 Sets the 8 LEDs in the fourth column
        // @param byte5 Sets the 8 LEDs in the fifth column
        // @param byte6 Sets the 8 LEDs in the sixth column
        // @param byte7 Sets the 8 LEDs in the seventh column
        // @param byte8 Sets the 8 LEDs in the eighth column
        void write (byte byte1, byte byte2, byte byte3, byte byte4,
                    byte byte5, byte byte6, byte byte7, byte byte8);

        // @brief
        // @param
        void write_array(byte matrix[]);

        // @brief
        // @param
        void write_char(char character);

        // @brief Sets the color of the leds.
        // @param r Sets how bright red should be when the LED should light up
        // @param g Sets how bright green should be when the LED should light up
        // @param b Sets how bright blue should be when the LED should light up
        // @param BGC_r Optional. Sets how strongly the red LED should be when the LED should not light up. Normally set to 0.
        // @param BGC_g Optional. Sets how strongly the green LED should be when the LED should not light up. Normally set to 0.
        // @param BGC_b Optional. Sets how strongly the blue LED should be when the LED should not light up. Normally set to 0. 
        void setColor(byte r, byte g, byte b, byte BGC_r = 0, byte BGC_g = 0, byte BGC_b = 0);

        // @brief Sets the settings for the potentiometer.
        // @param Optional. Set an upper limit. Normally set to 1024.
        // @param Optional. Set a lower limit. Normally set to 0.
        // @param Optional. Set a step factor. Normally set to 43.
        void ConfigPoti(int max = 1024, int min = 0, int faktor = -43);

        // @brief Indicates examples for the LED matrix.
        // @param pattern indicates the sample.
        void sample(int pattern);
        
            // @brief Evaluates which button is pressed on the board.
            // @param pcfState Gives the hex value of the pressed button.
            // @return Which button is pressed.
            int getButtonPress(char pcfState);

        void RotaryEncoder_Poti();

        // @brief
        // @param *X_DataOut Pointer
        // @param *Y_DataOut Pointer
        // @param *Z_DataOut Pointer
        void getSensorData(float *X_DataOut, float *Y_DataOut, float *Z_DataOut);

        // @brief
        void getSensor();

        // @brief
        // @param
        // @param
        void progressbar(byte value, byte line);
        
        // @brief Indicates examples for the Buzzer.
        // @param tone Provides sound examples.
        void Buzzer(int tone);
        
        // @brief Moves the servo to a specified position.
        // @param grad Specifies the value where the servo should be positioned. Normally set to 0.
        void setServo1(int grad = 0);
        
        // @brief Moves the servo to a specified position.
        // @param grad Specifies the value where the servo should be positioned. Normally set to 0.
        void setServo2(int grad = 0);
        
        // @brief Sets the characters for the 14-segment display.
        // @param character1 Writes the character at the first position of the display.
        // @param character2 Writes the character at the second position of the display.
        // @param character3 Writes the character at the third position of the display.
        // @param character4 Writes the character at the fourth position of the display.
        void setSegmentAnzeige(char character1, char character2, char character3, char character4);

        // @brief Write a space in all 14-segment positions. This means that the display no longer lights up, it clears it.
        void clearSegmentAnzeige();

        int button_1;   // < 
        int button_2;   // < 
        int button_3;   // < 

        float accelerometer_x;  // <
        float accelerometer_y;  // <
        float accelerometer_z;  // <

        int potentiometer_0;    // <
        int potentiometer_1;    // <
        int potentiometer_2;    // <


    private:
            bool isConnected();    
        void writeDataToLED();

        int basecolor;
        int backgroundcolor;

        int32_t maxPoti;
        int32_t minPoti;
        int32_t stepSize;

        unsigned long microseconds_now; 	
        unsigned long microseconds_saved;
};

extern MagentaCore magentaobj;
extern MagentaCore matrix; //For old times sake