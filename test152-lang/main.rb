#!/usr/bin/env ruby


def main
  source = ARGF.read
  program = parse(source)

  context = Context.new()
  context.set_variable("pi", Math::PI)
  context.set_function("sin", lambda { |x| Math::sin(x) })
  context.set_function("cos", lambda { |x| Math::cos(x) })
  context.set_function("tan", lambda { |x| Math::tan(x) })
  context.set_function("exp", lambda { |x| Math::exp(x) })
  context.set_function("print", lambda { |x| printf("%g\n", x) })

  program.run(context)
end


# AST ------------------------------------------------------------------------

class Context
  def initialize()
    @variables = {}
    @functions = {}
  end

  def set_variable(name, value)
    @variables[name] = value
  end

  def set_function(name, func)
    @functions[name] = func
  end

  def get_variable(name)
    @variables[name]
  end

  def get_function(name)
    @functions[name]
  end
end

class Statement
  def run(context)
  end
end

class CompoundStatement < Statement
  def initialize(statements)
    @statements = statements
  end

  def run(context)
    @statements.each do |stmt|
      stmt.run(context)
    end
  end
end

class AssignStatement < Statement
  def initialize(name, value)
    @name = name
    @value = value
  end

  def run(context)
    context.set_variable(@name, @value.evaluate(context))
  end
end

class DoStatement < Statement
  def initialize(expr)
    @expr = expr
  end

  def run(context)
    @expr.evaluate(context)
  end
end

class Expression
  def evaluate(context)
  end
end

class PosExpression < Expression
  def initialize(expr)
    @expr = expr
  end

  def evaluate(context)
    @expr.evaluate(context)
  end
end

class NegExpression < Expression
  def initialize(expr)
    @expr = expr
  end

  def evaluate(context)
    -@expr.evaluate(context)
  end
end

class AddExpression < Expression
  def initialize(lhs, rhs)
    @lhs = lhs
    @rhs = rhs
  end

  def evaluate(context)
    @lhs.evaluate(context) + @rhs.evaluate(context)
  end
end

class SubExpression < Expression
  def initialize(lhs, rhs)
    @lhs = lhs
    @rhs = rhs
  end

  def evaluate(context)
    @lhs.evaluate(context) - @rhs.evaluate(context)
  end
end

class MulExpression < Expression
  def initialize(lhs, rhs)
    @lhs = lhs
    @rhs = rhs
  end

  def evaluate(context)
    @lhs.evaluate(context) * @rhs.evaluate(context)
  end
end

class DivExpression < Expression
  def initialize(lhs, rhs)
    @lhs = lhs
    @rhs = rhs
  end

  def evaluate(context)
    @lhs.evaluate(context) / @rhs.evaluate(context)
  end
end

class PowExpression < Expression
  def initialize(lhs, rhs)
    @lhs = lhs
    @rhs = rhs
  end

  def evaluate(context)
    @lhs.evaluate(context) ** @rhs.evaluate(context)
  end
end

class NumberExpression < Expression
  def initialize(value)
    @value = value
  end

  def evaluate(context)
    @value
  end
end

class VarExpression < Expression
  def initialize(name)
    @name = name
  end

  def evaluate(context)
    context.get_variable(@name)
  end
end

class CallExpression < Expression
  def initialize(name, arg)
    @name = name
    @arg = arg
  end

  def evaluate(context)
    func = context.get_function(@name)
    func.(@arg.evaluate(context))
  end
end


# Parser ---------------------------------------------------------------------

def parse(source)
  tokens = tokenize(source)
  parse_compound_statement!(tokens)
end

def parse_compound_statement!(tokens)
  statements = []

  until tokens.empty? do
    if tokens[0].type == :end
      break
    end

    statements << parse_statement!(tokens)

    while tokens[0].type == :newline
      tokens.shift
    end
  end

  CompoundStatement.new(statements)
end

def parse_statement!(tokens)
  if tokens[0].type == :identifier and tokens[1].type == :equal
    return parse_assign_statement!(tokens)
  end

  if tokens[0].type == :do
    return parse_do_statement!(tokens)
  end

  raise "syntax error"
