#pragma once

#include <iostream>

enum class Errors {
    UnexpectedToken,
    UnexpectedNode,
    MissingToken,
    SyntaxErr,
};

void output_err_log(Errors err_kind);