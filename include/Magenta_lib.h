#pragma once
#include <Arduino.h>

class MagentaCore{
    public:
        MagentaCore();

            // @brief The function pcf_read() reads the status of several buttons or inputs connected to a port expander. This chip is connected to the device 
            // via an I²C connection. First, the function checks whether the port expander is accessible. If so, it requests the current data from the chip. 
            // The chip sends back a byte that stores which of the eight inputs are currently active (e.g., button pressed) or inactive. The function returns 
            // this value so that the main program knows which buttons are currently pressed.
            // @return pcfBuffer
            int pcf_read();

            // @brief The function read_io() processes data in the background and returns the value of the shift register to which the buttons are connected. 
            // The buttons are debounced and the button status is stored in the variables button_1 to button_3 and the Joystick. It also calculates the potentiometer 
            // values of the rotary encoders and stores them in potentiometer_1 to potentiometer_3. Finally, it reads the values of the acceleration 
            // sensor (ACC sensor) and stores them in the corresponding variables accelerometer_x to accelerometer_z.
            // @return pcfState
            int read_io();

        // @brief The init() function prepares all hardware components of the MagentaCore. It starts the I²C connection, switches on the LED matrix, 
        // sets the rotary encoders to zero, and activates them. Four PWM timers are reserved, two servos are set up, the display is started, and the 
        // LED colors are set. Finally, a background program for the buzzer is started.
        void init();

        // @brief The clear() function ensures that all LEDs are switched off. It essentially deletes the current light patterns, resetting the LED 
        // strip to the background color. This allows you to reset the LED display and start again from the beginning.
        // @param doLedUpdate Optional. Normally set to true.
        void clear(bool doLedUpdate = true);

        // @brief The setPicel() function 
        // @param x 
        // @param y
        // @param r
        // @param b
        // @param b
        // @param doLedUpdate Optional. Normally set to false.
        void setPixelColor(int x, int y, byte r, byte g, byte b, bool doLedUpdate = false);

        // @brief The write() function takes eight values, known as bytes, and stores them in a data array. These bytes can be colors or brightness levels 
        // for the LEDs, for example. Once the values have been stored in the array, another function is called which then sends this data to the LED strip 
        // so that the LEDs display the new settings. In short: write() updates the data for the LEDs with new values and ensures that the LEDs actually display the changes.
        // @param dataRow1 Sets the 8 LEDs in the first column.
        // @param dataRow2 Sets the 8 LEDs in the second column.
        // @param dataRow3 Sets the 8 LEDs in the third column.
        // @param dataRow4 Sets the 8 LEDs in the fourth column.
        // @param dataRow5 Sets the 8 LEDs in the fifth column.
        // @param dataRow6 Sets the 8 LEDs in the sixth column.
        // @param dataRow7 Sets the 8 LEDs in the seventh column.
        // @param dataRow8 Sets the 8 LEDs in the eighth column.
        void write (byte dataRow1, byte dataRow2, byte dataRow3, byte dataRow4,
                    byte dataRow5, byte dataRow6, byte dataRow7, byte dataRow8);

        // @brief The write_array() function takes an array with eight values and copies these values to the internal memory. This means that it takes the current values 
        // from the specified array and stores them in order to control the LEDs with these new settings later on. In short, this function updates the LED data with 
        // a whole series of values at once.
        // @param matrix[]: An array of 8 bytes to be copied into the data[] array.
        void write_array(byte matrixData[]);

        // @brief The write_char() function displays a single character on the LED matrix. It first clears the display, reads the appropriate character from a font table, 
        // and then activates the LEDs to display the character. Finally, the data is sent to the LEDs so that the character is visible.
        // @param character: The character to display on the matrix.
        void write_char(char character);

        // @brief
        // @param gradient Optional. Normally set to false.
        void setRainbowColor(bool gradient = false);

