#ifndef NODE_H
#define NODE_H

#include "qlib.h"

struct CodeChildren {
    union code_children {
        shared_ptr<Import> import;
        shared_ptr<Function> function;
        shared_ptr<Class> class_;
        shared_ptr<DeclareGlobalVar> declare_global_var;
    };
    enum {
        IMPORT,
        FUNCTION,
        CLASS,
        DECLARE_GLOBAL_VAR
    } type;
};

class Code {
   public:
    fs::path file_path;
    string name;
    string version;
    vector<CodeChildren> children;
};

class Import {
   public:
    fs::path path;
    string alias;
};

class DeclareGlobalVar {
   public:
    string name;
    shared_ptr<Type> type;
    shared_ptr<Expression> expression;
};

class DeclareLocalVar {
   public:
    string name;
    shared_ptr<Type> type;
    shared_ptr<Expression> expression;
};

class DeclareAttr {
   public:
    string name;
    shared_ptr<Type> type;
    shared_ptr<Expression> expression;
};

class DeclareArgs {
   public:
    string name;
    shared_ptr<Type> type;
    bool is_varargs;
};

class DeclareGeneric {
   public:
    vector<string> names;
};

class Type {
   public:
    bool is_const;
    string name;
    shared_ptr<Type> type;
    shared_ptr<UseGeneric> generic;
};

struct ExpressionChildren {
    union expression_children {
        shared_ptr<Term> term;
        shared_ptr<Operator> op;
    };
    enum {
        TERM,
        OPERATOR
    } type;
};

class Expression {
   public:
    vector<ExpressionChildren> children;
};

class AssignExpression {
   public:
    shared_ptr<Variable> var;
    shared_ptr<AssignOperator> assign_op;
    shared_ptr<Expression> expression;
};

class Term {
   public:
    enum TermType {
        ADDRESSOF,
        DEREFERENCE,
        NEGATE,
        BITWISE_NOT,
        NOT,
        CALL,
        VARIABLE,
        VALUE,
        EXPRESSION,
    } type;
    union term_value {
        shared_ptr<Term> term;
        shared_ptr<Variable> variable;
        shared_ptr<Expression> expression;
        shared_ptr<Value> value;
        shared_ptr<Call> call;
    } value;
};

class Call {
   public:
    shared_ptr<Variable> var;
    shared_ptr<UseGeneric> use_generic;
    vector<shared_ptr<Expression>> args;
};

class UseGeneric {
   public:
    vector<shared_ptr<Type>> types;
};

class Variable {
   public:
    enum VariableType {
        IDENTIFIER,
        EXPRESSION,
        CALL,
        SUBSCRIPT,
        ATTRIBUTE,
    } type;
    union variable_value {
        string identifier;
        shared_ptr<Expression> expression;
        shared_ptr<Call> call;
        shared_ptr<Variable> subscript;
        shared_ptr<Variable> attribute;
    } value;
};

class Value {
   public:
    enum ValueType {
        INT,
        CHAR,
        BOOL,
        null,
        STRING,
        FLOAT,
        LIST,
        POINTER,
        DICT,
        TUPLE
    } type;
    union value_data {
        int int_value;
        char char_value;
        bool bool_value;
        string string_value;
        shared_ptr<Float> float_value;
        shared_ptr<List> list_value;
        shared_ptr<Expression> pointer_value;
        shared_ptr<Dict> dict_value;
        shared_ptr<Tuple> tuple_value;
    } data;
};

class Pointer {
   public:
    shared_ptr<Type> type;
    shared_ptr<Expression> expression;
};

class List {
   public:
    shared_ptr<Type> type;
    vector<shared_ptr<Expression>> elements;
};

class Tuple {
   public:
    shared_ptr<Type> type;
    vector<shared_ptr<Expression>> elements;
};

class Dict {
   public:
    shared_ptr<Type> key_type;
    shared_ptr<Type> value_type;
    vector<pair<shared_ptr<Expression>, shared_ptr<Expression>>> elements;
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
};

class Function {
   public:
    string name;
    shared_ptr<DeclareGeneric> generic;
    vector<shared_ptr<DeclareArgs>> args;
    shared_ptr<Type> return_type;
    vector<shared_ptr<Statement>> body;
};

struct Subroutine {
    enum SubroutineType {
        FUNCTION,
        METHOD
    } type;
    union subroutine_value {
        shared_ptr<Function> function;
        shared_ptr<Method> method;
    } value;
};

class Class {
   public:
    string name;
    shared_ptr<DeclareGeneric> generic;
    vector<shared_ptr<DeclareAttr>> attrs;
    vector<shared_ptr<Subroutine>> subroutines;
};

class Method {
   public:
    string name;
    shared_ptr<DeclareGeneric> generic;
    vector<shared_ptr<DeclareArgs>> args;
    shared_ptr<Type> return_type;
    vector<shared_ptr<Statement>> body;
    string self_var_name;  // Name of the self variable in the method
};

struct Statement {
    enum StatementType {
        IF,
        FOR,
        WHILE,
        BREAK,
        RETURN,
        EXPRESSION
    } type;
    union statement_value {
        shared_ptr<If> if_statement;
        shared_ptr<For> for_statement;
        shared_ptr<While> while_statement;
        shared_ptr<Return> return_statement;
        shared_ptr<Expression> expression_statement;
        shared_ptr<Break> break_statement;
        shared_ptr<Continue> continue_statement;
        shared_ptr<DeclareLocalVar> declare_local_var;
        shared_ptr<AssignExpression> assign_expression;
    } value;
};

class If {
   public:
    int elif_count;
    bool has_else;
    vector<shared_ptr<Expression>> conditions;
    vector<vector<shared_ptr<Statement>>> bodys;
};

class For {
   public:
    string label;
    vector<shared_ptr<Statement>> body;
    union for_loop {
        struct {
            string var_name;
            shared_ptr<Type> type;
            shared_ptr<Expression> iterable;
        } py_for;
        struct {
            shared_ptr<AssignExpression> start;
            shared_ptr<Expression> condition;
            shared_ptr<Expression> step;
        } c_for;
    };
    enum ForLoopType {
        PY,
        C
    } loop_type;
};

class While {
   public:
    string label;
    shared_ptr<Expression> condition;
    vector<shared_ptr<Statement>> body;
};

class Return {
   public:
    shared_ptr<Expression> expression;
};

class Break {
   public:
    string label;
};

class Continue {
   public:
    string label;
};

class Float {
   public:
    int numerator;
    int denominator;
};

// class Comment {
//    public:
//     string text;
//     bool is_single;
// };

#endif  // NODE_H

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