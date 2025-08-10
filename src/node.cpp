#include "../include/node.h"

#include "../include/qlib.h"

shared_ptr<Expression> expression_n(int n) {
    return make_shared<Expression>(vector<ExpressionChildren>{ExpressionChildren(make_shared<Term>(Term(make_shared<Value>(Value(n)))))});
}

Code::Code() {
    cout << "empty Code" << endl;
}
Code::Code(fs::path file_path, string name, string version) : file_path(file_path), name(name), version(version) {}

Import::Import() {
    cout << "empty Import" << endl;
}
Import::Import(string alias) : path(STDLIBPATH / alias), alias(alias) {}
Import::Import(fs::path path, string alias) : path(path), alias(alias) {}

DeclareGlobalVar::DeclareGlobalVar() {
    cout << "empty DeclareGlobalVar" << endl;
}
DeclareGlobalVar::DeclareGlobalVar(string name, shared_ptr<Type> type, shared_ptr<Expression> expression) : name(name), type(type), expression(expression) {}

DeclareLocalVar::DeclareLocalVar() {
    cout << "empty DeclareLocalVar" << endl;
}
DeclareLocalVar::DeclareLocalVar(string name, shared_ptr<Type> type, shared_ptr<Expression> expression) : name(name), type(type), expression(expression) {}

DeclareAttr::DeclareAttr() {
    cout << "empty DeclareAttr" << endl;
}
DeclareAttr::DeclareAttr(string name, shared_ptr<Type> type, shared_ptr<Expression> expression) : name(name), type(type), expression(expression) {}

DeclareArgs::DeclareArgs() {
    cout << "empty DeclareArgs" << endl;
}
DeclareArgs::DeclareArgs(string name, shared_ptr<Type> type, bool is_varargs) : name(name), type(type), is_varargs(is_varargs) {}

DeclareGeneric::DeclareGeneric(vector<string> names) : names(names) {}

Type::Type() {
    cout << "empty Type" << endl;
}
Type::Type(string name, bool is_const, shared_ptr<UseGeneric> generic) : is_const(is_const), value(name), generic(generic) {}
Type::Type(shared_ptr<Type> type, bool is_const, shared_ptr<UseGeneric> generic) : is_const(is_const), value(type), generic(generic) {}

Expression::Expression(vector<ExpressionChildren> children) : children(children) {}

AssignOperator::AssignOperator(AssignOperatorType type) : type(type) {}

AssignExpression::AssignExpression() {
    cout << "empty AssignExpression" << endl;
}
AssignExpression::AssignExpression(shared_ptr<Variable> var, shared_ptr<Expression> expression, shared_ptr<AssignOperator> assign_op) {}

Term::Term() {
    cout << "empty Term" << endl;
}
Term::Term(TermType type, shared_ptr<Term> term) : value(make_pair(type, term)) {}
Term::Term(shared_ptr<Variable> variable) : value(variable) {}
Term::Term(shared_ptr<Expression> expression) : value(expression) {}
Term::Term(shared_ptr<Value> value) : value(value) {}
Term::Term(shared_ptr<Call> call) : value(call) {}

Call::Call() {
    cout << "empty Call" << endl;
}
Call::Call(shared_ptr<Variable> var, vector<shared_ptr<Expression>> args, shared_ptr<UseGeneric> use_generic) : var(var), args(args), use_generic(use_generic) {}

UseGeneric::UseGeneric(vector<shared_ptr<Type>> types) : types(types) {}

Variable::Variable() {
    cout << "empty Variable" << endl;
}
Variable::Variable(string identifier) : value(identifier) {}
Variable::Variable(shared_ptr<Expression> expression) : value(expression) {}
Variable::Variable(shared_ptr<Call> call) : value(call) {}
Variable::Variable(shared_ptr<Variable> subscript, shared_ptr<Expression> subscript_expression) : value(subscript), modifier(subscript_expression) {}
Variable::Variable(shared_ptr<Variable> attribute, string attribute_name) : value(attribute), modifier(attribute_name) {}

