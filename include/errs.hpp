#pragma once

#include <iostream>

enum class Errors {
    Missing_SELECT_keyword,
    Missing_CREATE_keyword,
    Missing_left_paren_token,
    MissingTableName,
    MissingColumn,
    ColumnNotFound,
    UnexpectedToken,
    InvaildColumn,
    SyntaxErr,
};

void output_err_log(Errors err_kind);