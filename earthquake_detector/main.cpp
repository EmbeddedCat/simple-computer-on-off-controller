#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <math.h>

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified();
int x_axis_values[100] = {0};
int y_axis_values[100] = {0};
int z_axis_values[100] = {0};

int next_axis_value = 0;
int is_filled = false;
int new_mo_x = 0;
int new_mo_y = 0;
int new_mo_z = 0;

int old_mo_x = 0;
int old_mo_y = 0;
int old_mo_z = 0;

void setup() {
   pinMode(13, OUTPUT);
   Serial.begin(9600); 
   while(!accel.begin())
   {
      Serial.println("No valid sensor found");
   }
}

void earthquake_detected()
{
  int alarm_sec  = 2;
  int time_start = millis();
  int time_end   = millis();
  while ((time_end - time_start) < alarm_sec*1000) {
    tone(13, 1000, 30);
    time_end = millis();
  }
}

int calculate_mo(int *arr)
{
  int sum = 0;

  for (int i = 0; i < 100; i++) {
    sum += arr[i];
  }
  return sum/100;
}

void loop() {
  sensors_event_t event; 
  accel.getEvent(&event);

  Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print(" | ");
  Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print(" | ");
  Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print(" | ");
  Serial.println("m/s^2 ");

  x_axis_values[next_axis_value] = event.acceleration.x;
  y_axis_values[next_axis_value] = event.acceleration.y;
  z_axis_values[next_axis_value] = event.acceleration.z;
  next_axis_value += 1;

  new_mo_x = calculate_mo(x_axis_values);
  new_mo_y = calculate_mo(y_axis_values);
  new_mo_z = calculate_mo(z_axis_values);

  if (is_filled) {
    if (abs(new_mo_x - old_mo_x) > 0.5 || abs(new_mo_y - old_mo_y) > 0.5 || abs(new_mo_z - old_mo_z) > 0.5) {
      earthquake_detected();
      is_filled = false;
      next_axis_value = 0;
    }
  }

  old_mo_x = new_mo_x;
  old_mo_y = new_mo_y;
  old_mo_z = new_mo_z;
  if (next_axis_value == 100) {
    is_filled = true;
    next_axis_value = 0;
  }

  if (is_filled) {
    delay(500);
  }
}

