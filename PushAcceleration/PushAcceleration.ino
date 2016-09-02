/*

  Sending out the board's data points through BLE characteristics.
  
*/

uint8_t CURRENT_VERSION_MAJOR = 0;
uint8_t CURRENT_VERSION_MINOR = 0;
uint8_t CURRENT_VERSION_PATCH = 1;

bool ledState = false;
unsigned long ledOnTimer = 0;
uint8_t version_info[8]; 

void setup() {
  // Bean Serial is at a fixed baud rate. Changing the value in Serial.begin() has no effect.
  Serial.begin(57600); 
  // Optional: Use Bean.setAccelerationRange() to set the sensitivity to 
  // something other than the default of ±2g.
  
  

  version_info[0] = CURRENT_VERSION_MAJOR;
  version_info[1] = CURRENT_VERSION_MINOR;
  version_info[2] = CURRENT_VERSION_PATCH;

  
  ledOnTimer = millis();
}





void loop() {

  // write out the version of this sketch
  Bean.setScratchData( 1, version_info, 3);

  
  // Get the current acceleration with range of ±2g, and a 
  // conversion of 3.91×10-3 g/unit or 0.03834(m/s^2)/units.
  AccelerationReading acceleration = Bean.getAcceleration();

  uint8_t buffer[8];
  memcpy( (void*)buffer, (void*)&acceleration, 6 );
  buffer[6] = Bean.getTemperature();
  buffer[7] = Bean.getBatteryLevel();
  // notify client  
  Bean.setScratchData( 2, buffer, 8 );


  printDebugInfoToSerial(acceleration);
  toggleLed(acceleration);
  
  Bean.sleep(250);
  
}

void toggleLed(AccelerationReading acceleration)
{  

  // just using this to turn on the colors
  char x = abs(acceleration.xAxis) / 2;
  char y = abs(acceleration.yAxis) / 2;
  char z = abs(acceleration.zAxis) / 2;
  if( shouldTurnOnLed() ) {
    Bean.setLed(x, y, z);
  } else {
    Bean.setLed(0, 0, 0);
  }
  
}

bool shouldTurnOnLed()
{
  unsigned long current = millis();
  unsigned long difference = current - ledOnTimer;
  if( difference > 5000 ) {
    ledOnTimer = current;
    ledState = !ledState;
  }

  return ledState;
}

void printDebugInfoToSerial(AccelerationReading acceleration)
{


    Serial.print("x:");
    Serial.print(acceleration.xAxis);
    Serial.print(", y: ");
    Serial.print(acceleration.yAxis);
    Serial.print(", z:");
    Serial.print(acceleration.zAxis);
    Serial.print(", batt:");
    Serial.println(Bean.getBatteryLevel());

    //  buffer[6] = Bean.getTemperature();
  //buffer[7] = Bean.getBatteryLevel();


}



