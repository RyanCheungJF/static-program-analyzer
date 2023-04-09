#include "Tokenizer.h"

std::deque<Token> Tokenizer::tokenize(std::stringstream& file) {
    std::deque<Token> tokens;

    while (peekNextChar(file) != EOF) {
        std::string tokenValue = "";

        while (std::isspace(peekNextChar(file))) {
            getNextChar(file); // Eat whitespace
        }

        if (std::isdigit(peekNextChar(file))) { // DIGIT: 0-9
            tokenValue += getNextChar(file);
            /* Between two alphanumeric tokens, there must be at least one whitespace
               in between them, so something like 123ab should throw an error. So we
               simply fetch all the alphanumeric characters & rely on isInteger() to
               check if it's a valid integer.*/
            while (std::isalnum(peekNextChar(file))) {
                tokenValue += getNextChar(file);
            }
            if (!isInteger(tokenValue)) {
                throw SyntaxErrorException("Integer does not follow format: 0 | NZDIGIT ( DIGIT )*  -> " + tokenValue);
            }
            tokens.emplace_back(Token(TokenType::INTEGER, tokenValue));
        }
        else if (std::isalpha(peekNextChar(file))) { // LETTER: A-Z | a-z
            tokenValue += getNextChar(file);
            while (std::isalnum(peekNextChar(file))) {
                tokenValue += getNextChar(file);
            }
            // Don't need a check for name here, since format for NAME -> LETTER (LETTER | DIGIT)*
            tokens.emplace_back(Token(TokenType::NAME, tokenValue));
        }
        // Between two non-alphanumeric tokens, there need not be a whitespace in between the tokens
        else if (peekNextChar(file) == AppConstants::NOT) { // Handle !, !=
            getNextChar(file);
            if (peekNextChar(file) == AppConstants::EQUAL_SIGN) {
                getNextChar(file);
                tokens.emplace_back(Token(TokenType::RELATIONAL_OPR, AppConstants::NOT_EQUALS));
            }
            else {
                tokens.emplace_back(Token(TokenType::NOT));
            }
        }
        else if (peekNextChar(file) == AppConstants::EQUAL_SIGN) { // Handle =, ==
            getNextChar(file);
            if (peekNextChar(file) == AppConstants::EQUAL_SIGN) {
                getNextChar(file);
                tokens.emplace_back(Token(TokenType::RELATIONAL_OPR, AppConstants::EQUALS));
            }
            else {
                tokens.emplace_back(Token(TokenType::ASSIGN));
            }
        }
        else if (peekNextChar(file) == AppConstants::GREATER ||
                 peekNextChar(file) == AppConstants::LESS) { // Handle <, >, <=, >=
            tokenValue += getNextChar(file);
            if (peekNextChar(file) == AppConstants::EQUAL_SIGN) {
                tokenValue += getNextChar(file);
                tokens.emplace_back(Token(TokenType::RELATIONAL_OPR, tokenValue));
            }
            else {
                tokens.emplace_back(Token(TokenType::RELATIONAL_OPR, tokenValue));
            }
        }
        else if (peekNextChar(file) == AppConstants::PLUS || peekNextChar(file) == AppConstants::MINUS) { // Handle +, -
            tokenValue += getNextChar(file);
            tokens.emplace_back(Token(TokenType::EXPR_ARITH_OPR, tokenValue));
        }
        else if (peekNextChar(file) == AppConstants::MULTIPLY || peekNextChar(file) == AppConstants::DIVIDE ||
                 peekNextChar(file) == AppConstants::MODULO) { // Handle *, /, %
            tokenValue += getNextChar(file);
            tokens.emplace_back(Token(TokenType::TERM_ARITH_OPR, tokenValue));
        }
        else if (peekNextChar(file) == AppConstants::AMPERSAND ||
                 peekNextChar(file) == AppConstants::VERTICAL_BAR) { // Handle &&, ||
            tokenValue += getNextChar(file);
            tokenValue += getNextChar(file);
            if (tokenValue == AppConstants::AND || tokenValue == AppConstants::OR) {
                tokens.emplace_back(Token(TokenType::BINARY_LOGICAL_OPR, tokenValue));
            }
            else {
                throw SyntaxErrorException("Expected either '&&' or '||', but got -> " + tokenValue);
            }
        }
        else { // Handle the rest of the tokens
            char tokenChar = getNextChar(file);

            switch (tokenChar) {
            case AppConstants::LEFT_BRACE:
                tokens.emplace_back(Token(TokenType::LEFT_BRACE));
                break;
            case AppConstants::RIGHT_BRACE:
                tokens.emplace_back(Token(TokenType::RIGHT_BRACE));
                break;
            case AppConstants::LEFT_PARENTHESIS:
                tokens.emplace_back(Token(TokenType::LEFT_PARENTHESIS));
                break;
            case AppConstants::RIGHT_PARENTHESIS:
                tokens.emplace_back(Token(TokenType::RIGHT_PARENTHESIS));
                break;
            case AppConstants::SEMICOLON:
                tokens.emplace_back(Token(TokenType::SEMICOLON));
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
    tokens.emplace_back(Token(TokenType::ENDOFFILE));

    return tokens;
}

char Tokenizer::getNextChar(std::stringstream& file) {
    return (char)file.get();
}

char Tokenizer::peekNextChar(std::stringstream& file) {
    return (char)file.peek();
}

bool Tokenizer::isInteger(std::string value) {
    // INTEGER: 0 | NZDIGIT ( DIGIT )*
    std::regex integerRegex("^[1-9][0-9]*$");
    return value == "0" || std::regex_match(value, integerRegex);
}