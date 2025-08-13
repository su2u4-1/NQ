#ifndef SYNTAX_TREE_H
#define SYNTAX_TREE_H

#include "qlib.h"

class Code;
class Import;
class DeclareGlobalVar;
class DeclareLocalVar;
class DeclareAttr;
class DeclareArgs;
class DeclareGeneric;
class Type;
class Expression;
class AssignExpression;
class Term;
class Call;
class UseGeneric;
class Variable;
class Value;
class List;
class Tuple;
class Dict;
class Operator;
class AssignOperator;
class Function;
class Class;
class Method;
class Statements;
class If;
class For;
class While;
class Return;
class Break;
class Continue;
class Float;

shared_ptr<Expression> expression_n(int n);

using CodeChildren = variant<shared_ptr<Import>, shared_ptr<Function>, shared_ptr<Class>, shared_ptr<DeclareGlobalVar>>;

class Code {
   public:
    fs::path file_path;
    string name;
    string version;
    vector<CodeChildren> children;
    Code();
    Code(fs::path file_path, string name, string version);
    string to_json() const;
};

class Import {
   public:
    fs::path path;
    string alias;
    Import();
    Import(string alias);
    Import(fs::path path, string alias);
    string to_json() const;
};

class DeclareGlobalVar {
   public:
    string name;
    shared_ptr<Type> type;
    shared_ptr<Expression> expression;
    DeclareGlobalVar();
    DeclareGlobalVar(string name, shared_ptr<Type> type, shared_ptr<Expression> expression = expression_n(0));
    string to_json() const;
};

class DeclareLocalVar {
   public:
    string name;
    shared_ptr<Type> type;
    shared_ptr<Expression> expression;
    DeclareLocalVar();
    DeclareLocalVar(string name, shared_ptr<Type> type, shared_ptr<Expression> expression = expression_n(0));
    string to_json() const;
};

class DeclareAttr {
   public:
    string name;
    shared_ptr<Type> type;
    shared_ptr<Expression> expression;
    DeclareAttr();
    DeclareAttr(string name, shared_ptr<Type> type, shared_ptr<Expression> expression = expression_n(0));
    string to_json() const;
};

class DeclareArgs {
   public:
    string name;
    shared_ptr<Type> type;
    bool is_varargs;
    DeclareArgs();
    DeclareArgs(string name, shared_ptr<Type> type, bool is_varargs = false);
    string to_json() const;
};

class DeclareGeneric {
   public:
    vector<string> names;
    DeclareGeneric(vector<string> names = {});
    string to_json() const;
};

class Type {
   public:
    bool is_const;
    variant<shared_ptr<Type>, string> value;
    shared_ptr<UseGeneric> generic;
    Type();
    Type(string name, shared_ptr<UseGeneric> generic = nullptr, bool is_const = false);
    Type(shared_ptr<Type> type, shared_ptr<UseGeneric> generic, bool is_const = false);
    string to_json() const;
};

using ExpressionChildren = variant<shared_ptr<Term>, shared_ptr<Operator>>;

class Expression {
   public:
    vector<ExpressionChildren> children;
    Expression(vector<ExpressionChildren> children = {});
    string to_json() const;
};

class AssignOperator {
   public:
    enum AssignOperatorType {
        ASSIGN,
        ADD_ASSIGN,
        SUB_ASSIGN,
        MUL_ASSIGN,
        DIV_ASSIGN,
        MOD_ASSIGN,
        LEFT_SHIFT_ASSIGN,
        RIGHT_SHIFT_ASSIGN,
        BIT_AND_ASSIGN,
        BIT_XOR_ASSIGN,
        BIT_OR_ASSIGN,
    } type;
    AssignOperator(AssignOperatorType type = ASSIGN);
    string to_json() const;
};

class AssignExpression {
   public:
    shared_ptr<Variable> var;
    shared_ptr<AssignOperator> assign_op;
    shared_ptr<Expression> expression;
    AssignExpression();
    AssignExpression(shared_ptr<Variable> var, shared_ptr<Expression> expression, shared_ptr<AssignOperator> assign_op = make_shared<AssignOperator>(AssignOperator()));
    string to_json() const;
};

enum TermType {
    ADDRESSOF,
    DEREFERENCE,
    NEGATE,
    BITWISE_NOT,
    NOT,
};

using TermValue = variant<pair<TermType, shared_ptr<Term>>, shared_ptr<Variable>, shared_ptr<Expression>, shared_ptr<Value>, shared_ptr<Call>>;

class Term {
   public:
    TermValue value;
    Term();
    Term(TermType type, shared_ptr<Term> term);
    Term(shared_ptr<Variable> variable);
    Term(shared_ptr<Expression> expression);
    Term(shared_ptr<Value> value);
    Term(shared_ptr<Call> call);
    string to_json() const;
};

