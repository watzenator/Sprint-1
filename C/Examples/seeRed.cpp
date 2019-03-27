/*
 *  https://www.dexterindustries.com/BrickPi/
 *  https://github.com/DexterInd/BrickPi3
 *
 *  Copyright (c) 2017 Dexter Industries
 *  Released under the MIT license (http://choosealicense.com/licenses/mit/).
 *  For more information, see https://github.com/DexterInd/BrickPi3/blob/master/LICENSE.md
 *
 *  This code is an example for reading the encoders of motors connected to the BrickPi3.
 *
 *  Hardware: Connect NXT sensors to the sensor ports. Color sensor to PORT_1. Ultrasonic sensor to PORT_2. Light sensor to PORT_3. Touch sensor to PORT_4 (EV3 or NXT touch sensor).
 *
 *  Results:  When you run this program, you should see the values for each sensor.
 *
 *  Example compile command:
 *    g++ -o program "sensors_nxt.c"
 *  Example run command:
 *    sudo ./program
 *
 */

#include "BrickPi3.cpp" // for BrickPi3
#include <stdio.h>      // for printf
#include <unistd.h>     // for usleep
#include <signal.h>     // for catching exit signals
#include <iostream>
#include <cmath>

BrickPi3 BP;

void exit_signal_handler(int signo);

int main(){
	signal(SIGINT, exit_signal_handler); // register the exit function for Ctrl+C

	BP.detect(); // Make sure that the BrickPi3 is communicating and that the firmware is compatible with the drivers.

	BP.set_sensor_type(PORT_1, SENSOR_TYPE_NXT_COLOR_FULL);

	sensor_color_t      Color1;
	int16_t cirBuffer[10][5];
	int index = 0;
	bool firstRun = true;
	bool seeBlack = false;
	while(true){
		BP.get_sensor(PORT_1, &Color1);
		//printf("Color sensor (S1): detected %d red %4d green %4d blue %4d ambient %4d", Color1.color, Color1.reflected_red, Color1.reflected_green, Color1.reflected_blue, Color1.ambient);
		//printf("\n");
		cirBuffer[index][0] = Color1.color;
		cirBuffer[index][1] = Color1.reflected_red;
		cirBuffer[index][2] = Color1.reflected_green;
		cirBuffer[index][3] = Color1.reflected_blue;
		cirBuffer[index][4] = Color1.ambient;
		if(not firstRun){
			for(int j = 0; j < 5; j++){
				int sum = 0;
				float variance = 0.0;
				for(int i = 0; i < 10; i++){
					sum += cirBuffer[i][j];
					}
				float avg = (float)sum/10;
				//std::cout << "Avg: " << avg << std::endl;
				for(int i = 0; i < 10; i++){
					variance += std::pow(cirBuffer[i][j] - avg, 2);
				}
				variance = variance/10;
				float stdDiv = std::sqrt(variance);
				std::cout << "stdDiv" << j << ": " << stdDiv << std::endl;
			}
		}
		if(index < 9) {index++;}
		else {index = 0; firstRun = false;}
		nanosleep((const struct timespec[]){{0, 900000000L}}, NULL);
	}
}

// Signal handler that will be called when Ctrl+C is pressed to stop the program
void exit_signal_handler(int signo){
  if(signo == SIGINT){
    BP.reset_all();    // Reset everything so there are no run-away motors
    exit(-2);
  }
}

