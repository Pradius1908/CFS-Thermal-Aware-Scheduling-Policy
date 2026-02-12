CC = gcc
CFLAGS = -Wall -O2
TARGET = rc_sched

SRC = \
	src/rc_thermal_scheduler.c \
	src/utils.c \
	src/rc_model.c

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)

