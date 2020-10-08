//Helix G-Code Parser

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <Titan.h>
#include <TitanMain.h>
//#include "HelixParser.h"

//extern struct node *current;
//extern struct node *input;
//extern struct node *head;
//extern struct node *tail;

struct instruction parseLine(struct node *input)
{
    struct instruction data;
    char trimLine[NXD_MQTT_MAX_MESSAGE_LENGTH];
    char *tmp;
    int i, j = 0;

    //trim string
    for (i = 0; i < NXD_MQTT_MAX_MESSAGE_LENGTH; i++)
    {
        if (!isspace(input->content[i]))
        {
            trimLine[j] = input->content[i];
            j++;
        }
    }
    //replace Es with Ds
    //required for proper functioning of ASCII to float
    for (i = 0; i < NXD_MQTT_MAX_MESSAGE_LENGTH; i++)
    {
        if (trimLine[i] == 'E' || trimLine[i] == 'e')
            trimLine[i] = 'D';
    }

    data.cmd[0] = input->content[0];
    data.cmd[1] = input->content[1];
    data.cmd[2] = input->content[2];
    data.cmd[3] = 0;

    memcpy (data.cmdString, input->content, NXD_MQTT_MAX_MESSAGE_LENGTH);

    if (strcmp (data.cmd, "G01") == 0)
    {
        //get position and speed settings for each motor
        data.z = returnNumber (trimLine, 'z');
        data.x = returnNumber (trimLine, 'x');
        data.y = returnNumber (trimLine, 'y');
        data.a = returnNumber (trimLine, 'a');
        data.f = returnNumber (trimLine, 'f');
    }
    else if (strcmp (data.cmd, "G10") == 0)
    {
        ///get temperature settings for each tool
        data.a = returnNumber (trimLine, 'a');
    }
    else if (strcmp (data.cmd, "G28") == 0)
    {

    }
    else if (strcmp (data.cmd, "G54") == 0)
    {

    }
    else if (strcmp (data.cmd, "MSS") == 0)
    {
        data.x = returnNumber (trimLine, 'x');
        data.y = returnNumber (trimLine, 'y');
        data.z = returnNumber (trimLine, 'z');
        data.a = returnNumber (trimLine, 'a');

        data.f = returnNumber (trimLine, 'f');
        data.j = returnNumber (trimLine, 'j');
    }
    else if ((strcmp (data.cmd, "M03") == 0) || (strcmp (data.cmd, "M04") == 0))
    {
//        char searchStr[5] = "HIGH";
        data.x = returnNumber (trimLine, 'x');
        data.y = returnNumber (trimLine, 'y');
        data.z = returnNumber (trimLine, 'z');
        data.a = returnNumber (trimLine, 'a');
        data.f = returnNumber (trimLine, 'f');
    }
    else if (strcmp (data.cmd, "RLY") == 0)
    {
        data.a = returnNumber (trimLine, 'a');
        data.b = returnNumber (trimLine, 'b');
        data.c = returnNumber (trimLine, 'c');
        data.d = returnNumber (trimLine, 'd');
    }
    else if (strcmp (data.cmd, "CAL") == 0)
    {
//        char searchStr[5] = "HIGH";
        data.x = returnNumber (trimLine, 'x');
        data.y = returnNumber (trimLine, 'y');
        data.z = returnNumber (trimLine, 'z');
        data.a = returnNumber (trimLine, 'a');
        data.f = returnNumber (trimLine, 'f');
    }
    else
    {
        if(DEBUG) printf ("\nInvalid instruction - Parser");
    }
    return data;
}

double returnNumber(char * searchString, char searchChar){
    char *tmp;


//    switch to upper if lower
    if (searchChar > 96 && searchChar < 123)
        searchChar -= 32;

    //check for upper or lower
    if (tmp = strchr (searchString + 3, searchChar))
    {
        //has upper
        //check for valid data
        if (isdigit(*(tmp + 1)) || (*(tmp + 1) == '.' && isdigit(*(tmp + 2)))
                || (*(tmp + 1) == '-' && isdigit(*(tmp + 2)))
                || (*(tmp + 1) == '-' && *(tmp + 2) == '.' && isdigit(*(tmp + 3))))
        {
            //number field is a digit OR is a decimal + digit OR is a negative + digit OR  is a negative + decimal + digit
            return atof (tmp + 1);

        }
        else
        {
//            invalid data
            return ~0; //return error code

        }
    }
    else if (tmp = strchr (searchString +3, searchChar - 32))
    {
        //check for valid data
        if (isdigit(*(tmp + 1)) || (*(tmp + 1) == '.' && isdigit(*(tmp + 2)))
                || (*(tmp + 1) == '-' && isdigit(*(tmp + 2)))
                || (*(tmp + 1) == '-' && *(tmp + 2) == '.' && isdigit(*(tmp + 3))))
        {
            //number field is a digit OR is a decimal + digit OR is a negative + digit OR  is a negative + decimal + digit
            return atof (tmp + 1);

        }
        else
        {
            //            invalid data
            return ~0; //return error code

        }
    }

    return ~0;

}
