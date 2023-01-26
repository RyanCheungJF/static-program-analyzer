// Following the grammar rules, these are all the possible token types
enum class TokenType {
	NAME,
	INTEGER,

	LEFT_BRACE, // {
	RIGHT_BRACE, // }
	LEFT_PARENTHESIS, // (
	RIGHT_PARENTHESIS, // )
	SEMICOLON, // ;

	// Expressions
	AND, // &&
	OR, // ||
	NOT, // !
	GREATER, // >
	GREATER_EQUAL, // >=
	LESS, // <
	LESS_EQUAL, // <=
	EQUAL, // ==
	NOT_EQUAL, // !=

	PLUS, // +
	MINUS, // -
	MULTIPLY, // *
	DIVIDE, // /
	MODULO, // %
	ASSIGN, // =

	// Keywords
	PROCEDURE,
	READ,
	PRINT,
	CALL,
	WHILE,
	IF,
	THEN,
	ELSE,
};