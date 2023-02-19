#include "Parser.h"

StmtNum currStatementNumber = 1;

std::unique_ptr<Program> Parser::parseProgram(std::deque<Token> tokens) {
	// Rule: procedure+
	currStatementNumber = 1; // Reset the statement number, upon parsing a new program
	std::vector<std::unique_ptr<Procedure>> procedureList;

	while (!tokens.front().isType(TokenType::ENDOFFILE)) {
		procedureList.push_back(parseProcedure(tokens));
	}

	if (procedureList.size() == 0) {
		throw SyntaxErrorException("Program should contain at least one procedure");
	}

	return std::make_unique<Program>(std::move(procedureList));
}

std::unique_ptr<Procedure> Parser::parseProcedure(std::deque<Token>& tokens) {
	// Rule: 'procedure' proc_name '{' stmtLst '}'
	if (!tokens.front().isType(TokenType::NAME) || !tokens.front().hasValue(AppConstants::PROCEDURE)) {
		throw SyntaxErrorException("Expected 'procedure' keyword, but got -> " + tokens.front().value);
	}
	tokens.pop_front();

	if (!tokens.front().isType(TokenType::NAME)) {
		throw SyntaxErrorException("Expected valid 'proc_name', but got -> " + tokens.front().value);
	}
	ProcName procedureName = tokens.front().value;
	tokens.pop_front();

	if (!tokens.front().isType(TokenType::LEFT_BRACE)) {
		throw SyntaxErrorException("Expected '{', but got -> " + tokens.front().value);
	}
	tokens.pop_front();

	auto statementList = parseStatementList(tokens);

	if (!tokens.front().isType(TokenType::RIGHT_BRACE)) {
		throw SyntaxErrorException("Expected '}', but got -> " + tokens.front().value);
	}
	tokens.pop_front();

	return std::make_unique<Procedure>(procedureName, std::move(statementList));
}

std::unique_ptr<StatementList> Parser::parseStatementList(std::deque<Token>& tokens) {
	// Rule: stmt+
	std::vector<std::unique_ptr<Statement>> statements;

	while (!tokens.front().isType(TokenType::RIGHT_BRACE) && !tokens.front().isType(TokenType::ENDOFFILE)) { // Reached end of statementList
		statements.push_back(parseStatement(tokens));
	}

	if (statements.size() == 0) {
		throw SyntaxErrorException("Statement List should contain at least one statement");
	}

	return std::make_unique<StatementList>(std::move(statements));
}

std::unique_ptr<Statement> Parser::parseStatement(std::deque<Token>& tokens) {
	// Rule: read | print | call | while | if | assign
	/* Variable names can be keywords/terminals. Thus, we handle the assign statements first,
	   if there is an equal operator, we know to parse it as an assign statement. E.g. read = read + 1;*/
	if (tokens.front().isType(TokenType::NAME)) {
		if (tokens.at(1).isType(TokenType::ASSIGN)) { // Assign Statement
			return parseAssignStatement(tokens);
		}
		else if (tokens.front().hasValue(AppConstants::READ)) {
			return parseReadStatement(tokens);
		}
		else if (tokens.front().hasValue(AppConstants::PRINT)) {
			return parsePrintStatement(tokens);
		}
		else if (tokens.front().hasValue(AppConstants::CALL)) {
			return parseCallStatement(tokens);
		}
		else if (tokens.front().hasValue(AppConstants::WHILE)) {
			return parseWhileStatement(tokens);
		}
		else if (tokens.front().hasValue(AppConstants::IF)) {
			return parseIfStatement(tokens);
		}
		else {
			throw SyntaxErrorException("Unexpected token when parsing statement -> " + tokens.front().value);
		}
	}
	else {
		throw SyntaxErrorException("Unexpected token when parsing statement -> " + tokens.front().value);
	}
}

std::unique_ptr<ReadStatement> Parser::parseReadStatement(std::deque<Token>& tokens) {
	// Rule: 'read' var_name';'
	tokens.pop_front(); // Pop 'read'

	if (!tokens.front().isType(TokenType::NAME)) {
		throw SyntaxErrorException("Expected var_name in read statement, but got -> " + tokens.front().value);
	}
	Ent varName = tokens.front().value;
	tokens.pop_front(); // Pop var_name

	if (!tokens.front().isType(TokenType::SEMICOLON)) {
		throw SyntaxErrorException("Expected ; at end of read statement, but got -> " + tokens.front().value);
	}
	tokens.pop_front(); // Pop ;

	return std::make_unique<ReadStatement>(currStatementNumber++, varName);
}

