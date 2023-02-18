#include "Tokenizer.h"

std::deque<Token> Tokenizer::tokenize(std::stringstream& file) {
	std::deque<Token> tokens;

	while (file.peek() != EOF) {
		std::string tokenValue = "";

		while (std::isspace(file.peek())) {
			file.get(); // Eat whitespace
		}

		if (std::isdigit(file.peek())) { // DIGIT: 0-9
			tokenValue += (char)(file.get());
			/* Between two alphanumeric tokens, there must be at least one whitespace
			   in between them, so something like 123ab should throw an error. So we simply
			   fetch all the alphanumeric characters & rely on isInteger() to check if it's 
			   a valid integer.*/
			while (std::isalnum(file.peek())) {
				tokenValue += (char)(file.get());
			}
			if (!isInteger(tokenValue)) {
				throw SyntaxErrorException("Integer does not follow format: 0 | NZDIGIT ( DIGIT )*  -> " + tokenValue);
			}
			tokens.push_back(Token(TokenType::INTEGER, tokenValue));
		}
		else if (std::isalpha(file.peek())) { // LETTER: A-Z | a-z
			tokenValue += (char)(file.get());
			while (std::isalnum(file.peek())) {
				tokenValue += (char)(file.get());
			}
			// Don't need a check for name here, since format for NAME -> LETTER (LETTER | DIGIT)*
			tokens.push_back(Token(TokenType::NAME, tokenValue));
		}
		// Between two non-alphanumeric tokens, there need not be a whitespace in between the tokens
		else if ((char)file.peek() == '!') { // Handle !, !=
			file.get();
			if ((char)file.peek() == '=') {
				file.get();
				tokens.push_back(Token(TokenType::RELATIONAL_OPR, "!="));
			}
			else {
				tokens.push_back(Token(TokenType::NOT, "!"));
			}
		}
		else if ((char)file.peek() == '=') { // Handle =, ==
			file.get();
			if ((char)file.peek() == '=') {
				file.get();
				tokens.push_back(Token(TokenType::RELATIONAL_OPR, "=="));
			}
			else {
				tokens.push_back(Token(TokenType::ASSIGN, "="));
			}
		}
		else if ((char)file.peek() == '<' || (char)file.peek() == '>') { // Handle <, >, <=, >=
			tokenValue += (char)(file.get());
			if ((char)file.peek() == '=') {
				tokenValue += (char)(file.get());
				tokens.push_back(Token(TokenType::RELATIONAL_OPR, tokenValue));
			}
			else {
				tokens.push_back(Token(TokenType::RELATIONAL_OPR, tokenValue));
			}
		} 
		else if ((char)file.peek() == '+' || (char)file.peek() == '-') { // Handle +, -
			tokenValue += (char)(file.get());
			tokens.push_back(Token(TokenType::EXPR_ARITH_OPR, tokenValue));
		} 
		else if ((char)file.peek() == '*' || (char)file.peek() == '/' || (char)file.peek() == '%') { // Handle *, /, %
			tokenValue += (char)(file.get());
			tokens.push_back(Token(TokenType::TERM_ARITH_OPR, tokenValue));
		}
		else if ((char)file.peek() == '&' || (char)file.peek() == '|') { // Handle &&, ||
			tokenValue += (char)(file.get());
			tokenValue += (char)(file.get());
			if (tokenValue == "&&" || tokenValue == "||") {
				tokens.push_back(Token(TokenType::BINARY_LOGICAL_OPR, tokenValue));
			}
			else {
				throw SyntaxErrorException("Expected either '&&' or '||', but got -> " + tokenValue);
			}
		}
		else { // Handle the rest of the tokens
			char tokenChar = (char)file.get();

			switch (tokenChar) {
			case '{':
				tokens.push_back(Token(TokenType::LEFT_BRACE, "{"));
				break;
			case '}':
				tokens.push_back(Token(TokenType::RIGHT_BRACE, "}"));
				break;
			case '(':
				tokens.push_back(Token(TokenType::LEFT_PARENTHESIS, "("));
				break;
			case ')':
				tokens.push_back(Token(TokenType::RIGHT_PARENTHESIS, ")"));
				break;
			case ';':
				tokens.push_back(Token(TokenType::SEMICOLON, ";"));
				break;
			default:
				// Getting the EOF is a bit buggy.
				if (tokenChar == EOF) {
					break;
				}
				throw SyntaxErrorException("Invalid token");
			}
		}
	}
	tokens.push_back(Token(TokenType::ENDOFFILE, "End of File"));

	return tokens;
}

bool Tokenizer::isInteger(std::string value) {
	// INTEGER: 0 | NZDIGIT ( DIGIT )*
	std::regex integerRegex("^[1-9][0-9]*$");
	return value == "0" || std::regex_match(value, integerRegex);
}