#include "../include/syntax_tree.h"

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
Type::Type(string name, shared_ptr<UseGeneric> generic, bool is_const) : is_const(is_const), value(name), generic(generic) {}
Type::Type(shared_ptr<Type> type, shared_ptr<UseGeneric> generic, bool is_const) : is_const(is_const), value(type), generic(generic) {}

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
Class::Class(string name, vector<shared_ptr<DeclareAttr>> attrs, vector<Subroutine> subroutines, shared_ptr<DeclareGeneric> generic) : name(name), attrs(attrs), subroutines(subroutines), generic(generic) {}

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
While::While(shared_ptr<Expression> condition, Statements body, bool has_else, Statements else_body, string label) : label(label), condition(condition), body(body), has_else(has_else), else_body(else_body) {}

Return::Return(shared_ptr<Expression> expression) : expression(expression ? expression : make_shared<Expression>(Expression())) {}

Break::Break(string label) : label(label) {}

Continue::Continue(string label) : label(label) {}

Float::Float() {
    cout << "empty Float" << endl;
}
Float::Float(int numerator, int denominator) : numerator(numerator), denominator(denominator) {}
bool Float::operator==(const Float &other) const {
    return numerator * other.denominator == other.numerator * denominator;
}
double Float::to_double() const {
    return static_cast<double>(numerator) / denominator;
}