std::unique_ptr<PrintStatement> Parser::parsePrintStatement(std::deque<Token>& tokens) {
	// Rule: 'print' var_name';'
	tokens.pop_front(); // Pop 'print'

	if (!tokens.front().isType(TokenType::NAME)) {
		throw SyntaxErrorException("Expected var_name in print statement, but got -> " + tokens.front().value);
	}
	Ent varName = tokens.front().value;
	tokens.pop_front(); // Pop var_name

	if (!tokens.front().isType(TokenType::SEMICOLON)) {
		throw SyntaxErrorException("Expected ; at end of print statement, but got -> " + tokens.front().value);
	}
	tokens.pop_front(); // Pop ;

	return std::make_unique<PrintStatement>(currStatementNumber++, varName);
}

std::unique_ptr<CallStatement> Parser::parseCallStatement(std::deque<Token>& tokens) {
	// Rule: 'call' proc_name';'
	tokens.pop_front(); // Pop 'call'

	if (!tokens.front().isType(TokenType::NAME)) {
		throw SyntaxErrorException("Expected proc_name in call statement, but got -> " + tokens.front().value);
	}
	ProcName procName = tokens.front().value;
	tokens.pop_front(); // Pop proc_name

	if (!tokens.front().isType(TokenType::SEMICOLON)) {
		throw SyntaxErrorException("Expected ; at end of call statement, but got -> " + tokens.front().value);
	}
	tokens.pop_front(); // Pop ;

	return std::make_unique<CallStatement>(currStatementNumber++, procName);
}

std::unique_ptr<WhileStatement> Parser::parseWhileStatement(std::deque<Token>& tokens) {
	// Rule: 'while' '(' cond_expr ')' '{' stmtLst '}'
	StmtNum stmtNum = currStatementNumber; // Need to note down the statement number, as the statement list gets processed first.
	currStatementNumber++;

	tokens.pop_front(); // Pop 'while'

	if (!tokens.front().isType(TokenType::LEFT_PARENTHESIS)) {
		throw SyntaxErrorException("Expected '(' in while statement, but got -> " + tokens.front().value);
	}
	tokens.pop_front(); // Pop '('

	// Parse conditional expr
	auto condExpr = parseConditionalExpression(tokens);

	if (!tokens.front().isType(TokenType::RIGHT_PARENTHESIS)) {
		throw SyntaxErrorException("Expected ')' in while statement, but got -> " + tokens.front().value);
	}
	tokens.pop_front(); // Pop ')'

	if (!tokens.front().isType(TokenType::LEFT_BRACE)) {
		throw SyntaxErrorException("Expected '{' in while statement, but got -> " + tokens.front().value);
	}
	tokens.pop_front(); // Pop '{'

	// Parse stmtList
	auto stmtList = parseStatementList(tokens);

	if (!tokens.front().isType(TokenType::RIGHT_BRACE)) {
		throw SyntaxErrorException("Expected '}' in while statement, but got -> " + tokens.front().value);
	}
	tokens.pop_front(); // Pop '}'

	return std::make_unique<WhileStatement>(stmtNum, std::move(condExpr), std::move(stmtList));
}

std::unique_ptr<IfStatement> Parser::parseIfStatement(std::deque<Token>& tokens) {
	// Rule: 'if' '(' cond_expr ')' 'then' '{' stmtLst '}' 'else' '{' stmtLst '}'
	StmtNum stmtNum = currStatementNumber;
	currStatementNumber++;

	tokens.pop_front(); // Pop 'if'

	if (!tokens.front().isType(TokenType::LEFT_PARENTHESIS)) {
		throw SyntaxErrorException("Expected '(' in if statement, but got -> " + tokens.front().value);
	}
	tokens.pop_front(); // Pop '('

	// Parse conditional expr
	auto condExpr = parseConditionalExpression(tokens);

	if (!tokens.front().isType(TokenType::RIGHT_PARENTHESIS)) {
		throw SyntaxErrorException("Expected ')' in if statement, but got -> " + tokens.front().value);
	}
	tokens.pop_front(); // Pop ')'

	if (!tokens.front().isType(TokenType::NAME) || !tokens.front().hasValue(AppConstants::THEN)) {
		throw SyntaxErrorException("Expected 'then' in if statement, but got -> " + tokens.front().value);
	}
	tokens.pop_front(); // Pop 'then'

	if (!tokens.front().isType(TokenType::LEFT_BRACE)) {
		throw SyntaxErrorException("Expected '{' in if statement, but got -> " + tokens.front().value);
	}
	tokens.pop_front(); // Pop '{'

	// Parse stmtList
	auto thenStmtList = parseStatementList(tokens);

	if (!tokens.front().isType(TokenType::RIGHT_BRACE)) {
		throw SyntaxErrorException("Expected '}' in if statement, but got -> " + tokens.front().value);
	}
	tokens.pop_front(); // Pop '}'

	if (!tokens.front().isType(TokenType::NAME) || !tokens.front().hasValue(AppConstants::ELSE)) {
		throw SyntaxErrorException("Expected 'else' in if statement, but got -> " + tokens.front().value);
	}
	tokens.pop_front(); // Pop 'else'

	if (!tokens.front().isType(TokenType::LEFT_BRACE)) {
		throw SyntaxErrorException("Expected '{' in if statement, but got -> " + tokens.front().value);
	}
	tokens.pop_front(); // Pop '{'

	// Parse stmtList
	auto elseStmtList = parseStatementList(tokens);

	if (!tokens.front().isType(TokenType::RIGHT_BRACE)) {
		throw SyntaxErrorException("Expected '}' in if statement, but got -> " + tokens.front().value);
	}
	tokens.pop_front(); // Pop '}'

	return std::make_unique<IfStatement>(stmtNum, std::move(condExpr), 
										std::move(thenStmtList), std::move(elseStmtList));
}

