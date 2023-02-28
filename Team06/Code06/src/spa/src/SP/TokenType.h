enum class TokenType {
  NAME,
  INTEGER,

  LEFT_BRACE,        // {
  RIGHT_BRACE,       // }
  LEFT_PARENTHESIS,  // (
  RIGHT_PARENTHESIS, // )
  SEMICOLON,         // ;

  BINARY_LOGICAL_OPR, // &&, ||
  NOT,                // !

  RELATIONAL_OPR, // >, <, >=, <=, ==, !=

  EXPR_ARITH_OPR, // +, -
  TERM_ARITH_OPR, // *, /, %

  ASSIGN, // =

  ENDOFFILE
};