end

def parse_assign_statement!(tokens)
  name = tokens.shift
  _eq = tokens.shift
  expr = parse_expression!(tokens)
  AssignStatement.new(name.value, expr)
end

def parse_do_statement!(tokens)
  _do = tokens.shift
  expr = parse_expression!(tokens)
  DoStatement.new(expr)
end

def parse_expression!(tokens)
  expr = parse_expression_term!(tokens)

  loop do
    case tokens[0].type
    when :plus
      tokens.shift
      term = parse_expression_term!(tokens)
      expr = AddExpression.new(expr, term)

    when :minus
      tokens.shift
      term = parse_expression_term!(tokens)
      expr = SubExpression.new(expr, term)

    else
      break
    end
  end

  expr
end

def parse_expression_term!(tokens)
  term = parse_expression_factor!(tokens)

  loop do
    case tokens[0].type
    when :star
      tokens.shift
      factor = parse_expression_factor!(tokens)
      term = MulExpression.new(term, factor)

    when :slash
      tokens.shift
      factor = parse_expression_factor!(tokens)
      term = DivExpression.new(term, factor)

    else
      break
    end
  end

  term
end

def parse_expression_factor!(tokens)
  factor = parse_expression_atom!(tokens)

  case tokens[0].type
  when :starstar
    tokens.shift
    exponent = parse_expression_factor!(tokens)
    factor = PowExpression.new(factor, exponent)
  end

  factor
end

def parse_expression_atom!(tokens)
  case tokens[0].type
  when :number
    return parse_expression_number!(tokens)

  when :identifier
    if tokens[1].type == :lparen
      return parse_expression_call!(tokens)
    else
      return parse_expression_var!(tokens)
    end

  when :lparen
    tokens.shift # lparen
    expr = parse_expression!(tokens)
    tokens.shift # rparen
    return expr

  when :plus
    tokens.shift
    return PosExpression.new(parse_expression_atom!(tokens))

  when :minus
    tokens.shift
    return NegExpression.new(parse_expression_atom!(tokens))
  end

  raise "syntax error: atomic expression is expected"
end

def parse_expression_number!(tokens)
  number = tokens.shift
  NumberExpression.new(number.value)
end

def parse_expression_call!(tokens)
  func = tokens.shift
  tokens.shift # lparen
  arg = parse_expression!(tokens)
  tokens.shift # rparen
  CallExpression.new(func.value, arg)
end

def parse_expression_var!(tokens)
  var = tokens.shift
  VarExpression.new(var.value)
end


# Lexer ----------------------------------------------------------------------

TOKENS = {
  "(" => :lparen,
  ")" => :rparen,
  "=" => :equal,
  "+" => :plus,
  "-" => :minus,
  "*" => :star,
  "/" => :slash,
  "**" => :starstar,
  "\n" => :newline,
}

TOKEN_KEYS = TOKENS.keys.sort do |a, b|
  b.length <=> a.length
end

KEYWORDS = {
  "do" => :do,
}

Token = Struct.new(:type, :value)


def tokenize(source)
  source = String.new(source)
  tokens = []

  until trim_space!(source).empty? do
    tokens << scan_token!(source)
  end

  tokens << Token.new(:end)
  tokens
end

def scan_token!(source)
  if source.start_with?(/^[-+]?[0-9]/)
    return scan_number!(source)
  end

  if source.start_with?(/^[a-zA-Z]/)
    return scan_name!(source)
  end

  TOKEN_KEYS.each do |key|
    if source.start_with?(key)
      return Token.new(TOKENS[key], source.slice!(0, key.length))
    end
  end

  raise "unrecognized token"
end

def scan_number!(source)
  Token.new(:number, source.slice!(/^[-+]?[0-9\.]+/).to_f)
end

def scan_name!(source)
  name = source.slice!(/^[a-zA-Z0-9_]+/)

  if KEYWORDS.has_key?(name)
    Token.new(KEYWORDS[name])
  else
    Token.new(:identifier, name)
  end
end

def trim_space!(source)
  source.sub!(/^ */, "")
end


main