string Code::to_json() const {
    string s = "", t = "";
    for (const auto &child : children) {
        if (holds_alternative<shared_ptr<Import>>(child))
            s += t + get<shared_ptr<Import>>(child)->to_json();
        else if (holds_alternative<shared_ptr<Function>>(child))
            s += t + get<shared_ptr<Function>>(child)->to_json();
        else if (holds_alternative<shared_ptr<Class>>(child))
            s += t + get<shared_ptr<Class>>(child)->to_json();
        else if (holds_alternative<shared_ptr<DeclareGlobalVar>>(child))
            s += t + get<shared_ptr<DeclareGlobalVar>>(child)->to_json();
        t = ",";
    }
    return "{\"node type\": \"Code\", \"file_path\": \"" + file_path.string() + "\", \"name\": \"" + name + "\", \"version\": \"" + version + "\", \"children\": [" + s + "]}";
}
string Import::to_json() const {
    return "{\"node type\": \"Import\", \"path\": \"" + path.string() + "\", \"alias\": \"" + alias + "\"}";
}
string DeclareGlobalVar::to_json() const {
    return "{\"node type\": \"DeclareGlobalVar\", \"name\": \"" + name + "\", \"type\": " + type->to_json() + ", \"expression\": " + expression->to_json() + "}";
}
string DeclareLocalVar::to_json() const {
    return "{\"node type\": \"DeclareLocalVar\", \"name\": \"" + name + "\", \"type\": " + type->to_json() + ", \"expression\": " + expression->to_json() + "}";
}
string DeclareAttr::to_json() const {
    return "{\"node type\": \"DeclareAttr\", \"name\": \"" + name + "\", \"type\": " + type->to_json() + ", \"expression\": " + expression->to_json() + "}";
}
string DeclareArgs::to_json() const {
    return "{\"node type\": \"DeclareArgs\", \"name\": \"" + name + "\", \"type\": " + type->to_json() + ", \"is_varargs\": " + (is_varargs ? "true" : "false") + "}";
}
string DeclareGeneric::to_json() const {
    string s = "", t = "";
    for (const auto &name : names) {
        s += t + "\"" + name + "\"";
        t = ",";
    }
    return "{\"node type\": \"DeclareGeneric\", \"names\": [" + s + "]}";
}
string Type::to_json() const {
    string value_str;
    if (holds_alternative<shared_ptr<Type>>(value))
        value_str = get<shared_ptr<Type>>(value)->to_json();
    else
        value_str = "\"" + get<string>(value) + "\"";
    auto a = is_const ? "true" : "false";
    return "{\"node type\": \"Type\", \"is_const\": " + string(is_const ? "true" : "false") + ", \"value\": " + value_str + ", \"generic\": " + (generic ? generic->to_json() : "null") + "}";
}
string Expression::to_json() const {
    string s = "", t = "";
    for (const auto &child : children) {
        if (holds_alternative<shared_ptr<Term>>(child))
            s += t + get<shared_ptr<Term>>(child)->to_json();
        else if (holds_alternative<shared_ptr<Operator>>(child))
            s += t + get<shared_ptr<Operator>>(child)->to_json();
        t = ",";
    }
    return "{\"node type\": \"Expression\", \"children\": [" + s + "]}";
}
string AssignExpression::to_json() const {
    return "{\"node type\": \"AssignExpression\", \"var\": " + var->to_json() + ", \"assign_op\": " + assign_op->to_json() + ", \"expression\": " + expression->to_json() + "}";
}
string Term::to_json() const {
    string value_str;
    if (holds_alternative<pair<TermType, shared_ptr<Term>>>(value)) {
        pair<TermType, shared_ptr<Term>> term_pair = get<pair<TermType, shared_ptr<Term>>>(value);
        string term_type = term_pair.first == ADDRESSOF     ? "address_of"
                           : term_pair.first == DEREFERENCE ? "deference"
                           : term_pair.first == NEGATE      ? "negate"
                           : term_pair.first == BITWISE_NOT ? "bitwise_not"
                           : term_pair.first == NOT         ? "not"
                                                            : "UNKNOWN";
        value_str = "{\"type\": \"" + term_type + "\", \"term\": " + term_pair.second->to_json() + "}";
    } else if (holds_alternative<shared_ptr<Value>>(value))
        value_str = get<shared_ptr<Value>>(value)->to_json();
    else if (holds_alternative<shared_ptr<Variable>>(value))
        value_str = get<shared_ptr<Variable>>(value)->to_json();
    else if (holds_alternative<shared_ptr<Expression>>(value))
        value_str = get<shared_ptr<Expression>>(value)->to_json();
    else if (holds_alternative<shared_ptr<Call>>(value))
        value_str = get<shared_ptr<Call>>(value)->to_json();
    return "{\"node type\": \"Term\", \"value\": " + value_str + "}";
}
string Call::to_json() const {
    string args_str = "", t = "";
    for (const auto &arg : args) {
        args_str += t + arg->to_json();
        t = ",";
    }
    return "{\"node type\": \"Call\", \"var\": " + var->to_json() + ", \"args\": [" + args_str + "], \"use_generic\": " + (use_generic ? use_generic->to_json() : "null") + "}";
}
string UseGeneric::to_json() const {
    string s = "", t = "";
    for (const auto &type : types) {
        s += t + type->to_json();
        t = ",";
    }
    return "{\"node type\": \"UseGeneric\", \"types\": [" + s + "]}";
}
string Variable::to_json() const {
    string value_str, modifier_str = "null";
    if (holds_alternative<string>(value))
        value_str = "\"" + get<string>(value) + "\"";
    else if (holds_alternative<shared_ptr<Expression>>(value))
        value_str = get<shared_ptr<Expression>>(value)->to_json();
    else if (holds_alternative<shared_ptr<Call>>(value))
        value_str = get<shared_ptr<Call>>(value)->to_json();
    else if (holds_alternative<shared_ptr<Variable>>(value)) {
        value_str = get<shared_ptr<Variable>>(value)->to_json();
        if (holds_alternative<shared_ptr<Variable>>(modifier))
            modifier_str = get<shared_ptr<Variable>>(modifier)->to_json();
        else
            modifier_str = "\"" + get<string>(modifier) + "\"";
    }
    return "{\"node type\": \"Variable\", \"value\": " + value_str + ", \"modifier\": " + modifier_str + "}";
}
string Value::to_json() const {
    string data_str;
    if (holds_alternative<int>(data))
        data_str = to_string(get<int>(data));
    else if (holds_alternative<char>(data))
        data_str = "\"" + string(1, get<char>(data)) + "\"";
    else if (holds_alternative<bool>(data))
        data_str = get<bool>(data) ? "true" : "false";
    else if (holds_alternative<string>(data))
        data_str = "\"" + get<string>(data) + "\"";
    else if (holds_alternative<shared_ptr<Float>>(data))
        data_str = get<shared_ptr<Float>>(data)->to_json();
    else if (holds_alternative<shared_ptr<List>>(data))
        data_str = get<shared_ptr<List>>(data)->to_json();
    else if (holds_alternative<shared_ptr<Expression>>(data))
        data_str = get<shared_ptr<Expression>>(data)->to_json();
    else if (holds_alternative<shared_ptr<Dict>>(data))
        data_str = get<shared_ptr<Dict>>(data)->to_json();
    else if (holds_alternative<shared_ptr<Tuple>>(data))
        data_str = get<shared_ptr<Tuple>>(data)->to_json();
    return "{\"node type\": \"Value\", \"data\": " + data_str + "}";
}
string List::to_json() const {
    string elements_str = "", t = "";
    for (const auto &element : elements) {
        elements_str += t + element->to_json();
        t = ",";
    }
    return "{\"node type\": \"List\", \"type\": " + type->to_json() + ", \"elements\": [" + elements_str + "]}";
}
string Tuple::to_json() const {
    string elements_str = "", t = "";
    for (const auto &element : elements) {
        elements_str += t + element->to_json();
        t = ",";
    }
    return "{\"node type\": \"Tuple\", \"type\": " + type->to_json() + ", \"elements\": [" + elements_str + "]}";
}
string Dict::to_json() const {
    string elements_str = "", t = "";
    for (const auto &element : elements) {
        elements_str += t + "{\"key\": " + element.first->to_json() + ", \"value\": " + element.second->to_json() + "}";
        t = ",";
    }
    return "{\"node type\": \"Dict\", \"key_type\": " + key_type->to_json() + ", \"value_type\": " + value_type->to_json() + ", \"elements\": [" + elements_str + "]}";
}
string Operator::to_json() const {
    string type_str;
    switch (type) {
        case MUL:
            type_str = "mul";
            break;
        case DIV:
            type_str = "div";
            break;
        case MOD:
            type_str = "mod";
            break;
        case ADD:
            type_str = "add";
            break;
        case SUB:
            type_str = "sub";
            break;
        case LEFT_SHIFT:
            type_str = "left_shift";
            break;
        case RIGHT_SHIFT:
            type_str = "right_shift";
            break;
        case LESS:
            type_str = "less";
            break;
        case LESS_EQUAL:
            type_str = "less_equal";
            break;
        case GREATER:
            type_str = "greater";
            break;
        case GREATER_EQUAL:
            type_str = "greater_equal";
            break;
        case EQUAL:
            type_str = "equal";
            break;
        case NOT_EQUAL:
            type_str = "not_equal";
            break;
        case BIT_AND:
            type_str = "bit_and";
            break;
        case BIT_XOR:
            type_str = "bit_xor";
            break;
        case BIT_OR:
            type_str = "bit_or";
            break;
        case AND:
            type_str = "and";
            break;
        case OR:
            type_str = "or";
            break;
        case NOT:
            type_str = "not";
            break;
        case BIT_NOT:
            type_str = "bit_not";
            break;
        case DEFERENCE:
            type_str = "deference";
            break;
        case ADDRESS_OF:
            type_str = "address_of";
            break;
    }
    return "{\"node type\": \"Operator\", \"type\": \"" + type_str + "\"}";
}
string AssignOperator::to_json() const {
    string type_str;
    switch (type) {
        case ASSIGN:
            type_str = "assign";
            break;
        case ADD_ASSIGN:
            type_str = "add_assign";
            break;
        case SUB_ASSIGN:
            type_str = "sub_assign";
            break;
        case MUL_ASSIGN:
            type_str = "mul_assign";
            break;
        case DIV_ASSIGN:
            type_str = "div_assign";
            break;
        case MOD_ASSIGN:
            type_str = "mod_assign";
            break;
        case LEFT_SHIFT_ASSIGN:
            type_str = "left_shift_assign";
            break;
        case RIGHT_SHIFT_ASSIGN:
            type_str = "right_shift_assign";
            break;
        case BIT_AND_ASSIGN:
            type_str = "bit_and_assign";
            break;
        case BIT_XOR_ASSIGN:
            type_str = "bit_xor_assign";
            break;
        case BIT_OR_ASSIGN:
            type_str = "bit_or_assign";
            break;
    }
    return "{\"node type\": \"AssignOperator\", \"type\": \"" + type_str + "\"}";
}
string Function::to_json() const {
    string args_str = "", t = "";
    for (const auto &arg : args) {
        args_str += t + arg->to_json();
        t = ",";
    }
    return "{\"node type\": \"Function\", \"name\": \"" + name + "\", \"generic\": " + generic->to_json() + ", \"args\": [" + args_str + "], \"return_type\": " + return_type->to_json() + ", \"body\": " + body.to_json() + "}";
}
string Class::to_json() const {
    string attrs_str = "", subroutines_str = "", t = "";
    for (const auto &attr : attrs) {
        attrs_str += t + attr->to_json();
        t = ",";
    }
    t = "";
    for (const auto &subroutine : subroutines) {
        if (holds_alternative<shared_ptr<Method>>(subroutine))
            subroutines_str += t + get<shared_ptr<Method>>(subroutine)->to_json();
        else if (holds_alternative<shared_ptr<Function>>(subroutine))
            subroutines_str += t + get<shared_ptr<Function>>(subroutine)->to_json();
        t = ",";
    }
    return "{\"node type\": \"Class\", \"name\": \"" + name + "\", \"generic\": " + generic->to_json() + ", \"attrs\": [" + attrs_str + "], \"subroutines\": [" + subroutines_str + "]}";
}
string Method::to_json() const {
    string args_str = "", t = "";
    for (const auto &arg : args) {
        args_str += t + arg->to_json();
        t = ",";
    }
    return "{\"node type\": \"Method\", \"name\": \"" + name + "\", \"generic\": " + generic->to_json() + ", \"args\": [" + args_str + "], \"return_type\": " + return_type->to_json() + ", \"body\": " + body.to_json() + ", \"self_var_name\": \"" + self_var_name + "\"}";
}
string Statements::to_json() const {
    string s = "", t = "";
    for (const auto &statement : statements) {
        if (holds_alternative<shared_ptr<If>>(statement))
            s += t + get<shared_ptr<If>>(statement)->to_json();
        else if (holds_alternative<shared_ptr<For>>(statement))
            s += t + get<shared_ptr<For>>(statement)->to_json();
        else if (holds_alternative<shared_ptr<While>>(statement))
            s += t + get<shared_ptr<While>>(statement)->to_json();
        else if (holds_alternative<shared_ptr<Return>>(statement))
            s += t + get<shared_ptr<Return>>(statement)->to_json();
        else if (holds_alternative<shared_ptr<Expression>>(statement))
            s += t + get<shared_ptr<Expression>>(statement)->to_json();
        else if (holds_alternative<shared_ptr<Break>>(statement))
            s += t + get<shared_ptr<Break>>(statement)->to_json();
        else if (holds_alternative<shared_ptr<Continue>>(statement))
            s += t + get<shared_ptr<Continue>>(statement)->to_json();
        else if (holds_alternative<shared_ptr<DeclareLocalVar>>(statement))
            s += t + get<shared_ptr<DeclareLocalVar>>(statement)->to_json();
        else if (holds_alternative<shared_ptr<AssignExpression>>(statement))
            s += t + get<shared_ptr<AssignExpression>>(statement)->to_json();
        t = ",";
    }
    return "{\"node type\": \"Statements\", \"statements\": [" + s + "]}";
}
string If::to_json() const {
    string conditions_str = "", bodys_str = "", t = "";
    for (const auto &condition : conditions) {
        conditions_str += t + condition->to_json();
        t = ",";
    }
    t = "";
    for (const auto &body : bodys) {
        bodys_str += t + body.to_json();
        t = ",";
    }
    return "{\"node type\": \"If\", \"has_else\": " + string(has_else ? "true" : "false") + ", \"conditions\": [" + conditions_str + "], \"bodys\": [" + bodys_str + "]}";
}
string For::to_json() const {
    string loop_str;
    if (holds_alternative<py_for_loop>(loop)) {
        const auto &py_loop = get<py_for_loop>(loop);
        loop_str = "{\"var_name\": \"" + py_loop.var_name + "\", \"type\": " + py_loop.type->to_json() + ", \"iterable\": " + py_loop.iterable->to_json() + "}";
    } else {
        const auto &c_loop = get<c_for_loop>(loop);
        loop_str = "{\"start\": " + c_loop.start->to_json() + ", \"condition\": " + c_loop.condition->to_json() + ", \"step\": " + c_loop.step->to_json() + "}";
    }
    return "{\"node type\": \"For\", \"label\": \"" + label + "\", \"body\": " + body.to_json() + ", \"loop\": " + loop_str + ", \"else_body\": " + else_body.to_json() + "}";
}
string While::to_json() const {
    return "{\"node type\": \"While\", \"label\": \"" + label + "\", \"condition\": " + condition->to_json() + ", \"body\": " + body.to_json() + ", \"has_else\": " + (has_else ? "true" : "false") + ", \"else_body\": " + else_body.to_json() + "}";
}
string Return::to_json() const {
    return "{\"node type\": \"Return\", \"expression\": " + expression->to_json() + "}";
}
string Break::to_json() const {
    return "{\"node type\": \"Break\", \"label\": \"" + label + "\"}";
}
string Continue::to_json() const {
    return "{\"node type\": \"Continue\", \"label\": \"" + label + "\"}";
}
string Float::to_json() const {
    return "{\"node type\": \"Float\", \"numerator\": " + to_string(numerator) + ", \"denominator\": " + to_string(denominator) + "}";
}
