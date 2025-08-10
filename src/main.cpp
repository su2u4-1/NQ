#include "../include/lexer.h"
#include "../include/qlib.h"

struct arguments {
    fs::path program_name = fs::path("output");
    vector<fs::path> files = {};
    vector<string> flags = {};
    fs::path output_ast_file = fs::path("output.ast");
    int output_ast_type = -1;
    bool output_token_flow_file = false;
    fs::path compile_file = fs::path("output.vm");
    bool compile = false;
    /*
    Do not output ast = -1
    The output file extension is .ast = 0
    The output file extension is _ast.json = 1
    */
};

vector<string> read_file(const fs::path& file_name) {
    ifstream input;
    input.open(file_name);
    if (input.fail())
        throw runtime_error("Could not open file " + file_name.string());
    vector<string> source_code;
    string line;
    while (getline(input, line)) {
        if (line.size() > 0) {
            if (line.back() == '\r')
                line.pop_back();
            if (line.back() != '\n')
                line.append("\n");
        }
        source_code.push_back(line);
    }
    input.close();
    return source_code;
}

arguments parse_arguments(int argc, char* argv[]) {
    arguments args;
    if (argc < 2) {
        cerr << "Usage: " << path_processing(argv[0]) << " <filename>" << endl;
        exit(1);
    }

    int state = 0;
    for (int i = 1; i < argc; i++) {
        string arg = string(argv[i]);
        if (arg[0] == '-') {
            args.flags.push_back(arg);
            if (arg == "--output-ast" || arg == "-oa") {
                args.output_ast_type = 0;
                args.output_ast_file = path_processing(args.program_name.replace_extension(".ast"));
            } else if (arg == "--output-ast-json" || arg == "-oaj") {
                args.output_ast_type = 1;
                args.output_ast_file = path_processing(args.program_name.replace_extension(".ast.json"));
            } else if (arg == "--output-ast-none" || arg == "-oan")
                args.output_ast_type = -1;
            else if (arg == "--output-token-flow" || arg == "-otf")
                args.output_token_flow_file = true;
            else if (arg == "--help" || arg == "-h") {
                cout << HELP_DOCS;
                exit(0);
            } else if (arg == "--output" || arg == "-o")
                state = 1;
            else if (arg == "--stdlibpath" || arg == "-sp")
                state = 2;
            else if (arg == "--compile" || arg == "-c") {
                args.compile_file = path_processing(args.program_name.replace_extension(".vm"));
                args.compile = true;
            } else if (arg == "--version" || arg == "-v") {
                cout << "Version: " << VERSION << endl;
                exit(0);
            } else
                throw "Error: Unknown argument " + arg;
        } else {
            fs::path t = path_processing(fs::absolute(arg));
            if (state == 0) {
                if (fs::exists(t) && t.extension() == ".nq")
                    args.files.push_back(t);
                else {
                    cerr << "Error: File " << t << " does not exist or is not a .nq file" << endl;
                    exit(1);
                }
            } else if (state == 1) {
                args.program_name = path_processing(t.parent_path() / t.stem());
                state = 0;
            } else if (state == 2) {
                STDLIBPATH = t;
                state = 0;
            }
        }
    }
    return args;
}

string ast_to_json(shared_ptr<Node> node) {
    stringstream output;
    output << "{";
    output << "\"type\":\"" << node->type << "\",";
    output << "\"value\":{";
    for (const auto& [k, v] : node->value)
        output << '"' << k << "\":\"" << v << "\",";
    output << "},";
    output << "\"children\":[";
    for (const shared_ptr<Node>& child : node->children)
        output << ast_to_json(child);
    output << "]";
    output << "},";
    return output.str();
}

string remove_json_trailing_comma(const string& json) {
    char n, p;
    bool str = false;
    string result;
    for (int i = 0; i < json.size(); i++) {
        p = n;
        n = json[i];
        if (n == '"')
            str = !str;
        if (!str && p == ',' && (n == '}' || n == ']'))
            result[result.size() - 1] = n;
        else
            result += n;
    }
    if (result[result.size() - 1] == ',')
        result[result.size() - 1] = ' ';
    return result;
}

string output_ast(const shared_ptr<Node> node, int ident) {
    stringstream output;
    output << string(ident * 4, ' ') << node->type << " (";
    for (const auto& [k, v] : node->value)
        output << k << ": " << v << ", ";
    if (node->children.size() == 0)
        output << ") {}" << endl;
    else {
        output << ")" << endl;
        output << string(ident * 4, ' ') << "{" << endl;
        ident++;
        for (const auto& child : node->children)
            output << output_ast(child, ident);
        ident--;
        output << string(ident * 4, ' ') << "}" << endl;
    }
    return output.str();
}

int main(int argc, char* argv[]) {
    try {
        arguments args = parse_arguments(argc, argv);
        if (args.files.size() == 0)
            throw runtime_error("No input files");
        map<const fs::path, vector<string>> source_code_set;
        for (const fs::path& file : args.files) {
            vector<string> source_code;
            try {
                source_code = read_file(file);
            } catch (const runtime_error& e) {
                cerr << "FileOpenError: " << e.what() << endl;
                exit(1);
            }
            source_code_set[file] = source_code;
            const vector<shared_ptr<Token>> tokens = lexer(source_code, file);
            if (args.output_token_flow_file) {
                ofstream output_file;
                output_file.open(fs::path(file).replace_extension(".tkf"));
                if (output_file.fail()) {
                    cerr << "Error: Could not open output file " << file << endl;
                    exit(1);
                }
                output_file << "Tokens for file: " << file << endl;
                for (const shared_ptr<Token>& token : tokens)
                    output_file << token->toString() << endl;
                output_file.close();
            }
        }
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        exit(1);
    }
    return 0;
}
