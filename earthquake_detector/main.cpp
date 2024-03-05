#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <math.h>

#define SAMPLES 100
#define SENSITIVITY 1

#define BLUE_LED_PIN 12
#define RED_LED_PIN 10
#define BUZZER_PIN  13

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified();

double x_max_threshold = 0;
double y_max_threshold = 0;
double z_max_threshold = 0;

double x_min_threshold = 0;
double y_min_threshold = 0;
double z_min_threshold = 0;

double find_min(const double *arr)
{
  double min = arr[0];
  for (int i = 1; i < SAMPLES; i++) {
    if (arr[i] < min) {
      min = arr[i];
    }
  }
  return min;
}

double find_max(const double *arr) 
{
  double max = arr[0];
  for (int i = 1; i < SAMPLES; i++) {
    if (arr[i] > max) {
      max = arr[i];
    }
  }
  return max;
}

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);

  Serial.begin(9600); 
  while(!accel.begin()) {
      Serial.println("No valid sensor found");
  }

  double x_axis_values[SAMPLES] = {0};
  double y_axis_values[SAMPLES] = {0};
  double z_axis_values[SAMPLES] = {0};
  sensors_event_t event; 

  for (int i = 0; i < SAMPLES; i++) {
    accel.getEvent(&event);
    x_axis_values[i] = event.acceleration.x;
    y_axis_values[i] = event.acceleration.y;
    z_axis_values[i] = event.acceleration.z;
  }

  x_max_threshold = find_max(x_axis_values) + SENSITIVITY;
  y_max_threshold = find_max(y_axis_values) + SENSITIVITY;
  z_max_threshold = find_max(z_axis_values) + SENSITIVITY;

  x_min_threshold = find_min(x_axis_values) - SENSITIVITY;
  y_min_threshold = find_min(y_axis_values) - SENSITIVITY;
  z_min_threshold = find_min(z_axis_values) - SENSITIVITY;
}

void earthquake_detected(void)
{
  int alarm_sec  = 2;
  int time_start = millis();
  int time_end   = millis();
  while ((time_end - time_start) < alarm_sec*1000) {
    tone(13, 1000, 30);
    time_end = millis();
  }
}

void earthquake_led_mode(void)
{
  digitalWrite(RED_LED_PIN, HIGH);
  digitalWrite(BLUE_LED_PIN, LOW);
}

void operating_led_mode(void)
{
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(BLUE_LED_PIN, HIGH);
}

void loop() {
  operating_led_mode();
  sensors_event_t event; 
  accel.getEvent(&event);
  double current_x;
  double current_y;
  double current_z;

  bool is_x_moved;
  bool is_y_moved;
  bool is_z_moved;

  Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print(" | ");
  Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print(" | ");
  Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print(" | ");
  Serial.println("m/s^2 ");

  current_x = event.acceleration.x;
  current_y = event.acceleration.y;
  current_z = event.acceleration.z;

  is_x_moved = (current_x > x_max_threshold) || (current_x < x_min_threshold);
  is_y_moved = (current_y > y_max_threshold) || (current_y < y_min_threshold);
  is_z_moved = (current_z > z_max_threshold) || (current_z < z_min_threshold);

  if (is_x_moved || is_y_moved || is_z_moved) {
    earthquake_led_mode();
    earthquake_detected();
  }

  delay(50);
}
