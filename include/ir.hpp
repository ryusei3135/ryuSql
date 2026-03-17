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
    std::array<uint32_t, 2> args;
};