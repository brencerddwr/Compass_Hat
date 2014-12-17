#include <Wire.h>
#include <LSM303.h>
#include <FastLED.h>
#include <math.h>

#define NUM_LEDS 1
// data pin that led data will be written out over
#define DATA_PIN 6

LSM303 compass;
CRGB leds[NUM_LEDS];

void setup(void) {
  // sanity check delay - allows reprogramming if accidentally blowing power w/leds
  delay(2000);
  Serial.begin(115200);
  Wire.begin();
  compass.init();
  compass.enableDefault();
  /*
	Calibration values; the default values of +/-32767 for each axis
   	lead to an assumed magnetometer bias of 0. Use the Calibrate example
   	program to determine appropriate values for your particular unit.
   	*/
  compass.m_min = (LSM303::vector<int16_t>){
    -32767, -32767, -32767      };
  compass.m_max = (LSM303::vector<int16_t>){
    +32767, +32767, +32767      };
  // setup neopixels
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  fill_solid ( &(leds[0]), NUM_LEDS, CRGB::Black);
  FastLED.show();
  delay(1000);
  FastLED.setBrightness(96);
}
void loop(void)
{
  /* Get a new sensor event */
  compass.read();
  double heading = compass.heading();
  double roll = (atan2(-compass.a.y,compass.a.x)*180)/PI;
  double pitch = (atan2(compass.a.x,sqrt(pow(compass.a.y,2)+pow(compass.a.z,2)))*180)/PI;

  Serial.print("X: "); 
  Serial.print(compass.m.x); 
  Serial.print("  ");
  Serial.print("Y: "); 
  Serial.print(compass.m.y); 
  Serial.print("  ");
  Serial.print("Z: "); 
  Serial.println(compass.m.z);

  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("X: "); 
  Serial.print(compass.a.x); 
  Serial.print("  ");
  Serial.print("Y: "); 
  Serial.print(compass.a.y); 
  Serial.print("  ");
  Serial.print("Z: "); 
  Serial.println(compass.a.z);
  /* Display calculated results */
  Serial.print("Pitch: ");
  Serial.print(pitch);
  Serial.print(" Roll: ");
  Serial.print(roll);
  Serial.print(" Compass Heading: ");
  Serial.println(compass.heading());

  if (heading >=315 && heading <360){
    leds[0] = CRGB::Green;
  }
  if (heading >= 0 && heading <45) {
    leds[0] = CRGB::Green;
  }
  if (heading >=45 && heading <135) {
    leds[0] = CRGB::Yellow;
  }
  if (heading >=135 && heading <225) {
    leds[0] = CRGB::Red;
  }
  if (heading >=225 && heading <315) {
    leds[0] = CRGB::Blue;
  }

  FastLED.show();
  FastLED.delay(500);
}



