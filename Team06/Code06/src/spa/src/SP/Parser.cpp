#include "Parser.h"

int currStatementNumber = 1;

std::unique_ptr<Program> Parser::parseProgram(std::deque<Token> tokens) {
	// Rule: procedure+
	std::cout << "Parsing Program" << std::endl;

	auto program = std::make_unique<Program>();

	while (tokens.front().type != TokenType::ENDOFFILE) {
		program->procedureList.push_back(parseProcedure(tokens));
	}

	if (program->procedureList.size() == 0) {
		throw SyntaxErrorException("Program should contain at least one procedure");
	}

	return program;
}

std::unique_ptr<Procedure> Parser::parseProcedure(std::deque<Token>& tokens) {
	// Rule: 'procedure' proc_name '{' stmtLst '}'
	std::cout << "Parsing Procedure" << std::endl;

	auto procedure = std::make_unique<Procedure>();

	if (tokens.front().type != TokenType::NAME || tokens.front().value != "procedure") {
		throw SyntaxErrorException("Expected 'procedure' keyword, but got -> " + tokens.front().value);
	}
	tokens.pop_front();

	if (tokens.front().type != TokenType::NAME) {
		throw SyntaxErrorException("Expected 'proc_name', but got -> " + tokens.front().value);
	}
	procedure->procedureName = tokens.front().value;
	tokens.pop_front();

	if (tokens.front().type != TokenType::LEFT_BRACE) {
		throw SyntaxErrorException("Expected '{', but got -> " + tokens.front().value);
	}
	tokens.pop_front();

	procedure->statementList = parseStatementList(tokens);

	if (tokens.front().type != TokenType::RIGHT_BRACE) {
		throw SyntaxErrorException("Expected '}', but got -> " + tokens.front().value);
	}
	tokens.pop_front();

	return procedure;
}

std::unique_ptr<StatementList> Parser::parseStatementList(std::deque<Token>& tokens) {
	// Rule: stmt+
	std::cout << "Parsing Statement List" << std::endl;

	auto statementList = std::make_unique<StatementList>();

	while (tokens.front().type != TokenType::RIGHT_BRACE) { // Reached end of statementList
		statementList->statements.push_back(parseStatement(tokens));
	}

	if (statementList->statements.size() == 0) {
		throw SyntaxErrorException("Statement List should contain at least one statement");
	}

	return statementList;
}

std::unique_ptr<Statement> Parser::parseStatement(std::deque<Token>& tokens) {
	// Rule: read | print | call | while | if | assign
	if (tokens.front().type == TokenType::NAME) {
		if (tokens.front().value == "read") {
			auto readStatement = parseReadStatement(tokens);
			return readStatement;
		} else if (tokens.front().value == "print") {
			auto printStatement = parsePrintStatement(tokens);
			return printStatement;
		} else if (tokens.front().value == "call") {
			auto callStatement = parseCallStatement(tokens);
			return callStatement;
		} else if (tokens.front().value == "while") {
			auto whileStatement = parseWhileStatement(tokens);
			return whileStatement;
		} else if (tokens.front().value == "if") {
			auto ifStatement = parseIfStatement(tokens);
			return ifStatement;
		} else { // Else, it must be an assign statement
			auto assignStatement = parseAssignStatement(tokens);
			return assignStatement;
		}
	} else {
		throw SyntaxErrorException("Unexpected token when parsing statement -> " + tokens.front().value);
	}
}

std::unique_ptr<ReadStatement> Parser::parseReadStatement(std::deque<Token>& tokens) {
	// Rule: 'read' var_name';'
	std::cout << "Parsing Read Statement" << std::endl;

	auto readStatement = std::make_unique<ReadStatement>();
	readStatement->statementNumber = currStatementNumber;
	currStatementNumber++;

	tokens.pop_front(); // Pop 'read'

	if (tokens.front().type != TokenType::NAME) {
		throw SyntaxErrorException("Expected var_name in read statement, but got -> " + tokens.front().value);
	}

	readStatement->varName = tokens.front().value;
	tokens.pop_front(); // Pop var_name

	if (tokens.front().type != TokenType::SEMICOLON) {
		throw SyntaxErrorException("Expected ; at end of read statement, but got -> " + tokens.front().value);
	}
	tokens.pop_front(); // Pop ;

	return readStatement;
}

