
SRC_DIR := ./src ./vendor
SRC_FILES := $(shell find $(SRC_DIR) -type f -name \*.cpp)

INC_DIRS := $(shell find $(SRC_DIR) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS = -std=c++17  $(INC_FLAGS)
LDFLAGS = -pthread

debug: CPPFLAGS += -O0 -g -DDEBUG
debug: clean Network

Network:
	g++ $(CPPFLAGS) -o Network $(SRC_FILES) $(LDFLAGS)

run: CPPFLAGS += -Ofast
run: clean Network
	sudo ./Network

clean:
	rm -f Network

close:
	netstat -np
	lsof -np 773
	gdb -p 773
	call close(11001)
	quit