Value::Value() : data(0) {}
Value::Value(int int_value) : data(int_value) {}
Value::Value(char char_value) : data(char_value) {}
Value::Value(bool bool_value) : data(bool_value) {}
Value::Value(string string_value) : data(string_value) {}
Value::Value(shared_ptr<Float> float_value) : data(float_value) {}
Value::Value(shared_ptr<List> list_value) : data(list_value) {}
Value::Value(shared_ptr<Expression> pointer_value) : data(pointer_value) {}
Value::Value(shared_ptr<Dict> dict_value) : data(dict_value) {}
Value::Value(shared_ptr<Tuple> tuple_value) : data(tuple_value) {}

List::List() {
    cout << "empty List" << endl;
}
List::List(shared_ptr<Type> type, vector<shared_ptr<Expression>> elements) : type(type), elements(elements) {}

Tuple::Tuple() {
    cout << "empty Tuple" << endl;
}
Tuple::Tuple(shared_ptr<Type> type, vector<shared_ptr<Expression>> elements) : type(type), elements(elements) {}

Dict::Dict() {
    cout << "empty Dict" << endl;
}
Dict::Dict(shared_ptr<Type> key_type, shared_ptr<Type> value_type, vector<pair<shared_ptr<Expression>, shared_ptr<Expression>>> elements) : key_type(key_type), value_type(value_type), elements(elements) {}

Operator::Operator() {
    cout << "empty Operator" << endl;
}
Operator::Operator(OperatorType type) : type(type) {}

Statements::Statements(vector<Statement> statements) : statements(statements) {}

Function::Function() {
    cout << "empty Function" << endl;
}
Function::Function(string name, shared_ptr<Type> return_type, Statements body, vector<shared_ptr<DeclareArgs>> args, shared_ptr<DeclareGeneric> generic) : name(name), return_type(return_type), body(body), args(args), generic(generic) {}

Class::Class() {
    cout << "empty Class" << endl;
}
Class::Class(string name, vector<shared_ptr<DeclareAttr>> attrs, vector<shared_ptr<Subroutine>> subroutines, shared_ptr<DeclareGeneric> generic) : name(name), attrs(attrs), subroutines(subroutines), generic(generic) {}

Method::Method() {
    cout << "empty Method" << endl;
}
Method::Method(string name, shared_ptr<Type> return_type, vector<shared_ptr<DeclareArgs>> args, Statements body, shared_ptr<DeclareGeneric> generic, string self_var_name) : name(name), return_type(return_type), args(args), body(body), generic(generic), self_var_name(self_var_name) {}

If::If() {
    cout << "empty If" << endl;
}
If::If(shared_ptr<Expression> condition, Statements body) : conditions({condition}), bodys({body}), has_else(false) {}
If::If(vector<shared_ptr<Expression>> conditions, vector<Statements> bodys, bool has_else) : conditions(conditions), bodys(bodys), has_else(has_else) {}

For::For() {
    cout << "empty For" << endl;
}
For::For(string var_name, shared_ptr<Type> type, shared_ptr<Expression> iterable, Statements body, Statements else_body, string label) : label(label), loop(py_for_loop{var_name, type, iterable}), body(body), else_body(else_body) {}
For::For(shared_ptr<AssignExpression> start, shared_ptr<Expression> condition, shared_ptr<Expression> step, Statements body, Statements else_body, string label) : label(label), loop(c_for_loop{start, condition, step}), body(body), else_body(else_body) {}

While::While() {
    cout << "empty While" << endl;
}
While::While(shared_ptr<Expression> condition, Statements body, Statements else_body, string label) : label(label), condition(condition), body(body), else_body(else_body) {}

Return::Return(shared_ptr<Expression> expression) : expression(expression ? expression : make_shared<Expression>(Expression())) {}

Break::Break(string label) : label(label) {}

Continue::Continue(string label) : label(label) {}

Float::Float() {
    cout << "empty Float" << endl;
}
Float::Float(int numerator, int denominator) : numerator(numerator), denominator(denominator) {}