std::unique_ptr<PrintStatement> Parser::parsePrintStatement(std::deque<Token>& tokens) {
	// Rule: 'print' var_name';'
	std::cout << "Parsing Print Statement" << std::endl;

	auto printStatement = std::make_unique<PrintStatement>();
	printStatement->statementNumber = currStatementNumber;
	currStatementNumber++;

	tokens.pop_front(); // Pop 'print'

	if (tokens.front().type != TokenType::NAME) {
		throw SyntaxErrorException("Expected var_name in print statement, but got -> " + tokens.front().value);
	}

	printStatement->varName = tokens.front().value;
	tokens.pop_front(); // Pop var_name

	if (tokens.front().type != TokenType::SEMICOLON) {
		throw SyntaxErrorException("Expected ; at end of print statement, but got -> " + tokens.front().value);
	}
	tokens.pop_front(); // Pop ;

	return printStatement;
}

std::unique_ptr<CallStatement> Parser::parseCallStatement(std::deque<Token>& tokens) {
	// Rule: 'call' proc_name';'
	std::cout << "Parsing Call Statement" << std::endl;

	auto callStatement = std::make_unique<CallStatement>();
	callStatement->statementNumber = currStatementNumber;
	currStatementNumber++;

	tokens.pop_front(); // Pop 'call'

	if (tokens.front().type != TokenType::NAME) {
		throw SyntaxErrorException("Expected proc_name in call statement, but got -> " + tokens.front().value);
	}

	callStatement->procName = tokens.front().value;
	tokens.pop_front(); // Pop proc_name

	if (tokens.front().type != TokenType::SEMICOLON) {
		throw SyntaxErrorException("Expected ; at end of call statement, but got -> " + tokens.front().value);
	}
	tokens.pop_front(); // Pop ;

	return callStatement;
}

std::unique_ptr<WhileStatement> Parser::parseWhileStatement(std::deque<Token>& tokens) {
	// Rule: 'while' '(' cond_expr ')' '{' stmtLst '}'
	std::cout << "Parsing While Statement" << std::endl;

	auto whileStatement = std::make_unique<WhileStatement>();
	whileStatement->statementNumber = currStatementNumber;
	currStatementNumber++;

	tokens.pop_front(); // Pop 'while'

	if (tokens.front().type != TokenType::LEFT_PARENTHESIS) {
		throw SyntaxErrorException("Expected '(' in while statement, but got -> " + tokens.front().value);
	}
	tokens.pop_front(); // Pop '('

	// Parse conditional expr
	whileStatement->condExpr = parseConditionalExpression(tokens);

	if (tokens.front().type != TokenType::RIGHT_PARENTHESIS) {
		throw SyntaxErrorException("Expected ')' in while statement, but got -> " + tokens.front().value);
	}
	tokens.pop_front(); // Pop ')'

	if (tokens.front().type != TokenType::LEFT_BRACE) {
		throw SyntaxErrorException("Expected '{' in while statement, but got -> " + tokens.front().value);
	}
	tokens.pop_front(); // Pop '{'

	// Parse stmtList
	whileStatement->stmtList = parseStatementList(tokens);

	if (tokens.front().type != TokenType::RIGHT_BRACE) {
		throw SyntaxErrorException("Expected '}' in while statement, but got -> " + tokens.front().value);
	}
	tokens.pop_front(); // Pop '}'

	return whileStatement;
}

std::unique_ptr<IfStatement> Parser::parseIfStatement(std::deque<Token>& tokens) {
	// Rule: 'if' '(' cond_expr ')' 'then' '{' stmtLst '}' 'else' '{' stmtLst '}'
	std::cout << "Parsing If Statement" << std::endl;

	auto ifStatement = std::make_unique<IfStatement>();
	ifStatement->statementNumber = currStatementNumber;
	currStatementNumber++;

	tokens.pop_front(); // Pop 'if'

	if (tokens.front().type != TokenType::LEFT_PARENTHESIS) {
		throw SyntaxErrorException("Expected '(' in if statement, but got -> " + tokens.front().value);
	}
	tokens.pop_front(); // Pop '('

	// Parse conditional expr
	ifStatement->condExpr = parseConditionalExpression(tokens);

	if (tokens.front().type != TokenType::RIGHT_PARENTHESIS) {
		throw SyntaxErrorException("Expected ')' in if statement, but got -> " + tokens.front().value);
	}
	tokens.pop_front(); // Pop ')'

	if (tokens.front().type != TokenType::NAME || tokens.front().value != "then") {
		throw SyntaxErrorException("Expected 'then' in if statement, but got -> " + tokens.front().value);
	}
	tokens.pop_front(); // Pop 'then'

	if (tokens.front().type != TokenType::LEFT_BRACE) {
		throw SyntaxErrorException("Expected '{' in if statement, but got -> " + tokens.front().value);
	}
	tokens.pop_front(); // Pop '{'

	// Parse stmtList
	ifStatement->thenStmtList = parseStatementList(tokens);

	if (tokens.front().type != TokenType::RIGHT_BRACE) {
		throw SyntaxErrorException("Expected '}' in if statement, but got -> " + tokens.front().value);
	}
	tokens.pop_front(); // Pop '}'

	if (tokens.front().type != TokenType::NAME || tokens.front().value != "else") {
		throw SyntaxErrorException("Expected 'else' in if statement, but got -> " + tokens.front().value);
	}
	tokens.pop_front(); // Pop 'else'

	if (tokens.front().type != TokenType::LEFT_BRACE) {
		throw SyntaxErrorException("Expected '{' in if statement, but got -> " + tokens.front().value);
	}
	tokens.pop_front(); // Pop '{'

	// Parse stmtList
	ifStatement->elseStmtList = parseStatementList(tokens);

	if (tokens.front().type != TokenType::RIGHT_BRACE) {
		throw SyntaxErrorException("Expected '}' in if statement, but got -> " + tokens.front().value);
	}
	tokens.pop_front(); // Pop '}'

	return ifStatement;
}

