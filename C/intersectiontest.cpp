 // for BrickPi3
#include "movement.hpp"
#include <stdio.h>      // for printf
#include <unistd.h>     // for usleep
#include <signal.h>     // for catching exit signals
#include <iostream>
// #define MAX_MOTORPOWER 100
// #define MIN_MOTORPOWER -100

int main(){


	signal(SIGINT, exit_signal_handler); // register the exit function for Ctrl+C

	BP.detect(); // Make sure that the BrickPi3 is communicating and that the firmware is compatible with the drivers.

	if(!voltageIsSafe){
		//cout << "Battery almost empty, exiting program..." << endl;
		BP.reset_all();
		exit(-5);
	}


	// Reset the encoders
	int32_t EncoderC = BP.offset_motor_encoder(PORT_C, BP.get_motor_encoder(PORT_C));
	int32_t EncoderB = BP.offset_motor_encoder(PORT_B, BP.get_motor_encoder(PORT_B));

	BP.set_sensor_type(PORT_1, SENSOR_TYPE_NXT_LIGHT_ON);
	BP.set_sensor_type(PORT_2, SENSOR_TYPE_NXT_ULTRASONIC);
	BP.set_sensor_type(PORT_3, SENSOR_TYPE_NXT_LIGHT_ON);
	BP.set_sensor_type(PORT_4, SENSOR_TYPE_TOUCH);


	sensor_light_t Light1;
	sensor_ultrasonic_t Ultrasonic2;
	sensor_light_t Light3;
	sensor_touch_t Touch4;

	const int8_t manoeuvreSpeed = 25;
	const int8_t basemotorspeed = 50;
	int8_t speedLeft = basemotorspeed;
	int8_t speedRight = basemotorspeed;

	bool sensorLeft = false;
	bool sensorRight = false;
	bool sensorTouch = false;

	if(!voltageIsSafe){
		printf("Battery almost empty, exiting program...");
		BP.reset_all();
		exit(-5);
	}

	sleep(2);
	while(true){
		// Read the encoders
		//int32_t EncoderC = BP.get_motor_encoder(PORT_C);
		//int32_t EncoderB = BP.get_motor_encoder(PORT_B);

		BP.get_sensor(PORT_1, &Light1);
		BP.get_sensor(PORT_2, &Ultrasonic2);
		BP.get_sensor(PORT_3, &Light3);
		BP.get_sensor(PORT_4, &Touch4);

		if(Light1.reflected > 2000){
			sensorLeft = false;
		}else{
			sensorLeft = true;
		}

		if(Light3.reflected > 2000 ){
			sensorRight = false;
		}else{
			sensorRight = true;
		}

		if(Touch4.pressed == 1){
			BP.reset_all();
			exit(-2);
		//}else if(Ultrasonic2.cm < 30){
		//	objects(Ultrasonic2.cm, manoeuvreSpeed);
		}else if(sensorLeft == 1 && sensorRight ==0){
			right(speedLeft, manoeuvreSpeed);
		}else if(sensorLeft == 0 && sensorRight == 1){
			left(speedRight, manoeuvreSpeed);
		}else if(sensorLeft == 0 && sensorRight == 0){
			intersection(manoeuvreSpeed);
		}
		else{
			forward(speedLeft, speedRight, basemotorspeed);
		}
	}
}




// Signal handler that will be called when Ctrl+C is pressed to stop the program
void exit_signal_handler(int signo){
	if(signo == SIGINT){
		BP.reset_all();    // Reset everything so there are no run-away motors
		exit(-2);
	}
}
