import pandas as pd
import matplotlib.pyplot as plt

baseline = pd.read_csv("../experiments/baseline.csv")
sched = pd.read_csv("../experiments/thermal.csv")

plt.figure(figsize=(8, 5))

plt.plot(baseline["time_s"], baseline["temperature_C"],
         label="Baseline (CFS only)", linewidth=2)

plt.plot(sched["time_s"], sched["temperature_C"],
         label="Thermal-aware CFS", linewidth=2)

plt.xlabel("Time (s)")
plt.ylabel("CPU Temperature (°C)")
plt.title("CPU Temperature: Baseline vs Thermal-Aware Scheduling")
plt.legend()
plt.grid(True)

plt.tight_layout()
plt.savefig("temperature_vs_time.png", dpi=150)
plt.show()

