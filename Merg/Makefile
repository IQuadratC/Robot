
SRC_DIR := ./src ./vendor
SRC_FILES := $(shell find $(SRC_DIR) -type f -name \*.cpp)

INC_DIRS := $(shell find $(SRC_DIR) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS = -std=c++17  $(INC_FLAGS)
LDFLAGS = -pthread -lspdlog -lpython3.6m -lstdc++fs -lrplidar_sdk -I/usr/include/python3.6 


debug: CPPFLAGS += -O0 -g -DDEBUG -L./vendor
debug: clean Robot

Robot:
	g++ $(CPPFLAGS) -o Robot $(SRC_FILES) $(LDFLAGS)

run: CPPFLAGS += -Ofast
run: clean Robot
	sudo ./Robot

clean:
	rm -f Robot