#include "../include/rc_model.h"

double predict_temperature(
    double T_curr,
    double power,
    double T_ambient,
    double R,
    double C,
    double dt
) {
    return T_curr + (dt / C) * (power - (T_curr - T_ambient) / R);
}

