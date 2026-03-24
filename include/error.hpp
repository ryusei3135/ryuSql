#pragma once


enum class Errors {
    InvalidString,
    InvalidTokenKind,
    UnexpectTokenKind,
    MissingTokenKind,
    SyntaxErr,
    // 実行中の自分自身のバグ
    NodeHasAlreadyAssigned
};