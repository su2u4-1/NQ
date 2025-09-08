#include "./include/parser.h"

#include "./include/qlib.h"

Parser::Parser() {
    tokens = {};
    file_name = "";
    index = 0;
    current_token = Token();
    version = "";
    import_files = {};
}
Token Parser::next_token() {
    return *tokens[index + 1];
}
void Parser::get_token() {
    if (index < tokens.size()) {
        current_token = *tokens[index++];
    } else {
        current_token = Token("EOF", "", file_name, {-1, -1});
    }
}
void Parser::rollback_token() {
    if (index > 0) {
        index--;
        current_token = *tokens[index - 1];
        if (current_token.type == "comment") {
            rollback_token();
        }
    } else
        current_token = Token("SOF", "", file_name, {-1, -1});
}
void Parser::parser_error(const string& msg) {
    parser_error(msg, current_token);
}
void Parser::parser_error(const string& msg, const Token& token) {
    error(msg, token.file_name, token.pos);
}
bool Parser::isCall() {}

shared_ptr<Code> Parser::parse(const vector<shared_ptr<Token>>& tokens, const fs::path& file_name, string version) {
    this->tokens = tokens;
    this->file_name = file_name;
    version = version;
    index = 0;
    import_files = {};
    get_token();
    shared_ptr<Code> code = make_shared<Code>(file_name, file_name.stem().string(), version);
    while (current_token.type != "EOF") {
        if (current_token == Token("keyword", "import")) {
            shared_ptr<Import> import = parse_import();
            code->children.push_back(import);
            code->import_files.push_back(import->path);
        } else if (current_token == Token("keyword", "var")) {
            for (shared_ptr<Declare> i : parse_declare(true)) {
                if (holds_alternative<shared_ptr<DeclareGlobalVar>>(*i))
                    code->children.push_back(get<shared_ptr<DeclareGlobalVar>>(*i));
                else if (holds_alternative<shared_ptr<DeclareLocalVar>>(*i))
                    parser_error("Expected DeclareGlobalVar, got " + get<shared_ptr<DeclareLocalVar>>(*i)->to_json(), current_token);
                else
                    parser_error("Unknown declare type", current_token);
            }
        } else if (current_token == Token("keyword", "func"))
            code->children.push_back(parse_function());
        else if (current_token == Token("keyword", "class"))
            code->children.push_back(parse_class());
        else
            parser_error("Unexpected token: " + current_token.toString());
        get_token();
    }
    return code;
}

shared_ptr<Import> Parser::parse_import() {
    get_token();
    string alias;
    fs::path path;
    if (current_token.type == "string") {
        path = current_token.value;
        import_files.push_back(path);
        get_token();
        if (current_token != Token("keyword", "as"))
            parser_error("Expected 'as' after import path", current_token);
        get_token();
        if (current_token.type != "identifier") {
            parser_error("Expected identifier after 'as'", current_token);
        }
        alias = current_token.value;
        get_token();
    } else if (current_token.type == "identifier") {
        if (!is_stdlib(current_token.value))
            parser_error("Expected string or identifier stdlib name after keyword 'import'", current_token);
        path = STDLIBPATH / current_token.value;
        alias = current_token.value;
        get_token();
    } else
        parser_error("Expected string or identifier stdlib name after keyword 'import'", current_token);
    if (current_token != Token("symbol", ";"))
        parser_error("Expected ';' after import statement", current_token);
    return make_shared<Import>(path, alias);
}

vector<shared_ptr<Declare>> Parser::parse_declare(bool is_global) {
    vector<shared_ptr<Declare>> declares;
    get_token();
    shared_ptr<Type> type = parse_type();
    do {
        get_token();
        string name = current_token.value;
        get_token();
        shared_ptr<Expression> expression = make_shared<Expression>();
        if (current_token == Token("symbol", "=")) {
            get_token();
            expression = parse_expression();
            get_token();
        }
        if (is_global)
            declares.push_back(make_shared<Declare>(DeclareGlobalVar{name, type, expression}));
        else
            declares.push_back(make_shared<Declare>(DeclareLocalVar{name, type, expression}));
    } while (current_token == Token("symbol", ","));
    if (current_token != Token("symbol", ";"))
        parser_error("Expected ';' after declare statement", current_token);
    return declares;
}

vector<shared_ptr<DeclareAttr>> Parser::parse_declare_attr() {
    vector<shared_ptr<DeclareAttr>> attrs;
    bool is_static = false;
    if (current_token == Token("keyword", "static"))
        is_static = true;
    get_token();
    bool is_public = false;
    if (current_token == Token("keyword", "public")) {
        is_public = true;
        get_token();
    }
    shared_ptr<Type> type = parse_type();
    do {
        get_token();
        string name = current_token.value;
        get_token();
        shared_ptr<Expression> expression = make_shared<Expression>();
        if (current_token == Token("symbol", "=")) {
            get_token();
            expression = parse_expression();
            get_token();
        }
        attrs.push_back(make_shared<DeclareAttr>(name, type, expression, is_static, is_public));
    } while (current_token == Token("symbol", ","));
    if (current_token != Token("symbol", ";"))
        parser_error("Expected ';' after declare statement", current_token);
    return attrs;
}

