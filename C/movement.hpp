#ifndef _MOVEMENT_HPP
#define _MOVEMENT_HPP

#include "BrickPi3.cpp" // for BrickPi3
#include <stdio.h>      // for printf
#include <unistd.h>     // for usleep
#include <signal.h>     // for catching exit signals
#include <iostream>

void forward(int8_t& speedL, int8_t& speedR, int8_t& motorspeed){
	BP.set_motor_power(PORT_C, motorspeed);
	BP.set_motor_power(PORT_B, motorspeed);
	speedL = motorspeed;
	speedR = motorspeed;
}

void right(int8_t& speed, int8_t& motorspeed){
	if(speed > - 20){
		speed--;
	}
	BP.set_motor_power(PORT_C, speed);
	BP.set_motor_power(PORT_B, motorspeed+((motorspeed-speed)/5));
}

void left(int8_t& speed, int8_t& motorspeed){
	if(speed > - 20){
		speed--;
	}
	BP.set_motor_power(PORT_C, motorspeed+((motorspeed-speed)/5));
	BP.set_motor_power(PORT_B, speed);
}

void brake(){
	BP.set_motor_power(PORT_C, 0);
	BP.set_motor_power(PORT_B, 0);
}

void intersection(const int8_t& motorspeed, sensor_light_t & light1, sensor_light_t & light2){
	BP.set_motor_power(PORT_C, 0);
	BP.set_motor_power(PORT_B, 0);
	printf("Welke kant wil je op: \n");
	int choice = getchar();

	if(choice == '0'){
		BP.set_motor_power(PORT_C, 15);
		BP.set_motor_power(PORT_B, 15);
		sleep(1);
		BP.set_motor_power(PORT_C, -motorspeed-10);
		BP.set_motor_power(PORT_B, motorspeed+10);
		//sleep(1);
	}else if(choice == '1'){
		BP.set_motor_power(PORT_C, 15);
		BP.set_motor_power(PORT_B, 15);
		sleep(1);
		BP.set_motor_power(PORT_B, -motorspeed-10);
		BP.set_motor_power(PORT_C, motorspeed+10);
		//sleep(1);
	}else if(choice == '2'){
		BP.set_motor_power(PORT_C, motorspeed);
		BP.set_motor_power(PORT_B, motorspeed);
		//sleep(1);
	}
  while( BP.get_sensor(PORT_1, &Light1), BP.get_sensor(PORT_3, &Light1)){
    (void);
  }
}

void objects(const int & distance, const int8_t& motorspeed = 35){
		BP.set_motor_power(PORT_B, ((distance - 5) * 2));
		BP.set_motor_power(PORT_C, ((distance - 5) * 2));
		if(distance <= 10){
			BP.set_motor_power(PORT_C, -motorspeed-10); //TODO:: remove -10 and +10
			BP.set_motor_power(PORT_B, motorspeed+10);
			sleep(1.9);
			BP.set_motor_power(PORT_C, motorspeed);
			BP.set_motor_power(PORT_B, motorspeed);
			sleep(3);
			BP.set_motor_power(PORT_C, motorspeed+10);
			BP.set_motor_power(PORT_B, -motorspeed-10);
			sleep(1.9);
			BP.set_motor_power(PORT_C, motorspeed);
			BP.set_motor_power(PORT_B, motorspeed);
			sleep(5);
			BP.set_motor_power(PORT_C, motorspeed+10);
			BP.set_motor_power(PORT_B, -motorspeed-10);
			sleep(1.9);
			BP.set_motor_power(PORT_C, motorspeed);
			BP.set_motor_power(PORT_B, motorspeed);
			sleep(3);
			BP.set_motor_power(PORT_C, -motorspeed-10);
			BP.set_motor_power(PORT_B, motorspeed+10);
			sleep(1.9);
		}
}

#endif
