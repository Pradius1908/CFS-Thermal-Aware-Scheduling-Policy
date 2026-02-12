# RC-Thermal-Scheduler

A CFS scheduling policy that takes thermal parameters into consideration to ensure lower overall CPU temperatures through frequency capping regulated mitigation.
Works ideally under high pressure.
NOT A REPLACEMENT FOR CFS, DO NOT TRY TO REPLACE CFS.

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


# STRESS TESTING PHASE
------------------------

## Terminal 1
make clean
make
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
cd ../experiments/  # must be ran simultaneously with stress command. hence the 5 seconds extra in stress
python3 temp_logger.py # rename .csv files to baseline when run without scheduler and to thermal when ran with scheduler

## Terminal 4
cd ../demo/
python 3 app.py

## Terminal 5
cd ../plots/
python3 plot_temperature.py # run after logging temperatures


# COMMON BUGS
-------------

If terminal says "sensor unavailable - monitoring only" while running, you likely have a different coretemp file
To find your coretemp file, terminate scheduler, and run this:

ls /sys/class/hwmon/

for i in /sys/class/hwmon/hwmon*; do
  echo "$i: $(cat $i/name 2>/dev/null)"
done

The hwmon that says "coretemp" in the loop is your temperature reading file. Replace the hwmon number in utils.c and app.py respectively.
Run make file commands once again and it should run perfectly then.
You'll know this if the terminal 1 displays current and predicted temperatures and if the temperature reading in app.py doesn't say N/A.