class Call {
   public:
    shared_ptr<Variable> var;
    shared_ptr<UseGeneric> use_generic;
    vector<shared_ptr<Expression>> args;
    Call();
    Call(shared_ptr<Variable> var, vector<shared_ptr<Expression>> args = {}, shared_ptr<UseGeneric> use_generic = nullptr);
    string to_json() const;
};

class UseGeneric {
   public:
    vector<shared_ptr<Type>> types;
    UseGeneric(vector<shared_ptr<Type>> types = {});
    string to_json() const;
};

using VariableValue = variant<string, shared_ptr<Expression>, shared_ptr<Call>, shared_ptr<Variable>>;

class Variable {
   public:
    VariableValue value;
    variant<shared_ptr<Expression>, string> modifier;
    Variable();
    Variable(string identifier);
    Variable(shared_ptr<Expression> expression);
    Variable(shared_ptr<Call> call);
    Variable(shared_ptr<Variable> subscript, shared_ptr<Expression> subscript_expression);
    Variable(shared_ptr<Variable> attribute, string attribute_name);
    string to_json() const;
};

using ValueData = variant<int, char, bool, string, shared_ptr<Float>, shared_ptr<List>, shared_ptr<Expression>, shared_ptr<Dict>, shared_ptr<Tuple>>;

class Value {
   public:
    ValueData data;
    Value();
    Value(int int_value);
    Value(char char_value);
    Value(bool bool_value);
    Value(string string_value);
    Value(shared_ptr<Float> float_value);
    Value(shared_ptr<List> list_value);
    Value(shared_ptr<Expression> pointer_value);
    Value(shared_ptr<Dict> dict_value);
    Value(shared_ptr<Tuple> tuple_value);
    string to_json() const;
};

class List {
   public:
    shared_ptr<Type> type;
    vector<shared_ptr<Expression>> elements;
    List();
    List(shared_ptr<Type> type, vector<shared_ptr<Expression>> elements = {});
    string to_json() const;
};

class Tuple {
   public:
    shared_ptr<Type> type;
    vector<shared_ptr<Expression>> elements;
    Tuple();
    Tuple(shared_ptr<Type> type, vector<shared_ptr<Expression>> elements = {});
    string to_json() const;
};

class Dict {
   public:
    shared_ptr<Type> key_type;
    shared_ptr<Type> value_type;
    vector<pair<shared_ptr<Expression>, shared_ptr<Expression>>> elements;
    Dict();
    Dict(shared_ptr<Type> key_type, shared_ptr<Type> value_type, vector<pair<shared_ptr<Expression>, shared_ptr<Expression>>> elements = {});
    string to_json() const;
};

class Operator {
   public:
    enum OperatorType {
        MUL,
        DIV,
        MOD,
        ADD,
        SUB,
        LEFT_SHIFT,
        RIGHT_SHIFT,
        LESS,
        LESS_EQUAL,
        GREATER,
        GREATER_EQUAL,
        EQUAL,
        NOT_EQUAL,
        BIT_AND,
        BIT_XOR,
        BIT_OR,
        AND,
        OR,
        NOT,
        BIT_NOT,
        DEFERENCE,
        ADDRESS_OF,
    } type;
    Operator();
    Operator(OperatorType type);
    string to_json() const;
};

using Statement = variant<shared_ptr<If>, shared_ptr<For>, shared_ptr<While>, shared_ptr<Return>, shared_ptr<Expression>, shared_ptr<Break>, shared_ptr<Continue>, shared_ptr<DeclareLocalVar>, shared_ptr<AssignExpression>>;

class Statements {
   public:
    vector<Statement> statements;
    Statements(vector<Statement> statements = {});
    string to_json() const;
};

class Function {
   public:
    string name;
    shared_ptr<DeclareGeneric> generic;
    vector<shared_ptr<DeclareArgs>> args;
    shared_ptr<Type> return_type;
    Statements body;
    Function();
    Function(string name, shared_ptr<Type> return_type, Statements body = Statements(), vector<shared_ptr<DeclareArgs>> args = {}, shared_ptr<DeclareGeneric> generic = nullptr);
    string to_json() const;
};

using Subroutine = variant<shared_ptr<Function>, shared_ptr<Method>>;

class Class {
   public:
    string name;
    shared_ptr<DeclareGeneric> generic;
    vector<shared_ptr<DeclareAttr>> attrs;
    vector<Subroutine> subroutines;
    Class();
    Class(string name, vector<shared_ptr<DeclareAttr>> attrs = {}, vector<Subroutine> subroutines = {}, shared_ptr<DeclareGeneric> generic = nullptr);
    string to_json() const;
};

