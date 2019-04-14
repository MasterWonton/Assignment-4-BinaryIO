/*
* FILE : ca4.cpp
* PROJECT : SENG1000 - Assign 04
* PROGRAMMER : Alexander Sharp
* FIRST VERSION : april 14, 2019
* DESCRIPTION :
*				this program intakes a .txt file containing a list of cities along with their flight times to next, and layover times. 
*				it parses out all of this data and converts it to a readable binary containing 4 byte time info, and a text file containing the city names delimited by carrage returns 
*/
#include <stdio.h>
#include "pch.h"
#include <iostream>
#include <string.h>

#pragma warning(disable: 4996)
#define MAX_CITIES 50
#define MAX_LINE_LENGTH 81

//prototypes
int parseLine(char inputLine[], struct CityInfo *pCityInfo);
int WriteNames(char fileLoc[], struct CityInfo inCities[MAX_CITIES], int length);
int WriteTimes(struct CityInfo inCities[MAX_CITIES], char fileLoc[], int length);
int CountLength(char *str);
int parseTime(char inputTime[], unsigned int *pHours, unsigned int *pMinutes);

//foreward struct declarations
struct MyTime;
struct CityInfo;

//datastructure for time storage
struct MyTime
{
	unsigned int hours;
	unsigned int minutes;
};
//datastructure for cities
struct CityInfo
{
	char cityName[81];
	struct MyTime flightTime;
	struct MyTime layoverTime;
};

//main program entry point
int main(int argc, char *argv[])
{
	if (argc != 2) //check argument count
	{
		printf("Usage: cA4 <inputFile>");
		return 0;
	}

	char *inputloc = argv[1]; //intake argument

	char timesloc[] = "theTimes.dat"; //location for times.dat
	char namesloc[] = "theCities.txt"; //location for cities.dat

	struct CityInfo myCities[MAX_CITIES];

	FILE *pFile;
	pFile = fopen(inputloc, "r");
	

	if (pFile == NULL) //error checking
	{
		printf("Error opening %s", inputloc);
		fclose(pFile);
		return 0;
	}
	
	char lineBuffer[MAX_LINE_LENGTH] = { 0 };
	int Itterator = 0;

	while (!feof(pFile))
	{
		fgets(lineBuffer, MAX_LINE_LENGTH, pFile);
		if(ferror(pFile))//error reading file
		{
			printf("Error reading %s", inputloc);
			fclose(pFile);
			return 0;
		}
		parseLine(lineBuffer, &myCities[Itterator]);

		Itterator++;
	}
	fclose(pFile);
	
	if(WriteNames(namesloc, myCities, Itterator-1) == 1) //populate our names file
	{
		return 0;
	}

	if (WriteTimes(myCities, timesloc, Itterator-1) == 1) //populate our dat file
	{
		return 0;
	}

}

//CountLength: used to detect the end of a string and return its size for buffer.
int CountLength(char *str)
{
	int runningTotal = 0;
	while (str[runningTotal] != NULL)
	{
		runningTotal++;
	}
	return runningTotal;
}

//
//WriteNames: Intake the address of our names in memory 
//			  then write to specified file location
int WriteNames(char fileLoc[], struct CityInfo inCities[MAX_CITIES], int length)
{
	FILE *pFile;
	pFile = fopen(fileLoc, "w");
	if (pFile == NULL) 
	{
		printf("Error writing to %s", fileLoc);
		return 1;
	}
	for (int i = 0; i < length; i++)
	{

		if (CountLength(inCities[i].cityName) != 0)
		{
			fwrite(inCities[i].cityName, CountLength(inCities[i].cityName), 1, pFile);
			if (ferror(pFile))//error checking
			{
				printf("Error writing to %s", fileLoc);
				fclose(pFile);
				return 1;
			}

			fwrite("\n", 1, 1, pFile);
			if (ferror(pFile)) //error checking
			{
				printf("Error writing to %s", fileLoc);
				fclose(pFile);
				return 1;
			}
		}
	}
	fclose(pFile);
	if (ferror(pFile)) //error checking
	{
		printf("Error closing %s", fileLoc);
		fclose(pFile);
	}

	return 0;

}

