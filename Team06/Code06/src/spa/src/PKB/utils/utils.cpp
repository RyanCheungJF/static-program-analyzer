#include "utils.h"

std::unique_ptr<Expression> pkb_utils::buildSubtree(std::string rhs) {
  std::stringstream ss;
  std::deque<Token> tokens;
  Tokenizer tk;
  Parser pr;

  ss << rhs;
  tokens = tk.tokenize(ss);
  std::unique_ptr<Expression> root = std::move(pr.parseExpression(tokens));
  return root;
}