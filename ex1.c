
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <memory.h>

#define ALFA  0.012299 // Moon mass / earth mass
#define BETA 0.012299 - 1// we always use it the the function in -B form so I multiplied it here
                        // by -1 an dI will use it as +B in the functions when needed
#define X "x"
#define Y "y"


#define BUFF 100

long double time; // T
long numberOfSteps; // n
long double timelabs; // have to calculate it fromt the input which would be time / number of steps
long prints; //m

FILE* outfile; // this is the output file we wana write to 

char *arVar[7];

/**
 * this struct has the location of the space ship adn tehvelocity on both axis
 */
struct spaceShipInfo
{
    long double locationX; // the location in the x axis this keeps changing
    long double locationY; // the location in the y axis this keeps changing
    long double velocityY; // the velocity in the x axis this keeps changing
    long double velocityX; // the velocity in the y axis this keeps changing
};


long double acceleration(struct spaceShipInfo s, char *isXAxis );
long double distance(struct spaceShipInfo s, double parameter);
void writesInOutputfile(FILE *outfile, long double locX, long double locY);
void newLocation(long double *location, long double *velocity);
int getlongDouble(long double *result, char string[]);
int inputVariables(struct spaceShipInfo* s);
int readfile(FILE* file, struct spaceShipInfo* s);
void newVelocity(long double *velocity, long double acceleration);
int forwardEuler(struct spaceShipInfo* s, long dtime);


/**
 * @param parameter ALFA in case we do D1 OR BETA if we are calculating D2
 * @return
 */
long double distance(struct spaceShipInfo s, double parameter)
{
    return (powl(powl((s.locationX + (parameter)), 2l) + powl(s.locationY, 2l), 1.5l));
}

/**
 *
 * @param isXAxis is 'x' when we are culculationg for X axis and 'y' if Y axis ( instead of having two functions)
 * @return
 */
long double acceleration(struct spaceShipInfo s, char *isXAxis)
{
    long double distance1 = distance(s, ALFA);
    long double distance2 = distance(s, BETA);
    assert( distance1 != 0);
    assert( distance1 != 0);
    if (strcmp(isXAxis , Y))
	{
        return ( s.locationX + (2 * s.velocityY) + ((BETA) * ((s.locationX + ALFA ) / distance1))
                 - ( ALFA * ((s.locationX + (BETA)) / distance2)));
    }
    else if (strcmp(isXAxis , X))
	{
        return ( s.locationY - (2 * s.velocityX) + ((BETA) * ((s.locationY) / distance1))
                 - (ALFA * ((s.locationY) / distance2)));

    }
	else 
	{
        return -1;
    }
}

/**
 *
 * @param the location  of the spaceship on of the X / Y axis
 * @param velocity the speed of the spaceship in x / Y axis Respectfully to the location
 * @return changes the location of the spaceship to the new location after specific time have passed
 */
void newLocation(long double *location, long double *velocity)
{
    *location = (*location + (*velocity * timelabs));
}

/**
 *
 * @param velocity the speed of the spaceship in x / Y axis
 * @param acceleration the acceleration of the spaceship after specifi time passes
 * @return changes the speed to the new speed of the spaceship after specific time have passed
 */
void newVelocity(long double *velocity, long double acceleration)
{
    *velocity = (*velocity + (acceleration * timelabs));
}

/**
 *
 * @param locationX of the spaceship in the X axis
 * @param locationY of the spaceship in the Y axis
 * @param velocityX the speed in X  axis
 * @param velocityY  the speed in Y  axis
 * @param timeSteps how many times we need to check  T / n
 * @return
 */
int forwardEuler(struct spaceShipInfo* s, long dtime)
{
    long double accelerationX = 0;
    long double accelerationY = 0;
    int i;
    int counter =0;
    for ( i = 0; i < dtime; i++ )
	{
		// get the new location since its all in pointers the orginal var changes value

		//calculate acceleration
		accelerationX = acceleration(*s, X);
		accelerationY = acceleration(*s, Y);

		//calculate new location
		newLocation(&s->locationX, &s->velocityX);
		newLocation(&s->locationY, &s->velocityY);

		// calculate new velocity
		newVelocity(&s->velocityX, accelerationX);
		newVelocity(&s->velocityY, accelerationY);


		if ( i % (numberOfSteps / prints ) == 0)
		{
            counter++;
            if ( counter == prints )
            {
                fprintf(outfile, "%.3Le, %.3Le\n", s->locationX, s->locationY);
            }
            else
            {
                writesInOutputfile(outfile, s->locationX, s->locationY);
            }
        }
    }

    return 0;
}

int check ()
{
    if ( time < 0 )
	{
        fprintf(stderr, "Time cant be Negative");
        return -1;
    }
	if ( timelabs < 0 )
	{
		return -1;
	}
    if (numberOfSteps % prints != 0)
    {
        return -1;
    }
    return 0;
}

/**
 *writed to the output file 
 *
 */
void writesInOutputfile(FILE *outfile, long double locX, long double locY)
{
        fprintf(outfile, "%.3Le, %.3Le, ", locX, locY);

}

/**
 * @param result the string reverted to number as ong double 
 * @param string the text we want to convert 
 * @return convert the text to long double and returns -1 in cse the file was not onlt numbers or was invaled in a way 
 */
