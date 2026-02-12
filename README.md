# RC-Thermal-Scheduler

rc-thermal-scheduler/
│
├── src/
│   ├── rc_thermal_scheduler.c              # Main predictive thermal controller
│   ├── rc_model.c                          # RC thermal model implementation
│   └── utils.c                             # System helpers (temperature, frequency, utilization)
│
├── include/
│   ├── rc_model.h                          # RC model interface
│   └── utils.h                             # Helper declarations
│
├── experiments/
│   ├── temp_logger.py                      # Baseline logger (CFS only)
│   └── results.csv                         # Active experiment log (current run)
│
├── demo/
│   └── app.py                              # Read-only observability panel
│
├── plots/
│   ├── plot_temperature.py                 # Plotting script
│   └── temperature_vs_time.png             # Generated comparison plot
│
├── results/
│   ├── beta/
│   │   ├── beta_normal_pressure.png        # Old scheduler output
│   │   ├── beta_moderate_pressure.png
│   │   └── beta_high_pressure.png
│   │ 
│   │
│   └── prototype/
│       ├── proto_normal_pressure.png       # Current scheduler output
│       ├── proto_moderate_pressure.png
│       ├── proto_high_pressure.png
│       └── proto_sustained_pressure.png
│
├── Makefile
├── README.md
└── .gitignore


we ARE CHARLIE KIIIRK 

# STRESS TESTING PHASE
------------------------

## Terminal 1
sudo ./rc_sched

## Terminal 2
Low/Normal Pressure
stress-ng --cpu 4 --timeout 65s

Medium Pressure 
stress-ng --cpu 6 --timeout 95s

High Pressure
stress-ng --cpu 8 --timeout 125s

Sustained Pressure
stress-ng --cpu 10 --timeout 155s

## Terminal 3 
python3 templogger.py

