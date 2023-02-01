#include "Parser.h"

std::unique_ptr<Program> Parser::parseProgram(std::deque<Token> tokens) {
	// Rule: procedure+
	std::cout << "Parsing Program" << std::endl;

	std::unique_ptr<Program> program = std::make_unique<Program>();

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

	std::unique_ptr<Procedure> procedure = std::make_unique<Procedure>();

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

	std::unique_ptr<StatementList> statementList = std::make_unique<StatementList>();

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

	std::unique_ptr<ReadStatement> readStatement = std::make_unique<ReadStatement>();

	tokens.pop_front(); // Pop 'read'

	if (tokens.front().type != TokenType::NAME) {
		std::cerr << "Expected var_name in read statement" << std::endl;
	}

	readStatement->varName = tokens.front().value;
	tokens.pop_front(); // Pop var_name

	if (tokens.front().type != TokenType::SEMICOLON) {
		std::cerr << "Expected ; at end of read statement" << std::endl;
	}
	tokens.pop_front(); // Pop ;

	return readStatement;
}

std::unique_ptr<PrintStatement> Parser::parsePrintStatement(std::deque<Token>& tokens) {
	// Rule: 'print' var_name';'
	std::cout << "Parsing Print Statement" << std::endl;

	std::unique_ptr<PrintStatement> printStatement = std::make_unique<PrintStatement>();

	tokens.pop_front(); // Pop 'print'

	if (tokens.front().type != TokenType::NAME) {
		std::cerr << "Expected var_name in print statement" << std::endl;
	}

	printStatement->varName = tokens.front().value;
	tokens.pop_front(); // Pop var_name

	if (tokens.front().type != TokenType::SEMICOLON) {
		std::cerr << "Expected ; at end of print statement" << std::endl;
	}
	tokens.pop_front(); // Pop ;

	return printStatement;
}

std::unique_ptr<CallStatement> Parser::parseCallStatement(std::deque<Token>& tokens) {
	// Rule: 'call' proc_name';'
	std::cout << "Parsing Call Statement" << std::endl;

	std::unique_ptr<CallStatement> callStatement = std::make_unique<CallStatement>();

	tokens.pop_front(); // Pop 'call'

	if (tokens.front().type != TokenType::NAME) {
		std::cerr << "Expected proc_name in call statement" << std::endl;
	}

	callStatement->procName = tokens.front().value;
	tokens.pop_front(); // Pop proc_name

	if (tokens.front().type != TokenType::SEMICOLON) {
		std::cerr << "Expected ; at end of call statement" << std::endl;
	}
	tokens.pop_front(); // Pop ;

	return callStatement;
}

std::unique_ptr<WhileStatement> Parser::parseWhileStatement(std::deque<Token>& tokens) {
	// Rule: 'while' '(' cond_expr ')' '{' stmtLst '}'
	std::cout << "Parsing While Statement" << std::endl;

	std::unique_ptr<WhileStatement> whileStatement = std::make_unique<WhileStatement>();

	tokens.pop_front(); // Pop 'while'

	if (tokens.front().type != TokenType::LEFT_PARENTHESIS) {
		std::cerr << "Expected '(' in while statement" << std::endl;
	}
	tokens.pop_front(); // Pop '('

	// Parse conditional expr
	whileStatement->condExpr = parseConditionalExpression(tokens);

	if (tokens.front().type != TokenType::RIGHT_PARENTHESIS) {
		std::cerr << "Expected ')' in while statement" << std::endl;
	}
	tokens.pop_front(); // Pop ')'

	if (tokens.front().type != TokenType::LEFT_BRACE) {
		std::cerr << "Expected '{' in while statement" << std::endl;
	}
	tokens.pop_front(); // Pop '{'

	// Parse stmtList
	whileStatement->stmtList = parseStatementList(tokens);

	if (tokens.front().type != TokenType::RIGHT_BRACE) {
		std::cerr << "Expected '}' in while statement" << std::endl;
	}
	tokens.pop_front(); // Pop '}'

	return whileStatement;
}

