#include "STK8321.h"
#include <Wire.h>

#define PID_SIZE	16
uint8_t chipid_temp = 0x00;
uint8_t stk8321_pid_list[PID_SIZE] = {STK8321_CHIPID_VAL, STK8BA50_X_CHIPID_VAL, STK8BA5X_CHIPID_VAL, STK8327_CHIPID_VAL};

void STK8321::ReadAccRegister(uint8_t reg, uint8_t *data)
{
    Wire.beginTransmission(STK8321_SLAVE_ADDRESS);
    Wire.write(reg);
    Wire.endTransmission();

    Wire.requestFrom(STK8321_SLAVE_ADDRESS, 1);    // request 1 bytes from slave device
    *data = Wire.read(); // receive a byte
}

void STK8321::WriteAccRegister(uint8_t reg, uint8_t data)
{
    Wire.beginTransmission(STK8321_SLAVE_ADDRESS);
    Wire.write(reg);
    Wire.write(data);
    Wire.endTransmission();
}

/*
 * Anymotion init register
 * Must be placed after the software reset
 *
 */
void STK8321::STK8321_Anymotion_init()
{
	unsigned char ARegAddr, ARegWriteValue;

	/* Enable X Y Z-axis any-motion (slope) interrupt */
    ARegAddr       = STK8321_REG_INTEN1;
    ARegWriteValue = STK8321_VAL_SLP_EN_X | STK8321_VAL_SLP_EN_Y | STK8321_VAL_SLP_EN_Z;
    WriteAccRegister(ARegAddr, ARegWriteValue);

	/* Set anymotion Interrupt trigger threshold */
    ARegAddr       = STK8321_REG_SLOPETHD;
    ARegWriteValue = STK8321_VAL_SLP_DFLT;
    WriteAccRegister(ARegAddr, ARegWriteValue);

	/* Enable any-motion */
    ARegAddr       = STK8321_REG_SIGMOT2;
    ARegWriteValue = STK8321_VAL_ANY_MOT_EN;
    WriteAccRegister(ARegAddr, ARegWriteValue);

	/* Map any-motion (slope) interrupt to INT1 */
    ARegAddr       = STK8321_REG_INTMAP1;
    ARegWriteValue = STK8321_VAL_ANYMOT2INT1;
    WriteAccRegister(ARegAddr, ARegWriteValue);
}

/*
 * Sigmotion init register
 * Must be placed after the software reset
 *
 */
void STK8321::STK8321_Sigmotion_init()
{
	unsigned char SRegAddr, SRegWriteValue;

	/* Enable X Y Z-axis sig-motion (slope) interrupt */
    SRegAddr       = STK8321_REG_INTEN1;
    SRegWriteValue = STK8321_VAL_SLP_EN_X | STK8321_VAL_SLP_EN_Y | STK8321_VAL_SLP_EN_Z;
    WriteAccRegister(SRegAddr, SRegWriteValue);

	/* Set sig-motion Interrupt trigger threshold */
    SRegAddr       = STK8321_REG_SLOPETHD;
    SRegWriteValue = STK8321_VAL_SLP_DFLT;
    WriteAccRegister(SRegAddr, SRegWriteValue);

	/* Enable significant motion */
    SRegAddr       = STK8321_REG_SIGMOT2;
    SRegWriteValue = STK8321_VAL_SIG_MOT_EN;
    WriteAccRegister(SRegAddr, SRegWriteValue);

	/* Map significant motion interrupt to INT1 */
    SRegAddr       = STK8321_REG_INTMAP1;
    SRegWriteValue = STK8321_VAL_SIGMOT2INT1;
    WriteAccRegister(SRegAddr, SRegWriteValue);
}

/*
 * Diable motion register
 * Must be placed after the software reset
 *
 */
void STK8321::STK8321_Disable_Motion()
{
	unsigned char ARegAddr, ARegWriteValue;

	/* Disable X Y Z-axis motion (slope) interrupt */
    ARegAddr       = STK8321_REG_INTEN1;
    ARegWriteValue = 0x00;
    WriteAccRegister(ARegAddr, ARegWriteValue);

	/* Disable motion */
    ARegAddr       = STK8321_REG_SIGMOT2;
    ARegWriteValue = 0x00;
    WriteAccRegister(ARegAddr, ARegWriteValue);
}


/* Disable Gsensor */
void STK8321::STK8321_Suspend_mode()
{
	unsigned char RegAddr, RegWriteValue;

    /* suspend mode enable */
	RegAddr       = STK8321_REG_POWMODE;
    RegWriteValue = STK8321_VAL_SUSPEND;
    WriteAccRegister(RegAddr, RegWriteValue);
}

