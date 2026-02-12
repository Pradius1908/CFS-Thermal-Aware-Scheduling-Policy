import time
import csv

TEMP_PATH = "/sys/class/hwmon/hwmon6/temp10_input"
FREQ_PATH = "/sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq"

INTERVAL = 1.0  # seconds
start_time = time.time()

with open("temp_only.csv", "w", newline="") as f:
    writer = csv.writer(f)
    writer.writerow(["time_s", "temperature_C", "freq_MHz"])

    try:
        while True:
            with open(TEMP_PATH, "r") as tf:
                temp_milli = int(tf.read().strip())
                temp_C = temp_milli / 1000.0

            with open(FREQ_PATH, "r") as ff:
                freq_khz = int(ff.read().strip())
                freq_mhz = freq_khz / 1000.0

            t = time.time() - start_time
            writer.writerow([round(t, 2), temp_C, freq_mhz])
            f.flush()

            time.sleep(INTERVAL)

    except KeyboardInterrupt:
        print("Baseline logging stopped.")

