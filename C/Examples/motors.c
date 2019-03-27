#include "BrickPi3.cpp" // for BrickPi3
#include <stdio.h>      // for printf
#include <unistd.h>     // for usleep
#include <signal.h>     // for catching exit signals
#include <iostream>
using namespace std;

BrickPi3 BP;

void exit_signal_handler(int signo);

int main(){
	signal(SIGINT, exit_signal_handler); // register the exit function for Ctrl+C
 
	BP.detect(); // Make sure that the BrickPi3 is communicating and that the firmware is compatible with the drivers.
  
	// Reset the encoders
	BP.offset_motor_encoder(PORT_A, BP.get_motor_encoder(PORT_A));
	BP.offset_motor_encoder(PORT_B, BP.get_motor_encoder(PORT_B));
	
	BP.set_sensor_type(PORT_1, SENSOR_TYPE_NXT_COLOR_FULL);
	BP.set_sensor_type(PORT_2, SENSOR_TYPE_NXT_ULTRASONIC);
	BP.set_sensor_type(PORT_3, SENSOR_TYPE_NXT_LIGHT_ON);
	BP.set_sensor_type(PORT_4, SENSOR_TYPE_TOUCH);

	sensor_color_t Color1;
	sensor_ultrasonic_t Ultrasonic2;
	sensor_light_t Light3;
	sensor_touch_t Touch4;

	int8_t motorspeed = 20;

	while(true){
		// Read the encoders
		int32_t EncoderA = BP.get_motor_encoder(PORT_A);
		int32_t EncoderB = BP.get_motor_encoder(PORT_B);

		BP.get_sensor(PORT_1, &Color1);
		BP.get_sensor(PORT_2, &Ultrasonic2);
		BP.get_sensor(PORT_3, &Light3);
		BP.get_sensor(PORT_4, &Touch4);


		printf("Touch sensor (S4): pressed %d   ", Touch4.pressed);
		if(Touch4.pressed == 1){
			BP.set_motor_power(PORT_B, motorspeed);
			BP.set_motor_power(PORT_A, motorspeed);
		}

		if((Color1.reflected_red < 375) && (Color1.reflected_green < 375) && (Color1.reflected_blue < 350)){
			BP.set_motor_power(PORT_B, 0);
		}else if((Color1.reflected_red > 375) && (Color1.reflected_green > 375) && (Color1.reflected_blue > 350)){
			BP.set_motor_power(PORT_A, motorspeed);
		}

		if(Light3.reflected > 1750){
			BP.set_motor_power(PORT_A, 0);
		}else if(Light3.reflected < 1750){
			BP.set_motor_power(PORT_A, motorspeed);
		}


		printf("Ultrasonic sensor (S2): CM %5.1f Inches %5.1f   ", Ultrasonic2.cm, Ultrasonic2.inch);
		if(Ultrasonic2.cm < 60){
			BP.set_motor_power(PORT_B, ((Ultrasonic2.cm - 10) * 2);
			BP.set_motor_power(PORT_A, ((Ultrasonic2.cm - 10) * 2));
		}


		// Display the encoder values
		printf("Encoder A: %6d  B: %6d\n", EncoderA, EncoderB);
    
    
		// Delay for 20ms
		usleep(20000);
	}
}

// Signal handler that will be called when Ctrl+C is pressed to stop the program
void exit_signal_handler(int signo){
	if(signo == SIGINT){
		BP.reset_all();    // Reset everything so there are no run-away motors
		exit(-2);
	}
}