int getlongDouble(long double *result, char string[])
{

    long length = strlen(string);

    // check if line was > 1000 - overflow error
    int i , n;
    n = 0;
    for (i = 0; i < length; i++)
	{
        if (string[i] == '\n')
		{
            break;
        }
        n++;
    }
    if ((n == length) && (string[length] != '\0'))
	{
        fprintf(stderr, "1, Over flow ");
        return -1;
    }

    // try to assign strtold to result
    char* ptline = 0;
    *result = strtold(string, &ptline);

    // see if checkPtr was not \n - if not we have a bad input
    if ((*ptline != '\n') && (string[length] != '\0'))
	{
        fprintf(stderr, " Over flow ");
        return -1;
    }
    return 0;
}

/**   
 *askes for vals from user
 */
int inputVariables(struct spaceShipInfo* s)
{

    char string[BUFF];
    int returnValue = 0; // if the get longdouble gets -1 in one input it will always be -1;


    printf("Enter initial pos x:\n");
    //read line to char*
    fgets(string, BUFF, stdin);
    returnValue = getlongDouble(&s->locationX, string);


    printf("Enter initial pos y:\n");
    //read line to char*
    fgets(string, BUFF, stdin);
    returnValue = getlongDouble(&s->locationY, string) || returnValue;


    printf("Enter initial vel x:\n");
    //read line to char*
    fgets(string, BUFF, stdin);
    returnValue = getlongDouble(&s->velocityX, string) || returnValue;


    printf("Enter initial vel y:\n");
    //read line to char*
    fgets(string, BUFF, stdin);
    returnValue = getlongDouble(&s->velocityY, string) || returnValue ;


    printf("Enter total time T:\n");
    //read line to char*
    fgets(string, BUFF, stdin);
    returnValue = getlongDouble(&time, string) || returnValue;
    printf("%Lf\n", time);


    printf("Enter num of steps:\n");
    //read line to char*
    fgets(string, BUFF, stdin);
    long double temp = 0;
    returnValue = getlongDouble(&temp, string) || returnValue ;
    numberOfSteps = (long) temp;


    printf("Enter num of steps to save:\n");
    //read line to char*
    fgets(string, BUFF, stdin);
    temp = 0;
    returnValue = getlongDouble(&temp, string) || returnValue;
    prints = (long) temp;
    timelabs = time / numberOfSteps;
    if (check())
	{
        return -1;
    }
    return returnValue;
}

/**
*gets the varibels from the input files and addes them to the global varibles  as numbers and not string 
*file : the input file we get from the arguments of the main 
*/
int readfile(FILE* file, struct spaceShipInfo* s)
{

    char in[BUFF] = "";
    char * token;
    int i = 0;
	int returnValue = 0;
    long double temp = 0;
    const char *stop = "\n / , / < / >";


    // reading the first line
    fgets(in, BUFF, file );
    token = strtok(in, stop);
    while (token != NULL)
	{
        arVar[i] = token;
        token = strtok(NULL, stop);
        i++;
    }
    returnValue = getlongDouble(&s->locationX, arVar[0]) || returnValue;
    returnValue = getlongDouble(&s->locationY, arVar[1]) || returnValue;
    returnValue = getlongDouble(&s->velocityX, arVar[2]) || returnValue;
    returnValue = getlongDouble(&s->velocityY, arVar[3]) || returnValue;


    // reading the second line
    fgets(in, BUFF, file );
    token = strtok(in, stop);
    while (token != NULL)
	{
        arVar[i] = token;
        token = strtok(NULL, stop);
        i++;
    }
    returnValue = getlongDouble(&time, arVar[4]) || returnValue;
    returnValue = getlongDouble(&temp, arVar[5]) || returnValue;
    numberOfSteps = (long) temp;
    temp = 0;
    returnValue = getlongDouble(&temp, arVar[6]) || returnValue;
    prints = (long) temp;

    if (check())
	{
        return -1;
    }
    timelabs = time / numberOfSteps;
    return returnValue;
}

int main(int argc, char **argv) 
{
    int check;
    struct spaceShipInfo s;
    struct spaceShipInfo* sp = &s;
    if(argc == 2)
	{
        outfile = fopen(argv[1], "w");
        if (outfile == NULL ) 
		{
            fprintf(stderr, "Error in input file");
            return(-1);
        } 
		else
        {
	        check = inputVariables(sp);
	        if (check)
			{
	            fprintf(stderr, "wrong input ");
	            fclose(outfile);
	            return(-1);
	        }
	        check = forwardEuler(sp, numberOfSteps);
	        if (check)
			{
	            fprintf(stderr, "something went wrong in the calculation ");
	            fclose(outfile);
	            return(-1);
	        }	
        	fclose(outfile);
    	}
	}
    else if (argc == 3)
	{
        FILE* inFile = fopen(argv[1], "r");
        outfile = fopen(argv[2], "w");
        if (outfile == NULL || inFile == NULL)
		{
            fprintf(stderr, "Error in input file");
            return(-1);
        } 
		else
        {
        	readfile(inFile, sp);
       	 	check = forwardEuler(sp, numberOfSteps);
        	if (check)	
			{
				fprintf(stderr, "something went wrong in the calculation ");
				fclose(outfile);
				return(-1);
        	}
        	fclose(inFile);
        	fclose(outfile);
    	}
    }
    else 
	{
        fprintf(stderr, " wrong configuration ");
    }
	return 0;
}
