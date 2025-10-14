
int button_last1      = 0;
int button_last2      = 0;
int button_last3      = 0;
int button_lastUp     = 0;
int button_lastRight  = 0;
int button_lastDown   = 0;
int button_lastLeft   = 0;
int button_lastCenter = 0;

int r     = 0;
int g     = 0;
int b     = 0;
int BGC_r = 0;
int BGC_g = 0;
int BGC_b = 0;

int change = 1;
int color = 1;
int segment = 0;
int acc = 0;
int servo = 1;
int buzzer = 1;

int move = 0;
int grad1 = 0;
int grad2 = 0;

void setup() 
{
  Serial.begin(115200);
  magentaobj.init();
  magentaobj.setSegmentAnzeige('!', '!', '!', '!');
}

// void Buzzer()
// {
//   switch (buzzer)
//   {
//     case HIGH:
//       magentaobj.Buzzer(PEEPHIGH);
//       break;
    
//     case LOW:
//       magentaobj.Buzzer(PEEPLOW);
//       break;
//   }
  
// }

void ChangeColor()
{
  switch (change)
  {
  case COLOR:
    // printf("->color\n");
    if(magentaobj.potentiometer_1 != potentiometer_last1 && magentaobj.potentiometer_1 >= potentiometer_last1)
    {
      r = r + 10;
      if (r >= 250)
      {
        buzzer = HIGH;
        // Buzzer();
        r = 250;
      }
      potentiometer_last1 = magentaobj.potentiometer_1;
    }
    if(magentaobj.potentiometer_1 != potentiometer_last1 && magentaobj.potentiometer_1 <= potentiometer_last1)
    {
      r = r - 10;
      if (r <= 0)
      {
        buzzer = LOW;
        // Buzzer();
        r = 0;
      }
      potentiometer_last1 = magentaobj.potentiometer_1;
    }

    if(magentaobj.potentiometer_2 != potentiometer_last2 && magentaobj.potentiometer_2 >= potentiometer_last2)
    {
      g = g + 10;
      if (g >= 250)
      {
        buzzer = HIGH;
        // Buzzer();
        g = 250;
      }
      potentiometer_last2 = magentaobj.potentiometer_2;
    }
    if(magentaobj.potentiometer_2 != potentiometer_last2 && magentaobj.potentiometer_2 <= potentiometer_last2)
    {
      g = g - 10;
      if (g <= 0)
      {
        buzzer = LOW;
        // Buzzer();
        g = 0;
      }
      potentiometer_last2 = magentaobj.potentiometer_2;
    }

    if(magentaobj.potentiometer_3 != potentiometer_last3 && magentaobj.potentiometer_3 >= potentiometer_last3)
    {
      b = b + 10;
      if (b >= 250)
      {
        buzzer = HIGH;
        // Buzzer();
        b = 250;
      }
      potentiometer_last3 = magentaobj.potentiometer_3;
    }
    if(magentaobj.potentiometer_3 != potentiometer_last3 && magentaobj.potentiometer_3 <= potentiometer_last3)
    {
        buzzer = LOW;
        // Buzzer();
      b = b - 10;
      if (b <= 0)
      {
        b = 0;
      }
      potentiometer_last3 = magentaobj.potentiometer_3;
    }

  break;
  
  case BGCOLOR:
  // printf("->bgcolor\n");
    if(magentaobj.potentiometer_1 != potentiometer_last1 && magentaobj.potentiometer_1 >= potentiometer_last1)
    {
      BGC_r = BGC_r + 10;
      if (BGC_r >= 250)
      {
        buzzer = HIGH;
        // Buzzer();
        BGC_r = 250;
      }
      potentiometer_last1 = magentaobj.potentiometer_1;
    }
    if(magentaobj.potentiometer_1 != potentiometer_last1 && magentaobj.potentiometer_1 <= potentiometer_last1)
    {
      BGC_r = BGC_r - 10;
      if (BGC_r <= 0)
      {
        buzzer = LOW;
        // Buzzer();
        BGC_r = 0;
      }
      potentiometer_last1 = magentaobj.potentiometer_1;
    }

    if(magentaobj.potentiometer_2 != potentiometer_last2 && magentaobj.potentiometer_2 >= potentiometer_last2)
    {
      BGC_g = BGC_g + 10;
      if (BGC_g >= 250)
      {
        buzzer = HIGH;
        // Buzzer();
        BGC_g = 250;
      }
      potentiometer_last2 = magentaobj.potentiometer_2;
    }
    if(magentaobj.potentiometer_2 != potentiometer_last2 && magentaobj.potentiometer_2 <= potentiometer_last2)
    {
      BGC_g = BGC_g - 10;
      if (BGC_g <= 0)
      {
        buzzer = LOW;
        // Buzzer();
        BGC_g = 0;
      }
      potentiometer_last2 = magentaobj.potentiometer_2;
    }

    if(magentaobj.potentiometer_3 != potentiometer_last3 && magentaobj.potentiometer_3 >= potentiometer_last3)
    {
      BGC_b = BGC_b + 10;
      if (BGC_b >= 250)
      {
        buzzer = HIGH;
        // Buzzer();
        BGC_b = 250;
      }
      potentiometer_last3 = magentaobj.potentiometer_3;
    }

    if(magentaobj.potentiometer_3 != potentiometer_last3 && magentaobj.potentiometer_3 <= potentiometer_last3)
    {
      BGC_b = BGC_b - 10;
      if (BGC_b <= 0)
      {
        buzzer = LOW;
        // Buzzer();
        BGC_b = 0;
      }
      potentiometer_last3 = magentaobj.potentiometer_3;
    }

  break;
  }
	
}

