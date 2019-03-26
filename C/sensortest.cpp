#include "BrickPi3.cpp" // for BrickPi3
#include <stdio.h>      // for printf
#include <unistd.h>     // for usleep
#include <signal.h>     // for catching exit signals

BrickPi3 BP;

void exit_signal_handler(int signo);

int main(){
  signal(SIGINT, exit_signal_handler); // register the exit function for Ctrl+C
  
  BP.detect(); // Make sure that the BrickPi3 is communicating and that the firmware is compatible with the drivers.
  
  int error;
  
  BP.set_sensor_type(PORT_1, SENSOR_TYPE_NXT_COLOR_FULL);
//  BP.set_sensor_type(PORT_2, SENSOR_TYPE_NXT_ULTRASONIC);
  BP.set_sensor_type(PORT_3, SENSOR_TYPE_NXT_LIGHT_ON);
//  BP.set_sensor_type(PORT_4, SENSOR_TYPE_TOUCH);
  
  sensor_color_t      Color1;
//  sensor_ultrasonic_t Ultrasonic2;
  sensor_light_t      Light3;
//  sensor_touch_t      Touch4;
  
  while(true){
    error = 0;
    
    if(BP.get_sensor(PORT_1, &Color1)){
      error++;
    }else if((Color1.reflected_red < 350) &&(Color1.reflected_green < 350) && (Color1.reflected_blue < 350)){
		printf("Zwarte lijn bij rgb     ");
	}else{
		printf("Wit bij rgb     ");
	}
    
    if(BP.get_sensor(PORT_3, &Light3)){
      error++;
    }else if(Light3.reflected > 1750){
		printf("Zwarte lijn bij reflected     ");
	}else{
		printf("Witte lijn bij reflected     ");
	}

    if(error == 2){
      printf("Waiting for sensors to be configured");
    }
    
    printf("\n");
    
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
