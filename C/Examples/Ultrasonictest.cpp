#include "BrickPi3.cpp" // for BrickPi3
#include <stdio.h>      // for printf
#include <unistd.h>     // for usleep
#include <signal.h>     // for catching exit signals
#include <iostream>
using namespace std;

void forward(int8_t& speedL, int8_t& speedR){
	BP.set_motor_power(PORT_C, motorspeed);
	BP.set_motor_power(PORT_B, motorspeed);
	speedL = motorspeed;
	speedR = motorspeed;
}

void left(int8_t& speed){
	BP.set_motor_power(PORT_C, speed--);
	BP.set_motor_power(PORT_B, motorspeed);
}

void right(int8_t& speed){
	BP.set_motor_power(PORT_C, motorspeed);
	BP.set_motor_power(PORT_B, speed--);
}

void brake(){
	BP.set_motor_power(PORT_C, 0);
	BP.set_motor_power(PORT_B, 0);
}


void object(){
		BP.set_motor_power(PORT_B, ((Ultrasonic2.cm - 10) * 2));
		BP.set_motor_power(PORT_A, ((Ultrasonic2.cm - 10) * 2));
}

int main(){
	signal(SIGINT, exit_signal_handler); // register the exit function for Ctrl+C
 
	BP.detect(); // Make sure that the BrickPi3 is communicating and that the firmware is compatible with the drivers.
  
	// Reset the encoders
	BP.offset_motor_encoder(PORT_C, BP.get_motor_encoder(PORT_C));
	BP.offset_motor_encoder(PORT_B, BP.get_motor_encoder(PORT_B));
	
	BP.set_sensor_type(PORT_1, SENSOR_TYPE_NXT_COLOR_FULL);
	BP.set_sensor_type(PORT_2, SENSOR_TYPE_NXT_ULTRASONIC);
	BP.set_sensor_type(PORT_3, SENSOR_TYPE_NXT_LIGHT_ON);
	BP.set_sensor_type(PORT_4, SENSOR_TYPE_TOUCH);

	sensor_color_t Color1;
	sensor_ultrasonic_t Ultrasonic2;
	sensor_light_t Light3;
	sensor_touch_t Touch4;

	int8_t motorspeed = 30;
	int8_t speedLeft = motorspeed;
	int8_t speedRight = motorspeed;

	while(true){
		// Read the encoders
		int32_t EncoderA = BP.get_motor_encoder(PORT_A);
		int32_t EncoderB = BP.get_motor_encoder(PORT_B);

		BP.get_sensor(PORT_1, &Color1);
		BP.get_sensor(PORT_2, &Ultrasonic2);
		BP.get_sensor(PORT_3, &Light3);
		BP.get_sensor(PORT_4, &Touch4);
		
		
		if(Ultrasonic2.cm < 60){
			object();
		}else if(sensorLeft == 1 && sensorRight == 1){
			forward(speedLeft, speedRight);
		}else if(sensorLeft == 1 && sensorRight ==0){
			right(speedLeft);
		}else if(sensorLeft == 0 && sensorRight == 1){
			left(speefRight);
		}else if(sensorLeft == 0 && sensorRight == 0){
			brake();
		}else{
			forward(speedLeft, speedRight);
		}
		
	}
	
}
