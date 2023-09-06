import subprocess

from builtin_functions import Param_Type, Array_Type, Return_Placeholder, return_, Fn, builtin_function_definitions
from builtin_type import Builtin_Type, stringify_builtin_type
from builtin_operator import Builtin_Operator, builtin_operator_definitions

def generate_alloc_identifier(value):
    return f"ast::Identifier{{\"{value}\"_sv, {{}}}}"

def generate_alloc_lt_integer(size):
    return f"allocate<ast::Lt_Integer>(allocator, ast::lt_integer_i32, {size}, Source_Info{{}})"

def generate_alloc_type(t):
    if isinstance(t, Array_Type):
        return f"allocate<ast::Type_Array>(allocator, {generate_alloc_type(t.base)}, {generate_alloc_lt_integer(t.size)}, Source_Info{{}})"
    elif isinstance(t, Builtin_Type):
        return f"ALLOC_BUILTIN(e_{stringify_builtin_type(t)})"
    else:
        raise TypeError("invalid type")

def generate_alloc_parameter(parameter):
    return f"ALLOC_PARAM(\"{parameter[0]}\"_sv, {generate_alloc_type(parameter[1])})"

def generate_alloc_parameter_array(parameters):
    parameter_list = [generate_alloc_parameter(p) for p in parameters]
    if len(parameter_list) > 0:
        return f"allocate<Array<ast::Fn_Parameter *>>(allocator, allocator, anton::variadic_construct, {','.join(parameter_list)})"
    else:
        return f"allocate<Array<ast::Fn_Parameter *>>(allocator, allocator)"

def generate_alloc_function(identifier, return_type, parameters):
    fn_return_type = generate_alloc_type(return_type)
    fn_parameter_array = generate_alloc_parameter_array(parameters)
    return f"ALLOC_FUNCTION(\"{identifier}\"_sv, {fn_return_type}, *{fn_parameter_array})"

def generate_functions(fn):
    def create_signature_generator(signature):
        # The enumerations within Param_Type are tuples containing one element, therefore we have to
        # unwrap them everywhere by accessing the first element.

        # calculate_min_length
        # Calculate the minimum length of all Param_Type.
        #
        # Returns:
        # The least length of all Param_Type, If no Param_Type is present in the signature, the
        # length is 1.
        #
        def calculate_min_length(signature):
            min_length = 99
            has_param_type = False
            for p in signature:
                t = p[1]
                if isinstance(t, Param_Type):
                    min_length = min(len(t.value), min_length)
                    has_param_type = True
            if has_param_type == True:
                return min_length
            else:
                return 1

        def signature_generator(signature, index):
            for p in signature:
                if isinstance(p[1], Param_Type):
                    yield (p[0], p[1].value[index])
                elif isinstance(p[1], Builtin_Type) or isinstance(p[1], Array_Type):
                    yield p
                else:
                    raise TypeError("invalid parameter type")

        min_length = calculate_min_length(signature)
        for index in range(0, min_length):
            yield signature_generator(signature, index)

    for g in create_signature_generator(fn.signature):
        return_parameter = next(g)
        yield generate_alloc_function(fn.identifier, return_parameter[1], g)

def generate_operator(operator):
    if len(operator.signature) == 3:
        # Binary operator.
        signature = zip((return_, "lhs", "rhs"), operator.signature)
    elif len(operator.signature) == 2:
        # Unary operator.
        signature = zip((return_, "value"), operator.signature)
    else:
        raise Exception("invalid operator")

    return_parameter = next(signature)
    return generate_alloc_function(operator.identifier, return_parameter[1], signature)

def write_get_builtin_functions_declarations(file, functions):
    file.write(f"""\
  Array<ast::Decl_Overloaded_Function*> get_builtin_functions_declarations(Allocator* allocator) {{
""")
    file.write(f"Array<ast::Decl_Overloaded_Function*> functions({len(functions)}, nullptr);\n")

    for i, (name, fn) in enumerate(functions.items()):
        identifier = f"\"{name}\"_sv"
        array = f"allocate<Array<ast::Decl_Function const*>>(allocator, allocator, anton::variadic_construct, {','.join(fn)})"
        file.write(f"functions[{i}] = allocate<ast::Decl_Overloaded_Function>(allocator, {identifier}, *{array});\n")

    file.write(f"""\
    return functions;
  }}
""")

def get_static_type_builtin_id(name):
    return f"builtin_{name}"

def write_get_builtin_type(file):
    file.write(f"""\
  ast::Type_Builtin const* get_builtin_type(ast::Type_Builtin_Kind const type) {{
    switch(type) {{
""")

    for v in Builtin_Type:
        name = stringify_builtin_type(v)
        file.write(f"    case ast::Type_Builtin_Kind::e_{name}: return &{get_static_type_builtin_id(name)};\n")

    file.write("""    }
  }
""")

def write_preamble(file):
    file.write("""\
// This file has been autogenerated.
// Do not modify manually.
//

#include <vush_ast/ast.hpp>
#include <vush_core/memory.hpp>

#define BUILTIN_TYPE(identifier, value) static constexpr ast::Type_Builtin identifier(ast::Type_Builtin_Kind::value, {})
#define ALLOC_BUILTIN(value) allocate<ast::Type_Builtin>(allocator, ast::Type_Builtin_Kind::value, Source_Info{})
#define ALLOC_PARAM(name, type) allocate<ast::Fn_Parameter>(allocator, ast::Identifier{name, {}}, type, ast::Identifier{""_sv, {}}, Source_Info{})
#define ALLOC_FUNCTION(identifier, return_type, parameter_array) \\
    allocate<ast::Decl_Function>(allocator, ast::Attr_List{}, ast::Identifier{identifier, {}}, \\
        parameter_array, return_type, ast::Node_List{}, true, Source_Info{})

namespace vush {
  using namespace anton::literals;

""")

def write_epilogue(file):
    file.write("""\
}
""")

def main():
    # TODO: --directory,-d option with default ./private/
    # TODO: --filename,-f option with default builtin_symbols_autogen.cpp

    functions = {}
    for fn in builtin_function_definitions:
        if fn.identifier not in functions:
            functions[fn.identifier] = []
        for f in generate_functions(fn):
            functions[fn.identifier].append(f)

    for op in builtin_operator_definitions:
        if op.identifier not in functions:
            functions[op.identifier] = []
        f = generate_operator(op)
        functions[op.identifier].append(f)

    file = open("./compiler/vush_autogen/builtin_symbols.cpp", "w")
    write_preamble(file)

    write_get_builtin_functions_declarations(file, functions)

    file.write("\n")

    for t in Builtin_Type:
        identifier = stringify_builtin_type(t)
        file.write(f"BUILTIN_TYPE({get_static_type_builtin_id(identifier)}, e_{identifier});\n")

    file.write("\n")

    write_get_builtin_type(file)

    write_epilogue(file)
    file.close()

    process = subprocess.run(["clang-format", "-i", "./compiler/vush_autogen/builtin_symbols.cpp"])
    process.check_returncode()

main()
