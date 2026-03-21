# ==== コンパイラ/インタプリタ ====
CXX := g++
RUSTC := rustc
PYTHON := python3

# ===== ディレクトリ =====
ROOT_DIR := .
SRC_DIR := $(ROOT_DIR)/src
INCLUDE_DIR := $(ROOT_DIR)/include
RUST_DIR := $(ROOT_DIR)/rust
SETTINGS_DIR := $(ROOT_DIR)/settings
SCRIPT_DIR := $(ROOT_DIR)/scripts
BUILD_DIR := $(ROOT_DIR)/build

# ==== パス ====
STRUCT_SRC := $(SETTINGS_DIR)/rust_c_structs.yaml
CPP_ENUM := $(INCLUDE_DIR)/ast_node.hpp
CPP_TOKEN := $(INCLUDE_DIR)/token.hpp
RUST_ENUM := $(RUST_DIR)/ast_node.rs
RUST_TOKEN := $(RUST_DIR)/token.rs

ARGV := $(CPP_ENUM) $(RUST_ENUM) $(CPP_TOKEN) $(RUST_TOKEN)

GEN_SCRIPT := $(SCRIPT_DIR)/generate_structs.py

# ==== オプション ====
CXXFLAGS := -O2 -std=c++23 -I$(INCLUDE_DIR)
RUSTFLAGS := --crate-type=staticlib -O

# ==== ソース ====
CPP_SRCS := $(shell find $(SRC_DIR) -name "*.cpp")
CPP_OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(CPP_SRCS))

# Rustは1つのライブラリとしてまとめる
RUST_LIB := $(BUILD_DIR)/librust.a

# ==== ターゲット ====
TARGET := db

# ==== ビルド ====
all: gen $(TARGET)

# enum生成
gen: $(CPP_ENUM) $(RUST_ENUM)

$(ARGV): $(STRUCT_SRC) $(GEN_SCRIPT) $(ARGV)
	$(PYTHON) $(GEN_SCRIPT) $(STRUCT_SRC) $(ARGV)

# C++
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(CPP_ENUM)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rust（まとめて1回ビルド）
$(RUST_LIB): $(RUST_ENUM)
	@mkdir -p $(BUILD_DIR)
	$(RUSTC) $(RUSTFLAGS) $< -o $@

# リンク
$(TARGET): $(CPP_OBJS) $(RUST_LIB)
	$(CXX) $^ -o $@

# ==== クリーン ====
clean:
	rm -rf $(BUILD_DIR) $(TARGET)