std::unique_ptr<AssignStatement> Parser::parseAssignStatement(std::deque<Token>& tokens) {
	// Rule: var_name '=' expr ';'
	// Check for name & '=' done in parseStatement()
	Ent varName = tokens.front().value;
	tokens.pop_front(); // Pop var_name
	tokens.pop_front(); // Pop "="

	auto expr = parseExpression(tokens);

	if (!tokens.front().isType(TokenType::SEMICOLON)) {
		throw SyntaxErrorException("Expected ';' at end of assign statement, but got -> " + tokens.front().value);
	}
	tokens.pop_front(); // Pop ;

	return std::make_unique<AssignStatement>(currStatementNumber++, varName, std::move(expr));
}

std::unique_ptr<ConditionalExpression> Parser::parseConditionalExpression(std::deque<Token>& tokens) {
	// Rule: rel_expr | '!' '(' cond_expr ')' | '(' cond_expr ')' '&&' '(' cond_expr ')' | '(' cond_expr ')' '||' '(' cond_expr ')'
	if (tokens.front().isType(TokenType::NOT)) { // '!' '(' path
		tokens.pop_front(); // Pop '!'

		if (!tokens.front().isType(TokenType::LEFT_PARENTHESIS)) {
			throw SyntaxErrorException("Expected '(' in not conditional expression, but got -> " + tokens.front().value);
		}
		tokens.pop_front(); // Pop '('

		auto condExpr = parseConditionalExpression(tokens);

		if (!tokens.front().isType(TokenType::RIGHT_PARENTHESIS)) {
			throw SyntaxErrorException("Expected ')' in not conditional expression, but got -> " + tokens.front().value);
		}
		tokens.pop_front(); // Pop ')'

		return std::make_unique<NotConditionalExpression>(std::move(condExpr));
	}
	else if (tokens.front().isType(TokenType::LEFT_PARENTHESIS)) { // Binary Cond Expr Path
		/* SIMPLE grammar is slightly complicated, as something like (3 + 2) > 0 would get wrongly
		 * parsed as a cond_expr, instead of rel_expr, thus we handle this case here, by iterating
		 * through the nested expression, to check for a relational operator. If there isn't any, then
		 * we should parse it as a relational expression */
		int idx = 0;
		bool parseAsRelExprFlag = true;
		while (!tokens.at(idx).isType(TokenType::RIGHT_PARENTHESIS)) {
			if (tokens.at(idx).isType(TokenType::RELATIONAL_OPR)) {
				parseAsRelExprFlag = false;
			}
			idx++;
		}

		if (parseAsRelExprFlag) {
			return parseRelationalExpression(tokens);
		}

		tokens.pop_front(); // Pop '('

		auto lhs = parseConditionalExpression(tokens);

		if (!tokens.front().isType(TokenType::RIGHT_PARENTHESIS)) {
			throw SyntaxErrorException("Expected ')' in binary conditional expression, but got -> " + tokens.front().value);
		}
		tokens.pop_front(); // Pop ')'

		if (!tokens.front().isType(TokenType::BINARY_LOGICAL_OPR)) {
			throw SyntaxErrorException("Expected '&&' or '||' in binary conditional expression, but got -> " + tokens.front().value);
		}
		Operator condOperator = tokens.front().value;
		tokens.pop_front(); // Pop '&&' or '||'

		if (!tokens.front().isType(TokenType::LEFT_PARENTHESIS)) {
			throw SyntaxErrorException("Expected '(' in binary conditional expression, but got -> " + tokens.front().value);
		}
		tokens.pop_front(); // Pop '('

		auto rhs = parseConditionalExpression(tokens);

		if (!tokens.front().isType(TokenType::RIGHT_PARENTHESIS)) {
			throw SyntaxErrorException("Expected ')' in conditional expression, but got -> " + tokens.front().value);
		}
		tokens.pop_front(); // Pop ')'

		return std::make_unique<BinaryConditionalExpression>(condOperator, std::move(lhs), std::move(rhs));
	}
	else { // Relational Expression
		return parseRelationalExpression(tokens);
	}
}

