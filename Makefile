CXX := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -Wpedantic -Werror -I src
TARGET := bin/grafos
SRCS := $(wildcard src/*.cpp)
OBJS := $(patsubst src/%.cpp, build/%.o, $(SRCS))
BUILD_DIR := build
COMPILE_COMMANDS := $(BUILD_DIR)/compile_commands.json

.PHONY: all clean compile_commands format-check

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

build/%.o: src/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(COMPILE_COMMANDS): $(SRCS) Makefile
	@mkdir -p $(BUILD_DIR)
	@printf '[\n' > $@
	@for src in $(SRCS); do \
		printf '  {\n' >> $@; \
		printf '    "directory": "$(CURDIR)",\n' >> $@; \
		printf '    "command": "$(CXX) $(CXXFLAGS) -c %s -o %s",\n' "$$src" \
			"$(BUILD_DIR)/$$(basename $$src .cpp).o" >> $@; \
		printf '    "file": "%s"\n' "$$src" >> $@; \
		printf '  }\n' >> $@; \
		printf '  ,\n' >> $@; \
	done
	@sed -i '$$d' $@
	@printf ']\n' >> $@

compile_commands: $(COMPILE_COMMANDS)

format-check:
	@clang-format --dry-run --Werror src/*.hpp src/*.cpp

clean:
	rm -rf $(BUILD_DIR)
	rm -f $(TARGET)
