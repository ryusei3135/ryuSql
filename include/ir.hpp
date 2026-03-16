#pragma once

#include "parser.hpp"
#include <span>


enum class OpCode {
    CreateTable,
    AddColumn,
    EndTable,
};

struct Instruction {
    OpCode op;
    std::span<size_t> args;
};