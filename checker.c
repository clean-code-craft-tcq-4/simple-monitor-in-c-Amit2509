#include <stdio.h>
#include <assert.h>
#define TEMP_MIN        0
#define TEMP_MAX        45
#define SOC_MIN         20
#define SOC_MAX         80
#define CHARGERATE_MAX  0.8
#define WARNING_TOLERANCE 0.05

//enum State {LOW_BREACH = 0, LOW_WARNING, NORMAL, HIGH_WARNING, HIGH_BREACH};
enum Languages {ENGLISH, GERMAN} language = ENGLISH;
//enum BatteryState {NOT_OK = -1, OK = 0, WARNING = 1};
//const char* States[] = {"LOW_BREACH", "LOW_WARNING", "NORMAL", "HIGH_WARNING", "HIGH_BREACH"};
int checkRange(char* parameter, float value, float min, float max);
int batteryIsOk(float temperature, float soc, float chargeRate);
void printMessage(enum Languages lang, char* EngMsz, char* GermanMsz);

//enum State TempState, SocState, ChargeRateState;

void printMessage(enum Languages lang, char* EngMsz, char* GermanMsz)
{
  printf("%s", (lang == ENGLISH) ? EngMsz : GermanMsz);
}

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
  isChargeRateOk = checkRange("CHARGERATE", chargeRate, 0, CHARGERATE_MAX); /*Assuming minimum allowed chargeRate is 0 and input to charge rate is always positive*/
  
  result = isTempOk*isSocOk*isChargeRateOk;
  if(result == 0)
  {
      printMessage(language, "Overall Battery State: NOT_OK\n", "Gesamtzustand der Batterie: NICHT_OK\n");
  }
  else
  {
      printMessage(language, "Overall Battery State: OK\n", "Gesamtzustand der Batterie: OK\n");
  } 
  return result;
}

int main() {
  /*Checking temperature range*/
  language = ENGLISH;
  assert(batteryIsOk(TEMP_MIN-1, 70, 0.7)==0);
  assert(batteryIsOk(TEMP_MIN+1, 70, 0.7)==1);
  assert(batteryIsOk(TEMP_MIN+10, 70, 0.7)==1);
  language = GERMAN;
  assert(batteryIsOk(TEMP_MAX-1, 70, 0.7)==1);
  assert(batteryIsOk(TEMP_MAX+1, 70, 0.7)==0);

  /*Checking SoC range*/
  assert(batteryIsOk(40, SOC_MIN-1, 0.7)==0);
  assert(batteryIsOk(40, SOC_MIN+1, 0.7)==1);
  assert(batteryIsOk(40, SOC_MIN+10, 0.7)==1);
  language = ENGLISH;
  assert(batteryIsOk(40, SOC_MAX-1, 0.7)==1);
  assert(batteryIsOk(40, SOC_MAX+1, 0.7)==0);

  /*Checking ChargeRate range*/
  assert(batteryIsOk(40, 60, 0)==0);
  assert(batteryIsOk(40, 60, 0.03)==1);
  language = GERMAN;
  assert(batteryIsOk(40, 60, 0.2)==1);
  assert(batteryIsOk(40, 60, CHARGERATE_MAX-0.03)==1);
  assert(batteryIsOk(40, 60, CHARGERATE_MAX+0.2)==0);
}
