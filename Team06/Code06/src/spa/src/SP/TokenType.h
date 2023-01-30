enum class TokenType {
	NAME,
	INTEGER,

	LEFT_BRACE, // {
	RIGHT_BRACE, // }
	LEFT_PARENTHESIS, // (
	RIGHT_PARENTHESIS, // )
	SEMICOLON, // ;

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

	ENDOFFILE
};