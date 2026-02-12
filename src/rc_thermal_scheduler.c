#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include "../include/rc_model.h"
#include "../include/utils.h"

/* =======================
   RC MODEL PARAMETERS
   ======================= */
#define R_THERMAL   1.0
#define C_THERMAL   10.0
#define DT          1.0

/* =======================
   CONTROL THRESHOLDS
   ======================= */
#define T_HIGH      55.0
#define T_LOW       50.0

/* =======================
   POWER MODEL
   ======================= */
#define ALPHA       5.0

/* =======================
   SAFETY
   ======================= */
#define ACTION_COOLDOWN 8

/* =======================
   STATE
   ======================= */
static int mitigation_active = 0;
static int mitigation_level = 0;   /* 0–3 */
static time_t last_action_time = 0;
static int original_max_freq = -1;

/* Adaptive environment temperature */
static double T_env = 42.0;

/* =======================
   UTILITY
   ======================= */

static int can_deescalate(void)
{
    time_t now = time(NULL);
    return difftime(now, last_action_time) >= ACTION_COOLDOWN;
}

/* =======================
   MITIGATION CONTROL
   ======================= */

/* Escalation ONLY */
static void escalate_mitigation(int new_level)
{
    if (new_level <= mitigation_level)
        return;

    if (original_max_freq <= 0)
        original_max_freq = read_max_frequency();

    if (original_max_freq <= 0)
        return;

    double factor;
    switch (new_level) {
        case 1: factor = 0.7; break;
        case 2: factor = 0.6; break;
        case 3: factor = 0.5; break;
        default: return;
    }

    write_max_frequency((int)(original_max_freq * factor));

    mitigation_level = new_level;
    mitigation_active = 1;
    last_action_time = time(NULL);

    printf("Mitigation LEVEL %d applied (cap %.0f%%)\n",
           new_level, factor * 100);
}

/* De-escalation ONLY */
static void deescalate_mitigation(void)
{
    if (!mitigation_active || !can_deescalate())
        return;

    mitigation_level--;

    if (mitigation_level <= 0) {
        /* Fully restore */
        if (original_max_freq > 0)
            write_max_frequency(original_max_freq);

        mitigation_level = 0;
        mitigation_active = 0;

        printf("Mitigation DISABLED (frequency restored)\n");
    } else {
        double factor;
        switch (mitigation_level) {
            case 1: factor = 0.7; break;
            case 2: factor = 0.6; break;
            default: factor = 1.0; break;
        }

        write_max_frequency((int)(original_max_freq * factor));

        printf("Mitigation LEVEL %d applied (cap %.0f%%)\n",
               mitigation_level, factor * 100);
    }

    last_action_time = time(NULL);
}

/* =======================
   MAIN LOOP
   ======================= */

int main(void)
{
    printf("RC-Based Thermal-Aware Scheduler Controller\n");
    printf("Multi-Level Predictive Mitigation\n");
    printf("------------------------------------------------\n");

    FILE *logf = fopen("experiments/results_scheduler.csv", "a");
    if (!logf) {
        perror("log file");
        return 1;
    }

    static int t = 0;

    while (1) {
        double T = read_temperature();
        double f = read_frequency();

        if (T < 0 || f < 0) {
            printf("Sensor unavailable — monitoring only\n");
            sleep((int)DT);
            continue;
        }

        /* Update adaptive ambient temperature */
        T_env = 0.98 * T_env + 0.02 * T;

        double util = estimate_utilization();
        double power = ALPHA * util * f;

        double T_pred = predict_temperature(
            T, power, T_env,
            R_THERMAL, C_THERMAL, DT
        );

        /* Conservative bias for prediction error */
        double error = T_pred - T;
        if (error > 2.0)
            T_pred += 1.0;

        fprintf(logf, "%d,%.2f,%.2f,%.2f,%d\n",
                t++, T, T_pred, f, mitigation_level);
        fflush(logf);

        printf("T=%.2f°C | T_pred=%.2f°C | f=%.2f GHz | L=%d\n",
               T, T_pred, f, mitigation_level);

        /* Control logic */
    /* Control logic */
        if (T_pred > T_HIGH + 5)
            escalate_mitigation(3);
        else if (T_pred > T_HIGH + 2)
            escalate_mitigation(2);
        else if (T_pred > T_HIGH)
            escalate_mitigation(1);
        else {
    /* De-escalation bands */
        if (mitigation_level == 3 && T_pred < T_HIGH + 2)
            deescalate_mitigation();
        else if (mitigation_level == 2 && T_pred < T_HIGH)
            deescalate_mitigation();
        else if (mitigation_level == 1 && T_pred < T_LOW)
            deescalate_mitigation();
        }


        sleep((int)DT);
    }

    return 0;
}

