#ifndef ADS1231_H
#define ADS1231_H

#include <inttypes.h>
#include "application.h"

#define MAX_ADS1231 6
#define INVALID_ADS1231 255

typedef struct  {
  uint8_t sclPin;
  uint8_t dataPin;
  uint8_t pwdnPin;
} ADS1231_t;

class ADS1231 {
  public:
    ADS1231();
    ~ADS1231();
		void attach( uint8_t sclPin , uint8_t dataPin , uint8_t pwdnPin );
    void power( uint8_t power_mode );
    uint8_t  check();
    long readData();
    long calibrate();
		void sclPulse();
	private:
    // index into the channel data for this ADS1231
    uint8_t ADS1231Index;
};
#endif
