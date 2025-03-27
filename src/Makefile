NAME = audio-to-midi

BUNDLE = ../$(NAME).lv2
BUILD_C_FLAGS += -I. -fPIC -DPIC
BUILD_LDFLAGS += -shared -lm

CC ?= gcc
CXX ?= g++

C_FLAGS = $(BUILD_C_FLAGS) -Wall -Wextra -pipe
LDFLAGS = $(BUILD_LDFLAGS) -Wl,--strip-all
LIBS = -lm

C_OBJS = $(NAME).o
TARGET = $(BUNDLE)/$(NAME).so

$(TARGET): $(C_OBJS)
	$(CC) $(LDFLAGS) $(C_OBJS) -o $(TARGET) $(LIBS)

%.o: %.c
	$(CC) $(C_FLAGS) -c $< -o $@

clean:
	rm -f *.o
	rm -f $(TARGET)