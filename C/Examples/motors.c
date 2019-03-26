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
  BP.set_sensor_type(PORT_4, SENSOR_TYPE_TOUCH);
  
  sensor_touch_t Touch4;
  
  while(true){
    // Read the encoders
    int32_t EncoderA = BP.get_motor_encoder(PORT_A);
    int32_t EncoderB = BP.get_motor_encoder(PORT_B);

    // Use the encoder value from motor A to control motors B, C, and D
   BP.get_sensor(PORT_4, &Touch4);
   printf("Touch sensor (S4): pressed %d   ", Touch4.pressed);
    if(Touch4.pressed == 1){
      cout << BP.set_motor_power(PORT_B, 40) << "\n";
      cout << BP.set_motor_power(PORT_A, 20) << "\n";
  }
   
    
    // Display the encoder values
    printf("Encoder A: %6d  B: %6d\n", EncoderA, EncoderB);
    
    
    // Delay for 20ms
    usleep(200000);
  }
}

// Signal handler that will be called when Ctrl+C is pressed to stop the program
void exit_signal_handler(int signo){
  if(signo == SIGINT){
    BP.reset_all();    // Reset everything so there are no run-away motors
    exit(-2);
  }
}
