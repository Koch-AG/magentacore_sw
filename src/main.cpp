#include <Magenta_lib.h>

#define TEST        0
#define ACC         1
#define POTI        2
#define BUZZER      3
#define SERVO       4
#define PROGRESSBAR 5
#define JOYSTICK    6
#define RE_BUTTONS  7
#define WRITE_CHAR  8
#define SET_PIXEL   9
#define SAMPLE      10
int testState = TEST;

#define PEEPHIGH 0
#define PEEPLOW  1
#define FURELISE 2
int buzzer = 3;

#define _180    1
#define _90     2
#define _0      3
int servoGrad = _180;

#define SHORT  5
#define LONG   20

#define ROW1 0
#define ROW2 1
#define ROW3 2
#define ROW4 3
#define ROW5 4
#define ROW6 5
#define ROW7 6
#define ROW8 7
int row = ROW1;

int potentiometer_last1 = 0;
int potentiometer_last2 = 0;
int potentiometer_last3 = 0;
int button_last1        = 0;
int button_last2        = 0;
int button_last3        = 0;
int button_lastUp       = 0;
int button_lastRight    = 0;
int button_lastDown     = 0;
int button_lastLeft     = 0;
int button_lastCenter   = 0;

void setup()
{
    Serial.begin(115200);
    magentaobj.init();
}

void BasicState()
{
    // printf("set the display to Basic state\n");
    magentaobj.write(0, 102, 255, 255, 126, 60, 24, 0);
}

void Test()
{
    magentaobj.setColor(255, 0, 0, 20, 5, 0);

    if(magentaobj.button_Right != button_lastRight && magentaobj.button_Right == 1)
    {
        magentaobj.setColor(255, 100, 0, 40, 20, 0);
        testState++;
        BasicState();
    }
    else
    {
        magentaobj.write(0, 102, 255, 255, 126, 60, 24, 0);
    }

    button_lastRight = magentaobj.button_Right;
}

