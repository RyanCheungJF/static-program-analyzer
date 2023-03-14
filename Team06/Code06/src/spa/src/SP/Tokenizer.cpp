#include "Tokenizer.h"

std::deque<Token> Tokenizer::tokenize(std::stringstream& file) {
    std::deque<Token> tokens;

    while (peekNextToken(file) != EOF) {
        std::string tokenValue = "";

        while (std::isspace(peekNextToken(file))) {
            getNextToken(file); // Eat whitespace
        }

        if (std::isdigit(peekNextToken(file))) { // DIGIT: 0-9
            tokenValue += getNextToken(file);
            /* Between two alphanumeric tokens, there must be at least one whitespace
               in between them, so something like 123ab should throw an error. So we
               simply fetch all the alphanumeric characters & rely on isInteger() to
               check if it's a valid integer.*/
            while (std::isalnum(peekNextToken(file))) {
                tokenValue += getNextToken(file);
            }
            if (!isInteger(tokenValue)) {
                throw SyntaxErrorException("Integer does not follow format: 0 | NZDIGIT ( DIGIT )*  -> " + tokenValue);
            }
            tokens.push_back(Token(TokenType::INTEGER, tokenValue));
        }
        else if (std::isalpha(peekNextToken(file))) { // LETTER: A-Z | a-z
            tokenValue += getNextToken(file);
            while (std::isalnum(peekNextToken(file))) {
                tokenValue += getNextToken(file);
            }
            // Don't need a check for name here, since format for NAME -> LETTER (LETTER | DIGIT)*
            tokens.push_back(Token(TokenType::NAME, tokenValue));
        }
        // Between two non-alphanumeric tokens, there need not be a whitespace in between the tokens
        else if (peekNextToken(file) == AppConstants::NOT) { // Handle !, !=
            getNextToken(file);
            if (peekNextToken(file) == AppConstants::EQUAL_SIGN) {
                getNextToken(file);
                tokens.push_back(Token(TokenType::RELATIONAL_OPR, AppConstants::NOT_EQUALS));
            }
            else {
                tokens.push_back(Token(TokenType::NOT));
            }
        }
        else if (peekNextToken(file) == AppConstants::EQUAL_SIGN) { // Handle =, ==
            getNextToken(file);
            if (peekNextToken(file) == AppConstants::EQUAL_SIGN) {
                getNextToken(file);
                tokens.push_back(Token(TokenType::RELATIONAL_OPR, AppConstants::EQUALS));
            }
            else {
                tokens.push_back(Token(TokenType::ASSIGN));
            }
        }
        else if (peekNextToken(file) == AppConstants::GREATER ||
                 peekNextToken(file) == AppConstants::LESS) { // Handle <, >, <=, >=
            tokenValue += getNextToken(file);
            if (peekNextToken(file) == AppConstants::EQUAL_SIGN) {
                tokenValue += getNextToken(file);
                tokens.push_back(Token(TokenType::RELATIONAL_OPR, tokenValue));
            }
            else {
                tokens.push_back(Token(TokenType::RELATIONAL_OPR, tokenValue));
            }
        }
        else if (peekNextToken(file) == AppConstants::PLUS ||
                 peekNextToken(file) == AppConstants::MINUS) { // Handle +, -
            tokenValue += getNextToken(file);
            tokens.push_back(Token(TokenType::EXPR_ARITH_OPR, tokenValue));
        }
        else if (peekNextToken(file) == AppConstants::MULTIPLY || peekNextToken(file) == AppConstants::DIVIDE ||
                 peekNextToken(file) == AppConstants::MODULO) { // Handle *, /, %
            tokenValue += getNextToken(file);
            tokens.push_back(Token(TokenType::TERM_ARITH_OPR, tokenValue));
        }
        else if (peekNextToken(file) == AppConstants::AMPERSAND ||
                 peekNextToken(file) == AppConstants::VERTICAL_BAR) { // Handle &&, ||
            tokenValue += getNextToken(file);
            tokenValue += getNextToken(file);
            if (tokenValue == AppConstants::AND || tokenValue == AppConstants::OR) {
                tokens.push_back(Token(TokenType::BINARY_LOGICAL_OPR, tokenValue));
            }
            else {
                throw SyntaxErrorException("Expected either '&&' or '||', but got -> " + tokenValue);
            }
        }
        else { // Handle the rest of the tokens
            char tokenChar = getNextToken(file);

            switch (tokenChar) {
            case AppConstants::LEFT_BRACE:
                tokens.push_back(Token(TokenType::LEFT_BRACE));
                break;
            case AppConstants::RIGHT_BRACE:
                tokens.push_back(Token(TokenType::RIGHT_BRACE));
                break;
            case AppConstants::LEFT_PARENTHESIS:
                tokens.push_back(Token(TokenType::LEFT_PARENTHESIS));
                break;
            case AppConstants::RIGHT_PARENTHESIS:
                tokens.push_back(Token(TokenType::RIGHT_PARENTHESIS));
                break;
            case AppConstants::SEMICOLON:
                tokens.push_back(Token(TokenType::SEMICOLON));
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
    tokens.push_back(Token(TokenType::ENDOFFILE));

    return tokens;
}

char Tokenizer::getNextToken(std::stringstream& file) {
    return (char)file.get();
}

char Tokenizer::peekNextToken(std::stringstream& file) {
    return (char)file.peek();
}

bool Tokenizer::isInteger(std::string value) {
    // INTEGER: 0 | NZDIGIT ( DIGIT )*
    std::regex integerRegex("^[1-9][0-9]*$");
    return value == "0" || std::regex_match(value, integerRegex);
}