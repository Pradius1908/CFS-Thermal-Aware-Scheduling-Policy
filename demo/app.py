import tkinter as tk
from tkinter import ttk
import time
import os

import psutil

# ------------------------
# Paths (adjust if needed)
# ------------------------
TEMP_PATH = "/sys/class/hwmon/hwmon7/temp10_input"
FREQ_PATH = "/sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq"
SCHED_LOG = "../experiments/results.csv"

UPDATE_MS = 1000

# ------------------------
# Read-only helpers
# ------------------------

def read_temp():
    try:
        with open(TEMP_PATH) as f:
            return int(f.read().strip()) / 1000.0
    except:
        return None

def read_freq():
    try:
        with open(FREQ_PATH) as f:
            return int(f.read().strip()) / 1000.0
    except:
        return None

def read_mitigation_level():
    if not os.path.exists(SCHED_LOG):
        return "N/A"
    try:
        with open(SCHED_LOG) as f:
            last = f.readlines()[-1]
            return last.strip().split(",")[-1]
    except:
        return "N/A"

def get_top_cpu_hogs(n=5):
    procs = []
    for p in psutil.process_iter(['pid', 'name', 'cpu_percent']):
        try:
            procs.append(p.info)
        except psutil.NoSuchProcess:
            pass

    procs.sort(key=lambda p: p['cpu_percent'], reverse=True)
    return procs[:n]

# ------------------------
# GUI update loop
# ------------------------

def update_ui():
    # System state
    T = read_temp()
    f = read_freq()
    mit = read_mitigation_level()

    temp_var.set(f"{T:.2f} °C" if T is not None else "N/A")
    freq_var.set(f"{f:.0f} MHz" if f is not None else "N/A")
    mit_var.set(mit)

    # CPU hogs table
    for row in hog_table.get_children():
        hog_table.delete(row)

    for p in get_top_cpu_hogs():
        hog_table.insert(
            "", "end",
            values=(p['pid'], p['name'], f"{p['cpu_percent']:.1f}")
        )

    root.after(UPDATE_MS, update_ui)

# ------------------------
# GUI layout
# ------------------------

root = tk.Tk()
root.title("Thermal-Aware Scheduler – Read-Only Observability Panel")

header = ttk.Label(
    root,
    text="Read-only system observability (no workload generation)",
    font=("TkDefaultFont", 10, "italic")
)
header.pack(pady=(5, 10))

info_frame = ttk.Frame(root)
info_frame.pack(pady=5)

temp_var = tk.StringVar(value="N/A")
freq_var = tk.StringVar(value="N/A")
mit_var = tk.StringVar(value="N/A")

ttk.Label(info_frame, text="CPU Temperature:").grid(row=0, column=0, sticky="e", padx=5)
ttk.Label(info_frame, textvariable=temp_var).grid(row=0, column=1, sticky="w")

ttk.Label(info_frame, text="CPU Frequency:").grid(row=1, column=0, sticky="e", padx=5)
ttk.Label(info_frame, textvariable=freq_var).grid(row=1, column=1, sticky="w")

ttk.Label(info_frame, text="Mitigation Level:").grid(row=2, column=0, sticky="e", padx=5)
ttk.Label(info_frame, textvariable=mit_var).grid(row=2, column=1, sticky="w")

# CPU hogs panel
ttk.Label(root, text="Top CPU-Consuming Tasks").pack(pady=(15, 5))

hog_table = ttk.Treeview(
    root,
    columns=("pid", "name", "cpu"),
    show="headings",
    height=6
)
hog_table.heading("pid", text="PID")
hog_table.heading("name", text="Command")
hog_table.heading("cpu", text="CPU %")

hog_table.column("pid", width=80, anchor="center")
hog_table.column("name", width=220)
hog_table.column("cpu", width=80, anchor="center")

hog_table.pack(fill="x", padx=10, pady=5)

footer = ttk.Label(
    root,
    text="Workloads are generated externally (e.g., stress-ng). This tool does not modify system behavior.",
    wraplength=420,
    font=("TkDefaultFont", 9)
)
footer.pack(pady=(10, 5))

# ------------------------
# Start UI loop
# ------------------------

update_ui()
root.mainloop()

