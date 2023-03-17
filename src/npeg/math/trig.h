#ifndef H_TRIG
#define H_TRIG

#include "types.h"

#define PI 3.141592653589793f
#define PI_DEVIDED_BY_180 0.017453292519943295f

f32 deg_to_rad(f32 degrees) { return degrees * PI_DEVIDED_BY_180; }

#endif // !H_TRIG