std::unique_ptr<IfStatement> Parser::parseIfStatement(std::deque<Token>& tokens) {
	// Rule: 'if' '(' cond_expr ')' 'then' '{' stmtLst '}' 'else' '{' stmtLst '}'
	std::cout << "Parsing If Statement" << std::endl;

	std::unique_ptr<IfStatement> ifStatement = std::make_unique<IfStatement>();

	tokens.pop_front(); // Pop 'if'

	if (tokens.front().type != TokenType::LEFT_PARENTHESIS) {
		std::cerr << "Expected '(' in if statement" << std::endl;
	}
	tokens.pop_front(); // Pop '('

	// Parse conditional expr
	ifStatement->condExpr = parseConditionalExpression(tokens);

	if (tokens.front().type != TokenType::RIGHT_PARENTHESIS) {
		std::cerr << "Expected ')' in if statement" << std::endl;
	}
	tokens.pop_front(); // Pop ')'

	if (tokens.front().type != TokenType::LEFT_BRACE) {
		std::cerr << "Expected '{' in if statement" << std::endl;
	}
	tokens.pop_front(); // Pop '{'

	// Parse stmtList
	ifStatement->thenStmtList = parseStatementList(tokens);

	if (tokens.front().type != TokenType::RIGHT_BRACE) {
		std::cerr << "Expected '}' in if statement" << std::endl;
	}
	tokens.pop_front(); // Pop '}'

	if (tokens.front().type != TokenType::NAME || tokens.front().value != "else") {
		std::cerr << "Expected 'else' in if statement" << std::endl;
	}

	if (tokens.front().type != TokenType::LEFT_BRACE) {
		std::cerr << "Expected '{' in if statement" << std::endl;
	}
	tokens.pop_front(); // Pop '{'

	// Parse stmtList
	ifStatement->elseStmtList = parseStatementList(tokens);

	if (tokens.front().type != TokenType::RIGHT_BRACE) {
		std::cerr << "Expected '}' in if statement" << std::endl;
	}
	tokens.pop_front(); // Pop '}'

	return ifStatement;
}

std::unique_ptr<AssignStatement> Parser::parseAssignStatement(std::deque<Token>& tokens) {
	// Rule: var_name '=' expr ';'
	std::cout << "Parsing Assign Statement" << std::endl;

	std::unique_ptr<AssignStatement> assignStatement = std::make_unique<AssignStatement>();

	assignStatement->varName = tokens.front().value;
	tokens.pop_front(); // Pop var_name

	if (tokens.front().type != TokenType::ASSIGN) {
		throw SyntaxErrorException("Expected '=', but got -> " + tokens.front().value);
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
		std::unique_ptr<NotConditionalExpression> notConditionalExpression;
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
		std::unique_ptr<BinaryConditionalExpression> binaryConditionalExpression;
		tokens.pop_front(); // Pop '('

		binaryConditionalExpression->lhs = parseConditionalExpression(tokens);

		if (tokens.front().type != TokenType::RIGHT_PARENTHESIS) {
			throw SyntaxErrorException("Expected ')' in binary conditional expression, but got -> " + tokens.front().value);
		}
		tokens.pop_front(); // Pop ')'

		if (tokens.front().type != TokenType::AND || tokens.front().type != TokenType::OR) {
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

	std::unique_ptr<RelationalExpression> relationalExpression;
	relationalExpression->lhs = parseRelationalFactor(tokens);

	if (tokens.front().type != TokenType::GREATER || tokens.front().type != TokenType::GREATER_EQUAL ||
		tokens.front().type != TokenType::LESS || tokens.front().type != TokenType::LESS_EQUAL ||
		tokens.front().type != TokenType::EQUAL || tokens.front().type != TokenType::NOT_EQUAL) {
		throw SyntaxErrorException("Expected a comparator in relational expression, but got -> " + tokens.front().value);
	}
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
		std::unique_ptr<MathExpression> mathExpression;
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
		std::unique_ptr<MathExpression> mathExpression;
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

		Constant constant(stoi(tokens.front().value));
		tokens.pop_front();
		return std::make_unique<Constant>(constant);
	} else if (tokens.front().type == TokenType::NAME) {
		std::cout << "Parsing Variable" << std::endl;

		Variable variable(tokens.front().value);
		tokens.pop_front();
		return std::make_unique<Variable>(variable);
	} else if (tokens.front().type == TokenType::LEFT_PARENTHESIS) {
		std::unique_ptr<Expression> expression;
		tokens.pop_front(); // Pop '('

		expression = parseExpression(tokens);

		if (tokens.front().type != TokenType::RIGHT_PARENTHESIS) {\
			throw SyntaxErrorException("Expected ')' in factor, but got -> " + tokens.front().value);
		}
		tokens.pop_front(); // Pop ')'

		return expression;
	} else {
		throw SyntaxErrorException("Likely messed up somewhere " + tokens.front().value);
	}
}