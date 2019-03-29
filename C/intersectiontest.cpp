#include "BrickPi3.cpp" // for BrickPi3
#include <stdio.h>      // for printf
#include <unistd.h>     // for usleep
#include <signal.h>     // for catching exit signals
//#include <iostream>
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
	//std::string keuze = "links";
	printf("Welke kant wil je op: \n");
	int choice = getchar();

	if(choice == '0'){
		BP.set_motor_power(PORT_C, 15);
		BP.set_motor_power(PORT_B, 15);
		sleep(1);
		BP.set_motor_power(PORT_C, -motorspeed-10);
		BP.set_motor_power(PORT_B, motorspeed+10);
		sleep(1);
	}else if(choice == '1'){
		BP.set_motor_power(PORT_C, motorspeed);
		BP.set_motor_power(PORT_B, 0);
		sleep(1);
		BP.set_motor_power(PORT_C, motorspeed);
		BP.set_motor_power(PORT_B, 0);
		sleep(1);
		BP.set_motor_power(PORT_C, motorspeed);
		BP.set_motor_power(PORT_B, 0);
		sleep(1);
	}else if(choice == '2'){
		BP.set_motor_power(PORT_C, motorspeed);
		BP.set_motor_power(PORT_B, motorspeed);
		sleep(1);
	}
}

void objects(int getal, int8_t& motorspeed){
		BP.set_motor_power(PORT_B, ((getal - 5) * 2));
		BP.set_motor_power(PORT_C, ((getal - 5) * 2));
		if(getal <= 10){
			BP.set_motor_power(PORT_C, -motorspeed-10);
			BP.set_motor_power(PORT_B, motorspeed+10);
			sleep(1.5);
			BP.set_motor_power(PORT_C, motorspeed);
			BP.set_motor_power(PORT_B, motorspeed);
			sleep(4);
			BP.set_motor_power(PORT_C, motorspeed+10);
			BP.set_motor_power(PORT_B, -motorspeed-10);
			sleep(1.5);
			BP.set_motor_power(PORT_C, motorspeed);
			BP.set_motor_power(PORT_B, motorspeed);
			sleep(4);
			BP.set_motor_power(PORT_C, motorspeed+10);
			BP.set_motor_power(PORT_B, -motorspeed-10);
			sleep(1.5);
			BP.set_motor_power(PORT_C, motorspeed);
			BP.set_motor_power(PORT_B, motorspeed);
			sleep(4);
			BP.set_motor_power(PORT_C, -motorspeed-10);
			BP.set_motor_power(PORT_B, motorspeed+10);
			sleep(1.5);
		}
}

bool voltageIsSafe(){
	printf("Battery voltage : %.3f\n", BP.get_voltage_battery());
  	printf("9v voltage      : %.3f\n", BP.get_voltage_9v());
  	printf("5v voltage      : %.3f\n", BP.get_voltage_5v());
  	printf("3.3v voltage    : %.3f\n", BP.get_voltage_3v3());

	if(BP.get_voltage_battery() < 10.9){
		return false;
	}
	return true;
}

int main(){
	signal(SIGINT, exit_signal_handler); // register the exit function for Ctrl+C
 
	BP.detect(); // Make sure that the BrickPi3 is communicating and that the firmware is compatible with the drivers.
  
	// Reset the encoders
	int32_t EncoderC = BP.offset_motor_encoder(PORT_C, BP.get_motor_encoder(PORT_C));
	int32_t EncoderB = BP.offset_motor_encoder(PORT_B, BP.get_motor_encoder(PORT_B));
	
	BP.set_sensor_type(PORT_1, SENSOR_TYPE_NXT_LIGHT_ON);
	BP.set_sensor_type(PORT_2, SENSOR_TYPE_NXT_ULTRASONIC);
	BP.set_sensor_type(PORT_3, SENSOR_TYPE_NXT_LIGHT_ON);
	BP.set_sensor_type(PORT_4, SENSOR_TYPE_TOUCH);

	sensor_light_t Light1;//verander dit
	sensor_ultrasonic_t Ultrasonic2;
	sensor_light_t Light3;
	sensor_touch_t Touch4;

	int8_t motorspeed = 25;
	int8_t speedLeft = motorspeed;
	int8_t speedRight = motorspeed;

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
		int32_t EncoderC = BP.get_motor_encoder(PORT_C);
		int32_t EncoderB = BP.get_motor_encoder(PORT_B);

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
			brake();
		}else if(Ultrasonic2.cm < 30){
			objects(Ultrasonic2.cm, motorspeed);
		}else if(sensorLeft == 1 && sensorRight ==0){
			right(speedLeft, motorspeed);
		}else if(sensorLeft == 0 && sensorRight == 1){
			left(speedRight, motorspeed);
		}else if(sensorLeft == 0 && sensorRight == 0){
			intersection(motorspeed, sensorLeft, sensorRight);
		}
		else{
			forward(speedLeft, speedRight, motorspeed);
		}

		printf("Encoder C: %6d  B: %6d Left: %6d Right: %6d \n", EncoderC, EncoderB, Light3.reflected, Light1.reflected);
		printf("Ultrasonic sensor (S2): CM %5.1f ", Ultrasonic2.cm);
	}
}

// Signal handler that will be called when Ctrl+C is pressed to stop the program
void exit_signal_handler(int signo){
	if(signo == SIGINT){
		BP.reset_all();    // Reset everything so there are no run-away motors
		exit(-2);
	}
}
