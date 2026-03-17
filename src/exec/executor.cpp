#include "exec.hpp"


void executor(IR& ir, std::vector<Token>& tokens) {
    for (Instruction node: ir) {
        switch (node.op) {
            case OpCode::CreateTable: {
                //
            }
            case OpCode::SelectTarget: {
                //
            }
        }
    }
}