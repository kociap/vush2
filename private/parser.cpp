#include <parser.hpp>

#include <anton/string_view.hpp>

#include <fstream>

// TODO: When matching keywords, ensure that the keyword is followed by non-identifier character (Find a cleaner way).
// TODO: Figure out a way to match operators that use overlapping symbols (+ and +=) in a clean way.
// TODO: const and array types.
// TODO: Add settings block.
// TODO: Should we include comma?

namespace vush {
    // keywords

    static constexpr anton::String_View kw_if = u8"if";
    static constexpr anton::String_View kw_else = u8"else";
    static constexpr anton::String_View kw_switch = u8"switch";
    static constexpr anton::String_View kw_case = u8"case";
    static constexpr anton::String_View kw_default = u8"default";
    static constexpr anton::String_View kw_for = u8"for";
    static constexpr anton::String_View kw_while = u8"while";
    static constexpr anton::String_View kw_do = u8"do";
    static constexpr anton::String_View kw_return = u8"return";
    static constexpr anton::String_View kw_break = u8"break";
    static constexpr anton::String_View kw_continue = u8"continue";
    static constexpr anton::String_View kw_true = u8"true";
    static constexpr anton::String_View kw_false = u8"false";
    static constexpr anton::String_View kw_from = u8"from";
    static constexpr anton::String_View kw_struct = u8"struct";
    static constexpr anton::String_View kw_import = u8"import";
    static constexpr anton::String_View kw_const = u8"const";

    // stages

    static constexpr anton::String_View stage_vertex = u8"vertex";
    static constexpr anton::String_View stage_fragment = u8"fragment";
    static constexpr anton::String_View stage_compute = u8"compute";

    // types

    static constexpr anton::String_View type_void = u8"void";
    static constexpr anton::String_View type_bool = u8"bool";
    static constexpr anton::String_View type_int = u8"int";
    static constexpr anton::String_View type_uint = u8"uint";
    static constexpr anton::String_View type_float = u8"float";
    static constexpr anton::String_View type_double = u8"double";
    static constexpr anton::String_View type_vec2 = u8"vec2";
    static constexpr anton::String_View type_vec3 = u8"vec3";
    static constexpr anton::String_View type_vec4 = u8"vec4";
    static constexpr anton::String_View type_dvec2 = u8"dvec2";
    static constexpr anton::String_View type_dvec3 = u8"dvec3";
    static constexpr anton::String_View type_dvec4 = u8"dvec4";
    static constexpr anton::String_View type_bvec2 = u8"bvec2";
    static constexpr anton::String_View type_bvec3 = u8"bvec3";
    static constexpr anton::String_View type_bvec4 = u8"bvec4";
    static constexpr anton::String_View type_ivec2 = u8"ivec2";
    static constexpr anton::String_View type_ivec3 = u8"ivec3";
    static constexpr anton::String_View type_ivec4 = u8"ivec4";
    static constexpr anton::String_View type_uvec2 = u8"uvec2";
    static constexpr anton::String_View type_uvec3 = u8"uvec3";
    static constexpr anton::String_View type_uvec4 = u8"uvec4";
    static constexpr anton::String_View type_mat2 = u8"mat2";
    static constexpr anton::String_View type_mat2x2 = u8"mat2x2";
    static constexpr anton::String_View type_mat3 = u8"mat3";
    static constexpr anton::String_View type_mat3x3 = u8"mat3x3";
    static constexpr anton::String_View type_mat4 = u8"mat4";
    static constexpr anton::String_View type_mat4x4 = u8"mat4x4";
    static constexpr anton::String_View type_mat2x3 = u8"mat2x3";
    static constexpr anton::String_View type_mat2x4 = u8"mat2x4";
    static constexpr anton::String_View type_mat3x2 = u8"mat3x2";
    static constexpr anton::String_View type_mat3x4 = u8"mat3x4";
    static constexpr anton::String_View type_mat4x2 = u8"mat4x2";
    static constexpr anton::String_View type_mat4x3 = u8"mat4x3";
    static constexpr anton::String_View type_dmat2 = u8"dmat2";
    static constexpr anton::String_View type_dmat2x2 = u8"dmat2x2";
    static constexpr anton::String_View type_dmat3 = u8"dmat3";
    static constexpr anton::String_View type_dmat3x3 = u8"dmat3x3";
    static constexpr anton::String_View type_dmat4 = u8"dmat4";
    static constexpr anton::String_View type_dmat4x4 = u8"dmat4x4";
    static constexpr anton::String_View type_dmat2x3 = u8"dmat2x3";
    static constexpr anton::String_View type_dmat2x4 = u8"dmat2x4";
    static constexpr anton::String_View type_dmat3x2 = u8"dmat3x2";
    static constexpr anton::String_View type_dmat3x4 = u8"dmat3x4";
    static constexpr anton::String_View type_dmat4x2 = u8"dmat4x2";
    static constexpr anton::String_View type_dmat4x3 = u8"dmat4x3";

    // separators and operators

    static constexpr anton::String_View token_brace_open = u8"{";
    static constexpr anton::String_View token_brace_close = u8"}";
    static constexpr anton::String_View token_bracket_open = u8"[";
    static constexpr anton::String_View token_bracket_close = u8"]";
    static constexpr anton::String_View token_paren_open = u8"(";
    static constexpr anton::String_View token_paren_close = u8")";
    static constexpr anton::String_View token_semicolon = u8";";
    static constexpr anton::String_View token_colon = u8":";
    static constexpr anton::String_View token_scope_resolution = u8"::";
    static constexpr anton::String_View token_comma = u8",";
    static constexpr anton::String_View token_question = u8"?";
    static constexpr anton::String_View token_dot = u8".";
    static constexpr anton::String_View token_double_quote = u8"\"";
    static constexpr anton::String_View token_plus = u8"+";
    static constexpr anton::String_View token_minus = u8"-";
    static constexpr anton::String_View token_multiply = u8"*";
    static constexpr anton::String_View token_divide = u8"/";
    static constexpr anton::String_View token_remainder = u8"%";
    static constexpr anton::String_View token_logic_and = u8"&&";
    static constexpr anton::String_View token_bit_and = u8"&";
    static constexpr anton::String_View token_logic_or = u8"||";
    static constexpr anton::String_View token_logic_xor = u8"^^";
    static constexpr anton::String_View token_bit_or = u8"|";
    static constexpr anton::String_View token_bit_xor = u8"^";
    static constexpr anton::String_View token_logic_not = u8"!";
    static constexpr anton::String_View token_bit_not = u8"~";
    static constexpr anton::String_View token_bit_lshift = u8"<<";
    static constexpr anton::String_View token_bit_rshift = u8">>";
    static constexpr anton::String_View token_equal = u8"==";
    static constexpr anton::String_View token_not_equal = u8"!=";
    static constexpr anton::String_View token_less = u8"<";
    static constexpr anton::String_View token_greater = u8">";
    static constexpr anton::String_View token_less_equal = u8"<=";
    static constexpr anton::String_View token_greater_equal = u8">=";
    static constexpr anton::String_View token_assign = u8"=";
    static constexpr anton::String_View token_drill = u8"->";
    static constexpr anton::String_View token_increment = u8"++";
    static constexpr anton::String_View token_decrement = u8"--";
    static constexpr anton::String_View token_compound_plus = u8"+=";
    static constexpr anton::String_View token_compound_minus = u8"-=";
    static constexpr anton::String_View token_compound_multiply = u8"*=";
    static constexpr anton::String_View token_compound_divide = u8"/=";
    static constexpr anton::String_View token_compound_remainder = u8"%=";
    static constexpr anton::String_View token_compound_bit_and = u8"&=";
    static constexpr anton::String_View token_compound_bit_or = u8"|=";
    static constexpr anton::String_View token_compound_bit_xor = u8"^=";
    static constexpr anton::String_View token_compound_bit_lshift = u8"<<=";
    static constexpr anton::String_View token_compound_bit_rshift = u8">>=";

