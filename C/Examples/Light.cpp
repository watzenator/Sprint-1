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

	vector<float> lightBuffer[10]
	int index = 0;
	bool firstRun = true;
	while(true){
		BP.get_sensor(PORT_3, &Light3);
		//printf("Light sensor reflected: %4d", Light3.reflected);
		//printf("\n");
		lightBuffer[index] = Light3.reflected;
		float variance = 0.0;
		float sum = 0;
		if(not firstRun){
			for(int i = 0; i < 10; i++){
			sum += lightBuffer[i];
			}
			float average = sum/10;
			for(int j = 0; j < 10; j++){
				variance += std::pow(lightBuffer[i] - avg, 2);
			}
			variance /= 10;
			float deviation = std::sqrt(variance);
      std::cout << "Deviation: " << deviation << std::endl;
			
		}
		if(index < 9){
			index++;
		}else{
			index = 0
			firstRun = false;
		}
		sleep(1);
	}
}

// Signal handler that will be called when Ctrl+C is pressed to stop the program
void exit_signal_handler(int signo){
  if(signo == SIGINT){
    BP.reset_all();    // Reset everything so there are no run-away motors
    exit(-2);
  }
}