std::unique_ptr<AssignStatement> Parser::parseAssignStatement(std::deque<Token>& tokens) {
	// Rule: var_name '=' expr ';'
	std::cout << "Parsing Assign Statement" << std::endl;

	auto assignStatement = std::make_unique<AssignStatement>();
	assignStatement->statementNumber = currStatementNumber;
	currStatementNumber++;

	assignStatement->varName = tokens.front().value;
	tokens.pop_front(); // Pop var_name

	if (tokens.front().type != TokenType::ASSIGN) {
		throw SyntaxErrorException("Expected '=' in assign statement, but got -> " + tokens.front().value);
	}
	tokens.pop_front(); // Pop "="

	assignStatement->expr = parseExpression(tokens);

	if (tokens.front().type != TokenType::SEMICOLON) {
		throw SyntaxErrorException("Expected ';' at end of assign statement, but got -> " + tokens.front().value);
	}
	tokens.pop_front(); // Pop ;

	return assignStatement;
}

std::unique_ptr<ConditionalExpression> Parser::parseConditionalExpression(std::deque<Token>& tokens) {
	// Rule: rel_expr | '!' '(' cond_expr ')' | '(' cond_expr ')' '&&' '(' cond_expr ')' | '(' cond_expr ')' '||' '(' cond_expr ')'
	// Need to handle distinguishment between parsing a rel_expr & expr.
	std::cout << "Parsing Conditional Expression" << std::endl;

	if (tokens.front().type == TokenType::NOT) { // '!' '(' path
		auto notConditionalExpression = std::make_unique<NotConditionalExpression>();
		tokens.pop_front(); // Pop '!'

		if (tokens.front().type != TokenType::LEFT_PARENTHESIS) {
			throw SyntaxErrorException("Expected '(' in not conditional expression, but got -> " + tokens.front().value);
		}
		tokens.pop_front(); // Pop '('

		notConditionalExpression->condExpr = parseConditionalExpression(tokens);

		if (tokens.front().type != TokenType::RIGHT_PARENTHESIS) {
			throw SyntaxErrorException("Expected ')' in not conditional expression, but got -> " + tokens.front().value);
		}
		tokens.pop_front(); // Pop ')'

		return notConditionalExpression;
	} else if (tokens.front().type == TokenType::LEFT_PARENTHESIS) { // Binary Cond Expr Path
		auto binaryConditionalExpression = std::make_unique<BinaryConditionalExpression>();
		tokens.pop_front(); // Pop '('

		binaryConditionalExpression->lhs = parseConditionalExpression(tokens);

		if (tokens.front().type != TokenType::RIGHT_PARENTHESIS) {
			throw SyntaxErrorException("Expected ')' in binary conditional expression, but got -> " + tokens.front().value);
		}
		tokens.pop_front(); // Pop ')'

		if (tokens.front().type != TokenType::AND && tokens.front().type != TokenType::OR) {
			throw SyntaxErrorException("Expected '&&' or '||' in binary conditional expression, but got -> " + tokens.front().value);
		}
		binaryConditionalExpression->condOperator = tokens.front().value;
		tokens.pop_front(); // Pop '&&' or '||'

		if (tokens.front().type != TokenType::LEFT_PARENTHESIS) {
			throw SyntaxErrorException("Expected '(' in binary conditional expression, but got -> " + tokens.front().value);
		}
		tokens.pop_front(); // Pop '('

		binaryConditionalExpression->rhs = parseConditionalExpression(tokens);

		if (tokens.front().type != TokenType::RIGHT_PARENTHESIS) {
			throw SyntaxErrorException("Expected ')' in conditional expression, but got -> " + tokens.front().value);
		}
		tokens.pop_front(); // Pop ')'

		return binaryConditionalExpression;
	} else { // Relational Expression
		return parseRelationalExpression(tokens);
	}
}

