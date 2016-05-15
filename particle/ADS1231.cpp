//include the declaration for this class
#include "ADS1231.h"

// static array of ADS1231 structures
static ADS1231_t ADS1231s[MAX_ADS1231];
// the total number of attached ADS1231
uint8_t ADS1231Count = 0;

//<<constructor>> setup the ADS1231
ADS1231::ADS1231() {
  if( ADS1231Count < MAX_ADS1231) {
    // assign a ADS1231 index to this instance
    this->ADS1231Index = ADS1231Count++;
  }
  else {
    // too many servos
    this->ADS1231Index = INVALID_ADS1231 ;
  }
}

//<<destructor>>
ADS1231::~ADS1231() {
/*nothing to destruct*/
}

//turn on resp. off the ads
void ADS1231::attach( uint8_t sclPin , uint8_t dataPin , uint8_t pwdnPin ) {
  if( ADS1231Count < MAX_ADS1231) {
    ADS1231s[this->ADS1231Index].sclPin = sclPin;
    ADS1231s[this->ADS1231Index].dataPin = dataPin;
    ADS1231s[this->ADS1231Index].pwdnPin = pwdnPin;

    // block interrupts
    uint8_t oldSREG, SREG;
    oldSREG = SREG;

    noInterrupts();
    // initialize the clock pin as an output:
    pinMode(sclPin, OUTPUT);
    pinResetFast(ADS1231s[this->ADS1231Index].sclPin);
    // initialize the data pin as an input:
    pinMode(dataPin, INPUT);
    pinResetFast(ADS1231s[this->ADS1231Index].dataPin);
    // initialize the power down pin as an output:
    pinMode(pwdnPin, OUTPUT);
    // reenable interrupts
    SREG = oldSREG;
    // reset ads1232 with a 10ms pulse on power down pin:
    power(LOW);
    delay(10);
    power(HIGH);
  }
}

//turn on resp. off the ads
void ADS1231::power(uint8_t power_mode) {
  if(power_mode == LOW) {
    pinResetFast(ADS1231s[this->ADS1231Index].pwdnPin);
  }
  else {
    pinSetFast(ADS1231s[this->ADS1231Index].pwdnPin);
  }
}

//check for new data ready
uint8_t ADS1231::check() {
  return !pinReadFast(ADS1231s[this->ADS1231Index].dataPin);
}

//read data from ads1231
long ADS1231::readData(){
  long dataValue = 0;
  uint8_t inputStatus;
  for( uint8_t count = 0 ; count < 25 ; count++ ){
    // send pulse to SCL and read the data
    sclPulse();
    inputStatus = pinReadFast(ADS1231s[this->ADS1231Index].dataPin);
    dataValue |= (long)(inputStatus) << (31 - count);
  }
  return dataValue;
}

//read data from ads1231 and calibrate
long ADS1231::calibrate() {
  long dataValue = 0;
  dataValue = readData();
  sclPulse();
  return dataValue;
}

//send pulse to ads1231
void ADS1231::sclPulse(){
  // set sclPin high
  pinSetFast(ADS1231s[this->ADS1231Index].sclPin);
  //		delayMicroseconds(1);
  // set sclPin low
  pinResetFast(ADS1231s[this->ADS1231Index].sclPin);
}
