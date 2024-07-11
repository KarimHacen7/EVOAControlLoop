#include <Arduino.h>
#include <TM1637Display.h>

#define CONTROL_ADC_PIN 12
#define JDSU_ADC_PIN 14
#define DISPLAY_CLK 2
#define DISPLAY_DIO 4
#define SETPOINT_READINGS_NUMBER 10

const int motorControlPins[4] = {22, 23, 21, 19};
const int motorControlSequence[4][4] = {{0,1,0,1},
                                        {1,1,0,0},
                                        {1,0,1,0},
                                        {0,0,1,1}};
uint8_t displayData[] = { 0xff, 0xff, 0xff, 0xff };
                                        
int currentStep = 0;                                        
uint16_t setPoint = 0;
uint16_t JDSUValue = 0;
int difference = 0;

TM1637Display display(DISPLAY_CLK, DISPLAY_DIO);

void getSetPoint(void);
void singleStep(int sentido);

void setup() {
  for(int i = 0; i <= 3; i++)
  {
    pinMode(motorControlPins[i], OUTPUT);
  }
  pinMode(CONTROL_ADC_PIN, INPUT);
  pinMode(JDSU_ADC_PIN, INPUT);
  Serial.begin(115200);
  display.setBrightness(0x0f);
}

void loop() {
  getSetPoint();
  JDSUValue = analogRead(JDSU_ADC_PIN);
  
  display.showNumberDec(setPoint, true);
  //Serial.print("Control:");
  //Serial.println(setPoint);
  //Serial.print("JDSU:");
  //Serial.println(JDSUValue);
  

  difference = setPoint - JDSUValue;
  if(abs(difference) < 50)
  {}
  else if(difference > 0)
  {
    singleStep(1);
  }
  else
  {
    singleStep(0);
  }
}


// This function gets the setpoint from the input potentiometer. 
// It measures SETPOINT_READINGS_NUMBER times at aproximately 200 hertz from CONTROL_ADC_PIN, 
// and averages the result to filter high frequency components.
void getSetPoint(void)
{
  uint32_t acumulator = 0;
  for(int i = 0; i < SETPOINT_READINGS_NUMBER; i++)
  {
    acumulator += analogRead(CONTROL_ADC_PIN);
    delay(5);
  }
  setPoint = acumulator/SETPOINT_READINGS_NUMBER;
  if(setPoint < 204){setPoint = 204;}
  if(setPoint > 3891){setPoint = 3891;}
}

// This function outputs the control word necessary for stepping the attenuator once. 
// Its single argument indicates which direction is to be used. 1 for increasing attenuation (and ADC measurement) and 0 for the contrary
// The control word is held for DELAY_HOLD_OUTPUT milliseconds, and then turned off to avoid dissipating energy unnecessarily
void singleStep(int sentido)
{
  if (sentido == 1)
  {
    currentStep++;
    if (currentStep == 4){currentStep = 0;}
  }
  else if (sentido == 0)
  {
    currentStep--;
    if (currentStep == -1){currentStep = 3;}
  }
  else
  {
    return;
  }
  for(int i = 0; i < sizeof(motorControlPins)/sizeof(motorControlPins[0]); i++)
  {
    digitalWrite(motorControlPins[i], motorControlSequence[currentStep][i]);
  }
  delay(20); 
  for(int i = 0; i < sizeof(motorControlPins)/sizeof(motorControlPins[0]); i++)
  {
    digitalWrite(motorControlPins[i], 0);
  }
}
