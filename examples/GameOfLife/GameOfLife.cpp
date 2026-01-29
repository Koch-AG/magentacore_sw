/**
*   @file   GameOfLife.cpp
*   @author Nora Hueppi (nhueppi@kochag.ch)
*   @date  	14.12.2014
*	@date	14.01.2015
*	@date	29.04.2025	(New project version)
*   @brief  Main File of GameOfLife Example
*   @bug    No known bugs.
*/

#include <Magenta_lib.h>

#define ZERONEIGHBORS   0 
#define ONENEIGHBORS    1
#define TWONEIGHBORS    2
#define THREENEIGHBORS  3
#define FOURNEIGHBORS   4
#define FIVENEIGHBORS   5
#define SIXNEIGHBORS    6
#define SEVENNEIGHBORS  7
#define EIGHTNEIGHBORS  8

typedef struct 
{
    int8_t x;
    int8_t y;
}coordinate_t;

int button_last1        = 0;
int button_last2        = 0;
int button_last3        = 0;

void setup()
{
	//Initalisierung der MagentaCore HW Komponenten
	magentaobj.init();
}

void twoDarray_in_oneDarray(uint8_t idk_data[8][8])
{
    byte hihiArray[8] = {0, 0, 0, 0, 0, 0, 0, 0};

    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            hihiArray[i] |= (idk_data[i][j] << 7-j); // wenn farben implementieren andere lösung finden
        }
    }

    magentaobj.write_array(hihiArray);

}

int neighborsCount(int x, int y, uint8_t data[8][8])
{
    coordinate_t coordinate_list[8];
    uint8_t neighbors = 0;

    coordinate_list[0].x = x-1;
    coordinate_list[0].y = y-1;
    coordinate_list[1].x = x-1;
    coordinate_list[1].y = y;
    coordinate_list[2].x = x-1;
    coordinate_list[2].y = y+1;
    coordinate_list[3].x = x;
    coordinate_list[3].y = y-1;
    coordinate_list[4].x = x;
    coordinate_list[4].y = y+1;
    coordinate_list[5].x = x+1;
    coordinate_list[5].y = y-1;
    coordinate_list[6].x = x+1;
    coordinate_list[6].y = y;
    coordinate_list[7].x = x+1;
    coordinate_list[7].y = y+1;

    for(int i = 0; i < 8; i++)
    {
        coordinate_list[i].x = (coordinate_list[i].x == -1 ? 7 : coordinate_list[i].x);
        coordinate_list[i].x = (coordinate_list[i].x == 8 ? 0 : coordinate_list[i].x);
        coordinate_list[i].y = (coordinate_list[i].y == -1 ? 7 : coordinate_list[i].y);
        coordinate_list[i].y = (coordinate_list[i].y == 8 ? 0 : coordinate_list[i].y);
    }
    
    for(int i = 0; i < 8; i++)
    {
        if(data[coordinate_list[i].x][coordinate_list[i].y] == 1)
        {
            printf("x:%i = y:%i", coordinate_list[i].x, coordinate_list[i].y);
            neighbors += 1;
        }
    }
    printf("\n");

    return neighbors;
}

void HelloNeighbors(uint8_t data[8][8])
{
    uint8_t dataNew[8][8];
    uint8_t LifeGame = 0;
    printf("\n");
    for(int y = 0; y < 8; y++)
    {
        for(int x = 0; x < 8; x++)
        {
            LifeGame = neighborsCount(x, y, data);
            printf("%i ", LifeGame);
            switch (LifeGame)
            {
                case TWONEIGHBORS:
                    dataNew[x][y] = data[x][y];
                break;
                case THREENEIGHBORS:
                    dataNew[x][y] = 1;
                break;
                case ZERONEIGHBORS:
                case ONENEIGHBORS:
                case FOURNEIGHBORS:
                case FIVENEIGHBORS:
                case SIXNEIGHBORS:
                case SEVENNEIGHBORS:
                case EIGHTNEIGHBORS:
                    dataNew[x][y] = 0;
                break;
            }
            // twoDarray_in_oneDarray(data);
            // printf("x: %i = y: %i\n",x, y);
            // printf("%i\n", LifeGame);
        }
        printf("\n");
    }
    for(int x = 0; x < 8; x++)
    {
        for(int y = 0; y < 8; y++)
        {
            data[x][y] = dataNew[x][y];
        }
    }    
} 

void loop()
{
    magentaobj.read_io();

    static uint8_t dataOld[8][8] = {
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 1, 1, 1, 0, 0},
        {0, 1, 0, 0, 0, 1, 0, 0},  //!Nur 1 oder 0 verwenden!
        {0, 0, 0, 0, 0, 1, 0, 0},
        {0, 1, 0, 0, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
    };

    twoDarray_in_oneDarray(dataOld);
    if(magentaobj.button_2 == 1)
    {
        HelloNeighbors(dataOld);
    }
    if(magentaobj.button_3 != button_last3 && magentaobj.button_3 == 1)
    {
        HelloNeighbors(dataOld);
    }
    button_last3 = magentaobj.button_3;
}
