#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <math.h>


#include "encoder.h"
#include "uart.h"
#include "pwm.h"
#include "millis.h"
#include "pid.h"

#define PULSES_PER_REV_angle 1000  // Số xung mỗi vòng
#define PULSES_PER_REV_posit 600
#define array_length 50
#define PWM_MAX  240  // Giá trị tối đa của PWM
#define PWM_MIN -240    // Giá trị tối thiểu của PWM

PID_t pid;  // Biến PID

double cart_posit[array_length];

double setpoint = 179.5 ;  // Trung bình của 179 và 182
double Kp = 20 , Ki = 8, Kd = 9.99;
volatile double angle = 0.0;
double angle_array[array_length];
float dt = 1;  
float control_signal;

/*
  // Dead zone: nếu góc nằm trong khoảng 179-182, không cần điều chỉnh
  if (currentPos >= 179 && currentPos <= 182) {
      output = 0;
  }
*/



  void go_right(double pwm_value ) {
    //PWM_SetDutyCycle(0,1,0);
    PWM_SetDutyCycle(1, 1, pwm_value);
    PWM_SetDutyCycle(3, 1, 0);
   //PORTE &= ~(1 << 3);

  }
  
  void go_left(double pwm_value ) {
    PWM_SetDutyCycle(3, 1, pwm_value); // 80% Duty Cycle
    PWM_SetDutyCycle(1, 1, 0);
    //PORTB &= ~(1 << 5);

    //PWM_SetDutyCycle(1,1,0);
  }
  
  void go_stop() {
    PWM_SetDutyCycle(0,1,0);
    PWM_SetDutyCycle(1,1,0);
  }
  void motor_control(int16_t pwm_value){
    if(pwm_value > 0){
      go_left(abs(pwm_value));
    }else if ((pwm_value) < 0)
    {
      go_right(abs(pwm_value));
    }else{
      go_stop();
  }
}
  

ISR(INT0_vect) {
  if ((PINH & (1 << PH6))) {  // Kiểm tra mức logic của PD3
      encoder1_position++;
  } else {
      encoder1_position--;
  }
}

/*
ISR(INT2_vect){
    if(PINH & (1<<PH5)){
        encoder2_position++;
    }else{
        encoder2_position--;
    }
}
    */
void read_value(){
  //char buffer[10];
  angle = ((float)encoder1_position / PULSES_PER_REV_angle) * 360.0;
  while (angle < 0) angle += 360;  // Đưa về phạm vi 0 - 360
  while (angle >= 360) angle -= 360;
  angle_array[array_length] = angle;
  /*
  if (angle > 360.0 || angle < -360.0) {
    angle = fmod(angle, 360);
  }
   */ 
   // Gửi góc qua UART
  // itoa(angle, buffer,10);
   //dtostrf(angle, 6, 2, buffer);
  // UART_send_string(buffer);
  // UART_send_string(" degree\n");

   //cart_posit[array_length] = encoder2_position;
   //char buffer1[10];
   // Gửi vị trí encoder 2 qua UART
   //itoa(cart_posit[array_length], buffer1, 10);
   //UART_send_string(buffer1);
   //UART_send_string("\n");
}

int main(void) {
  PWM_Init(1, 1, FAST_PWM, (1 << CS11)); // Timer1 - OC1A
  PWM_Init(3, 1, FAST_PWM, (1 << CS31)); // Timer3 - OC3A
  UART_init(9600);
    millis_setup();
    encoder_init();
    // Khởi tạo PID
    PID_t pid;
    PID_Init(&pid, Kd, Ki, Kp, PWM_MIN , PWM_MAX); // Giới hạn PWM trong khoảng -255 đến 255
    pid.setpoint = setpoint;
    

    while (1) {
      read_value();  // Đọc giá trị encoder
      
      if(angle_array[array_length] > 160 && angle_array[array_length] <270){
      control_signal = PID_Compute(&pid, angle_array[array_length], dt);
      
      // if (angle_array[array_length] >= 178 && angle_array[array_length] <= 183){
      //   control_signal = 0;
        
      // }else if (angle_array[array_length] > 160 && angle_array[array_length] <270) { 
      //       control_signal = PID_Compute(&pid, angle_array[array_length], dt);
      }else{
        control_signal = 0;
      }
        
      
   motor_control(control_signal); // Gọi motor_control sau khi cập nhật control_signal
      char buffer[10];
      char pwm_buffer[10];
       itoa(angle_array[array_length], buffer,10);
   //dtostrf(angle, 6, 2, buffer);
       UART_send_string(buffer);
        itoa(control_signal, pwm_buffer, 10);
        UART_send_string("  PWM: ");
        UART_send_string(pwm_buffer);
        UART_send_string("\n");
  
      
    }
}