void Accelerometer_Write()
{
    static byte data[8];
    static int move = 0;

    magentaobj.setColor(255, 100, 0, 40, 20, 0);

    move = ((magentaobj.accelerometer_x + 1) * 4);

    if(magentaobj.button_Right != button_lastRight && magentaobj.button_Right == 1)
    {
        magentaobj.setColor(200, 200, 0, 0, 0, 0);
        testState++;
        BasicState();
    }
    else if(magentaobj.button_Left != button_lastLeft && magentaobj.button_Left == 1)
    {
        magentaobj.setColor(255, 0, 0, 20, 5, 0);
        if(testState >= 1)
        {
            testState--;
        }
        BasicState();
    }
	else
    {
        if((magentaobj.accelerometer_y >= 0.8) && (magentaobj.accelerometer_y <= 0.95))
        {
            magentaobj.write((0x80 >> move), 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
        }
        if((magentaobj.accelerometer_y >= 0.5) && (magentaobj.accelerometer_y <= 0.7))
        {
            magentaobj.write(0x00, (0x80 >> move), 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
        }
        if((magentaobj.accelerometer_y >= 0.2) && (magentaobj.accelerometer_y <= 0.4))
        {
            magentaobj.write(0x00, 0x00, (0x80 >> move), 0x00, 0x00, 0x00, 0x00, 0x00);
        }
        if((magentaobj.accelerometer_y >= 0) && (magentaobj.accelerometer_y <= 0.2))
        {
            magentaobj.write(0x00, 0x00, 0x00, (0x80 >> move), 0x00, 0x00, 0x00, 0x00);
        }

        if((magentaobj.accelerometer_y >= -0.2) && (magentaobj.accelerometer_y <= -0))
        {
            magentaobj.write(0x00, 0x00, 0x00, 0x00, (0x80 >> move), 0x00, 0x00, 0x00);
        }
        if((magentaobj.accelerometer_y >=  -0.4) && (magentaobj.accelerometer_y <= -0.2))
        {
            magentaobj.write(0x00, 0x0, 0x00, 0x00, 0x00, (0x80 >> move), 0x00, 0x00);
        }
        if((magentaobj.accelerometer_y >= -0.7) && (magentaobj.accelerometer_y <= -0.5))
        {
            magentaobj.write(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, (0x80 >> move), 0x00);
        }
        if((magentaobj.accelerometer_y >= -0.95) && (magentaobj.accelerometer_y <= -0.8))
        {
            magentaobj.write(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, (0x80 >> move));
        }
    }

    button_lastRight = magentaobj.button_Right;
    button_lastLeft = magentaobj.button_Left;
}

void Poti()
{
    static int red   = 200;
    static int green = 200;
    static int blue  = 0;

    magentaobj.setColor(red, green, blue, 0, 0, 0);
    

    if (magentaobj.button_2 != button_last2 && magentaobj.button_2 == 1)
    {
        red = 0;
        green = 0;
        blue = 0;
    }
    button_last2 = magentaobj.button_2;

    if(magentaobj.potentiometer_1 != potentiometer_last1 && magentaobj.potentiometer_1 >= potentiometer_last1)
    {
        red = red + 8;
        if (red > 255)
        {
            red = 255;
        }
        potentiometer_last1 = magentaobj.potentiometer_1;
    }
    if(magentaobj.potentiometer_1 != potentiometer_last1 && magentaobj.potentiometer_1 <= potentiometer_last1)
    {
        red = red - 8;
        if (red <= 0)
        {
            red = 0;
        }
        potentiometer_last1 = magentaobj.potentiometer_1;
    }

    if(magentaobj.potentiometer_2 != potentiometer_last2 && magentaobj.potentiometer_2 >= potentiometer_last2)
    {
        green = green + 8;
        if (green > 255)
        {
            green = 255;
        }
        potentiometer_last2 = magentaobj.potentiometer_2;
    }
    if(magentaobj.potentiometer_2 != potentiometer_last2 && magentaobj.potentiometer_2 <= potentiometer_last2)
    {
        green = green - 8;
        if (green <= 0)
        {
            green = 0;
        }
        potentiometer_last2 = magentaobj.potentiometer_2;
    }

    if(magentaobj.potentiometer_3 != potentiometer_last3 && magentaobj.potentiometer_3 >= potentiometer_last3)
    {
        blue = blue + 8;
        if (blue > 255)
        {
            blue = 255;
        }
        potentiometer_last3 = magentaobj.potentiometer_3;
    }
    if(magentaobj.potentiometer_3 != potentiometer_last3 && magentaobj.potentiometer_3 <= potentiometer_last3)
    {
        blue = blue - 8;
        if (blue <= 0)
        {
            blue = 0;
        }
        potentiometer_last3 = magentaobj.potentiometer_3;
    }

    if(magentaobj.button_Right != button_lastRight && magentaobj.button_Right == 1)
    {
        magentaobj.setColor(80, 255, 30, 8, 25, 3);
        testState++;
        BasicState();
        red   = 200;
        green = 200;
        blue  = 0;
    }
    else if(magentaobj.button_Left != button_lastLeft && magentaobj.button_Left == 1)
    {
        magentaobj.setColor(255, 100, 0, 40, 20, 0);
        testState--;
        BasicState();
        red   = 200;
        green = 200;
        blue  = 0;
    }
    else 
    {
        magentaobj.write(255, 255, 255, 255, 255, 255, 255, 255);
    }

    button_lastRight = magentaobj.button_Right;
    button_lastLeft = magentaobj.button_Left;
}

void Buzzer()
{
    magentaobj.setColor(80, 255, 30, 8, 25, 3);

    if(magentaobj.button_1 != button_last1 && magentaobj.button_1 == 1)
    {
        buzzer = PEEPHIGH;
        magentaobj.playBuzzer(PEEPHIGH);
    }
    if(magentaobj.button_2 != button_last2 && magentaobj.button_2 == 1)
    {
        buzzer = PEEPLOW;
        magentaobj.playBuzzer(PEEPLOW);
    }
    if(magentaobj.button_3 != button_last3 && magentaobj.button_3 == 1)
    {
        buzzer = FURELISE;
        magentaobj.playBuzzer(FURELISE);
    }

    button_last1 = magentaobj.button_1;
    button_last2 = magentaobj.button_2;
    button_last3 = magentaobj.button_3;

    if(magentaobj.button_Up != button_lastUp && magentaobj.button_Up == 1)
    {
        magentaobj.stopPlaying();
    }
    button_lastUp = magentaobj.button_Up;


    if(magentaobj.button_Right != button_lastRight && magentaobj.button_Right == 1)
    {
        magentaobj.setColor(0, 255, 0, 0, 10, 0);
        testState++;
        BasicState();
    }
    else if(magentaobj.button_Left != button_lastLeft && magentaobj.button_Left == 1)
    {
        magentaobj.setColor(200, 200, 0, 0, 0, 0);
        testState--;
        BasicState();
    }
    else
    {
        switch (buzzer)
        {
            case PEEPHIGH:
                magentaobj.write(24, 60, 90, 153, 153, 24, 24, 24);
            break;

            case PEEPLOW:
                magentaobj.write(24, 24, 24, 153, 153, 90, 60, 24);
            break;

            case FURELISE:
                magentaobj.write(0, 102, 255, 255, 126, 60, 24, 0);
            break;
        }
    }

    button_lastRight = magentaobj.button_Right;
    button_lastLeft = magentaobj.button_Left;
}

void Servo()
{
    static int grad = 0;
    static int counter = 0;

    magentaobj.setColor(0, 255, 0, 0, 10, 0);

    magentaobj.setServo1(grad);
    magentaobj.setServo2(grad);

    if(magentaobj.button_Right != button_lastRight && magentaobj.button_Right == 1)
    {
        magentaobj.setColor(0, 0, 255, 0, 0, 30);
        testState++;
        BasicState();
        counter = 0;
    }
    else if(magentaobj.button_Left != button_lastLeft && magentaobj.button_Left == 1)
    {
        magentaobj.setColor(80, 255, 30, 8, 25, 3);
        testState--;
        BasicState();
        counter = 0;
    }
    else
    {
        switch (servoGrad)
        {
            case _180:
                counter ++;

                if (counter >= 20)
                {
                    counter = 0;
                    servoGrad = _90;
                }

                magentaobj.write(0, 0x7F, 0xED, 0x7D, 0x6D, 0x7F, 0, 0);
                grad = 180;
            break;

            case _90:
                counter ++;

                if (counter >= 20)
                {
                    counter = 0;
                    servoGrad = _0;
                }

                magentaobj.write(0, 0x77, 0x55, 0x75, 0x15, 0x17, 0, 0);
                grad = 90;
            break;
            case _0:
                counter ++;

                if (counter >= 20)
                {
                    counter = 0;
                    servoGrad = _180;
                }

                magentaobj.write(0, 0x1C, 0x14, 0x14, 0x14, 0x1C, 0, 0);
                grad = 0;
            break;
        }
    }

    button_lastRight = magentaobj.button_Right;
    button_lastLeft = magentaobj.button_Left;
}

void Progressbar()
{
    static int x = 1;
    static int y = 0;

    magentaobj.setColor(0, 0, 255, 0, 0, 30);
    magentaobj.clear(false);
    

    if(magentaobj.button_Right != button_lastRight && magentaobj.button_Right == 1)
    {
        magentaobj.setColor(0, 255, 255, 0, 30, 30);
        testState++;
        BasicState();
        x = 1;
        y = 0;
    }
    else if(magentaobj.button_Left != button_lastLeft && magentaobj.button_Left == 1)
    {
        magentaobj.setColor(0, 255, 0, 0, 10, 0);
        testState--;
        BasicState();
        x = 1;
        y = 0;
    }
    else
    {
        if(y >= 0)
        {
            if(x >= 0)
            {
                // printf("Write\n");
                magentaobj.progressbar(x, y);
                x++;
            }
            if(x >= 9)
            {
                x = 1;
                y++;
            }
        }
        if(y >= 8)
        {
            y = 0;
        }
    }

    button_lastRight = magentaobj.button_Right;
    button_lastLeft = magentaobj.button_Left;
}

void Joystick_WriteArray()
{
    byte data[] = {0, 0, 0, 0, 0, 0, 0, 0};
    int xly = 255;
    static int y = 0;
    static int x = 0b0000001;

    magentaobj.setColor(0, 255, 255, 0, 30, 30);

    if(magentaobj.button_Down != button_lastDown && magentaobj.button_Down == 1)
    {
        y += 1;
        if(y >= 7)
        {
            y = 7;
        }
    }
    if(magentaobj.button_Up != button_lastUp && magentaobj.button_Up == 1)
    {
        y -= 1;
        if(y <= 0)
        {
            y = 0;
        }
    }
    if(magentaobj.button_Right != button_lastRight && magentaobj.button_Right == 1)
    {
        x  = x >> 1;
        if(x <= 1)
        {
            x = 1;
        }
    }
    if(magentaobj.button_Left != button_lastLeft && magentaobj.button_Left == 1)
    {
        x  = x << 1;
        if(x >= 128)
        {
            x = 128;
        }
    }

    if(magentaobj.button_Center == 1)
    {
        data[y] = xly;
    }
    else
    {
        data[y] = x;
    }
    button_lastDown = magentaobj.button_Down;
    button_lastUp = magentaobj.button_Up;
    button_lastRight = magentaobj.button_Right;
    button_lastLeft = magentaobj.button_Left;


    if(magentaobj.button_3 != button_last3 && magentaobj.button_3 == 1)
    {
        magentaobj.setColor(255, 0, 255, 40, 0, 40);
        testState++;
        BasicState();
        y = 0;
        x = 0b0000001;
    }
    else if(magentaobj.button_1 != button_last1 && magentaobj.button_1 == 1)
    {
        magentaobj.setColor(0, 0, 255, 0, 0, 30);
        testState--;
        BasicState();
        y = 0;
        x = 0b0000001;
    }
    else
    {
        magentaobj.write_array(data);
    }

    button_lastRight = magentaobj.button_Right;
    button_lastLeft = magentaobj.button_Left;
}

void RE_Buttons()
{
    magentaobj.setColor(255, 0, 255, 40, 0, 40);

    

    if(magentaobj.button_Right != button_lastRight && magentaobj.button_Right == 1)
    {
        magentaobj.setColor(105, 0, 255, 10, 0, 40);
        testState++;
        BasicState();
    }
    else if(magentaobj.button_Left != button_lastLeft && magentaobj.button_Left == 1)
    {
        magentaobj.setColor(0, 255, 255, 0, 30, 30);
        testState--;
        BasicState();
    }
    else
    {
        if (magentaobj.button_1 != button_last1 && magentaobj.button_1 == 1)
        {
            magentaobj.write(192, 192, 192, 192, 192, 192, 192, 192);
        }
        if (magentaobj.button_2 != button_last2 && magentaobj.button_2 == 1)
        {
            magentaobj.write(24, 24, 24, 24, 24, 24, 24, 24);
        }
        if (magentaobj.button_3 != button_last3 && magentaobj.button_3 == 1)
        {
            magentaobj.write(3, 3, 3, 3, 3, 3, 3, 3);
        }

        button_last1 = magentaobj.button_1;
        button_last2 = magentaobj.button_2;
        button_last3 = magentaobj.button_3;
    }

    button_lastRight = magentaobj.button_Right;
    button_lastLeft = magentaobj.button_Left;
}

void Write_Char()
{
    static int letter = 65;

    magentaobj.setColor(105, 0, 255, 10, 0, 40);

    if(magentaobj.button_3 != button_last3 && magentaobj.button_3 == 1)
    {
        if(letter < 97)
        {
            letter = letter + 32;
        }
        else if(letter >= 97)
        {
            letter = letter - 31;
        }
        if(letter == 91)
        {
            letter = 65;
        }
    }
    if(magentaobj.button_1 != button_last1 && magentaobj.button_1 == 1)
    {
        if(letter < 97)
        {
            letter = letter + 31;
        }
        else if(letter >= 97)
        {
            letter = letter - 32;
        }
        if(letter == 96)
        {
            letter = 122;
        }
    }

    button_last1 = magentaobj.button_1;
    button_last3 = magentaobj.button_3;

    if(magentaobj.button_Right != button_lastRight && magentaobj.button_Right == 1)
    {
        magentaobj.setColor(0, 0, 0, 0, 0, 0);
        testState++;
        BasicState();
        letter = 65;
    }
    else if(magentaobj.button_Left != button_lastLeft && magentaobj.button_Left == 1)
    {
        magentaobj.setColor(255, 0, 255, 40, 0, 40);
        testState--;
        BasicState();
        letter = 65;
    }
    else
    {
        magentaobj.write_char(letter);
    }

    button_lastRight = magentaobj.button_Right;
    button_lastLeft = magentaobj.button_Left;
}

void SetPixelColor()
{
    static int x_Achse = 0;
    static int y_Achse = 0;
    static int red   = 0;
    static int green = 0;
    static int blue  = 0;

    printf("X:%i - Y:%i\n", x_Achse, y_Achse);
    printf("R:%i - G:%i - B:%i\n", red, green, blue);

    if (magentaobj.button_1 != button_last1 && magentaobj.button_1 == 1)
    {
        x_Achse++;
        if(x_Achse >= 8)
        {
            y_Achse ++;
            x_Achse = 0;
        }
    }
    if(magentaobj.button_2 != button_last2 && magentaobj.button_2 == 1)
    {
        x_Achse = 0;
        y_Achse = 0;
    }


    if(magentaobj.potentiometer_1 != potentiometer_last1 && magentaobj.potentiometer_1 >= potentiometer_last1)
    {
        red = red + 8;
        if (red > 255)
        {
            red = 255;
        }
        potentiometer_last1 = magentaobj.potentiometer_1;
    }
    if(magentaobj.potentiometer_1 != potentiometer_last1 && magentaobj.potentiometer_1 <= potentiometer_last1)
    {
        red = red - 8;
        if (red <= 0)
        {
            red = 0;
        }
        potentiometer_last1 = magentaobj.potentiometer_1;
    }

    if(magentaobj.potentiometer_2 != potentiometer_last2 && magentaobj.potentiometer_2 >= potentiometer_last2)
    {
        green = green + 8;
        if (green > 255)
        {
            green = 255;
        }
        potentiometer_last2 = magentaobj.potentiometer_2;
    }
    if(magentaobj.potentiometer_2 != potentiometer_last2 && magentaobj.potentiometer_2 <= potentiometer_last2)
    {
        green = green - 8;
        if (green <= 0)
        {
            green = 0;
        }
        potentiometer_last2 = magentaobj.potentiometer_2;
    }

    if(magentaobj.potentiometer_3 != potentiometer_last3 && magentaobj.potentiometer_3 >= potentiometer_last3)
    {
        blue = blue + 8;
        if (blue > 255)
        {
            blue = 255;
        }
        potentiometer_last3 = magentaobj.potentiometer_3;
    }
    if(magentaobj.potentiometer_3 != potentiometer_last3 && magentaobj.potentiometer_3 <= potentiometer_last3)
    {
        blue = blue - 8;
        if (blue <= 0)
        {
            blue = 0;
        }
        potentiometer_last3 = magentaobj.potentiometer_3;
    }

    button_last1 = magentaobj.button_1;
    button_last2 = magentaobj.button_2;


    if(magentaobj.button_Right != button_lastRight && magentaobj.button_Right == 1)
    {
        magentaobj.setRainbowColor();
        if(testState <= 11)
        {
            testState++;
        }
        BasicState();
        x_Achse = 0;
        y_Achse = 0;
        red   = 0;
        green = 0;
        blue  = 0;
    }
    else if(magentaobj.button_Left != button_lastLeft && magentaobj.button_Left == 1)
    {
        magentaobj.setColor(105, 0, 255, 10, 0, 40);
        testState--;
        BasicState();
        x_Achse = 0;
        y_Achse = 0;
        red   = 0;
        green = 0;
        blue  = 0;
    }
    else
    {
        magentaobj.setPixelColor(x_Achse, y_Achse, red, green, blue);

        if( y_Achse >=8)
        {
            y_Achse = 0;
            magentaobj.setPixelColor(x_Achse, y_Achse, red, green, blue, true);
        }
    }

    button_lastRight = magentaobj.button_Right;
    button_lastLeft = magentaobj.button_Left;
}

void Sample()
{
    static int counter = 0;
    static int muster = 0;

    counter ++;
    
    if(counter >= 20)
    {
        muster = 1;
        if(counter >= 40)
        {
            muster = 0;
            counter = 0;
        }
    }

    magentaobj.setRainbowColor();

    if(magentaobj.button_Left != button_lastLeft && magentaobj.button_Left == 1)
    {
        magentaobj.setColor(0, 0, 0, 0, 0, 0);
        testState--;
        BasicState();
        counter = 0;
        muster = 0;
    }
    else
    {
        magentaobj.sample(muster);
    }

    button_lastLeft = magentaobj.button_Left;
}

void loop()
{
    magentaobj.read_io();

    switch(testState)
    {
        case TEST:
            magentaobj.setSegmentAnzeige('T', 'E', 'S', 'T');
            Test();
        break;

        case ACC:
            magentaobj.setSegmentAnzeige('A', 'c', 'c',' ');
            Accelerometer_Write();
        break;

        case POTI:
            magentaobj.setSegmentAnzeige('P', 'o', 't','i');
            Poti();
        break;

        case BUZZER:
            magentaobj.setSegmentAnzeige('B', 'u', 'z','z');
            Buzzer();
        break;

        case SERVO:
            magentaobj.setSegmentAnzeige('S', 'e', 'r','v');
            Servo();
        break;

        case PROGRESSBAR:
            magentaobj.setSegmentAnzeige('P', 'r', 'o','g');
            Progressbar();
        break;

        case JOYSTICK:
            magentaobj.setSegmentAnzeige('J', 'o', 'y','s');
            Joystick_WriteArray();
        break;

        case RE_BUTTONS:
            magentaobj.setSegmentAnzeige('R', 'E', 'B','u');
            RE_Buttons();
        break;

        case WRITE_CHAR:
            magentaobj.setSegmentAnzeige('W', 'C', 'h','a');
            Write_Char();
        break;

        case SET_PIXEL:
            magentaobj.setSegmentAnzeige('S', 'P', 'x', 'l');
            SetPixelColor();
        break;

        case SAMPLE:
            magentaobj.setSegmentAnzeige('S', 'a', 'm','p');
            Sample();
        break;
    }
    delay(100);
}
