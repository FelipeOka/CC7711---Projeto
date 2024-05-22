#include <stdio.h>
#include <math.h>
#include <webots/robot.h>
#include <webots/motor.h>
#include <webots/distance_sensor.h>
#include <webots/led.h>
#include <webots/supervisor.h>

#define TIME_STEP 256
#define QtddSensoresProx 8
#define QtddLeds 10

bool caixa_se_moveu(const double *pos_atual, const double *pos_anterior, double limiar) {
  double distancia = sqrt(pow(pos_atual[0] - pos_anterior[0], 2) +
                          pow(pos_atual[1] - pos_anterior[1], 2) +
                          pow(pos_atual[2] - pos_anterior[2], 2));
  return distancia > limiar;
}

bool robot_se_moveu(const double *pos_atual, const double *pos_anterior, double limiar) {
  double distancia = sqrt(pow(pos_atual[0] - pos_anterior[0], 2) +
                          pow(pos_atual[1] - pos_anterior[1], 2) +
                          pow(pos_atual[2] - pos_anterior[2], 2));
  return distancia > limiar;
}

int main(int argc, char **argv) {
  wb_robot_init();

  WbDeviceTag MotorEsquerdo = wb_robot_get_device("left wheel motor");
  WbDeviceTag MotorDireito = wb_robot_get_device("right wheel motor");

  wb_motor_set_position(MotorEsquerdo, INFINITY);
  wb_motor_set_position(MotorDireito, INFINITY);
  wb_motor_set_velocity(MotorEsquerdo, 0);
  wb_motor_set_velocity(MotorDireito, 0);

  WbDeviceTag SensorProx[QtddSensoresProx];
  for (int i = 0; i < QtddSensoresProx; i++) {
    char sensor_name[5];
    sprintf(sensor_name, "ps%d", i);
    SensorProx[i] = wb_robot_get_device(sensor_name);
    wb_distance_sensor_enable(SensorProx[i], TIME_STEP);
  }

  WbDeviceTag Leds[QtddLeds];
  for (int i = 0; i < QtddLeds; i++) {
    char led_name[5];
    sprintf(led_name, "led%d", i);
    Leds[i] = wb_robot_get_device(led_name);
  }
  	
  WbNodeRef caixaLeve = wb_supervisor_node_get_from_def("caixaLeve");
  const double *caixa_position = wb_supervisor_node_get_position(caixaLeve);
  double last_caixa_position[3] = {caixa_position[0], caixa_position[1], caixa_position[2]};

  WbNodeRef robot_node = wb_supervisor_node_get_self();
  const double *robot_position;
  double last_robot_position[3] = {0, 0, 0};
  
  bool caixa_encontrada = false;
  int travamento_contador = 0;
  const int TRAVAMENTO_LIMITE = 5;
  const double LIMIAR_MOVIMENTO = 0.001;

  while (wb_robot_step(TIME_STEP) != -1) {
    if (caixa_encontrada) {
      wb_motor_set_velocity(MotorEsquerdo, 0);
      wb_motor_set_velocity(MotorDireito, 0);
      for (int i = 0; i < QtddLeds; i++) {
        wb_led_set(Leds[i], 1);
      }
      printf("Caixa leve encontrada! Robô parado e LEDs acesos.\n");
      continue;
    }

    caixa_position = wb_supervisor_node_get_position(caixaLeve);
    robot_position = wb_supervisor_node_get_position(robot_node);

    printf("Posição da caixaLeve - x: %f, y: %f, z: %f\n", caixa_position[0], caixa_position[1], caixa_position[2]);
    printf("Posição do robô - x: %f, y: %f, z: %f\n", robot_position[0], robot_position[1], robot_position[2]);

    double distance_to_caixa = sqrt(pow(caixa_position[0] - robot_position[0], 2) + 
                                    pow(caixa_position[1] - robot_position[1], 2) + 
                                    pow(caixa_position[2] - robot_position[2], 2));
    printf("Distância até a caixa: %f\n", distance_to_caixa);

    if (caixa_se_moveu(caixa_position, last_caixa_position, 0.002)) {
      printf("Movimento detectado: A caixa se moveu!\n");
      caixa_encontrada = true;
      continue;
    }

    // Atualiza a posição anterior da caixa
    for (int i = 0; i < 3; i++) {
      last_caixa_position[i] = caixa_position[i];
    }

    if (robot_se_moveu(robot_position, last_robot_position, LIMIAR_MOVIMENTO)) {
      travamento_contador = 0; // Resetar contador de travamento se o robô se moveu
    } else {
      travamento_contador++; // Incrementar contador de travamento se o robô não se moveu
    }

    // Atualiza a posição anterior do robô
    for (int i = 0; i < 3; i++) {
      last_robot_position[i] = robot_position[i];
    }

    double left_speed = 6.28;
    double right_speed = 6.28;

    if (distance_to_caixa < 1.0) {
      left_speed = 3.14;
      right_speed = 3.14;
    }

    double front_left_sensor = wb_distance_sensor_get_value(SensorProx[0]);
    double front_right_sensor = wb_distance_sensor_get_value(SensorProx[7]);

    if (front_left_sensor > front_right_sensor) {
      left_speed *= 0.5;
    } else if (front_right_sensor > front_left_sensor) {
      right_speed *= 0.5;
    }

    if (travamento_contador > TRAVAMENTO_LIMITE) {
      printf("Robô travado! Girando para liberar...\n");
      // Fazer o robô girar para tentar se liberar
      left_speed = -6.28;
      right_speed = 6.28;
      travamento_contador = 0; // Resetar contador de travamento após a manobra
    }

    wb_motor_set_velocity(MotorEsquerdo, left_speed);
    wb_motor_set_velocity(MotorDireito, right_speed);
  }

  wb_robot_cleanup();
  return 0;
}
