#include "./include/parser.h"

#include "./include/qlib.h"

Parser::Parser() {
    index = 0;
    file_name = "";
    current_token = Token();
    tokens = {};
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
    get_token();
    shared_ptr<Code> code = make_shared<Code>(file_name, file_name.stem().string(), version);
    while (current_token.type != "EOF") {
        if (current_token == Token("keyword", "import")) {
            shared_ptr<Import> import = parse_import();
            code->children.push_back(import);
            code->import_files.push_back(import->path);
        } else if (current_token == Token("keyword", "var")) {
            for (shared_ptr<Declare> i : parse_declare(GLOBAL_VAR)) {
                if (holds_alternative<shared_ptr<DeclareGlobalVar>>(*i))
                    code->children.push_back(get<shared_ptr<DeclareGlobalVar>>(*i));
                else if (holds_alternative<shared_ptr<DeclareLocalVar>>(*i))
                    parser_error("Expected DeclareGlobalVar, got " + get<shared_ptr<DeclareLocalVar>>(*i)->to_json(), current_token);
                else if (holds_alternative<shared_ptr<DeclareAttr>>(*i))
                    parser_error("Expected DeclareGlobalVar, got " + get<shared_ptr<DeclareAttr>>(*i)->to_json(), current_token);
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
