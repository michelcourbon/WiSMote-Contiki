#include <signal.h>

#include "contiki-conf.h"
#include "lib/sensors.h"

#include "multsht15-sensors.h"
#include "sensors/sht1x-sensor.h"

// new_sht1X : SDA_port, SDA_bit, SCL_port, SCL_bit, PWR_port, PWR_bit

// arago sht snesor board on J7 extension
NEW_SHT1X(upper_sensor, "SHT15 UPPER",  3,7, 5,4, 3,5);
//NEW_SHT1X(middle_upper_sensor, "SHT15 MIDDLE UPPER",6,4,6,5, 6,0);
//NEW_SHT1X(middle_lower_sensor, "SHT15 MIDDLE LOWER",6,4,6,5, 6,2);
NEW_SHT1X(lower_sensor, "SHT15 LOWER", 8,4, 8,6, 8,5);

