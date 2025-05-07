#pragma once
#include <Arduino.h>

class MagentaCore{
    public:
        MagentaCore();

            // @brief checks if the PCF8574 I/O expander is connected, requests one byte of data from it over I2C, waits for the data.
            // @return pcfBuffer
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
        // @param byte1 Sets the 8 LEDs in the first column.
        // @param byte2 Sets the 8 LEDs in the second column.
        // @param byte3 Sets the 8 LEDs in the third column.
        // @param byte4 Sets the 8 LEDs in the fourth column.
        // @param byte5 Sets the 8 LEDs in the fifth column.
        // @param byte6 Sets the 8 LEDs in the sixth column.
        // @param byte7 Sets the 8 LEDs in the seventh column.
        // @param byte8 Sets the 8 LEDs in the eighth column.
        void write (byte byte1, byte byte2, byte byte3, byte byte4,
                    byte byte5, byte byte6, byte byte7, byte byte8);

        // @brief takes an array of 8 bytes (matrix[]) and copies its values into the data[] array, one byte at a time.
        // @param matrix[]: An array of 8 bytes to be copied into the data[] array.
        void write_array(byte matrix[]);

        // @brief displays a character on a matrix display by reading the font data for the character, clearing the display, and writing the corresponding pixel data to the matrix.
        // @param character: The character to display on the matrix.
        void write_char(char character);

        // @brief Sets the color of the leds.
        // @param r Sets how bright red should be when the LED should light up.
        // @param g Sets how bright green should be when the LED should light up.
        // @param b Sets how bright blue should be when the LED should light up.
        // @param BGC_r Optional. Sets how strongly the red LED should be when the LED should not light up. Normally set to 0.
        // @param BGC_g Optional. Sets how strongly the green LED should be when the LED should not light up. Normally set to 0.
        // @param BGC_b Optional. Sets how strongly the blue LED should be when the LED should not light up. Normally set to 0. 
        void setColor(byte r, byte g, byte b, byte BGC_r = 0, byte BGC_g = 0, byte BGC_b = 0);

        // @brief Sets the settings for the potentiometer.
        // @param Optional. Set an upper limit. Normally set to 1024.
        // @param Optional. Set a lower limit. Normally set to 0.
        // @param Optional. Set a step factor. Normally set to 43.
        void ConfigPoti(int max = 1024, int min = 0, int faktor = -40);

        // @brief Indicates examples for the LED matrix.
        // @param pattern indicates the sample.
        void sample(int pattern);
        
            // @brief Evaluates which button is pressed on the board.
            // @param pcfState Gives the hex value of the pressed button.
            // @return Which button is pressed.
            int getButtonPress(char pcfState);
        
        // @brief updates virtual potentiometer values based on rotary encoder input, applying step-based increments and enforcing optional min/max limits.
        void RotaryEncoder_Poti();

        // @brief fetches X, Y, and Z acceleration values from the STK8321 sensor and stores them in the given output pointers.
        // @param *X_DataOut Pointer to store the X-axis acceleration value.
        // @param *Y_DataOut Pointer to store the Y-axis acceleration value.
        // @param *Z_DataOut Pointer to store the Z-axis acceleration value.
        void getSensorData(float *X_DataOut, float *Y_DataOut, float *Z_DataOut);

        // @brief reads accelerometer data and assigns the values to internal variables, applying a coordinate transformation by swapping and negating the X and Y axes.
        void getSensor();

        // @brief displays a horizontal progress bar on a specific LED matrix line by shifting a bitmask based on the value and updating the LED display.
        // @param value Determines how much of the progress bar is filled (typically from 0 to 8). A higher value results in more LEDs lit.
        // @param line Specifies which row (or line) of the LED matrix the progress bar should be drawn on.
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

        int button_1;   // < Stores the states (e.g., pressed or not) of buttons.
        int button_2;   // < Stores the states (e.g., pressed or not) of buttons.
        int button_3;   // < Stores the states (e.g., pressed or not) of buttons.

        float accelerometer_x;  // < Stores the accelerometer readings for each axis (X).
        float accelerometer_y;  // < Stores the accelerometer readings for each axis (Y).
        float accelerometer_z;  // < Stores the accelerometer readings for each axis (Z).

        int potentiometer_0;    // < Stores the values for the three potentiometers.
        int potentiometer_1;    // < Stores the values for the three potentiometers.
        int potentiometer_2;    // < Stores the values for the three potentiometers.


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