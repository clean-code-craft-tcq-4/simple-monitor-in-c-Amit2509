#include <stdio.h>
#include <assert.h>
#define TEMP_MIN        0
#define TEMP_MAX        45
#define SOC_MIN         20
#define SOC_MAX         80
#define CHARGERATE_MAX  0.8
int checkWithinRange(float value, float min, float max);
int checkLessThanMax(float value, float max);
int batteryIsOk(float temperature, float soc, float chargeRate);

int checkWithinRange(float value, float min, float max)
{
  if ((value < min) || (value > max))
  {
    return 0;
  }
  else
  {
    return 1;
  }   
}

int checkLessThanMax(float value, float max)
{
  if (value > max)
  {
    return 0;
  }
  else
  {
    return 1;
  }   
}


int batteryIsOk(float temperature, float soc, float chargeRate) {
  int isTempOk = 0;
  int isSocOk = 0;
  int isChargeRateOk = 0;
  isTempOk       = checkWithinRange(temperature, TEMP_MIN, TEMP_MAX);
  isSocOk        = checkWithinRange(soc, SOC_MIN, SOC_MAX);
  isChargeRateOk = checkLessThanMax(chargeRate, CHARGERATE_MAX); 
  return (isTempOk && isSocOk && isChargeRateOk);
}

int main() {
  /*Checking temperature range*/
  assert(!batteryIsOk(TEMP_MIN-1, 70, 0.7));
  assert(batteryIsOk(TEMP_MIN+1, 70, 0.7));
  assert(!batteryIsOk(TEMP_MAX+1, 70, 0.7));

  /*Checking SOC range*/
    assert(!batteryIsOk(40, SOC_MIN-1, 0));
    assert(batteryIsOk(40, SOC_MIN+1, 0));
    assert(!batteryIsOk(40, SOC_MAX+1, 0));
  
  /*Checking chargeRate range*/
  assert(batteryIsOk(40, 70, CHARGERATE_MAX-0.1));
  assert(!batteryIsOk(40, 70, CHARGERATE_MAX+0.1));
}
