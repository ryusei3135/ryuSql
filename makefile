# ==== コンパイラ ====
CXX := g++
PYTHON := python3
CARGO := cargo

# ===== ディレクトリ =====
ROOT_DIR := .
SRC_DIR := $(ROOT_DIR)/src
INCLUDE_DIR := $(ROOT_DIR)/include
RUST_DIR := $(ROOT_DIR)/rust
SETTINGS_DIR := $(ROOT_DIR)/settings
SCRIPT_DIR := $(ROOT_DIR)/scripts
BUILD_DIR := $(ROOT_DIR)/build

# ==== enum生成 ====
ENUM_SRC := $(SETTINGS_DIR)/rust_c_structs.yaml
GEN_SCRIPT := $(SCRIPT_DIR)/generate_structs.py
CPP_ENUM := $(INCLUDE_DIR)/ast_node.hpp
RUST_ENUM := $(RUST_DIR)/src/ast_node.rs
TOKEN_CPP := $(INCLUDE_DIR)/token.hpp
TOKEN_RUST := $(RUST_DIR)/src/token.rs

GENERATE_DATA := $(CPP_ENUM) $(RUST_ENUM) $(TOKEN_CPP) $(TOKEN_RUST)

# ==== Rust ====
RUST_LIB := $(RUST_DIR)/target/release/librustlib.a

# ==== C++ ====
CXXFLAGS := -O2 -std=c++23 -I$(INCLUDE_DIR)

CPP_SRCS := $(shell find $(SRC_DIR) -name "*.cpp")
CPP_OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(CPP_SRCS))

TARGET := db

# ==== ビルド ====
all: gen $(TARGET)

# enum生成
gen: $(GENERATE_DATA)

$(GENERATE_DATA): $(ENUM_SRC) $(GEN_SCRIPT)
	$(PYTHON) $(GEN_SCRIPT) $(ENUM_SRC) $(GENERATE_DATA)

# C++
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(CPP_ENUM)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rust
$(RUST_LIB): $(RUST_ENUM)
	$(CARGO) build --release --manifest-path $(RUST_DIR)/Cargo.toml

# リンク
$(TARGET): $(CPP_OBJS) $(RUST_LIB)
	$(CXX) $^ -o $@ \
	    -L$(RUST_DIR)/target/release -lrustlib \
	    -lpthread -ldl

# ==== クリーン ====
clean:
	rm -rf $(BUILD_DIR) $(TARGET)
	$(CARGO) clean --manifest-path $(RUST_DIR)/Cargo.toml