        // @brief The setColor() function sets the foreground and background colors by allowing you to specify the red, green, and blue values in decimal (0-255), hex (0x00-0xFF), 
        // or binary (0b00000000-0b11111111). The smaller the number, the darker or less bright the LED will be. The values are stored in the variables basecolor (r, g, b) and
        // backgroundcolor (BGC_r, BGC_g, BGC_b), with the individual color components combined into a single number. This allows you to flexibly adjust the colors and easily apply
        // them to the LEDs later. The colors for the basecolor must be set. Background colors do not need to be set; if the background color is not specified, it is set to 0 (off).
        // @param r Sets how bright red should be when the LED should light up.
        // @param g Sets how bright green should be when the LED should light up.
        // @param b Sets how bright blue should be when the LED should light up.
        // @param backgroundColor_r Optional. Sets how strongly the red LED should be when the LED should not light up. Normally set to 0.
        // @param backgroundColor_g Optional. Sets how strongly the green LED should be when the LED should not light up. Normally set to 0.
        // @param backgroundColor_b Optional. Sets how strongly the blue LED should be when the LED should not light up. Normally set to 0. 
        void setColor(byte r, byte g, byte b, byte backgroundColor_r = 0, byte backgroundColor_g = 0, byte backgroundColor_b = 0);

        // @brief The configPoti() function sets the max and min values for the potentiometer and stores the step size (factor) in stepSize. If max is less than min,
        // they are swapped so that everything is correct.
        // @param max Optional. Set an upper limit. Normally set to 1024.
        // @param min Optional. Set a lower limit. Normally set to 0.
        // @param faktor Optional. Set a step factor. Normally set to 43.
        void configPoti(int max = 1024, int min = 0, int faktor = -40);

        // @brief The sample() function displays different patterns on the LED matrix, depending on the pattern value. For patterns 0 and 1, it sets the colors and fills the data with 
        // alternating stripe patterns (alternating between 0101... and 1010...). After selection, the data is sent to the LEDs. For other patterns, it remains empty..
        // @param pattern indicates the sample.
        void sample(int pattern);
        
            // @brief The getButtonPress() function checks the status of a button code (pcfState) using bitwise AND operations. If certain bits (e.g., BUTTON_RE_L, BUTTON_RE_M, etc.) 
            // are set to 0, it recognizes that the corresponding button has been pressed, sets a variable (e.g., button_1) to 1, and returns the button code. If no button is recognized, 
            // it returns 0.
            // @param pcfState Gives the hex value of the pressed button.
            // @return Which button is pressed.
            int getButtonPress(char pcfState);
        
        // @brief The updateRotaryEncoderPoti() function reads the counter readings from three rotary encoders, multiplies them by a step value (stepSize), and adds them to the potentiometer 
        // values. The values are then limited to the specified maximum and minimum values (maxPoti, minPoti) to prevent overflow or underflow.
        void updateRotaryEncoderPoti();

        // @brief The getSensor() function reads the sensor data for the x, y, and z axes and stores it in the variables accelerometer_x, accelerometer_y, and accelerometer_z. 
        // The values for the x and y axes are reversed or negated to adjust the orientation.
        // @param *X_DataOut Pointer to store the X-axis acceleration value.
        // @param *Y_DataOut Pointer to store the Y-axis acceleration value.
        // @param *Z_DataOut Pointer to store the Z-axis acceleration value.
        void getSensorData(float *dataOut_x, float *dataOut_y, float *dataOut_z);

        // @brief The getSensorData() function sets the pointers for the x, y, and z values to 0 and then calls the STK8321_Getregister_data() method to load the current sensor 
        // data into the transferred variables.
        void getSensor();

        // @brief The progressbar() function updates the data for a bar (line) on the LED matrix using a value to represent the progress. It shifts a mask (0xFF00) by value bits 
        // to the right and extracts the lower 8 bits to fill the bar accordingly. The data is then transferred to the LEDs.
        // @param value Determines how much of the progress bar is filled (typically from 0 to 8). A higher value results in more LEDs lit.
        // @param line Specifies which row (or line) of the LED matrix the progress bar should be drawn on.
        void progressbar(byte value, byte line);

