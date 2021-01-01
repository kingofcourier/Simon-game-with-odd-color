/*
 * Author:Joseph Li
 * Student Number:92432731
 * Lab Section:L1V
 * Date:             12/6/2020 12:14:20 AM
 *
 * Purpose: To create a modified Simon Game with only checking the button
 * push and the odd position of the light lit. 
 *
 */
#define _CRT_SECURE_NO_WARNINGS   
#include <stdio.h>    
#include <stdlib.h>  
#include <Windows.h>    
#include <DAQlib.h>
#include <time.h>

#define ON 1
#define OFF 0
#define lose 0
#define win 1
#define switch0 0
#define switch1 1
#define switch2 2
#define switch3 3
#define maxseq 5
int randInt(int lower, int upper) {
	int r;/*generate a random number*/
	int range = upper - lower;
	r = rand() % (range + 1) + lower;
	return r;
}/*generate a random number*/
void generateSequence(int length, int data[]) {
	int index;
	int lower = 0;
	int upper = 3;
	srand((unsigned)time(NULL));
	for (index = 0; index < length; index++)
	{
		data[index] = randInt(lower, upper);
	}

}/*make a sequence with the random number*/

void lit_up(int length, int data_color[])
{
	int index;
	int k = 0;
	for (index = 0; index < length; index++)
	{
		digitalWrite(data_color[index], ON);
		Sleep(800);
		digitalWrite(data_color[index], OFF);
		Sleep(800);
	}

}/*light up the LEDs by following the sequence*/


int check(int turn, int data_color[])
{
	for (int i = 0; i*2< turn; i++)
	{
		int time = millis();
		int key = -1;
		int backcheck = -1;
		while(key==-1&&millis()-time<50000)/*record the key and set a time
										   limit for the answering action*/
		{
			key = record();
		}
		backcheck = 0;/*check if the buttons came back up*/
		while (backcheck != -1)
		{
			backcheck = record();
		}
		if (key != data_color[i*2])
		{
			return lose;
		}
	}

	return win;

}/*check if the button pushed is the same as the LEDs lit up in the odd
 position of the sequence*/


int record(void)
{
	int index;

	int key = -1;
	int switch0_status = digitalRead(switch0);
	int switch1_status = digitalRead(switch1);
	int switch2_status = digitalRead(switch2);
	int switch3_status = digitalRead(switch3);

	if (switch0_status == ON)
	{
		key = 0;
	}
	else if (switch1_status == ON)
	{
		key = 1;
	}
	else if (switch2_status == ON)
	{
		key = 2;
	}
	else if (switch3_status == ON)
	{
		key = 3;
	}
	return key;
}/*record the key pressed*/


void foo(int max_seq/*the length of the random generated sequence*/, int data_color[])
{
	int currentTurn;
	int index2;
	int index3;
	

	generateSequence(max_seq, data_color);
	for (currentTurn = 1; currentTurn <= max_seq; currentTurn++)
	{
		lit_up(currentTurn, data_color);
		if (check(currentTurn, data_color) == lose)
		{
			for (index3 = 0; index3 < 3; index3++)/*flash the red light
												  3 times to indicate a lose*/
			{
				digitalWrite(1, ON);
				Sleep(200);
				digitalWrite(1, OFF);
				Sleep(200);

			}
			index2 = max_seq + 2;
			currentTurn = max_seq + 2;
		}
		Sleep(200);
	}
	if (currentTurn == max_seq+1)
	{
		for (index3 = 0; index3 < 3; index3++)/*flash the green light 3 times
											  to indicate a win*/
		{
			digitalWrite(0, ON);
			Sleep(200);
			digitalWrite(0, OFF);
			Sleep(200);
		}

	}


}/*general fucntion body*/

void controlloop(void)
{
	while (continueSuperLoop())
	{
		int data_color[maxseq] = { 0 };		
		foo(maxseq, data_color);
	}
}/*check if the user wants to quit*/

int main(void)
{
	/* initialize simulator and run controlloop */
	if (setupDAQ(6)) {
		controlloop();
	}
	else {
		printf("Cannot initialize the DAQ\n");
	}
	return 0;
}