class Method {
   public:
    string name;
    shared_ptr<DeclareGeneric> generic;
    vector<shared_ptr<DeclareArgs>> args;
    shared_ptr<Type> return_type;
    Statements body;
    string self_var_name;  // Name of the self variable in the method
    Method();
    Method(string name, shared_ptr<Type> return_type, vector<shared_ptr<DeclareArgs>> args = {}, Statements body = Statements(), shared_ptr<DeclareGeneric> generic = nullptr, string self_var_name = "self");
    string to_json() const;
};

class If {
   public:
    bool has_else;
    vector<shared_ptr<Expression>> conditions;
    vector<Statements> bodys;
    If();
    If(shared_ptr<Expression> condition, Statements body = Statements());
    If(vector<shared_ptr<Expression>> conditions, vector<Statements> bodys = {Statements()}, bool has_else = false);
    string to_json() const;
};

struct py_for_loop {
    string var_name;
    shared_ptr<Type> type;
    shared_ptr<Expression> iterable;
};

struct c_for_loop {
    shared_ptr<AssignExpression> start;
    shared_ptr<Expression> condition;
    shared_ptr<Expression> step;
};

class For {
   public:
    string label;
    Statements body;
    variant<py_for_loop, c_for_loop> loop;
    Statements else_body;
    For();
    For(string var_name, shared_ptr<Type> type, shared_ptr<Expression> iterable, Statements body = Statements(), Statements else_body = Statements(), string label = "");
    For(shared_ptr<AssignExpression> start, shared_ptr<Expression> condition, shared_ptr<Expression> step, Statements body = Statements(), Statements else_body = Statements(), string label = "");
    string to_json() const;
};

class While {
   public:
    string label;
    shared_ptr<Expression> condition;
    Statements body;
    Statements else_body;
    bool has_else;
    While();
    While(shared_ptr<Expression> condition, Statements body = Statements(), bool has_else = false, Statements else_body = Statements(), string label = "");
    string to_json() const;
};

class Return {
   public:
    shared_ptr<Expression> expression;
    Return(shared_ptr<Expression> expression = expression_n(0));
    string to_json() const;
};

class Break {
   public:
    string label;
    Break(string label = "");
    string to_json() const;
};

class Continue {
   public:
    string label;
    Continue(string label = "");
    string to_json() const;
};

class Float {
   public:
    int numerator;
    int denominator;
    Float();
    Float(int numerator, int denominator = 1);
    string to_json() const;
    double to_double() const;
    bool operator==(const Float& other) const;
};

using Node = variant<Code, Import, DeclareGlobalVar, DeclareLocalVar, DeclareAttr, DeclareArgs, DeclareGeneric, Type, Expression, AssignExpression, Term, Call, UseGeneric, Variable, Value, List, Tuple, Dict, Operator, AssignOperator, Function, Class, Method, Statements, If, For, While, Return, Break, Continue, Float>;

#endif  // SYNTAX_TREE_H

// '*'  mul           MUL,
// '/'  div           DIV,
// '%'  mod           MOD,
// '+'  add           ADD,
// '-'  sub           SUB,
// '<<' left_shift    LEFT_SHIFT,
// '>>' right_shift   RIGHT_SHIFT,
// '<'  less          LESS,
// '<=' less_equal    LESS_EQUAL,
// '>'  greater       GREATER,
// '>=' greater_equal GREATER_EQUAL,
// '==' equal         EQUAL,
// '!=' not_equal     NOT_EQUAL,
// '&'  bit_and       BIT_AND,
// '^'  bit_xor       BIT_XOR,
// '|'  bit_or        BIT_OR,
// '&&' and           AND,
// '||' or            OR,
// '!'  not           NOT,
// '~'  bit_not       BIT_NOT,
// '@'  deference     DEFERENCE,
// '$'  address_of    ADDRESS_OF,

// '='   assign             ASSIGN,
// '+='  add_assign         ADD_ASSIGN,
// '-='  sub_assign         SUB_ASSIGN,
// '*='  mul_assign         MUL_ASSIGN,
// '/='  div_assign         DIV_ASSIGN,
// '%='  mod_assign         MOD_ASSIGN,
// '<<=' left_shift_assign  LEFT_SHIFT_ASSIGN,
// '>>=' right_shift_assign RIGHT_SHIFT_ASSIGN,
// '&='  bit_and_assign     BIT_AND_ASSIGN,
// '^='  bit_xor_assign     BIT_XOR_ASSIGN,
// '|='  bit_or_assign      BIT_OR_ASSIGN,