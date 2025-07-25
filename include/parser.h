#ifndef PARSER_H
#define PARSER_H

#include "qlib.h"

class Parser {
   public:
    Parser(const vector<shared_ptr<Token>>& tokens, const fs::path& file_name);
    vector<shared_ptr<Token>> tokens;
    fs::path file_name;
    int index;
    Token current_token;
    Token next_token();
    void get_token();
    void rollback_token();
    void parser_error(const string& msg);
    void parser_error(const string& msg, const Token& token);
    bool isCall();
    shared_ptr<Node> parse();
    shared_ptr<Node> parse_import();
    vector<shared_ptr<Node>> parse_declare(bool attr, bool global);
    shared_ptr<Node> parse_type();
    shared_ptr<Node> parse_expression();
    shared_ptr<Node> parse_term();
    shared_ptr<Node> parse_variable(shared_ptr<Node> var);
    shared_ptr<Node> parse_use_generic();
    shared_ptr<Node> parse_declare_generic();
    shared_ptr<Node> parse_call(shared_ptr<Node> var);
    shared_ptr<Node> parse_function();
    shared_ptr<Node> parse_class();
    shared_ptr<Node> parse_method();
    vector<shared_ptr<Node>> parse_declare_args();
    shared_ptr<Node> parse_arr();
    shared_ptr<Node> parse_tuple();
    shared_ptr<Node> parse_dict();
    shared_ptr<Node> parse_statements();
    shared_ptr<Node> parse_if();
    shared_ptr<Node> parse_for();
    shared_ptr<Node> parse_while();
    shared_ptr<Node> parse_break();
    shared_ptr<Node> parse_return();
};

#endif  // PARSER_H
