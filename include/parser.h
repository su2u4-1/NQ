#ifndef PARSER_H
#define PARSER_H

#include "qlib.h"
#include "syntax_tree.h"

class Parser {
   public:
    Parser();
    vector<shared_ptr<Token>> tokens;
    fs::path file_name;
    int index;
    Token current_token;
    string version;
    vector<fs::path> import_files;
    Token next_token();
    void get_token();
    void rollback_token();
    void parser_error(const string& msg);
    void parser_error(const string& msg, const Token& token);
    bool isCall();
    shared_ptr<Code> parse(const vector<shared_ptr<Token>>& tokens, const fs::path& file_name, string version = "none");
    shared_ptr<Import> parse_import();
    vector<shared_ptr<Declare>> parse_declare(bool is_global);
    vector<shared_ptr<DeclareAttr>> parse_declare_attr();
    shared_ptr<Type> parse_type();
    shared_ptr<Expression> parse_expression();
    shared_ptr<Term> parse_term();
    shared_ptr<Variable> parse_variable(shared_ptr<Variable> var);
    shared_ptr<UseGeneric> parse_use_generic();
    shared_ptr<DeclareGeneric> parse_declare_generic();
    shared_ptr<Call> parse_call(shared_ptr<Variable> var);
    shared_ptr<Function> parse_function();
    shared_ptr<Class> parse_class();
    shared_ptr<Method> parse_method();
    vector<shared_ptr<DeclareArgs>> parse_declare_args();
    shared_ptr<List> parse_list();
    shared_ptr<Tuple> parse_tuple();
    shared_ptr<Dict> parse_dict();
    Statements parse_statements();
    shared_ptr<If> parse_if();
    shared_ptr<For> parse_for();
    shared_ptr<While> parse_while();
    shared_ptr<Break> parse_break();
    shared_ptr<Return> parse_return();
    shared_ptr<Continue> parse_continue();
    shared_ptr<Float> parse_float();
};

#endif  // PARSER_H
