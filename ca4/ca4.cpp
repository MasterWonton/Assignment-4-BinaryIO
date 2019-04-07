
#include <stdio.h>
#include "pch.h"
#include <iostream>

#pragma warning(disable: 4996)
#define MAX_CITIES 5
#define MAX_LINE_LENGTH 256
int main(int argc, char *argv[])
{
	void ParseLine(char *InputLine, int *pLayoverTime, int *pFlightTime, char *pName);
	int WriteNames(char fileLoc[], char names[MAX_CITIES][MAX_LINE_LENGTH], int length);
	int WriteTimes(int flightTimes[], int layoverTimes[], char inTimeLoc[]);
	int CountLength(char *str);

	char *inputloc = argv[1]; 

	char timesloc[] = "theTimes.dat"; 
	char namesloc[] = "theCities.txt";

	int flightTimes[MAX_CITIES] = { -1 };
	int layoverTimes[MAX_CITIES] = { -1 };
	char cityNames[MAX_CITIES][MAX_LINE_LENGTH] = { 0 };

	FILE *pFile;
	pFile = fopen(inputloc, "r");
	

	if (pFile == NULL)
	{
		printf("Error reading %s", inputloc);
		return 0;
	}
	
	char lineBuffer[MAX_LINE_LENGTH] = { 0 };
	int Itterator = 0;
	while (!feof(pFile))
	{
		fgets(lineBuffer, MAX_LINE_LENGTH, pFile);
		ParseLine(lineBuffer, &layoverTimes[Itterator], &flightTimes[Itterator], cityNames[Itterator]);

		Itterator++;
	}
	fclose(pFile);
	
	WriteTimes(flightTimes, layoverTimes, timesloc);
	WriteNames(namesloc, cityNames, MAX_CITIES);
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
int WriteNames(char fileLoc[], char names[MAX_CITIES][MAX_LINE_LENGTH], int length)
{
	FILE *pFile;
	pFile = fopen(fileLoc, "w");
	if (pFile == NULL) 
	{
		printf("Error opening %s", fileLoc);
		return 1;
	}
	for (int i = 0; i < length; i++)
	{
		fwrite(names[i], CountLength(names[i]), 1, pFile);
		fwrite("\n", 1, 1, pFile);
	}
	fclose(pFile);
	return 0;

}

//WriteTimes: intake the memory adresses of our layover and flight times
//			  and write them to a binary in 2 byte hour minute format
int WriteTimes(int flightTimes[], int layoverTimes[], char fileLoc[])
{
	FILE *pFile;
	pFile = fopen(fileLoc, "wb+");
	if (pFile == NULL)
	{
		printf("Error opening %s", fileLoc);
		return 1;
	}

	int buffer = 0;
	for (int i = 0; i < sizeof(flightTimes); i++)
	{
		if (flightTimes[i] == -1)
		{
			break;
		}
		else
		{
			buffer = flightTimes[i] / 60;
			fwrite(&buffer, sizeof(char), 1, pFile);

			buffer = flightTimes[i] % 60;
			fwrite(&buffer, sizeof(char), 1, pFile);

			buffer = layoverTimes[i] / 60;
			fwrite(&buffer, sizeof(char), 1, pFile);

			buffer = layoverTimes[i] % 60;
			fwrite(&buffer, sizeof(char), 1, pFile);
		}
	}

	fclose(pFile);
	return 0;
}

//	ParseLine: intake an input string representing one line or airport and
//             return layover time, flight time, and name into pointed locations.
void ParseLine(char *InputLine, int *pLayoverTime, int *pFlightTime, char *pName)
{
	char delim[] = ",:";
	int hours = 0;
	int minutes = 0;

	char *ptr = strtok(InputLine, delim);
	strncpy(pName, ptr, strlen(ptr));


	ptr = strtok(NULL, delim);
	hours = atoi(ptr);
	ptr = strtok(NULL, delim);
	minutes = atoi(ptr);

	*pFlightTime = ((hours * 60) + minutes);

	ptr = strtok(NULL, delim);
	hours = atoi(ptr);
	ptr = strtok(NULL, delim);
	minutes = atoi(ptr);

	*pLayoverTime = ((hours * 60) + minutes);
}
