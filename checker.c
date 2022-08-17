#include <stdio.h>
#include <assert.h>
#define TEMP_MIN        0
#define TEMP_MAX        45
#define SOC_MIN         20
#define SOC_MAX         80
#define CHARGERATE_MAX  0.8
#define WARNING_TOLERANCE 0.05

enum State {LOW_BREACH = 0, LOW_WARNING, NORMAL, HIGH_WARNING, HIGH_BREACH};
//enum BatteryState {NOT_OK = -1, OK = 0, WARNING = 1};
const char* States[] = {"LOW_BREACH", "LOW_WARNING", "NORMAL", "HIGH_WARNING", "HIGH_BREACH"};
int checkRange(char* parameter, float value, float min, float max);
int batteryIsOk(float temperature, float soc, float chargeRate);

enum State TempState, SocState, ChargeRateState;

int checkIsWithinLimit(char* parameter, float value, float min, float max)
{
  int result = 1;
  if (value <= min)
  {
    result    = 0;
    printf ("LOW_%s_BREACH\n", parameter);
  }
  else if (value > max)
  {
    result    = 0;
    printf ("HIGH_%s_BREACH\n", parameter);
  }
  return result;
}

void checkWarning(char* parameter, float value, float min, float max)
{
  if (value <= (min + max*WARNING_TOLERANCE))
  {
    printf ("LOW_%s_WARNING\n", parameter);
  }
  else if (value >(max - max*WARNING_TOLERANCE))
  {
    printf ("HIGH_%s_WARNING\n", parameter);
  }
  else
  {
    printf ("NORMAL_%s\n", parameter);
  }
}

int checkRange(char* parameter, float value, float min, float max)
{ 
  int result = checkIsWithinLimit(parameter, value, min, max);
  if (result == 1)
  {
    checkWarning(parameter, value, min, max);
  } 
  return result;
}

int batteryIsOk(float temperature, float soc, float chargeRate) {
  int result;
  int isTempOk, isSocOk, isChargeRateOk;
  isTempOk       = checkRange("TEMPERATURE", temperature, TEMP_MIN, TEMP_MAX);
  isSocOk        = checkRange("SoC", soc, SOC_MIN, SOC_MAX);
  isChargeRateOk = checkRange("CHARGERATE", chargeRate, 0, CHARGERATE_MAX); /*Assuming minimum allowed chargeRate is 0*/
  
  result = isTempOk*isSocOk*isChargeRateOk;
  if(result == 0)
  {
      printf("Overall Battery State: NOT_OK (At least one parameter is out of its min/max range)\n");
  }
  else
  {
     printf("Overall Battery State: OK!! (All parameters are within the Limit)\n");
  } 
  return result;
}

int main() {
  /*Checking temperature range*/
  assert(batteryIsOk(TEMP_MIN-1, 70, 0.7)==0);
  assert(batteryIsOk(TEMP_MIN+1, 70, 0.7)==1);
  assert(batteryIsOk(TEMP_MIN+10, 70, 0.7)==1);
  assert(batteryIsOk(TEMP_MAX-1, 70, 0.7)==1);
  assert(batteryIsOk(TEMP_MAX+1, 70, 0.7)==0);
}