    static bool is_whitespace(char32 c) {
        return (c <= 32) | (c == 127);
    }

    static bool is_digit(char32 c) {
        return c >= 48 && c < 58;
    }

    static bool is_alpha(char32 c) {
        return (c >= 97 && c < 123) || (c >= 65 && c < 91);
    }

    static bool is_first_identifier_character(char32 c) {
        return c == '_' || is_alpha(c);
    }

    static bool is_identifier_character(char32 c) {
        return c == '_' || is_digit(c) || is_alpha(c);
    }

    class Lexer_State {
    public:
        i64 stream_offset;
        i64 line;
        i64 column;
    };

    constexpr char32 eof_char32 = (char32)EOF;

    class Lexer {
    public:
        Lexer(std::istream& file): _stream(file) {}

        bool match(anton::String_View const string, bool const must_not_be_followed_by_identifier_char = false) {
            ignore_whitespace_and_comments();

            Lexer_State const state_backup = get_current_state();
            for(char32 c: string.chars()) {
                if(get_next() != c) {
                    restore_state(state_backup);
                    return false;
                }
            }
            if(must_not_be_followed_by_identifier_char) {
                return !is_identifier_character(peek_next());
            } else {
                return true;
            }
        }

        // TODO: String interning if it becomes too slow/memory heavy.
        bool match_identifier(anton::String& out) {
            ignore_whitespace_and_comments();

            // No need to backup the lexer state since we can predict whether the next
            // sequence of characters is an identifier using only the first character.
            char32 const next_char = peek_next();
            if(!is_first_identifier_character(next_char)) {
                return false;
            }

            get_next();
            out += next_char;
            for(char32 peek = peek_next(); is_identifier_character(peek); peek = peek_next()) {
                out += peek;
                get_next();
            }
            return true;
        }

        bool match_eof() {
            ignore_whitespace_and_comments();
            char32 const next_char = peek_next();
            return next_char == eof_char32;
        }

        void ignore_whitespace_and_comments() {
            while(true) {
                char32 const next_char = peek_next();
                if(is_whitespace(next_char)) {
                    get_next();
                    continue;
                }

                if(next_char == U'/') {
                    get_next();
                    char32 const next_next_char = peek_next();
                    if(next_next_char == U'/') {
                        get_next();
                        while(get_next() != U'\n') {}
                        continue;
                    } else if(next_next_char == U'*') {
                        get_next();
                        for(char32 c1 = get_next(), c2 = peek_next(); c1 != U'*' || c2 != U'/'; c1 = get_next(), c2 = _stream.peek()) {}
                        get_next();
                        continue;
                    } else {
                        // Not a comment. End skipping.
                        unget();
                        break;
                    }
                }

                break;
            }
        }

        Lexer_State get_current_state() {
            ignore_whitespace_and_comments();
            return {_stream.tellg(), _line, _column};
        }

        void restore_state(Lexer_State const state) {
            _stream.seekg(state.stream_offset, std::ios_base::beg);
            _line = state.line;
            _column = state.column;
        }

        char32 get_next() {
            char32 const c = _stream.get();
            if(c == '\n') {
                _line += 1;
                _column = 0;
            } else {
                _column += 1;
            }
            return c;
        }

        char32 peek_next() {
            return _stream.peek();
        }

        void unget() {
            _stream.unget();
        }

    private:
        std::istream& _stream;
        i64 _line = 0;
        i64 _column = 0;
    };

    class Parser {
    public:
        Parser(std::istream& stream): _lexer(stream) {}

        Owning_Ptr<Declaration_List> build_ast() {
            Owning_Ptr declarations = new Declaration_List;
            while(!_lexer.match_eof()) {
                if(Declaration* declaration = try_declaration()) {
                    declarations->append(declaration);
                } else {
                    return nullptr;
                }
            }
            return declarations.release();
        }

        [[nodiscard]] Parse_Error get_last_error() const {
            return _last_error;
        }

    private:
        Lexer _lexer;
        Parse_Error _last_error;

        void set_error(anton::String_View const message, Lexer_State const state) {
            if(state.stream_offset > _last_error.file_offset) {
                _last_error.message = message;
                _last_error.line = state.line;
                _last_error.column = state.column;
                _last_error.file_offset = state.stream_offset;
            }
        }

        void set_error(anton::String_View const message) {
            Lexer_State const state = _lexer.get_current_state();
            if(state.stream_offset > _last_error.file_offset) {
                _last_error.message = message;
                _last_error.line = state.line;
                _last_error.column = state.column;
                _last_error.file_offset = state.stream_offset;
            }
        }

        Declaration* try_declaration() {
            if(Declaration_If* declaration_if = try_declaration_if()) {
                return declaration_if;
            }

            if(Import_Decl* import_decl = try_import_decl()) {
                return import_decl;
            }

            if(Struct_Decl* struct_decl = try_struct_decl()) {
                return struct_decl;
            }

            if(Pass_Stage_Declaration* pass_stage = try_pass_stage_declaration()) {
                return pass_stage;
            }

            if(Function_Declaration* function_declaration = try_function_declaration()) {
                return function_declaration;
            }

            if(Variable_Declaration* variable_declaration = try_variable_declaration()) {
                return variable_declaration;
            }

            if(Constant_Declaration* constant = try_constant_declaration()) {
                return constant;
            }

            return nullptr;
        }

