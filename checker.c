#include <stdio.h>
#include <assert.h>
#define TEMP_MIN        0
#define TEMP_MAX        45
#define SOC_MIN         20
#define SOC_MAX         80
#define CHARGERATE_MAX  0.8
#define WARNING_TOLERANCE 0.05

enum State {LOW_BREACH = 0, LOW_WARNING, NORMAL, HIGH_WARNING, HIGH_BREACH};
enum BatteryState {NOT_OK = -1, OK = 0, WARNING = 1};
const char* States[] = {"LOW_BREACH", "LOW_WARNING", "NORMAL", "HIGH_WARNING", "HIGH_BREACH"};
enum State checkWithinRange(char* parameter, float value, float min, float max);
enum BatteryState batteryIsOk(float temperature, float soc, float chargeRate);

enum State TempState, SocState, ChargeRateState;

enum State checkWithinRange(char* parameter, float value, float min, float max)
{
  enum State currState = NORMAL;
  if (value <= min)
  {
    currState = LOW_BREACH;
  }
  else if ((value > min) && (value <=(min + max*WARNING_TOLERANCE)))
  {
    currState = LOW_WARNING;
  }
  else if ((value >(max - max*WARNING_TOLERANCE)) && (value <= max))
  {
    currState = HIGH_WARNING;
  }
  else if (value > max)
  {
    currState = HIGH_BREACH;
  }
  printf ("%s state: %s\n", parameter, States[currState]);
  return currState;
}



enum BatteryState batteryIsOk(float temperature, float soc, float chargeRate) {
  enum BatteryState batteryState;
  TempState       = checkWithinRange("Temperature", temperature, TEMP_MIN, TEMP_MAX);
  SocState        = checkWithinRange("Charge", soc, SOC_MIN, SOC_MAX);
  ChargeRateState = checkWithinRange("ChargeRate", chargeRate, 0, CHARGERATE_MAX); /*Assuming minimum allowed chargeRate is 0*/
  
/*  if((TempState == LOW_BREACH)||(TempState == HIGH_BREACH)||(SocState == LOW_BREACH)||(SocState == HIGH_BREACH)||(ChargeRateState == LOW_BREACH)||(ChargeRateState == HIGH_BREACH))
  {
      batteryState = NOT_OK;
      printf("Overall Battery State: NOT_OK (At least one parameter is out of its min/max range)\n");
  }
  else if((TempState == LOW_WARNING)||(TempState == HIGH_WARNING)||(SocState == LOW_WARNING)||(SocState == HIGH_WARNING)||(ChargeRateState == LOW_WARNING)||(ChargeRateState == HIGH_WARNING))
  {
      batteryState = WARNING;
      printf("Overall Battery State: WARNING!! (At least one parameter is reaching its min/max limit)\n");
  }
  else
  {*/
     batteryState = OK;
     printf("Overall Battery State: OK!! (All the parameters are in normal range)\n");
  //}
  
  return batteryState;
}

int main() {
  /*Checking temperature range*/
  assert(batteryIsOk(TEMP_MIN-1, 70, 0.7)==NOT_OK);
  assert(batteryIsOk(TEMP_MIN+1, 70, 0.7)==WARNING);
  assert(batteryIsOk(TEMP_MIN+10, 70, 0.7)==OK);
  assert(batteryIsOk(TEMP_MAX-1, 70, 0.7)==WARNING);
  assert(batteryIsOk(TEMP_MAX+1, 70, 0.7)==NOT_OK);
}