shared_ptr<Type> Parser::parse_type() {
    bool is_const = false;
    if (current_token == Token("keyword", "const")) {
        is_const = true;
        get_token();
    }
    if (current_token.type == "identifier" || current_token.type == "builtin_type") {
        string type_name = current_token.value;
        get_token();
        shared_ptr<UseGeneric> generic = make_shared<UseGeneric>();
        if (current_token == Token("symbol", "<"))
            generic = parse_use_generic();
        return make_shared<Type>(type_name, generic, is_const);
    } else
        parser_error("Expected identifier or builtin type after", current_token);
}

shared_ptr<Expression> Parser::parse_expression(shared_ptr<Expression> left) {
    if (left == nullptr)
        return parse_expression(make_shared<Expression>(make_shared<Term>(parse_term())));
    get_token();
    int p = operator_priority(current_token.value);
    if (current_token.type == "symbol" && p > -1) {
        shared_ptr<Operator> op = make_shared<Operator>(current_token.value);
        get_token();
        shared_ptr<Term> right = parse_term();
        if (p < operator_priority(get<ExpressionNode>(left->value).op->type))
            return make_shared<Expression>(get<ExpressionNode>(left->value).left, get<ExpressionNode>(left->value).op, make_shared<Expression>(get<ExpressionNode>(left->value).right, op, right));
        return parse_expression(make_shared<Expression>(left, op, right));
    }
    rollback_token();
    return left;
}

shared_ptr<Term> Parser::parse_term() {
    // TODO
}

shared_ptr<Variable> Parser::parse_variable(shared_ptr<Variable> var) {
    // TODO
}

shared_ptr<UseGeneric> Parser::parse_use_generic() {
    vector<shared_ptr<Type>> types;
    do {
        get_token();
        types.push_back(parse_type());
        get_token();
    } while (current_token == Token("symbol", ","));
    if (current_token != Token("symbol", ">"))
        parser_error("Expected '>' after use generic declaration", current_token);
}

shared_ptr<DeclareGeneric> Parser::parse_declare_generic() {
    vector<string> names;
    do {
        get_token();
        if (current_token.type != "identifier")
            parser_error("Expected identifier after ',' in declare generic", current_token);
        names.push_back(current_token.value);
        get_token();
    } while (current_token == Token("symbol", ","));
    if (current_token != Token("symbol", ">"))
        parser_error("Expected '>' after declare generic", current_token);
    return make_shared<DeclareGeneric>(names);
}

shared_ptr<Call> Parser::parse_call(shared_ptr<Variable> var) {
    // TODO
}

shared_ptr<Function> Parser::parse_function() {
    // TODO
}

shared_ptr<Class> Parser::parse_class() {
    // TODO
}

shared_ptr<Method> Parser::parse_method() {
    // TODO
}

vector<shared_ptr<DeclareArgs>> Parser::parse_declare_args() {
    // TODO
}

shared_ptr<List> Parser::parse_list() {
    vector<shared_ptr<Expression>> elements;
    do {
        get_token();
        elements.push_back(parse_expression());
        get_token();
    } while (current_token == Token("symbol", ","));
    if (current_token != Token("symbol", "]"))
        parser_error("Expected ',' or ']' in list declaration", current_token);
    return make_shared<List>(Type(), elements);
}

shared_ptr<Tuple> Parser::parse_tuple() {
    // TODO
}

shared_ptr<Dict> Parser::parse_dict() {
    // TODO
}

Statements Parser::parse_statements() {
    // TODO
}

shared_ptr<If> Parser::parse_if() {
    // TODO
}

shared_ptr<For> Parser::parse_for() {
    // TODO
}

shared_ptr<While> Parser::parse_while() {
    // TODO
}

shared_ptr<Break> Parser::parse_break() {
    string label;
    get_token();
    if (current_token.type == "identifier") {
        label = current_token.value;
        get_token();
    }
    if (current_token != Token("symbol", ";"))
        parser_error("Expected ';' after break statement", current_token);
    return make_shared<Break>(label);
}

shared_ptr<Return> Parser::parse_return() {
    shared_ptr<Expression> expression = make_shared<Expression>();
    get_token();
    if (current_token != Token("symbol", ";")) {
        expression = parse_expression();
        get_token();
        if (current_token != Token("symbol", ";"))
            parser_error("Expected ';' after return statement", current_token);
    }
    return make_shared<Return>(expression);
}

shared_ptr<Continue> Parser::parse_continue() {
    string label;
    get_token();
    if (current_token.type == "identifier") {
        label = current_token.value;
        get_token();
    }
    if (current_token != Token("symbol", ";"))
        parser_error("Expected ';' after continue statement", current_token);
    return make_shared<Continue>(label);
}

shared_ptr<Float> Parser::parse_float() {
    int a, b, c;
    if (current_token.type != "int")
        parser_error("Expected integer part of float", current_token);
    a = stoi(current_token.value);
    get_token();
    if (current_token != Token("symbol", "."))
        parser_error("Expected '.' in float", current_token);
    get_token();
    if (current_token.type != "int")
        parser_error("Expected fractional part of float", current_token);
    b = stoi(current_token.value);
    c = pow(10, current_token.value.size());
    return make_shared<Float>(a * c + b, c);
}