        // @brief The playBuzzer() function plays different tones depending on the value of tone. For 0, 1, or 2, a specific melody or tone sequence 
        // (e.g., BeepHigh, BeepLow, HauntedHouse) is started at the buzzer pin.
        // @param tone Provides sound examples.
        void playBuzzer(int tone);

        // @brief
        // @param lengt
        // @param sound
        void setBuzzerPlay(unsigned long duration, unsigned int frequency);

        // @brief
        void stopPlaying();
        
        // @brief The isBuzzerPlaying() function is a simple status-check function that returns whether the buzzer is playing.
        // @return 1 if the Buzzer ist playing 0 if it doesn't play
        bool isBuzzerPlaying();
        
        // @brief The setServo1() function sets the servo to the position specified in degrees (0-180). The servo compares the current position with the desired position in the background.
        // @param grad Specifies the value where the servo should be positioned. Normally set to 0.
        void setServo1(int grad = 0);
        
        // @brief The setServo2() function sets the servo to the position specified in degrees (0-180). The servo compares the current position with the desired position in the background.
        // @param grad Specifies the value where the servo should be positioned. Normally set to 0.
        void setServo2(int grad = 0);
        
        // @brief The function setSegmentDisplay() displays four individual characters (character1 to character4) on a 4-digit display by calling the writeChar() 
        // method for each position (0 to 3). This displays the desired characters on the display. To enter the letters, you can write, for example, 'A', 'A', 'A', 'A' 
        // in the brackets, and the segment display would then show A A A A. You can choose any letters between A and Z and write 4-letter words.
        // @param character1 Writes the character at the first position of the display.
        // @param character2 Writes the character at the second position of the display.
        // @param character3 Writes the character at the third position of the display.
        // @param character4 Writes the character at the fourth position of the display.
        void setSegmentAnzeige(char character1, char character2, char character3, char character4);

        // @brief The clearSegmentDisplay() function clears the display by writing spaces (' ') to all four positions. This causes nothing to appear on the 4-digit display.
        void clearSegmentAnzeige();

        int button_1;        // < Stores the state (e.g., pressed or not) of the button. If Buttons = 1, it means that the buttons are pressed.
        int button_2;        // < Stores the state (e.g., pressed or not) of the button. If Buttons = 1, it means that the buttons are pressed.
        int button_3;        // < Stores the state (e.g., pressed or not) of the button. If Buttons = 1, it means that the buttons are pressed.
        int button_Up;       // < Stores the state (e.g., pressed or not) of the button. If Buttons = 1, it means that the buttons are pressed.
        int button_Right;    // < Stores the state (e.g., pressed or not) of the button. If Buttons = 1, it means that the buttons are pressed.
        int button_Down;     // < Stores the state (e.g., pressed or not) of the button. If Buttons = 1, it means that the buttons are pressed.
        int button_Left;     // < Stores the state (e.g., pressed or not) of the button. If Buttons = 1, it means that the buttons are pressed.
        int button_Center;   // < Stores the state (e.g., pressed or not) of the button. If Buttons = 1, it means that the buttons are pressed.

        float accelerometer_x;  // < Stores the accelerometer readings for each axis (X).
        float accelerometer_y;  // < Stores the accelerometer readings for each axis (Y).
        float accelerometer_z;  // < Stores the accelerometer readings for each axis (Z).

        int potentiometer_1;    // < Stores the values for the three potentiometers.
        int potentiometer_2;    // < Stores the values for the three potentiometers.
        int potentiometer_3;    // < Stores the values for the three potentiometers.


    private:
            bool isConnected();    
        void writeDataToLED();
        void writePixelsToLED();
        void hsv2rgb(uint32_t h, uint32_t s, uint32_t v, uint32_t *r, uint32_t *g, uint32_t *b);

        int basecolor;
        int backgroundcolor;
        int rainbowcolor;
            bool rainbowBaseColor = false;

        int32_t maxPoti;
        int32_t minPoti;
        int32_t stepSize;

        unsigned long microseconds_now; 	
        unsigned long microseconds_saved;
};

extern MagentaCore magentaobj;
extern MagentaCore matrix; //For old times sake