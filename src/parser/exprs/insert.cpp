#include "parser.hpp"


// enum class ExprFlags {
//     NextTokenIs_INSERT_,
//     NextTokenIs_INTO_,
//     NextTokenIsTableName,
// };

// struct ExprInsert : AstPush {
//     ExprFlags flags;

//     ExprInsert() {
//         flags = ExprFlags::NextTokenIs_INSERT_;
//     }
// };

// void create_insert_node(const std::vector<Token>& tokens) {
//     ExprInsert status;

//     for (size_t i = 0; i < tokens.size(); i++) {
//         switch (tokens[i].token_kind) {
//             case TokenKind::Insert: {
//                 if (status.flags == ExprFlags::NextTokenIs_INSERT_) {
//                     status.flags = ExprFlags::NextTokenIs_INTO_;
//                 }
//             }
//             case TokenKind::Into: {
//                 if (status.flags == ExprFlags::NextTokenIs_INTO_) {
//                     status.flags = ExprFlags::NextTokenIsTableName;
//                 }
//             }
//             case TokenKind::STRING: {
//                 //
//             }
//             case TokenKind::LeftParen: {
//                 //
//             }
//         }
//     }
// }