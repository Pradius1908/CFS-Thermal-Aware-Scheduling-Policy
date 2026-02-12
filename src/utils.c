#include <stdio.h>

#include "../include/utils.h"

/* =======================
   HARDWARE PATHS
   ======================= */
#define TEMP_PATH "/sys/class/hwmon/hwmon7/temp10_input"
#define FREQ_CUR_PATH "/sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq"
#define FREQ_MAX_PATH "/sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq"

/* =======================
   SENSOR FUNCTIONS
   ======================= */

double read_temperature(void)
{
    FILE *fp = fopen(TEMP_PATH, "r");
    if (!fp) return -1.0;

    int milli;
    if (fscanf(fp, "%d", &milli) != 1) {
        fclose(fp);
        return -1.0;
    }

    fclose(fp);
    return milli / 1000.0;
}

double read_frequency(void)
{
    FILE *fp = fopen(FREQ_CUR_PATH, "r");
    if (!fp) return -1.0;

    int khz;
    if (fscanf(fp, "%d", &khz) != 1) {
        fclose(fp);
        return -1.0;
    }

    fclose(fp);
    return khz / 1e6;
}

int read_max_frequency(void)
{
    FILE *fp = fopen(FREQ_MAX_PATH, "r");
    if (!fp) return -1;

    int freq;
    if (fscanf(fp, "%d", &freq) != 1) {
        fclose(fp);
        return -1;
    }

    fclose(fp);
    return freq;
}

void write_max_frequency(int freq)
{
    FILE *fp = fopen(FREQ_MAX_PATH, "w");
    if (!fp) return;

    fprintf(fp, "%d", freq);
    fclose(fp);
}

/* =======================
   UTILIZATION (SAFE STUB)
   ======================= */

double estimate_utilization(void)
{
    /* Conservative placeholder.
       No per-process inspection. */
    return 0.7;
}