bool STK8321::STK8321_Check_chipid()
{
	uint8_t RegAddr = STK_REG_CHIPID;
    int i = 0, pid_num = (sizeof(stk8321_pid_list) / sizeof(stk8321_pid_list[0]));

    ReadAccRegister(RegAddr, &chipid_temp);
    for (i = 0; i < pid_num; i++)
    {
        if (chipid_temp == stk8321_pid_list[i])
        {
        	//("read stkchip id ok, chip_id = 0x%x", chipid_temp);
            return true;
        }
    }
	//ERRLN("read stkchip id fail!");
    return false;
}

/*
 * Initializes an example function
 * The initial configuration is in active mode.
 * STK8321 is in the gear of ODR=2000Hz, and the range is set to +/-4G
 *
 */
int STK8321::STK8321_Initialization()
{
    unsigned char RegAddr, RegWriteValue;

	if(!STK8321_Check_chipid())
	{
		return -1;
    }

    /* soft-reset */
	RegAddr       = STK8321_REG_SWRST;
    RegWriteValue = STK8321_VAL_RST_DFLTS;
    WriteAccRegister(RegAddr, RegWriteValue);
	delay(50);//unit ms

    /* set range, resolution */
    RegAddr       = STK8321_REG_RANGESEL;
    RegWriteValue = STK8321_RANGE_4G; // range = +/-4g
    WriteAccRegister(RegAddr, RegWriteValue);

	/* set power mode */
	RegAddr 	  = STK8321_REG_POWMODE;
	RegWriteValue = STK8321_VAL_SLEEP_05;	// active mode
	WriteAccRegister(RegAddr, RegWriteValue);

	/* set bandwidth */
    RegAddr       = STK8321_REG_BWSEL;
    RegWriteValue = STK8321_VAL_BW_1000; // bandwidth = 1000Hz
    WriteAccRegister(RegAddr, RegWriteValue);

	//STK8321_Anymotion_init();
	//STK8321_Sigmotion_init();

    /* set i2c watch dog */
    RegAddr       = STK8321_REG_INTFCFG;
    RegWriteValue = STK8321_VAL_I2C_WDT_EN; // enable watch dog
    WriteAccRegister(RegAddr, RegWriteValue);

    /* int config */
    RegAddr       = STK8321_REG_INTCFG1;
    RegWriteValue = STK8321_VAL_INT_LV; // INT1/INT2 push-pull, active high
    WriteAccRegister(RegAddr, RegWriteValue);

	return chipid_temp;
}

int STK8321::STK8321_Get_Sensitivity()
{
    int sensitivity = 0;
	if(0x86 == chipid_temp)
	{
	   //resolution = 10 bit
       sensitivity = 1 << 9;
	}
    else
    {
       //resolution = 12 bit
       sensitivity = 1 << 11;
	}
    //range = +/-4g
    sensitivity = sensitivity / 4;
    return sensitivity;
}

/* Read data from registers */
void STK8321::STK8321_Getregister_data(float *X_DataOut, float *Y_DataOut, float *Z_DataOut)
{
    uint8_t RegAddr, RegReadValue[2];
    int16_t x, y, z;

    RegAddr      	= STK8321_REG_XOUT1;
    RegReadValue[0] = 0x00;
	ReadAccRegister(RegAddr, &RegReadValue[0]);
    RegAddr      	= STK8321_REG_XOUT2;
    RegReadValue[1] = 0x00;
	ReadAccRegister(RegAddr, &RegReadValue[1]);
	x = (short int)(RegReadValue[1] << 8 | RegReadValue[0]);

	RegAddr      	= STK8321_REG_YOUT1;
    RegReadValue[0] = 0x00;
	ReadAccRegister(RegAddr, &RegReadValue[0]);
    RegAddr      	= STK8321_REG_YOUT2;
    RegReadValue[1] = 0x00;
	ReadAccRegister(RegAddr, &RegReadValue[1]);
	y = (short int)(RegReadValue[1] << 8 | RegReadValue[0]);

	RegAddr      	= STK8321_REG_ZOUT1;
    RegReadValue[0] = 0x00;
	ReadAccRegister(RegAddr, &RegReadValue[0]);
    RegAddr      	= STK8321_REG_ZOUT2;
    RegReadValue[1] = 0x00;
	ReadAccRegister(RegAddr, &RegReadValue[1]);
	z = (short int)(RegReadValue[1] << 8 | RegReadValue[0]);

	if(0x86 == chipid_temp)
	{
		//resolution = 10 bit
        x >>= 6;
        *X_DataOut = (float) x / STK8321_Get_Sensitivity();

        y >>= 6;
        *Y_DataOut = (float) y / STK8321_Get_Sensitivity();

        z >>= 6;
        *Z_DataOut = (float) z / STK8321_Get_Sensitivity();
	}
    else
    {
        //resolution = 12 bit
        x >>= 4;
        *X_DataOut = (float) x / STK8321_Get_Sensitivity();

        y >>= 4;
        *Y_DataOut = (float) y / STK8321_Get_Sensitivity();

        z >>= 4;
        *Z_DataOut = (float) z / STK8321_Get_Sensitivity();
	}
}