
SRC_DIR := ./src ./vendor
SRC_FILES := $(shell find $(SRC_DIR) -type f -name \*.cpp)

INC_DIRS := $(shell find $(SRC_DIR) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS = -std=c++17  $(INC_FLAGS)
LDFLAGS = -pthread


debug: CPPFLAGS += -O0 -g -DDEBUG -L./vendor
debug: clean SLAM

SLAM:
	g++ $(CPPFLAGS) -o SLAM $(SRC_FILES) $(LDFLAGS)

run: CPPFLAGS += -Ofast
run: clean SLAM
	sudo ./SLAM

clean:
	rm -f SLAM