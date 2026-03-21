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
ENUM_SRC := $(SETTINGS_DIR)/ast_enum.yaml
CPP_ENUM := $(INCLUDE_DIR)/ast_node.hpp
RUST_ENUM := $(RUST_DIR)/ast_node.rs

GEN_SCRIPT := $(SCRIPT_DIR)/generate_ast.py

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

$(CPP_ENUM) $(RUST_ENUM): $(ENUM_SRC) $(GEN_SCRIPT) $(CPP_ENUM) $(RUST_ENUM)
	$(PYTHON) $(GEN_SCRIPT) $(ENUM_SRC) $(CPP_ENUM) $(RUST_ENUM)

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