void Accelerometer()
{
  move = ((magentaobj.accelerometer_x + 1) * 4);
	
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

void Servo()
{
  switch (servo)
  {
    case SERVO1:
      magentaobj.setServo1(grad1);
      break;
    
    case SERVO2:
      magentaobj.setServo2(grad2);
      break;
  }
}

void ReadButton()
{
  if(magentaobj.button_1 != button_last1 && magentaobj.button_1 == 1)
  {
    acc = ACC;
    segment = NORA;
    servo = SERVO1;

    grad1 = random(1, 180);

    Servo();
  }

  if(magentaobj.button_2 != button_last2 && magentaobj.button_2 == 1)
  {
    change = COLOR;
    servo = SERVO1;

    grad1 = random(1, 180);

    Servo();
  }

  if(magentaobj.button_3 != button_last3 && magentaobj.button_3 == 1)
  {
    change = BGCOLOR;
    servo = SERVO1;

    grad1 = random(1, 180);

    Servo();
  }

  if(magentaobj.button_Center != button_lastCenter && magentaobj.button_Center == 1)
  {
    acc = 0;
    servo = SERVO2;

    grad2 = random(1, 180);

    Servo();

    r     = 100;
    g     = 100;
    b     = 100;
    BGC_r = 30;
    BGC_g = 30;
    BGC_b = 30;
    segment = REMO;
  }

  if(magentaobj.button_Up != button_lastUp && magentaobj.button_Up == 1)
  {
    acc = 0;
    servo = SERVO2;

    grad2 = random(1, 180);

    Servo();

    r     = 140;
    g     = 0;
    b     = 0;
    BGC_r = 50;
    BGC_g = 20;
    BGC_b = 0;
  }

  if(magentaobj.button_Right != button_lastRight && magentaobj.button_Right == 1)
  {
    acc = 0;
    servo = SERVO2;

    grad2 = random(1, 180);

    Servo();

    r     = 0;
    g     = 0;
    b     = 150;
    BGC_r = 0;
    BGC_g = 20;
    BGC_b = 30;
  }

  if(magentaobj.button_Down != button_lastDown && magentaobj.button_Down == 1)
  {
    acc = 0;
    servo = SERVO2;

    grad2 = random(1, 180);

    Servo();

    r     = 0;
    g     = 100;
    b     = 0;
    BGC_r = 0;
    BGC_g = 50;
    BGC_b = 20;
  }

  if(magentaobj.button_Left != button_lastLeft && magentaobj.button_Left == 1)
  {
    acc = 0;
    servo = SERVO2;

    grad2 = random(1, 180);

    Servo();

    r     = 70;
    g     = 0;
    b     = 140;
    BGC_r = 20;
    BGC_g = 0;
    BGC_b = 50;
  }  
  
  button_last1 = magentaobj.button_1;
  button_last2 = magentaobj.button_2;
  button_last3 = magentaobj.button_3;
  button_lastCenter = magentaobj.button_Center;
  button_lastUp = magentaobj.button_Up;
  button_lastRight = magentaobj.button_Right;
  button_lastDown = magentaobj.button_Down;
  button_lastLeft = magentaobj.button_Left;
}

void printStats()
{
//   printf("change: %i\n", change);
//   printf("Colors: %i-%i-%i-%i-%i-%i\n", r,g,b,BGC_r, BGC_g, BGC_b);
//   printf("color: %i\n", color);
//   printf("segment: %i\n", segment);
//   printf("Button1: %i\n", magentaobj.button_1);
//   printf("button1_last: %i\n", button_last1);
//   printf("Button2: %i\n", magentaobj.button_2);
//   printf("button2_last: %i\n", button_last2);
//   printf("Button3: %i\n", magentaobj.button_3);
//   printf("button3_last: %i\n", button_last3);
  printf("Button_UP: %i\n", magentaobj.button_Up);
  printf("button_UP_last: %i\n", button_lastUp);
  printf("Button_RIGHT: %i\n", magentaobj.button_Right);
  printf("button_RIGHT_last: %i\n", button_lastRight);
  printf("Button_DOWN: %i\n", magentaobj.button_Down);
  printf("button_DOWN_last: %i\n", button_lastDown);
  printf("Button_LEFT: %i\n", magentaobj.button_Left);
  printf("button_LEFT_last: %i\n", button_lastLeft);
  printf("Button_CENTER: %i\n", magentaobj.button_Center);
  printf("button_CENTER_last: %i\n", button_lastCenter);
  // printf("Acc: %i\n", acc);
  // printf ("Grad 1: %i\n", grad1);
  // printf ("Grad 2: %i\n", grad2);
  // printf("Buzzer: %i", buzzer);
}

void loop() 
{
  delay(100);
  magentaobj.read_io();

  printStats();

  ReadButton();

  switch (segment)
  {
    case NORA:
      magentaobj.setSegmentAnzeige('N', 'O', 'R', 'A');
      break;
  
    case REMO:
      magentaobj.setSegmentAnzeige('R', 'E', 'M', 'O');   
      break;
  }

  switch (acc)
  {
    case ACC:
      Accelerometer();
      break;
    
    default:
        Counter ++;
      if(Counter <= 1)
      {
        magentaobj.sample(1);
      }
      
      if(Counter > 1)
      {
        magentaobj.sample(0);
        if(Counter >= 2)
        {
          Counter = 0;
        }
      }
      break;
  }

  ChangeColor();
 
  magentaobj.setColor(r, g, b, BGC_r, BGC_g, BGC_b);
}