        Declaration_If* try_declaration_if() {
            Lexer_State const state_backup = _lexer.get_current_state();
            if(!_lexer.match(kw_if, true)) {
                set_error(u8"expected 'if'");
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            Owning_Ptr condition = try_expression();
            if(!condition) {
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            if(!_lexer.match(token_brace_open)) {
                set_error(u8"expected '{'");
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            Owning_Ptr true_declarations = new Declaration_List;
            while(!_lexer.match(token_brace_close)) {
                if(_lexer.match_eof()) {
                    set_error(u8"unexpected end of file");
                    _lexer.restore_state(state_backup);
                    return nullptr;
                }

                if(Declaration* declaration = try_declaration()) {
                    true_declarations->append(declaration);
                } else {
                    return nullptr;
                }
            }

            if(_lexer.match(kw_else, true)) {
                if(Declaration_If* if_declaration = try_declaration_if()) {
                    Owning_Ptr false_declarations = new Declaration_List;
                    false_declarations->append(if_declaration);
                    return new Declaration_If(condition.release(), true_declarations.release(), false_declarations.release());
                } else {
                    if(!_lexer.match(token_brace_open)) {
                        set_error(u8"expected '{'");
                        _lexer.restore_state(state_backup);
                        return nullptr;
                    }

                    Owning_Ptr false_declarations = new Declaration_List;
                    while(!_lexer.match(token_brace_close)) {
                        if(_lexer.match_eof()) {
                            set_error(u8"unexpected end of file");
                            _lexer.restore_state(state_backup);
                            return nullptr;
                        }

                        if(Declaration* declaration = try_declaration()) {
                            false_declarations->append(declaration);
                        } else {
                            return nullptr;
                        }
                    }

                    if(!_lexer.match(token_brace_close)) {
                        set_error(u8"expected '}' after expression");
                        _lexer.restore_state(state_backup);
                        return nullptr;
                    }

                    return new Declaration_If(condition.release(), true_declarations.release(), false_declarations.release());
                }
            } else {
                return new Declaration_If(condition.release(), true_declarations.release(), nullptr);
            }
        }

        Import_Decl* try_import_decl() {
            Lexer_State const state_backup = _lexer.get_current_state();
            if(!_lexer.match(kw_import, true)) {
                set_error(u8"expected 'import'");
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            if(String_Literal* string = try_string_literal()) {
                return new Import_Decl(string->value);
            } else {
                _lexer.restore_state(state_backup);
                return nullptr;
            }
        }

        Variable_Declaration* try_variable_declaration() {
            Lexer_State const state_backup = _lexer.get_current_state();
            Owning_Ptr var_type = try_type();
            if(!var_type) {
                set_error(u8"expected type");
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            Owning_Ptr<Identifier> var_name = nullptr;
            if(anton::String identifier; _lexer.match_identifier(identifier)) {
                var_name = new Identifier(anton::move(identifier));
            } else {
                set_error(u8"expected variable name");
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            Owning_Ptr<Expression> initializer = nullptr;
            if(_lexer.match(token_assign)) {
                initializer = try_expression();
                if(!initializer) {
                    _lexer.restore_state(state_backup);
                    return nullptr;
                }
            }

            if(!_lexer.match(token_semicolon)) {
                set_error(u8"expected ';' after variable declaration");
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            return new Variable_Declaration(var_type.release(), var_name.release(), initializer.release());
        }

        Constant_Declaration* try_constant_declaration() {
            Lexer_State const state_backup = _lexer.get_current_state();
            if(!_lexer.match(kw_const)) {
                set_error(u8"expected 'const'");
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            Owning_Ptr var_type = try_type();
            if(!var_type) {
                set_error(u8"expected type");
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            Owning_Ptr<Identifier> var_name = nullptr;
            if(anton::String identifier; _lexer.match_identifier(identifier)) {
                var_name = new Identifier(anton::move(identifier));
            } else {
                set_error(u8"expected variable name");
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            Owning_Ptr<Expression> initializer = nullptr;
            if(_lexer.match(token_assign)) {
                initializer = try_expression();
                if(!initializer) {
                    _lexer.restore_state(state_backup);
                    return nullptr;
                }
            }

            if(!_lexer.match(token_semicolon)) {
                set_error(u8"expected ';' after constant declaration");
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            return new Constant_Declaration(var_type.release(), var_name.release(), initializer.release());
        }

        Struct_Decl* try_struct_decl() {
            Lexer_State const state_backup = _lexer.get_current_state();
            if(!_lexer.match(kw_struct, true)) {
                set_error(u8"expected 'struct'");
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            Owning_Ptr<Identifier> struct_name;
            if(anton::String name; _lexer.match_identifier(name)) {
                struct_name = new Identifier(anton::move(name));
            } else {
                set_error(u8"expected identifier");
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            if(!_lexer.match(token_brace_open)) {
                set_error(u8"expected '{'");
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            if(_lexer.match(token_brace_close)) {
                set_error(u8"empty structs are not allowed");
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            Owning_Ptr struct_decl = new Struct_Decl(struct_name.release());
            while(true) {
                if(Variable_Declaration* decl = try_variable_declaration()) {
                    struct_decl->append(decl);
                } else {
                    break;
                }
            }

            if(struct_decl->size() == 0) {
                set_error(u8"empty structs are not allowed");
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            if(!_lexer.match(token_brace_close)) {
                set_error(u8"expected '}'");
                _lexer.restore_state(state_backup);
            }

            return struct_decl.release();
        }

        Pass_Stage_Declaration* try_pass_stage_declaration() {
            Lexer_State const state_backup = _lexer.get_current_state();
            Owning_Ptr return_type = try_type();
            if(!return_type) {
                set_error(u8"expected type");
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            Owning_Ptr<Identifier> pass = nullptr;
            if(anton::String pass_name_str; _lexer.match_identifier(pass_name_str)) {
                pass = new Identifier(anton::move(pass_name_str));
            } else {
                set_error(u8"expected pass name");
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            if(!_lexer.match(token_scope_resolution)) {
                set_error(u8"expected '::' after pass name");
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            static constexpr anton::String_View stage_types_strings[] = {stage_vertex, stage_fragment, stage_compute};
            static constexpr Pass_Stage_Type stage_types[] = {Pass_Stage_Type::vertex, Pass_Stage_Type::fragment, Pass_Stage_Type::compute};

            Pass_Stage_Type stage_type;
            {
                bool found = false;
                for(i64 i = 0; i < 3; ++i) {
                    if(_lexer.match(stage_types_strings[i], true)) {
                        stage_type = stage_types[i];
                        found = true;
                        break;
                    }
                }

                if(!found) {
                    set_error(u8"expected stage type");
                    _lexer.restore_state(state_backup);
                    return nullptr;
                }
            }

            Owning_Ptr param_list = try_function_param_list(true);
            if(!param_list) {
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            Owning_Ptr function_body = try_function_body();
            if(!function_body) {
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            return new Pass_Stage_Declaration(pass.release(), stage_type, param_list.release(), return_type.release(), function_body.release());
        }

        Function_Declaration* try_function_declaration() {
            Lexer_State const state_backup = _lexer.get_current_state();
            Owning_Ptr return_type = try_type();
            if(!return_type) {
                set_error(u8"expected type");
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            Owning_Ptr<Identifier> name = nullptr;
            if(anton::String fn_name; _lexer.match_identifier(fn_name)) {
                name = new Identifier(anton::move(fn_name));
            } else {
                set_error(u8"expected function name");
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            Owning_Ptr param_list = try_function_param_list(false);
            if(!param_list) {
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            Owning_Ptr function_body = try_function_body();
            if(!function_body) {
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            return new Function_Declaration(name.release(), param_list.release(), return_type.release(), function_body.release());
        }

        Function_Param_List* try_function_param_list(bool const allow_sourced_params) {
            Lexer_State const state_backup = _lexer.get_current_state();
            if(!_lexer.match(token_paren_open)) {
                set_error(u8"expected '('");
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            if(_lexer.match(token_paren_close)) {
                return new Function_Param_List;
            }

            // Match parameters
            Owning_Ptr param_list = new Function_Param_List;
            {
                do {
                    if(Function_Param* param = try_function_param(allow_sourced_params)) {
                        param_list->append_parameter(param);
                    } else {
                        _lexer.restore_state(state_backup);
                        return nullptr;
                    }
                } while(_lexer.match(token_comma));
            }

            if(!_lexer.match(token_paren_close)) {
                set_error(u8"expected ')' after function parameter list");
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            return param_list.release();
        }

        Function_Param* try_function_param(bool const allow_sourced_params) {
            Lexer_State const state_backup = _lexer.get_current_state();
            if(Function_Param_If* param_if = try_function_param_if(allow_sourced_params)) {
                return param_if;
            }

            Owning_Ptr<Identifier> identifier = nullptr;
            if(anton::String identifier_str; _lexer.match_identifier(identifier_str)) {
                identifier = new Identifier(anton::move(identifier_str));
            } else {
                set_error(u8"expected parameter name");
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            if(!_lexer.match(token_colon)) {
                set_error(u8"expected ':' after parameter name");
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            Owning_Ptr parameter_type = try_type();
            if(!parameter_type) {
                set_error("expected parameter type");
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            if(_lexer.match(kw_from, true)) {
                if(!allow_sourced_params) {
                    set_error(u8"illegal sourced parameter declaration");
                    _lexer.restore_state(state_backup);
                    return nullptr;
                }

                if(anton::String identifier_str; _lexer.match_identifier(identifier_str)) {
                    Identifier* source = new Identifier(anton::move(identifier_str));
                    return new Sourced_Function_Param(identifier.release(), parameter_type.release(), source);
                } else {
                    set_error("expected parameter source after 'from'");
                    _lexer.restore_state(state_backup);
                    return nullptr;
                }
            } else {
                return new Ordinary_Function_Param(identifier.release(), parameter_type.release());
            }
        }

        Function_Param_If* try_function_param_if(bool const allow_sourced_params) {
            Lexer_State const state_backup = _lexer.get_current_state();
            if(!_lexer.match(kw_if, true)) {
                set_error(u8"expected '{'");
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            Owning_Ptr condition = try_expression();
            if(!condition) {
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            if(!_lexer.match(token_brace_open)) {
                set_error(u8"expected '{'");
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            Owning_Ptr true_param = try_function_param(allow_sourced_params);
            if(!true_param) {
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            if(!_lexer.match(token_brace_close)) {
                set_error(u8"expected '}'");
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            if(_lexer.match(kw_else, true)) {
                if(Function_Param_If* param_if = try_function_param_if(allow_sourced_params)) {
                    return new Function_Param_If(condition.release(), true_param.release(), param_if);
                } else {
                    if(!_lexer.match(token_brace_open)) {
                        set_error(u8"expected '{'");
                        _lexer.restore_state(state_backup);
                        return nullptr;
                    }

                    Owning_Ptr false_param = try_function_param(allow_sourced_params);
                    if(!false_param) {
                        _lexer.restore_state(state_backup);
                        return nullptr;
                    }

                    if(!_lexer.match(token_brace_close)) {
                        set_error(u8"expected '}'");
                        _lexer.restore_state(state_backup);
                        return nullptr;
                    }

                    return new Function_Param_If(condition.release(), true_param.release(), false_param.release());
                }
            } else {
                return new Function_Param_If(condition.release(), true_param.release(), nullptr);
            }
        }

        Function_Body* try_function_body() {
            if(!_lexer.match(token_brace_open)) {
                set_error("expected '{' at the beginning of function body");
                return nullptr;
            }

            if(_lexer.match(token_brace_close)) {
                return new Function_Body(nullptr);
            }

            Owning_Ptr statements = try_statement_list();
            if(statements->size() == 0) {
                return nullptr;
            }

            if(!_lexer.match(token_brace_close)) {
                set_error("expected '}' at the end of the function body");
                return nullptr;
            }

            return new Function_Body(statements.release());
        }

        Statement_List* try_statement_list() {
            Statement_List* statements = new Statement_List;
            while(true) {
                if(Block_Statement* block_statement = try_block_statement()) {
                    statements->append(block_statement);
                    continue;
                }

                if(If_Statement* if_statement = try_if_statement()) {
                    statements->append(if_statement);
                    continue;
                }

                if(Switch_Statement* switch_statement = try_switch_statement()) {
                    statements->append(switch_statement);
                    continue;
                }

                if(For_Statement* for_statement = try_for_statement()) {
                    statements->append(for_statement);
                    continue;
                }

                if(While_Statement* while_statement = try_while_statement()) {
                    statements->append(while_statement);
                    continue;
                }

                if(Do_While_Statement* do_while_statement = try_do_while_statement()) {
                    statements->append(do_while_statement);
                    continue;
                }

                if(Return_Statement* return_statement = try_return_statement()) {
                    statements->append(return_statement);
                    continue;
                }

                if(Break_Statement* break_statement = try_break_statement()) {
                    statements->append(break_statement);
                    continue;
                }

                if(Continue_Statement* continue_statement = try_continue_statement()) {
                    statements->append(continue_statement);
                    continue;
                }

                if(Variable_Declaration* decl = try_variable_declaration()) {
                    Declaration_Statement* decl_stmt = new Declaration_Statement(decl);
                    statements->append(decl_stmt);
                    continue;
                }

                if(Constant_Declaration* decl = try_constant_declaration()) {
                    Declaration_Statement* decl_stmt = new Declaration_Statement(decl);
                    statements->append(decl_stmt);
                    continue;
                }

                if(Expression_Statement* expr_stmt = try_expression_statement()) {
                    statements->append(expr_stmt);
                    continue;
                }

                return statements;
            }
        }

        Type* try_type() {
            static constexpr anton::String_View builtin_types_strings[] = {
                type_void,    type_bool,   type_int,     type_uint,    type_float,   type_double,  type_vec2,    type_vec3,    type_vec4,
                type_dvec2,   type_dvec3,  type_dvec4,   type_bvec2,   type_bvec3,   type_bvec4,   type_ivec2,   type_ivec3,   type_ivec4,
                type_uvec2,   type_uvec3,  type_uvec4,   type_mat2,    type_mat2x2,  type_mat3,    type_mat3x3,  type_mat4,    type_mat4x4,
                type_mat2x3,  type_mat2x4, type_mat3x2,  type_mat3x4,  type_mat4x2,  type_mat4x3,  type_dmat2,   type_dmat2x2, type_dmat3,
                type_dmat3x3, type_dmat4,  type_dmat4x4, type_dmat2x3, type_dmat2x4, type_dmat3x2, type_dmat3x4, type_dmat4x2, type_dmat4x3};

            static constexpr Builtin_GLSL_Type builtin_types[] = {
                Builtin_GLSL_Type::glsl_void,    Builtin_GLSL_Type::glsl_bool,    Builtin_GLSL_Type::glsl_int,     Builtin_GLSL_Type::glsl_uint,
                Builtin_GLSL_Type::glsl_float,   Builtin_GLSL_Type::glsl_double,  Builtin_GLSL_Type::glsl_vec2,    Builtin_GLSL_Type::glsl_vec3,
                Builtin_GLSL_Type::glsl_vec4,    Builtin_GLSL_Type::glsl_dvec2,   Builtin_GLSL_Type::glsl_dvec3,   Builtin_GLSL_Type::glsl_dvec4,
                Builtin_GLSL_Type::glsl_bvec2,   Builtin_GLSL_Type::glsl_bvec3,   Builtin_GLSL_Type::glsl_bvec4,   Builtin_GLSL_Type::glsl_ivec2,
                Builtin_GLSL_Type::glsl_ivec3,   Builtin_GLSL_Type::glsl_ivec4,   Builtin_GLSL_Type::glsl_uvec2,   Builtin_GLSL_Type::glsl_uvec3,
                Builtin_GLSL_Type::glsl_uvec4,   Builtin_GLSL_Type::glsl_mat2,    Builtin_GLSL_Type::glsl_mat2,    Builtin_GLSL_Type::glsl_mat3,
                Builtin_GLSL_Type::glsl_mat3,    Builtin_GLSL_Type::glsl_mat4,    Builtin_GLSL_Type::glsl_mat4,    Builtin_GLSL_Type::glsl_mat2x3,
                Builtin_GLSL_Type::glsl_mat2x4,  Builtin_GLSL_Type::glsl_mat3x2,  Builtin_GLSL_Type::glsl_mat3x4,  Builtin_GLSL_Type::glsl_mat4x2,
                Builtin_GLSL_Type::glsl_mat4x3,  Builtin_GLSL_Type::glsl_dmat2,   Builtin_GLSL_Type::glsl_dmat2,   Builtin_GLSL_Type::glsl_dmat3,
                Builtin_GLSL_Type::glsl_dmat3,   Builtin_GLSL_Type::glsl_dmat4,   Builtin_GLSL_Type::glsl_dmat4,   Builtin_GLSL_Type::glsl_dmat2x3,
                Builtin_GLSL_Type::glsl_dmat2x4, Builtin_GLSL_Type::glsl_dmat3x2, Builtin_GLSL_Type::glsl_dmat3x4, Builtin_GLSL_Type::glsl_dmat4x2,
                Builtin_GLSL_Type::glsl_dmat4x3};

            for(i64 i = 0; i < 45; ++i) {
                if(_lexer.match(builtin_types_strings[i], true)) {
                    return new Builtin_Type(builtin_types[i]);
                }
            }

            if(anton::String name; _lexer.match_identifier(name)) {
                return new User_Defined_Type(anton::move(name));
            } else {
                set_error("expected identifier");
                return nullptr;
            }
        }

        Block_Statement* try_block_statement() {
            Lexer_State const state_backup = _lexer.get_current_state();
            if(!_lexer.match(token_brace_open)) {
                set_error("expected '{' at the start of the block");
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            if(_lexer.match(token_brace_close)) {
                return new Block_Statement(nullptr);
            }

            Owning_Ptr statements = try_statement_list();
            if(statements->size() == 0) {
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            if(!_lexer.match(token_brace_close)) {
                set_error("expected '}' at the end of the block");
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            return new Block_Statement(statements.release());
        }

        If_Statement* try_if_statement() {
            Lexer_State const state_backup = _lexer.get_current_state();
            if(!_lexer.match(kw_if, true)) {
                set_error(u8"expected 'if'");
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            Owning_Ptr condition = try_expression();
            if(!condition) {
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            Owning_Ptr true_statement = try_block_statement();
            if(!true_statement) {
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            if(_lexer.match(kw_else, true)) {
                if(If_Statement* if_statement = try_if_statement()) {
                    return new If_Statement(condition.release(), true_statement.release(), if_statement);
                } else {
                    if(Block_Statement* false_statement = try_block_statement()) {
                        return new If_Statement(condition.release(), true_statement.release(), false_statement);
                    } else {
                        _lexer.restore_state(state_backup);
                        return nullptr;
                    }
                }
            } else {
                return new If_Statement(condition.release(), true_statement.release(), nullptr);
            }
        }

        Switch_Statement* try_switch_statement() {
            Lexer_State const state_backup = _lexer.get_current_state();
            if(!_lexer.match(kw_switch, true)) {
                set_error(u8"expected 'switch'");
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            Owning_Ptr match_expression = try_expression();
            if(!match_expression) {
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            if(!_lexer.match(token_brace_open)) {
                set_error(u8"expected '{'");
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            Owning_Ptr switch_statement = new Switch_Statement(match_expression.release());
            while(true) {
                if(_lexer.match(kw_case, true)) {
                    Owning_Ptr condition = try_expression();
                    if(!condition) {
                        _lexer.restore_state(state_backup);
                        return nullptr;
                    }

                    if(!_lexer.match(token_colon)) {
                        set_error(u8"expected ':' after case label");
                        _lexer.restore_state(state_backup);
                        return nullptr;
                    }

                    Owning_Ptr statement_list = try_statement_list();
                    if(statement_list) {
                        switch_statement->append(new Case_Statement(condition.release(), statement_list.release()));
                    } else {
                        _lexer.restore_state(state_backup);
                        return nullptr;
                    }
                } else if(_lexer.match(kw_default, true)) {
                    if(!_lexer.match(token_colon)) {
                        set_error(u8"expected ':' after case label");
                        _lexer.restore_state(state_backup);
                        return nullptr;
                    }

                    Owning_Ptr statement_list = try_statement_list();
                    if(statement_list) {
                        switch_statement->append(new Default_Case_Statement(statement_list.release()));
                    } else {
                        _lexer.restore_state(state_backup);
                        return nullptr;
                    }
                } else {
                    break;
                }
            }

            if(!_lexer.match(token_brace_close)) {
                set_error(u8"expected '}'");
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            return switch_statement.release();
        }

        For_Statement* try_for_statement() {
            Lexer_State const state_backup = _lexer.get_current_state();
            if(!_lexer.match(kw_for, true)) {
                set_error("expected 'for'");
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            // Match variable

            Owning_Ptr<Variable_Declaration> variable_declaration = nullptr;
            if(!_lexer.match(token_semicolon)) {
                Owning_Ptr var_type = try_type();
                if(!var_type) {
                    set_error("expected type");
                    _lexer.restore_state(state_backup);
                    return nullptr;
                }

                Owning_Ptr<Identifier> var_name = nullptr;
                if(anton::String identifier; _lexer.match_identifier(identifier)) {
                    var_name = new Identifier(anton::move(identifier));
                } else {
                    set_error("expected variable name");
                    _lexer.restore_state(state_backup);
                    return nullptr;
                }

                Owning_Ptr<Expression> initializer = nullptr;
                if(_lexer.match(token_assign)) {
                    initializer = try_expression();
                    if(!initializer) {
                        _lexer.restore_state(state_backup);
                        return nullptr;
                    }
                }

                if(!_lexer.match(token_semicolon)) {
                    set_error("expected ';' after variable declaration");
                    _lexer.restore_state(state_backup);
                    return nullptr;
                }

                variable_declaration = new Variable_Declaration(var_type.release(), var_name.release(), initializer.release());
            }

            Owning_Ptr<Expression> condition = nullptr;
            if(!_lexer.match(token_semicolon)) {
                condition = try_expression();
                if(!condition) {
                    _lexer.restore_state(state_backup);
                    return nullptr;
                }

                if(!_lexer.match(token_semicolon)) {
                    set_error("expected ';' after variable declaration");
                    _lexer.restore_state(state_backup);
                    return nullptr;
                }
            }

            Owning_Ptr post_expression = try_expression();

            Owning_Ptr block = try_block_statement();
            if(!block) {
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            return new For_Statement(variable_declaration.release(), condition.release(), post_expression.release(), block.release());
        }

        While_Statement* try_while_statement() {
            Lexer_State const state_backup = _lexer.get_current_state();
            if(!_lexer.match(kw_while, true)) {
                set_error("expected 'while'");
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            Owning_Ptr condition = try_expression();
            if(!condition) {
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            Owning_Ptr block = try_block_statement();
            if(!block) {
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            return new While_Statement(condition.release(), block.release());
        }

        Do_While_Statement* try_do_while_statement() {
            Lexer_State const state_backup = _lexer.get_current_state();
            if(!_lexer.match(kw_do, true)) {
                set_error("expected 'do'");
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            Owning_Ptr block = try_block_statement();
            if(!block) {
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            if(!_lexer.match(kw_while, true)) {
                set_error("expected 'while'");
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            Owning_Ptr condition = try_expression();
            if(!condition) {
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            if(!_lexer.match(token_semicolon)) {
                set_error("expected ';' after do-while statement");
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            return new Do_While_Statement(condition.release(), block.release());
        }

        Return_Statement* try_return_statement() {
            Lexer_State const state_backup = _lexer.get_current_state();
            if(!_lexer.match(kw_return, true)) {
                set_error("expected 'return'");
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            Owning_Ptr return_expr = try_expression();
            if(!return_expr) {
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            if(!_lexer.match(token_semicolon)) {
                set_error("expected ';' after return statement");
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            return new Return_Statement(return_expr.release());
        }

        Break_Statement* try_break_statement() {
            Lexer_State const state_backup = _lexer.get_current_state();
            if(!_lexer.match(kw_break, true)) {
                set_error(u8"expected 'break'");
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            if(!_lexer.match(token_semicolon)) {
                set_error(u8"expected ';' after break statement");
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            return new Break_Statement();
        }

        Continue_Statement* try_continue_statement() {
            Lexer_State const state_backup = _lexer.get_current_state();
            if(!_lexer.match(kw_continue, true)) {
                set_error(u8"expected 'continue'");
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            if(!_lexer.match(token_semicolon)) {
                set_error(u8"expected ';' after continue statement");
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            return new Continue_Statement();
        }

        Expression_Statement* try_expression_statement() {
            Lexer_State const state_backup = _lexer.get_current_state();
            Owning_Ptr expression = try_expression();
            if(!expression) {
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            if(!_lexer.match(token_semicolon)) {
                set_error("expected ';' at the end of statement");
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            return new Expression_Statement(expression.release());
        }

        Expression* try_expression() {
            if(Expression* expr = try_assignment_expression()) {
                return expr;
            } else {
                return nullptr;
            }
        }

        Expression* try_assignment_expression() {
            Lexer_State const state_backup = _lexer.get_current_state();
            Owning_Ptr lhs = try_logic_or_expr();
            if(!lhs) {
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            Arithmetic_Assignment_Type type;
            bool is_direct = false;
            if(_lexer.match(token_assign)) {
                is_direct = true;
            } else if(_lexer.match(token_compound_plus)) {
                type = Arithmetic_Assignment_Type::plus;
            } else if(_lexer.match(token_compound_minus)) {
                type = Arithmetic_Assignment_Type::minus;
            } else if(_lexer.match(token_compound_multiply)) {
                type = Arithmetic_Assignment_Type::multiply;
            } else if(_lexer.match(token_compound_divide)) {
                type = Arithmetic_Assignment_Type::divide;
            } else if(_lexer.match(token_compound_remainder)) {
                type = Arithmetic_Assignment_Type::remainder;
            } else if(_lexer.match(token_compound_bit_lshift)) {
                type = Arithmetic_Assignment_Type::lshift;
            } else if(_lexer.match(token_compound_bit_rshift)) {
                type = Arithmetic_Assignment_Type::rshift;
            } else if(_lexer.match(token_compound_bit_and)) {
                type = Arithmetic_Assignment_Type::bit_and;
            } else if(_lexer.match(token_compound_bit_or)) {
                type = Arithmetic_Assignment_Type::bit_or;
            } else if(_lexer.match(token_compound_bit_xor)) {
                type = Arithmetic_Assignment_Type::bit_xor;
            } else {
                return lhs.release();
            }

            Owning_Ptr rhs = try_assignment_expression();
            if(!rhs) {
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            if(is_direct) {
                return new Assignment_Expression(lhs.release(), rhs.release());
            } else {
                return new Arithmetic_Assignment_Expression(type, lhs.release(), rhs.release());
            }
        }

        Expression* try_elvis_expr() {
            Lexer_State const state_backup = _lexer.get_current_state();
            Owning_Ptr cond = try_logic_or_expr();
            if(!cond) {
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            if(!_lexer.match(token_question)) {
                return cond.release();
            }

            // TODO: is using try_expression here correct?

            Owning_Ptr true_expr = try_expression();
            if(!true_expr) {
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            if(!_lexer.match(token_colon)) {
                set_error(u8"expected ':'");
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            Owning_Ptr false_expr = try_expression();
            if(!false_expr) {
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            return new Elvis_Expr(cond.release(), true_expr.release(), false_expr.release());
        }

        Expression* try_logic_or_expr() {
            Lexer_State const state_backup = _lexer.get_current_state();
            Owning_Ptr lhs = try_logic_xor_expr();
            if(!lhs) {
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            while(_lexer.match(token_logic_or)) {
                if(Expression* rhs = try_logic_xor_expr()) {
                    lhs = new Logic_Or_Expr(lhs.release(), rhs);
                } else {
                    _lexer.restore_state(state_backup);
                    return nullptr;
                }
            }

            return lhs.release();
        }

        Expression* try_logic_xor_expr() {
            Lexer_State const state_backup = _lexer.get_current_state();
            Owning_Ptr lhs = try_logic_and_expr();
            if(!lhs) {
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            while(_lexer.match(token_logic_xor)) {
                if(Expression* rhs = try_logic_and_expr()) {
                    lhs = new Logic_Xor_Expr(lhs.release(), rhs);
                } else {
                    _lexer.restore_state(state_backup);
                    return nullptr;
                }
            }

            return lhs.release();
        }

        Expression* try_logic_and_expr() {
            Lexer_State const state_backup = _lexer.get_current_state();
            Owning_Ptr lhs = try_bit_or_expr();
            if(!lhs) {
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            while(_lexer.match(token_logic_and)) {
                if(Expression* rhs = try_bit_or_expr()) {
                    lhs = new Logic_And_Expr(lhs.release(), rhs);
                } else {
                    _lexer.restore_state(state_backup);
                    return nullptr;
                }
            }

            return lhs.release();
        }

        Expression* try_bit_or_expr() {
            Lexer_State const state_backup = _lexer.get_current_state();
            Owning_Ptr lhs = try_bit_xor_expr();
            if(!lhs) {
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            if(Lexer_State const check_backup = _lexer.get_current_state(); _lexer.match(token_compound_bit_or) || _lexer.match(token_logic_or)) {
                _lexer.restore_state(check_backup);
                return lhs.release();
            }

            while(_lexer.match(token_bit_or)) {
                if(Expression* rhs = try_bit_xor_expr()) {
                    lhs = new Bit_Or_Expr(lhs.release(), rhs);
                } else {
                    _lexer.restore_state(state_backup);
                    return nullptr;
                }
            }

            return lhs.release();
        }

        Expression* try_bit_xor_expr() {
            Lexer_State const state_backup = _lexer.get_current_state();
            Owning_Ptr lhs = try_bit_and_expr();
            if(!lhs) {
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            if(Lexer_State const check_backup = _lexer.get_current_state(); _lexer.match(token_compound_bit_xor) || _lexer.match(token_logic_xor)) {
                _lexer.restore_state(check_backup);
                return lhs.release();
            }

            while(_lexer.match(token_bit_xor)) {
                if(Expression* rhs = try_bit_and_expr()) {
                    lhs = new Bit_Xor_Expr(lhs.release(), rhs);
                } else {
                    _lexer.restore_state(state_backup);
                    return nullptr;
                }
            }

            return lhs.release();
        }

        Expression* try_bit_and_expr() {
            Lexer_State const state_backup = _lexer.get_current_state();
            Owning_Ptr lhs = try_relational_equality_expression();
            if(!lhs) {
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            if(Lexer_State const check_backup = _lexer.get_current_state(); _lexer.match(token_compound_bit_and) || _lexer.match(token_logic_and)) {
                _lexer.restore_state(check_backup);
                return lhs.release();
            }

            while(_lexer.match(token_bit_and)) {
                if(Expression* rhs = try_relational_equality_expression()) {
                    lhs = new Bit_And_Expr(lhs.release(), rhs);
                } else {
                    _lexer.restore_state(state_backup);
                    return nullptr;
                }
            }

            return lhs.release();
        }

        Expression* try_relational_equality_expression() {
            Lexer_State const state_backup = _lexer.get_current_state();
            Owning_Ptr lhs = try_relational_expression();
            if(!lhs) {
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            while(true) {
                if(_lexer.match(token_equal)) {
                    if(Expression* rhs = try_relational_expression()) {
                        lhs = new Relational_Equality_Expression(true, lhs.release(), rhs);
                    } else {
                        _lexer.restore_state(state_backup);
                        return nullptr;
                    }
                } else if(_lexer.match(token_not_equal)) {
                    if(Expression* rhs = try_relational_expression()) {
                        lhs = new Relational_Equality_Expression(false, lhs.release(), rhs);
                    } else {
                        _lexer.restore_state(state_backup);
                        return nullptr;
                    }
                } else {
                    return lhs.release();
                }
            }
        }

        Expression* try_relational_expression() {
            Lexer_State const state_backup = _lexer.get_current_state();
            Owning_Ptr lhs = try_lshift_rshift_expr();
            if(!lhs) {
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            while(true) {
                Relational_Type type;
                if(_lexer.match(token_less_equal)) {
                    type = Relational_Type::less_equal;
                } else if(_lexer.match(token_greater_equal)) {
                    type = Relational_Type::greater_equal;
                } else if(_lexer.match(token_less)) {
                    type = Relational_Type::less_than;
                } else if(_lexer.match(token_greater)) {
                    type = Relational_Type::greater_than;
                } else {
                    return lhs.release();
                }

                if(Expression* rhs = try_lshift_rshift_expr()) {
                    lhs = new Relational_Expression(type, lhs.release(), rhs);
                } else {
                    _lexer.restore_state(state_backup);
                    return nullptr;
                }
            }
        }

        Expression* try_lshift_rshift_expr() {
            Lexer_State const state_backup = _lexer.get_current_state();
            Owning_Ptr lhs = try_add_sub_expr();
            if(!lhs) {
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            if(Lexer_State const check_backup = _lexer.get_current_state();
               _lexer.match(token_compound_bit_lshift) || _lexer.match(token_compound_bit_rshift)) {
                _lexer.restore_state(check_backup);
                return lhs.release();
            }

            while(true) {
                if(_lexer.match(token_bit_lshift)) {
                    if(Expression* rhs = try_add_sub_expr()) {
                        lhs = new LShift_Expr(lhs.release(), rhs);
                    } else {
                        _lexer.restore_state(state_backup);
                        return nullptr;
                    }
                } else if(_lexer.match(token_bit_rshift)) {
                    if(Expression* rhs = try_add_sub_expr()) {
                        lhs = new RShift_Expr(lhs.release(), rhs);
                    } else {
                        _lexer.restore_state(state_backup);
                        return nullptr;
                    }
                } else {
                    return lhs.release();
                }
            }
        }

        Expression* try_add_sub_expr() {
            Lexer_State const state_backup = _lexer.get_current_state();
            Owning_Ptr lhs = try_mul_div_mod_expr();
            if(!lhs) {
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            if(Lexer_State const check_backup = _lexer.get_current_state(); _lexer.match(token_compound_plus) || _lexer.match(token_compound_minus)) {
                _lexer.restore_state(check_backup);
                return lhs.release();
            }

            while(true) {
                if(_lexer.match(token_plus)) {
                    if(Expression* rhs = try_mul_div_mod_expr()) {
                        lhs = new Add_Expr(lhs.release(), rhs);
                    } else {
                        _lexer.restore_state(state_backup);
                        return nullptr;
                    }
                } else if(_lexer.match(token_minus)) {
                    if(Expression* rhs = try_mul_div_mod_expr()) {
                        lhs = new Sub_Expr(lhs.release(), rhs);
                    } else {
                        _lexer.restore_state(state_backup);
                        return nullptr;
                    }
                } else {
                    return lhs.release();
                }
            }
        }

        Expression* try_mul_div_mod_expr() {
            Lexer_State const state_backup = _lexer.get_current_state();
            Owning_Ptr lhs = try_unary_expression();
            if(!lhs) {
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            if(Lexer_State const check_backup = _lexer.get_current_state();
               _lexer.match(token_compound_multiply) || _lexer.match(token_compound_divide) || _lexer.match(token_compound_remainder)) {
                _lexer.restore_state(check_backup);
                return lhs.release();
            }

            while(true) {
                if(_lexer.match(token_multiply)) {
                    if(Expression* rhs = try_unary_expression()) {
                        lhs = new Mul_Expr(lhs.release(), rhs);
                    } else {
                        _lexer.restore_state(state_backup);
                        return nullptr;
                    }
                } else if(_lexer.match(token_divide)) {
                    if(Expression* rhs = try_unary_expression()) {
                        lhs = new Div_Expr(lhs.release(), rhs);
                    } else {
                        _lexer.restore_state(state_backup);
                        return nullptr;
                    }
                } else if(_lexer.match(token_remainder)) {
                    if(Expression* rhs = try_unary_expression()) {
                        lhs = new Mod_Expr(lhs.release(), rhs);
                    } else {
                        _lexer.restore_state(state_backup);
                        return nullptr;
                    }
                } else {
                    return lhs.release();
                }
            }
        }

        Expression* try_unary_expression() {
            Lexer_State const state_backup = _lexer.get_current_state();
            if(_lexer.match(token_increment)) {
                if(Expression* expr = try_unary_expression()) {
                    return new Prefix_Inc_Expr(expr);
                } else {
                    _lexer.restore_state(state_backup);
                    return nullptr;
                }
            } else if(_lexer.match(token_decrement)) {
                if(Expression* expr = try_unary_expression()) {
                    return new Prefix_Dec_Expr(expr);
                } else {
                    _lexer.restore_state(state_backup);
                    return nullptr;
                }
            } else if(_lexer.match(token_plus)) {
                if(Expression* expr = try_unary_expression()) {
                    return new Unary_Expression(Unary_Type::plus, expr);
                } else {
                    _lexer.restore_state(state_backup);
                    return nullptr;
                }
            } else if(_lexer.match(token_minus)) {
                if(Expression* expr = try_unary_expression()) {
                    return new Unary_Expression(Unary_Type::minus, expr);
                } else {
                    _lexer.restore_state(state_backup);
                    return nullptr;
                }
            } else if(_lexer.match(token_logic_not)) {
                if(Expression* expr = try_unary_expression()) {
                    return new Unary_Expression(Unary_Type::logic_not, expr);
                } else {
                    _lexer.restore_state(state_backup);
                    return nullptr;
                }
            } else if(_lexer.match(token_bit_not)) {
                if(Expression* expr = try_unary_expression()) {
                    return new Unary_Expression(Unary_Type::bit_not, expr);
                } else {
                    _lexer.restore_state(state_backup);
                    return nullptr;
                }
            } else {
                return try_postfix_expression();
            }
        }

        Expression* try_postfix_expression() {
            Lexer_State const state_backup = _lexer.get_current_state();
            Owning_Ptr primary_expr = try_primary_expression();
            if(!primary_expr) {
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            Owning_Ptr<Expression> expr = primary_expr.release();
            while(true) {
                if(_lexer.match(token_dot)) {
                    if(anton::String name; _lexer.match_identifier(name)) {
                        expr = new Member_Access_Expression(expr.release(), new Identifier(anton::move(name)));
                    } else {
                        set_error("expected function name");
                        _lexer.restore_state(state_backup);
                        return nullptr;
                    }
                } else if(_lexer.match(token_bracket_open)) {
                    Owning_Ptr index = try_expression();
                    if(!index) {
                        _lexer.restore_state(state_backup);
                        return nullptr;
                    }

                    if(!_lexer.match(token_bracket_close)) {
                        set_error("expected ']'");
                        _lexer.restore_state(state_backup);
                        return nullptr;
                    }

                    expr = new Array_Access_Expression(expr.release(), index.release());
                } else if(_lexer.match(token_increment)) {
                    expr = new Postfix_Inc_Expr(expr.release());
                } else if(_lexer.match(token_decrement)) {
                    expr = new Postfix_Dec_Expr(expr.release());
                } else {
                    break;
                }
            }

            return expr.release();
        }

        Expression* try_primary_expression() {
            if(Paren_Expr* paren_expr = try_paren_expr()) {
                return paren_expr;
            }

            if(Expression_If* expression_if = try_expression_if()) {
                return expression_if;
            }

            if(Float_Literal* float_literal = try_float_literal()) {
                return float_literal;
            }

            if(Integer_Literal* integer_literal = try_integer_literal()) {
                return integer_literal;
            }

            if(Bool_Literal* bool_literal = try_bool_literal()) {
                return bool_literal;
            }

            if(Function_Call_Expression* function_call = try_function_call_expression()) {
                return function_call;
            }

            if(Identifier_Expression* identifier_expression = try_identifier_expression()) {
                return identifier_expression;
            }

            return nullptr;
        }

        Paren_Expr* try_paren_expr() {
            Lexer_State const state_backup = _lexer.get_current_state();
            if(!_lexer.match(token_paren_open)) {
                set_error(u8"expected '('");
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            Owning_Ptr paren_expression = try_expression();
            if(!paren_expression) {
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            if(!_lexer.match(token_paren_close)) {
                set_error("expected ')'");
                _lexer.restore_state(state_backup);
                return nullptr;
            } else {
                return new Paren_Expr(paren_expression.release());
            }
        }

        Expression_If* try_expression_if() {
            Lexer_State const state_backup = _lexer.get_current_state();
            if(!_lexer.match(kw_if, true)) {
                set_error(u8"expected 'if'");
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            Owning_Ptr condition = try_expression();
            if(!condition) {
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            if(!_lexer.match(token_brace_open)) {
                set_error(u8"expected '{'");
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            if(_lexer.match(token_brace_close)) {
                set_error(u8"expected an expression before '}'");
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            Owning_Ptr true_expression = try_expression();
            if(!true_expression) {
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            if(!_lexer.match(token_brace_close)) {
                set_error(u8"expected '}' after expression");
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            if(!_lexer.match(kw_else, true)) {
                set_error(u8"expected an 'else' branch");
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            if(Expression_If* expression_if = try_expression_if()) {
                return new Expression_If(condition.release(), true_expression.release(), expression_if);
            } else {
                if(!_lexer.match(token_brace_open)) {
                    set_error(u8"expected '{'");
                    _lexer.restore_state(state_backup);
                    return nullptr;
                }

                if(_lexer.match(token_brace_close)) {
                    set_error(u8"expected an expression before '}'");
                    _lexer.restore_state(state_backup);
                    return nullptr;
                }

                Owning_Ptr false_expression = try_expression();
                if(!false_expression) {
                    _lexer.restore_state(state_backup);
                    return nullptr;
                }

                if(!_lexer.match(token_brace_close)) {
                    set_error(u8"expected '}' after expression");
                    _lexer.restore_state(state_backup);
                    return nullptr;
                }

                return new Expression_If(condition.release(), true_expression.release(), false_expression.release());
            }
        }

        Function_Call_Expression* try_function_call_expression() {
            Lexer_State const state_backup = _lexer.get_current_state();
            Owning_Ptr<Identifier> identifier = nullptr;
            if(anton::String name; _lexer.match_identifier(name)) {
                identifier = new Identifier(anton::move(name));
            } else {
                set_error("expected function name");
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            if(!_lexer.match(token_paren_open)) {
                set_error("expected '(' after function name");
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            Owning_Ptr arg_list = new Argument_List;
            if(_lexer.match(token_paren_close)) {
                return new Function_Call_Expression(identifier.release(), arg_list.release());
            }

            do {
                if(Expression* expression = try_expression()) {
                    arg_list->append(expression);
                } else {
                    _lexer.restore_state(state_backup);
                    return nullptr;
                }
            } while(_lexer.match(token_comma));

            if(!_lexer.match(token_paren_close)) {
                set_error("expected ')'");
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            return new Function_Call_Expression(identifier.release(), arg_list.release());
        }

        Float_Literal* try_float_literal() {
            _lexer.ignore_whitespace_and_comments();
            Lexer_State const state_backup = _lexer.get_current_state();

            anton::String number;
            if(char32 const next_char = _lexer.peek_next(); next_char == '-' || next_char == U'+') {
                number += next_char;
                _lexer.get_next();
            }

            i64 pre_point_digits = 0;
            for(char32 next_char = _lexer.peek_next(); is_digit(next_char); ++pre_point_digits) {
                number += next_char;
                _lexer.get_next();
                next_char = _lexer.peek_next();
            }

            if(pre_point_digits == 0) {
                number += U'0';
            }

            bool has_period = false;
            i64 post_point_digits = 0;
            if(_lexer.peek_next() == '.') {
                has_period = true;
                number += '.';
                _lexer.get_next();
                for(char32 next_char = _lexer.peek_next(); is_digit(next_char); ++post_point_digits) {
                    number += next_char;
                    _lexer.get_next();
                    next_char = _lexer.peek_next();
                }

                if(post_point_digits == 0) {
                    number += U'0';
                }
            }

            if(pre_point_digits == 0 && post_point_digits == 0) {
                set_error("not a floating point constant");
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            bool has_e = false;
            if(char32 const e = _lexer.peek_next(); e == U'e' || e == U'E') {
                has_e = true;
                number += 'E';
                _lexer.get_next();
                if(char32 const sign = _lexer.peek_next(); sign == '-' || sign == U'+') {
                    number += sign;
                    _lexer.get_next();
                }

                i64 e_digits = 0;
                for(char32 next_char = _lexer.peek_next(); is_digit(next_char); ++e_digits) {
                    number += next_char;
                    _lexer.get_next();
                    next_char = _lexer.peek_next();
                }

                if(e_digits == 0) {
                    set_error("exponent has no digits");
                    _lexer.restore_state(state_backup);
                    return nullptr;
                }
            }

            if(!has_e && !has_period) {
                set_error(u8"not a floating point constant");
                _lexer.restore_state(state_backup);
                return nullptr;
            }

            if(is_identifier_character(_lexer.peek_next())) {
                anton::String suffix;
                while(is_identifier_character(_lexer.peek_next())) {
                    suffix += _lexer.get_next();
                }

                if(suffix == "f" || suffix == "F" || suffix == "lf" || suffix == "LF") {
                    number += suffix;
                } else {
                    anton::String error = "invalid suffix '" + suffix + "' on floating point constant";
                    set_error(error);
                    _lexer.restore_state(state_backup);
                    return nullptr;
                }
            }

            return new Float_Literal(anton::move(number));
        }

        Integer_Literal* try_integer_literal() {
            _lexer.ignore_whitespace_and_comments();

            Lexer_State const state_backup = _lexer.get_current_state();

            anton::String out;
            char32 next = _lexer.peek_next();
            if(next == '-' || next == '+') {
                out += next;
                _lexer.get_next();
            }

            // TODO: Add handling for leading zeros.
            // TODO: Add hexadecimal and binary literals.

            i64 length = 0;
            while(is_digit(_lexer.peek_next())) {
                out += _lexer.get_next();
                length += 1;
            }

            if(length != 0) {
                return new Integer_Literal(anton::move(out));
            } else {
                set_error("expected more than 0 digits");
                _lexer.restore_state(state_backup);
                return nullptr;
            }
        }

        Bool_Literal* try_bool_literal() {
            if(_lexer.match(kw_true, true)) {
                return new Bool_Literal(true);
            } else if(_lexer.match(kw_false, true)) {
                return new Bool_Literal(false);
            } else {
                set_error("expected bool literal");
                return nullptr;
            }
        }

        String_Literal* try_string_literal() {
            Lexer_State const state_backup = _lexer.get_current_state();
            if(!_lexer.match(token_double_quote)) {
                set_error(u8"expected \"");
                return nullptr;
            }

            anton::String string;
            while(true) {
                char32 next_char = _lexer.peek_next();
                if(next_char == U'\\') {
                    string += (char)_lexer.get_next();
                    string += (char)_lexer.get_next();
                } else if(next_char == eof_char32) {
                    set_error(u8"unexpected end of file");
                    _lexer.restore_state(state_backup);
                    return nullptr;
                } else if(next_char == U'\"') {
                    _lexer.get_next();
                    return new String_Literal(anton::move(string));
                } else {
                    string += (char)next_char;
                    _lexer.get_next();
                }
            }
        }

        Identifier_Expression* try_identifier_expression() {
            if(anton::String name; _lexer.match_identifier(name)) {
                Identifier* identifier = new Identifier(anton::move(name));
                return new Identifier_Expression(identifier);
            } else {
                set_error("expected an identifer");
                return nullptr;
            }
        }
    };

    Expected<Owning_Ptr<Declaration_List>, Parse_Error> parse_file(anton::String const& path) {
        std::ifstream file(path.data());
        if(!file) {
            anton::String msg = "could not open file " + path + " for reading.";
            return {expected_error, anton::move(msg), 0, 0, 0};
        }

        Parser parser(file);
        Owning_Ptr ast = parser.build_ast();
        if(ast) {
            return {expected_value, anton::move(ast)};
        } else {
            Parse_Error error = parser.get_last_error();
            return {expected_error, anton::move(error)};
        }
    }
} // namespace vush
