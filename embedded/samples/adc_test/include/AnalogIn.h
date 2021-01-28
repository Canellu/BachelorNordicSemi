#ifndef ANALOG_IN_H
#define ANALOG_IN_H

#include <stdint.h>
#include <stdbool.h>


float AnalogRead(int channel);

#define BAD_ANALOG_READ -123


#endif