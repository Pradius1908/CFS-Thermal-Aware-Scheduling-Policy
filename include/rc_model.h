#ifndef RC_MODEL_H
#define RC_MODEL_H

double predict_temperature(
    double T_curr,
    double power,
    double T_ambient,
    double R,
    double C,
    double dt
);

#endif