std::unique_ptr<ConditionalExpression> Parser::parseRelationalExpression(std::deque<Token>& tokens) {
	/* Rule: rel_factor '>' rel_factor | rel_factor '>=' rel_factor |
			 rel_factor '<' rel_factor | rel_factor '<=' rel_factor |
			 rel_factor '==' rel_factor | rel_factor '!=' rel_factor
	*/
	auto lhs = parseRelationalFactor(tokens);

	if (!tokens.front().isType(TokenType::RELATIONAL_OPR)) {
		throw SyntaxErrorException("Expected a comparator in relational expression, but got -> " + tokens.front().value);
	}
	Operator relationalOperator = tokens.front().value;
	tokens.pop_front(); // Pop relational comparator

	auto rhs = parseRelationalFactor(tokens);
	return std::make_unique<RelationalExpression>(relationalOperator, std::move(lhs), std::move(rhs));
}

std::unique_ptr<Expression> Parser::parseRelationalFactor(std::deque<Token>& tokens) {
	// Rule: var_name | const_value | expr
	// From this, we know that we can parse tokens.front() as either a constant or variable.
	if (tokens.at(1).isType(TokenType::RELATIONAL_OPR) || tokens.at(1).isType(TokenType::RIGHT_PARENTHESIS)) {
		if (tokens.front().isType(TokenType::INTEGER)) {
			return parseConstant(tokens);
		}
		else if (tokens.front().isType(TokenType::NAME)) {
			return parseVariable(tokens);
		}
	}
	else {
		return parseExpression(tokens);
	}
}

std::unique_ptr<Expression> Parser::parseExpression(std::deque<Token>& tokens) {
	/* Rule: expr: expr '+' term | expr '-' term | term
	*  After eliminating left recursion:
	*    expr: term(expr')
	*    expr': '+' term(expr') | '-' term(expr') | epsilon
	*/
	auto lhs = parseTerm(tokens);

	if (tokens.front().isType(TokenType::EXPR_ARITH_OPR)) {
		Operator mathOperator = tokens.front().value;
		tokens.pop_front(); // Pop the '+' or '-'
		auto rhs = parseExpression(tokens);
		return std::make_unique<MathExpression>(mathOperator, std::move(lhs), std::move(rhs));
	}
	else { // Reached the epsilon
		return lhs;
	}
}

std::unique_ptr<Expression> Parser::parseTerm(std::deque<Token>& tokens) {
	/* Rule: term '*' factor | term '/' factor | term '%' factor | factor
	*  After eliminating left recursion:
	*    term: factor(term')
	*    term': '*' factor(term') | '/' factor(term') | '%' factor(term') | epsilon
	*/
	auto lhs = parseFactor(tokens);

	if (tokens.front().isType(TokenType::TERM_ARITH_OPR)) {
		Operator mathOperator = tokens.front().value;
		tokens.pop_front(); // Pop the '*' or '/' or '%'
		auto rhs = parseTerm(tokens);
		return std::make_unique<MathExpression>(mathOperator, std::move(lhs), std::move(rhs));
	}
	else { // Reached the epsilon
		return lhs;
	}
}

std::unique_ptr<Expression> Parser::parseFactor(std::deque<Token>& tokens) {
	// Rule: var_name | const_value | '(' expr ')'
	if (tokens.front().isType(TokenType::INTEGER)) {
		return parseConstant(tokens);
	}
	else if (tokens.front().isType(TokenType::NAME)) {
		return parseVariable(tokens);
	}
	else if (tokens.front().isType(TokenType::LEFT_PARENTHESIS)) {
		tokens.pop_front(); // Pop '('

		auto expression = parseExpression(tokens);

		if (!tokens.front().isType(TokenType::RIGHT_PARENTHESIS)) {
			throw SyntaxErrorException("Expected ')' in factor, but got -> " + tokens.front().value);
		}
		tokens.pop_front(); // Pop ')'

		return expression;
	}
	else {
		throw SyntaxErrorException("Parsing factor failed, got -> " + tokens.front().value);
	}
}

std::unique_ptr<Expression> Parser::parseConstant(std::deque<Token>& tokens) {
	// Rule: INTEGER
	Const value = stoi(tokens.front().value);
	tokens.pop_front();
	return std::make_unique<Constant>(value);
}

std::unique_ptr<Expression> Parser::parseVariable(std::deque<Token>& tokens) {
	// Rule: NAME
	Ent value = tokens.front().value;
	tokens.pop_front();
	return std::make_unique<Variable>(value);
}