//WriteTimes: intake the memory adresses of our layover and flight times
//			  and write them to a binary in 2 byte hour minute format
int WriteTimes(struct CityInfo inCities[MAX_CITIES], char fileLoc[], int length)
{
	FILE *pFile;
	pFile = fopen(fileLoc, "wb+");
	if (pFile == NULL)
	{
		printf("Error opening %s", fileLoc);
		fclose(pFile);
		return 1;

	}

	int buffer = 0;
	for (int i = 0; i < MAX_CITIES; i++)
	{
		if (inCities[i].flightTime.hours == NULL)
		{
			break;
		}
		else
		{
			buffer = inCities[i].flightTime.hours;
			fwrite(&buffer, sizeof(char), 1, pFile);
			if (ferror(pFile)) //error checking
			{
				printf("Error writing to %s", fileLoc);
				fclose(pFile);
				return 1;
			}

			buffer = inCities[i].flightTime.minutes;
			fwrite(&buffer, sizeof(char), 1, pFile);
			if (ferror(pFile)) //error checking
			{
				printf("Error writing to %s", fileLoc);
				fclose(pFile);
				return 1;
			}

			buffer = inCities[i].layoverTime.hours;
			fwrite(&buffer, sizeof(char), 1, pFile);
			if (ferror(pFile)) //error checking
			{
				printf("Error writing to %s", fileLoc);
				fclose(pFile);
				return 1;
			}

			buffer = inCities[i].layoverTime.minutes;
			fwrite(&buffer, sizeof(char), 1, pFile);
			if (ferror(pFile)) //error checking
			{
				printf("Error writing to %s", fileLoc);
				fclose(pFile);
				return 1;
			}
		}
	}

	fclose(pFile);
	if (ferror(pFile)) //error checking
	{
		printf("Error closing %s", fileLoc);
		fclose(pFile);
		return 1;
	}
	return 0;
}

//	ParseLine: intake an input string representing one line or airport and
//             return layover time, flight time, and name into pointed locations.
int parseLine(char inputLine[], struct CityInfo *pCityInfo)
{
	int counter = 0;
	int startIndex = 0;
	char buffer[81] = { '\0' };

	//input error checking
	if (strrchr(inputLine, ',') == strchr(inputLine, ',') || strchr(inputLine, ',') == 0 || strchr(inputLine, ',') == NULL || strrchr(inputLine, ',') == strchr(inputLine, '\0') - 1)
	{
		return 0;
	}
	else
	{
		//copy city name to struct
		while (inputLine[counter] != '\n')
		{
			if (inputLine[counter] == ',')
			{
				for (int i = 0; i < counter; i++)
				{
					(*pCityInfo).cityName[i] = inputLine[i];
				}

				(*pCityInfo).cityName[counter] = 0;

				counter++;
				break;
			}
			counter++;
		}

		startIndex = counter;
		int iterator = startIndex;
		//copy flight time to buffer
		while (inputLine[counter] != '\n')
		{
			if (inputLine[counter] == ',')
			{
				for (int i = 0; i < counter - startIndex; i++)
				{
					buffer[i] = inputLine[iterator];
					iterator++;
				}
				buffer[counter - startIndex] = 0;

				counter++;
				break;
			}
			counter++;
		}
		//pass buffer to parsetime and populate flight time in struct
		if (parseTime(buffer, &(pCityInfo->flightTime.hours), &(pCityInfo->flightTime.minutes)) == 0)
		{
			return 0; //error
		}
		startIndex = counter;
		//copy layover time to buffer
		while (inputLine[counter] != '\n')
		{
			buffer[counter - startIndex] = inputLine[counter];
			counter++;
		}

		buffer[counter - startIndex] = 0;
		//pass buffer to parsetime and populate layovertime in struct
		if (parseTime(buffer, &(pCityInfo->layoverTime.hours), &(pCityInfo->layoverTime.minutes)) == 0)
		{
			return 0; //error
		}
		return 1;
	}
}

//FUNCTION parseTime: will take a time in format(hh:mm) and pointers to memory locations to store unsigned integer values for the minutes and hours.
//RETURN: return 0 if any input error occurs, otherwise return 1
int parseTime(char inputTime[], unsigned int *pHours, unsigned int *pMinutes) {
	int count = 0;
	int start = 0;
	char buffer[81] = { 0 };

	//error checking
	if (strchr(inputTime, ':') == 0 || strchr(inputTime, ':') == strchr(inputTime, '\0') - 1 || (strchr(inputTime, ':') == NULL))
	{
		return 0; //error
	}

	else
	{
		//populate buffer with hours
		while (inputTime[count] != '\0')
		{
			if (inputTime[count] == ':')
			{
				for (int i = 0; i < count; i++)
				{
					buffer[i] = inputTime[i];
				}

				count++;
				break;
			}

			count++;
		}

		*pHours = atoi(buffer); //convert hours to int and pass them out
		start = count;

		//flush buffer
		for (int i = 0; i < strlen(buffer); i++)
		{
			buffer[i] = '\0';
		}
		//populate buffer with minutes
		while (inputTime[count] != '\0')
		{
			buffer[count - start] = inputTime[count];
			count++;
		}

		buffer[count - start] = '\0';
		*pMinutes = atoi(buffer);//convert buffer to int and pass it out

		return 1;
	}
}