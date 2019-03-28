#include "BrickPi3.cpp" // for BrickPi3
#include <stdio.h>      // for printf
#include <unistd.h>     // for usleep
#include <signal.h>     // for catching exit signals
#include <iostream>
using namespace std;

BrickPi3 BP;

void exit_signal_handler(int signo);

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

void intersection(int8_t& motorspeed, bool& sensorLeft, bool& sensorRight){
	BP.set_motor_power(PORT_C, 0);
	BP.set_motor_power(PORT_B, 0);
	string keuze = "";
	cout << "Welke kant wil je op: ";
	cin >> keuze;
	cout << "\n";
	if(keuze ==  "links"){
		while(true){
			while(sensorRight == 1){
				BP.set_motor_power(PORT_C, 0);
				BP.set_motor_power(PORT_B, motorspeed);
			}while(sensorRight == 0){
				BP.set_motor_power(PORT_C, 0);
				BP.set_motor_power(PORT_B, motorspeed);
			}while(sensorRight == 1){
				BP.set_motor_power(PORT_C, 0);
				BP.set_motor_power(PORT_B, motorspeed);
			}
			break;
		}
	}else if(keuze == "rechts"){
		while(true){
			while(sensorLeft == 1){
				BP.set_motor_power(PORT_C, motorspeed);
				BP.set_motor_power(PORT_B, 0);
			}while(sensorLeft == 0){
				BP.set_motor_power(PORT_C, motorspeed);
				BP.set_motor_power(PORT_B, 0);
			}while(sensorLeft == 1){
				BP.set_motor_power(PORT_C, motorspeed);
				BP.set_motor_power(PORT_B, 0);
			}
			break;
		}
	}else if(keuze == "rechtdoor"){
		while(true){
			while(sensorRight == 1 && sensorLeft == 1){
				BP.set_motor_power(PORT_C, motorspeed);
				BP.set_motor_power(PORT_B, motorspeed);
			}
			if(sensorRight== 0 && sensorLeft == 0){
				break;
			}
		}
	}
}

void objects(int getal){
		BP.set_motor_power(PORT_B, ((getal - 50) * 2));
		BP.set_motor_power(PORT_C, ((getal - 50) * 2));
}

int main(){
	signal(SIGINT, exit_signal_handler); // register the exit function for Ctrl+C
 
	BP.detect(); // Make sure that the BrickPi3 is communicating and that the firmware is compatible with the drivers.
  
	// Reset the encoders
	int32_t EncoderC = BP.offset_motor_encoder(PORT_C, BP.get_motor_encoder(PORT_C));
	int32_t EncoderB = BP.offset_motor_encoder(PORT_B, BP.get_motor_encoder(PORT_B));
	
	BP.set_sensor_type(PORT_1, SENSOR_TYPE_NXT_COLOR_FULL);
	BP.set_sensor_type(PORT_2, SENSOR_TYPE_NXT_ULTRASONIC);
	BP.set_sensor_type(PORT_3, SENSOR_TYPE_NXT_LIGHT_ON);
	BP.set_sensor_type(PORT_4, SENSOR_TYPE_TOUCH);

	sensor_color_t Color1;
	sensor_ultrasonic_t Ultrasonic2;
	sensor_light_t Light3;
	sensor_touch_t Touch4;

	int8_t motorspeed = 25;
	int8_t speedLeft = motorspeed;
	int8_t speedRight = motorspeed;
	
	bool sensorLeft = false;
	bool sensorRight = false;
	bool sensorTouch = false;
	
	while(true){
		// Read the encoders
		int32_t EncoderC = BP.get_motor_encoder(PORT_C);
		int32_t EncoderB = BP.get_motor_encoder(PORT_B);

		BP.get_sensor(PORT_1, &Color1);
		BP.get_sensor(PORT_2, &Ultrasonic2);
		BP.get_sensor(PORT_3, &Light3);
		BP.get_sensor(PORT_4, &Touch4);
		
		if(Color1.reflected_red < 500){
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
			sensorTouch = true;
		}else{
			sensorTouch = false;
		}
		
		if(sensorTouch == 1){
			brake();
		//}else if(Ultrasonic2.cm < 30){
		//	objects(Ultrasonic2.cm);
		}else if(sensorLeft == 1 && sensorRight == 1){
			forward(speedLeft, speedRight, motorspeed);
		}else if(sensorLeft == 1 && sensorRight ==0){
			right(speedLeft, motorspeed);
		}else if(sensorLeft == 0 && sensorRight == 1){
			left(speedRight, motorspeed);
		}else if(sensorLeft == 0 && sensorRight == 0){
			intersection(motorspeed, sensorLeft, sensorRight);
		}else{
			forward(speedLeft, speedRight, motorspeed);
		}
		
		printf("Encoder C: %6d  B: %6d Red: %6d\n", EncoderC, EncoderB, Color1.reflected_red);
	}
	
}

// Signal handler that will be called when Ctrl+C is pressed to stop the program
void exit_signal_handler(int signo){
	if(signo == SIGINT){
		BP.reset_all();    // Reset everything so there are no run-away motors
		exit(-2);
	}
}