std::unique_ptr<ConditionalExpression> Parser::parseRelationalExpression(std::deque<Token>& tokens) {
	/* Rule: rel_factor '>' rel_factor | rel_factor '>=' rel_factor |
			 rel_factor '<' rel_factor | rel_factor '<=' rel_factor |
		     rel_factor '==' rel_factor | rel_factor '!=' rel_factor */
	std::cout << "Parsing Relational Expression" << std::endl;

	auto relationalExpression = std::make_unique<RelationalExpression>();
	relationalExpression->lhs = parseRelationalFactor(tokens);

	if (tokens.front().type != TokenType::GREATER && tokens.front().type != TokenType::GREATER_EQUAL &&
		tokens.front().type != TokenType::LESS && tokens.front().type != TokenType::LESS_EQUAL &&
		tokens.front().type != TokenType::EQUAL && tokens.front().type != TokenType::NOT_EQUAL) {
		throw SyntaxErrorException("Expected a comparator in relational expression, but got -> " + tokens.front().value);
	}
	relationalExpression->relationalOperator = tokens.front().value;
	tokens.pop_front(); // Pop relational comparator

	relationalExpression->rhs = parseRelationalFactor(tokens);
	return relationalExpression;
}

std::unique_ptr<Expression> Parser::parseRelationalFactor(std::deque<Token>& tokens) {
	// Rule: var_name | const_value | expr
	// Not quite sure how to distinguish which path to take, for now just parse it as an expression.
	std::cout << "Parsing Relational Factor" << std::endl;

	return parseExpression(tokens);
}

std::unique_ptr<Expression> Parser::parseExpression(std::deque<Token>& tokens) {
	/* Rule: expr: expr '+' term | expr '-' term | term
	*  After eliminating left recursion:
	*    expr: term(expr')
	*    expr': '+' term(expr') | '-' term(expr') | epsilon
	*/ 
	std::cout << "Parsing Expression" << std::endl;

	std::unique_ptr<Expression> lhs = parseTerm(tokens);

	if (tokens.front().type == TokenType::PLUS || tokens.front().type == TokenType::MINUS) {
		auto mathExpression = std::make_unique<MathExpression>();
		mathExpression->lhs;
		mathExpression->mathOperator = tokens.front().value;
		tokens.pop_front(); // Pop the '+' or '-'
		mathExpression->rhs = parseExpression(tokens);
		return mathExpression;
	} else { // Reached the epsilon
		return lhs;
	}
}

std::unique_ptr<Expression> Parser::parseTerm(std::deque<Token>& tokens) {
	/* Rule: term '*' factor | term '/' factor | term '%' factor | factor
	*  After eliminating left recursion:
	*    term: factor(term')
    *    term': '*' factor(term') | '/' factor(term') | '%' factor(term') | epsilon
	*/
	std::cout << "Parsing Term" << std::endl;

	std::unique_ptr<Expression> lhs = parseFactor(tokens);

	if (tokens.front().type == TokenType::MULTIPLY || tokens.front().type == TokenType::DIVIDE ||
		tokens.front().type == TokenType::MODULO) {
		auto mathExpression = std::make_unique<MathExpression>();
		mathExpression->lhs;
		mathExpression->mathOperator = tokens.front().value;
		tokens.pop_front(); // Pop the '*' or '/' or '%'
		mathExpression->rhs = parseTerm(tokens);
		return mathExpression;
	} else { // Reached the epsilon
		return lhs;
	}
}

std::unique_ptr<Expression> Parser::parseFactor(std::deque<Token>& tokens) {
	// Rule: var_name | const_value | '(' expr ')'
	std::cout << "Parsing Factor" << std::endl;

	if (tokens.front().type == TokenType::INTEGER) {
		std::cout << "Parsing Constant" << std::endl;

		auto constant = std::make_unique<Constant>();
		constant->value = stoi(tokens.front().value);
		tokens.pop_front();
		return constant;
	} else if (tokens.front().type == TokenType::NAME) {
		std::cout << "Parsing Variable" << std::endl;

		auto variable = std::make_unique<Variable>();
		variable->name = tokens.front().value;
		tokens.pop_front();
		return variable;
	} else if (tokens.front().type == TokenType::LEFT_PARENTHESIS) {
		std::unique_ptr<Expression> expression;
		tokens.pop_front(); // Pop '('

		expression = parseExpression(tokens);

		if (tokens.front().type != TokenType::RIGHT_PARENTHESIS) {
			throw SyntaxErrorException("Expected ')' in factor, but got -> " + tokens.front().value);
		}
		tokens.pop_front(); // Pop ')'

		return expression;
	} else {
		throw SyntaxErrorException("Parsing factor failed, got -> " + tokens.front().value);
	}
}