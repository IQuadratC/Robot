
SRC_DIR := ./src 

SRC_FILES := $(shell find $(SRC_DIR) -type f -name \*.cpp)

CPPFLAGS = -std=c++17  $(INC_FLAGS)
LDFLAGS = -lpython3.7m -I ../Roboter/depen/spdlog/

debug: CPPFLAGS += -O0 -g -DDEBUG
debug: clean Roboter

Roboter:
	g++ $(CPPFLAGS) -o Roboter $(SRC_FILES) $(LDFLAGS)

run: CPPFLAGS += -Ofast
run: clean Roboter
	sudo ./Roboter

clean:
	rm -f Roboter

