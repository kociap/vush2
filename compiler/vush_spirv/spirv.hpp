#pragma once

#include <anton/ilist.hpp>
#include <anton/string.hpp>

#include <vush_core/types.hpp>
#include <vush_spirv/spirv_fwd.hpp>

namespace vush::spirv {
  enum struct Instr_Kind {
    // Debug instructions
    e_string,
    e_line,
    // Extension instructions
    e_extension,
    e_ext_instr_import,
    e_ext_instr,
    // Mode instructions
    e_memory_model,
    e_entry_point,
    // e_execution_mode,
    e_capability,
    // Type instructions
    e_type_void,
    e_type_bool,
    e_type_int,
    e_type_float,
    e_type_vector,
    e_type_matrix,
    e_type_image,
    e_type_sampler,
    e_type_sampled_image,
    e_type_array,
    e_type_runtime_array,
    e_type_struct,
    e_type_pointer,
    e_type_function,
    // Constant creation instructions
    e_constant_true,
    e_constant_false,
    e_constant,
    e_constant_composite,
    // Memory instructions
    e_variable,
    e_load,
    e_store,
    e_access_chain,
    // Function instructions
    e_function,
    e_function_parameter,
    e_function_end,
    e_function_call,
    // Conversion instructions
    e_convert_f2u,
    e_convert_f2s,
    e_convert_s2f,
    e_convert_u2f,
    e_uconvert,
    e_sconvert,
    e_fconvert,
    e_convert_ptr2u,
    e_convert_u2ptr,
    // Composite instructions
    // e_vector_shuffle,
    e_composite_construct,
    e_composite_extract,
    e_composite_insert,
    e_copy_object,
    e_transpose,
    // Arithmetic instructions
    e_snegate,
    e_fnegate,
    e_iadd,
    e_fadd,
    e_isub,
    e_fsub,
    e_imul,
    e_fmul,
    e_udiv,
    e_sdiv,
    e_fdiv,
    e_umod,
    e_srem,
    e_smod,
    e_frem,
    e_fmod,
    e_vec_times_scalar,
    e_mat_times_scalar,
    e_vec_times_mat,
    e_mat_times_vec,
    e_mat_times_mat,
    e_outer_product,
    e_dot,
    // Bit instructions
    e_shr_logical,
    e_shr_arithmetic,
    e_shl,
    e_bit_or,
    e_bit_xor,
    e_bit_and,
    e_bit_not,
    // Relational instructions
    e_logical_eq,
    e_logical_neq,
    e_logical_or,
    e_logical_and,
    e_logical_not,
    e_select,
    e_ieq,
    e_ineq,
    e_ugt,
    e_sgt,
    e_uge,
    e_sge,
    e_ult,
    e_slt,
    e_ule,
    e_sle,
    e_foeq,
    e_fueq,
    e_foneq,
    e_funeq,
    e_folt,
    e_fult,
    e_fogt,
    e_fugt,
    e_fole,
    e_fule,
    e_foge,
    e_fuge,
    // Derivative instructions
    e_dPdx,
    e_dPdy,
    e_fwidth,
    e_dPdx_fine,
    e_dPdy_fine,
    e_fwidth_fine,
    e_dPdx_coarse,
    e_dPdy_coarse,
    e_fwidth_coarse,
    // Control-flow instructions
    e_phi,
    // e_loop_merge,
    // e_selection_merge,
    e_label,
    e_branch,
    e_brcond,
    e_switch,
    e_return,
    e_return_value,
    e_terminate,
    e_unreachable,
  };

  struct Instr: public anton::IList_DNode {
    u32 id;
    Instr_Kind instr_kind;
    Instr_label* label = nullptr;

    Instr(Instr_Kind instr_kind, u32 id): id(id), instr_kind(instr_kind) {}
  };

  template<typename T>
  [[nodiscard]] bool instanceof(Instr const* instr);

  template<typename T>
  [[nodiscard]] bool instanceof(Instr const& instr)
  {
    return instanceof<T>(&instr);
  }

  // get_result_type
  //
  // Returns:
  // The result_type member of the instruction if it has one or nullptr.
  //
  [[nodiscard]] Instr* get_result_type(Instr* instruction);

  struct Module {
    anton::IList<spirv::Instr> capabilities;
    anton::IList<spirv::Instr> extensions;
    anton::IList<spirv::Instr> imports;
    // Memory model, entry points, execution modes.
    anton::IList<spirv::Instr> declarations;
    anton::IList<spirv::Instr> debug;
    anton::IList<spirv::Instr> annotations;
    anton::IList<spirv::Instr> types;
    anton::IList<spirv::Instr> functions;
  };

  [[nodiscard]] u32 calculate_bound(Module const& module);

#define UNARY_INSTR(IDENTIFIER, KIND)                          \
  struct IDENTIFIER: public Instr {                            \
    Instr* result_type;                                        \
    Instr* operand;                                            \
                                                               \
    IDENTIFIER(u32 id, Instr* result_type, Instr* operand)     \
      : Instr(Instr_Kind::KIND, id), result_type(result_type), \
        operand(operand)                                       \
    {                                                          \
    }                                                          \
  };

#define BINARY_INSTR(IDENTIFIER, KIND)                                 \
  struct Instr_##IDENTIFIER: public Instr {                            \
    Instr* result_type;                                                \
    Instr* operand1;                                                   \
    Instr* operand2;                                                   \
                                                                       \
    Instr_##IDENTIFIER(u32 id, Instr* result_type, Instr* operand1,    \
                       Instr* operand2)                                \
      : Instr(Instr_Kind::KIND, id), result_type(result_type),         \
        operand1(operand1), operand2(operand2)                         \
    {                                                                  \
    }                                                                  \
  };                                                                   \
                                                                       \
  [[nodiscard]] Instr_##IDENTIFIER* make_instr_##IDENTIFIER(           \
    Allocator* allocator, u32 id, Instr* result_type, Instr* operand1, \
    Instr* operand2);

#define TYPED_INSTR(IDENTIFIER, KIND)                         \
  struct Instr_##IDENTIFIER: public Instr {                   \
    Instr* result_type;                                       \
    Instr_##IDENTIFIER(u32 id, Instr* result_type)            \
      : Instr(Instr_Kind::KIND, id), result_type(result_type) \
    {                                                         \
    }                                                         \
  };                                                          \
                                                              \
  [[nodiscard]] Instr_##IDENTIFIER* make_instr_##IDENTIFIER(  \
    Allocator* allocator, u32 id, Instr* result_type);

#define ID_INSTR(IDENTIFIER, KIND)                     \
  struct IDENTIFIER: public Instr {                    \
    IDENTIFIER(u32 id): Instr(Instr_Kind::KIND, id) {} \
  };

#define NOTHING_INSTR(IDENTIFIER, KIND)                      \
  struct Instr_##IDENTIFIER: public Instr {                  \
    Instr_##IDENTIFIER(): Instr(Instr_Kind::KIND, 0) {}      \
  };                                                         \
                                                             \
  [[nodiscard]] Instr_##IDENTIFIER* make_instr_##IDENTIFIER( \
    Allocator* allocator);

  struct Instr_string: public Instr {
    anton::String string;

    Instr_string(u32 id, anton::String&& string)
      : Instr(Instr_Kind::e_string, id), string(ANTON_MOV(string))
    {
    }
  };

  struct Instr_line: public Instr {
    Instr_string* file;
    u32 line;
    u32 column;

    Instr_line(Instr_string* file, u32 line, u32 column)
      : Instr(Instr_Kind::e_line, 0), file(file), line(line), column(column)
    {
    }
  };

  struct Instr_extension: public Instr {
    anton::String name;

    Instr_extension(anton::String&& name)
      : Instr(Instr_Kind::e_extension, 0), name(ANTON_MOV(name))
    {
    }
  };

  struct Instr_ext_instr_import: public Instr {
    anton::String name;

    Instr_ext_instr_import(u32 id, anton::String&& name)
      : Instr(Instr_Kind::e_ext_instr_import, id), name(ANTON_MOV(name))
    {
    }
  };

  struct Instr_ext_instr: public Instr {
    Instr* result_type;
    Instr_ext_instr_import* set;
    u32 instruction;
    Array<Instr*> operands;

    Instr_ext_instr(u32 id, Instr* result_type, Instr_ext_instr_import* set,
                    u32 instruction, Allocator* allocator)
      : Instr(Instr_Kind::e_ext_instr, id), result_type(result_type), set(set),
        instruction(instruction), operands(allocator)
    {
    }

    Instr_ext_instr(u32 id, Instr* result_type, Instr_ext_instr_import* set,
                    u32 instruction, Allocator* allocator,
                    anton::Slice<Instr* const> operands)
      : Instr(Instr_Kind::e_ext_instr, id), result_type(result_type), set(set),
        instruction(instruction), operands(allocator, anton::range_construct,
                                           operands.begin(), operands.end())
    {
    }
  };

  enum struct Addressing_Model {
    e_logical = 0,
    e_physical32 = 1,
    e_physical64 = 2,
    e_physical_storage_buffer64 = 5348,
  };

  enum struct Memory_Model {
    e_glsl450 = 1,
    e_vulkan = 3,
  };

  struct Instr_memory_model: public Instr {
    Addressing_Model addressing_model;
    Memory_Model memory_model;

    Instr_memory_model(Addressing_Model addressing_model,
                       Memory_Model memory_model)
      : Instr(Instr_Kind::e_memory_model, 0),
        addressing_model(addressing_model), memory_model(memory_model)
    {
    }
  };

  [[nodiscard]] Instr_memory_model*
  make_instr_memory_model(Allocator* allocator, Addressing_Model am,
                          Memory_Model mm);

  enum struct Execution_Model {
    e_vertex = 0,
    e_tessellation_control = 1,
    e_tessellation_evaluation = 2,
    e_geometry = 3,
    e_fragment = 4,
    e_glcompute = 5,
    e_kernel = 6,
  };

  struct Instr_entry_point: public Instr {
    Instr_function* entry_point;
    anton::String name;
    Array<Instr*> interface;
    Execution_Model execution_model;

    Instr_entry_point(Instr_function* entry_point, anton::String&& name,
                      Execution_Model execution_model, Allocator* allocator)
      : Instr(Instr_Kind::e_entry_point, 0), entry_point(entry_point),
        name(ANTON_MOV(name)), interface(allocator),
        execution_model(execution_model)
    {
    }
  };

  [[nodiscard]] Instr_entry_point*
  make_instr_entry_point(Allocator* allocator, Instr_function* entry_point,
                         anton::String&& name, Execution_Model execution_model);

  enum struct Capability {
    e_matrix = 0,
    e_shader = 1,
    e_geometry = 2,
    e_tessellation = 3,
    e_addresses = 4,
    e_linkage = 5,
    e_float16 = 9,
    e_float64 = 10,
    e_int64 = 11,
    e_int16 = 22,
    e_clip_distance = 32,
    e_cull_distance = 33,
    e_int8 = 39,
    e_draw_parameters = 4427,
  };

  struct Instr_capability: public Instr {
    Capability capability;

    Instr_capability(Capability capability)
      : Instr(Instr_Kind::e_capability, 0), capability(capability)
    {
    }
  };

  [[nodiscard]] Instr_capability* make_instr_capability(Allocator* allocator,
                                                        Capability capability);

  ID_INSTR(Instr_type_void, e_type_void);

  [[nodiscard]] Instr_type_void* make_instr_type_void(Allocator* allocator,
                                                      u32 id);

  ID_INSTR(Instr_type_bool, e_type_bool);

  [[nodiscard]] Instr_type_bool* make_instr_type_bool(Allocator* allocator,
                                                      u32 id);

  struct Instr_type_int: public Instr {
    u32 width;
    bool signedness;

    Instr_type_int(u32 id, u32 width, bool signedness)
      : Instr(Instr_Kind::e_type_int, id), width(width), signedness(signedness)
    {
    }
  };

  [[nodiscard]] Instr_type_int*
  make_instr_type_int(Allocator* allocator, u32 id, u32 width, bool signedness);

  struct Instr_type_float: public Instr {
    u32 width;
    // There are no alternative FP encodings, hence we omit the member.

    Instr_type_float(u32 id, u32 width)
      : Instr(Instr_Kind::e_type_float, id), width(width)
    {
    }
  };

  [[nodiscard]] Instr_type_float* make_instr_type_float(Allocator* allocator,
                                                        u32 id, u32 width);

  struct Instr_type_vector: public Instr {
    Instr* component_type;
    u32 component_count;

    Instr_type_vector(u32 id, Instr* component_type, u32 component_count)
      : Instr(Instr_Kind::e_type_vector, id), component_type(component_type),
        component_count(component_count)
    {
    }
  };

  [[nodiscard]] Instr_type_vector* make_instr_type_vector(Allocator* allocator,
                                                          u32 id,
                                                          Instr* component_type,
                                                          u32 component_count);

  struct Instr_type_matrix: public Instr {
    Instr* column_type;
    u32 column_count;

    Instr_type_matrix(u32 id, Instr* column_type, u32 column_count)
      : Instr(Instr_Kind::e_type_matrix, id), column_type(column_type),
        column_count(column_count)
    {
    }
  };

  [[nodiscard]] Instr_type_matrix* make_instr_type_matrix(Allocator* allocator,
                                                          u32 id,
                                                          Instr* column_type,
                                                          u32 column_count);

  enum struct Dimensionality {
    e_1D = 0,
    e_2D = 1,
    e_3D = 2,
    e_cube = 3,
    e_rect = 4,
    e_buffer = 5,
    e_subpass_data = 6,
  };

  enum struct Image_Format {
    e_unknown = 0,
    e_rgba32f = 1,
    e_rgba16f = 2,
    e_r32f = 3,
    e_rgba8 = 4,
    e_rgba8_snorm = 5,
    e_rg32f = 6,
    e_rg16f = 7,
    e_r11fg11fb10f = 8,
    e_r16f = 9,
    e_rgba16 = 10,
    e_rgb10a2 = 11,
    e_rg16 = 12,
    e_rg8 = 13,
    e_r16 = 14,
    e_r8 = 15,
    e_rgba16_snorm = 16,
    e_rg16_snorm = 17,
    e_rg8_snorm = 18,
    e_r16_snorm = 19,
    e_r8_snorm = 20,
    e_rgba32i = 21,
    e_rgba16i = 22,
    e_rgba8i = 23,
    e_r32i = 24,
    e_rg32i = 25,
    e_rg16i = 26,
    e_rg8i = 27,
    e_r16i = 28,
    e_r81 = 29,
    e_rgba32u = 30,
    e_rgba16u = 31,
    e_rgba8u = 32,
    e_r32u = 33,
    e_rgb10a2u = 34,
    e_rg32u = 35,
    e_rg16u = 36,
    e_rg8u = 37,
    e_r16u = 38,
    e_r8u = 39,
  };

  enum struct Access_Qualifier {
    e_read = 0,
    e_write = 1,
    e_readwrite = 2,
  };

  // Instr_type_image
  //
  // If dimensionality is subpass_data, sampled must be 2, image_format must be
  // unknown and execution model must be fragment.
  //
  struct Instr_type_image: public Instr {
    Instr* sampled_type;
    Dimensionality dimensionality;
    u8 depth;
    u8 arrayed;
    u8 sampled;
    Image_Format image_format;
    Access_Qualifier access_qualifier;
  };

  ID_INSTR(Instr_type_sampler, e_type_sampler);

  struct Instr_type_sampled_image: public Instr {
    // The type must not have a dimensionality of subpass_data or buffer.
    Instr_type_image* image_type;

    Instr_type_sampled_image(u32 id, Instr_type_image* image_type)
      : Instr(Instr_Kind::e_type_sampled_image, id), image_type(image_type)
    {
    }
  };

  struct Instr_type_array: public Instr {
    Instr* element_type;
    Instr* length;

    Instr_type_array(u32 id, Instr* element_type, Instr* length)
      : Instr(Instr_Kind::e_type_array, id), element_type(element_type),
        length(length)
    {
    }
  };

  [[nodiscard]] Instr_type_array* make_instr_type_array(Allocator* allocator,
                                                        u32 id,
                                                        Instr* element_type,
                                                        Instr* length);

  struct Instr_type_runtime_array: public Instr {
    Instr* element_type;

    Instr_type_runtime_array(u32 id, Instr* element_type)
      : Instr(Instr_Kind::e_type_runtime_array, id), element_type(element_type)
    {
    }
  };

  [[nodiscard]] Instr_type_runtime_array*
  make_instr_type_runtime_array(Allocator* allocator, u32 id,
                                Instr* element_type);

  struct Instr_type_struct: public Instr {
    Array<Instr*> field_types;

    Instr_type_struct(u32 id, Allocator* allocator)
      : Instr(Instr_Kind::e_type_struct, id), field_types(allocator)
    {
    }

    Instr_type_struct(u32 id, Allocator* allocator,
                      anton::Slice<Instr* const> field_types)
      : Instr(Instr_Kind::e_type_struct, id),
        field_types(allocator, anton::range_construct, field_types.begin(),
                    field_types.end())
    {
    }
  };

  [[nodiscard]] Instr_type_struct* make_instr_type_struct(Allocator* allocator,
                                                          u32 id);

  enum struct Storage_Class {
    e_uniform_constant = 0,
    e_input = 1,
    e_uniform = 2,
    e_output = 3,
    e_workgroup = 4,
    e_cross_workgroup = 5,
    e_private = 6,
    e_function = 7,
    e_generic = 8,
    e_push_constant = 9,
    e_atomic_counter = 10,
    e_image = 11,
    e_storage_buffer = 12,
  };

  struct Instr_type_pointer: public Instr {
    Instr* type;
    Storage_Class storage_class;

    Instr_type_pointer(u32 id, Instr* type, Storage_Class storage_class)
      : Instr(Instr_Kind::e_type_pointer, id), type(type),
        storage_class(storage_class)
    {
    }
  };

  [[nodiscard]] Instr_type_pointer*
  make_instr_type_pointer(Allocator* allocator, u32 id, Instr* type,
                          Storage_Class storage_class);

  struct Instr_type_function: public Instr {
    Instr* return_type;
    Array<Instr*> parameter_types;

    Instr_type_function(u32 id, Instr* return_type, Allocator* allocator)
      : Instr(Instr_Kind::e_type_function, id), return_type(return_type),
        parameter_types(allocator)
    {
    }

    Instr_type_function(u32 id, Instr* return_type, Allocator* allocator,
                        anton::Slice<Instr* const> parameter_types)
      : Instr(Instr_Kind::e_type_function, id), return_type(return_type),
        parameter_types(allocator, anton::range_construct,
                        parameter_types.begin(), parameter_types.end())
    {
    }
  };

  [[nodiscard]] Instr_type_function*
  make_instr_type_function(Allocator* allocator, u32 id, Instr* return_type);

  TYPED_INSTR(constant_true, e_constant_true);
  TYPED_INSTR(constant_false, e_constant_false);

  struct Instr_constant: public Instr {
    Instr* result_type;
    u8 byte_length;
    u32 word1 = 0;
    u32 word2 = 0;

    Instr_constant(u32 id, Instr* result_type, u8 byte_length, void* data)
      : Instr(Instr_Kind::e_constant, id), result_type(result_type),
        byte_length(byte_length)
    {
      memcpy(&word1, data, byte_length);
    }
  };

  // make_instr_constant_i32
  //
  // Parameters:
  // result_type - must be a i32 type.
  //
  [[nodiscard]] Instr_constant* make_instr_constant_i32(Allocator* allocator,
                                                        u32 id,
                                                        Instr* result_type,
                                                        i32 value);

  // make_instr_constant_u32
  //
  // Parameters:
  // result_type - must be a u32 type.
  //
  [[nodiscard]] Instr_constant* make_instr_constant_u32(Allocator* allocator,
                                                        u32 id,
                                                        Instr* result_type,
                                                        u32 value);

  // make_instr_constant_f32
  //
  // Parameters:
  // result_type - must be a f32 type.
  //
  [[nodiscard]] Instr_constant* make_instr_constant_f32(Allocator* allocator,
                                                        u32 id,
                                                        Instr* result_type,
                                                        f32 value);

  // make_instr_constant_f64
  //
  // Parameters:
  // result_type - must be a f64 type.
  //
  [[nodiscard]] Instr_constant* make_instr_constant_f64(Allocator* allocator,
                                                        u32 id,
                                                        Instr* result_type,
                                                        f64 value);

  struct Instr_constant_composite: public Instr {
    Instr* result_type;
    Array<Instr*> constituents;

    Instr_constant_composite(u32 id, Instr* result_type, Allocator* allocator)
      : Instr(Instr_Kind::e_constant_composite, id), result_type(result_type),
        constituents(allocator)
    {
    }

    Instr_constant_composite(u32 id, Instr* result_type, Allocator* allocator,
                             anton::Slice<Instr* const> constituents)
      : Instr(Instr_Kind::e_constant_composite, id), result_type(result_type),
        constituents(allocator, anton::range_construct, constituents.begin(),
                     constituents.end())
    {
    }
  };

  struct Instr_variable: public Instr {
    Instr* result_type;
    Storage_Class storage_class;

    Instr_variable(u32 id, Instr* result_type, Storage_Class storage_class)
      : Instr(Instr_Kind::e_variable, id), result_type(result_type),
        storage_class(storage_class)
    {
    }
  };

  [[nodiscard]] Instr_variable*
  make_instr_variable(Allocator* allocator, u32 id, Instr* result_type,
                      Storage_Class storage_class);

  struct Instr_load: public Instr {
    Instr* result_type;
    Instr* pointer;

    Instr_load(u32 id, Instr* result_type, Instr* pointer)
      : Instr(Instr_Kind::e_load, id), result_type(result_type),
        pointer(pointer)
    {
    }
  };

  [[nodiscard]] Instr_load* make_instr_load(Allocator* allocator, u32 id,
                                            Instr* result_type, Instr* pointer);

  struct Instr_store: public Instr {
    Instr* pointer;
    Instr* object;

    Instr_store(Instr* pointer, Instr* object)
      : Instr(Instr_Kind::e_store, 0), pointer(pointer), object(object)
    {
    }
  };

  [[nodiscard]] Instr_store* make_instr_store(Allocator* allocator,
                                              Instr* pointer, Instr* object);

  struct Instr_access_chain: public Instr {
    Instr* result_type;
    Instr* base;
    Array<Instr*> indices;

    Instr_access_chain(u32 id, Instr* result_type, Instr* base,
                       Allocator* allocator)
      : Instr(Instr_Kind::e_access_chain, id), result_type(result_type),
        base(base), indices(allocator)
    {
    }

    Instr_access_chain(u32 id, Instr* result_type, Instr* base,
                       Allocator* allocator, anton::Slice<Instr* const> indices)
      : Instr(Instr_Kind::e_access_chain, id), result_type(result_type),
        base(base), indices(allocator, anton::range_construct, indices.begin(),
                            indices.end())
    {
    }
  };

  [[nodiscard]] Instr_access_chain*
  make_instr_access_chain(Allocator* allocator, u32 id, Instr* result_type,
                          Instr* base, anton::Slice<Instr* const> indices);

  template<typename... Ts>
  [[nodiscard]] Instr_access_chain*
  make_instr_access_chain(Allocator* allocator, u32 id, Instr* result_type,
                          Instr* base, Ts*... indices)
  {
    Instr* array[sizeof...(indices)] = {indices...};
    return make_instr_access_chain(allocator, id, result_type, base,
                                   anton::Slice{array});
  }

  struct Instr_function: public Instr {
    Instr_type_function* function_type;

    Instr_function(u32 id, Instr_type_function* function_type)
      : Instr(Instr_Kind::e_function, id), function_type(function_type)
    {
    }
  };

  [[nodiscard]] Instr_function*
  make_instr_function(Allocator* allocator, u32 id,
                      Instr_type_function* function_type);

  TYPED_INSTR(function_parameter, e_function_parameter);
  NOTHING_INSTR(function_end, e_function_end);

  // Instr_function_call
  //
  // We do not store the result_type in this structure as it is the exact same
  // type as the return type of function.
  //
  struct Instr_function_call: public Instr {
    Instr_function* function;
    Array<Instr*> arguments;

    Instr_function_call(u32 id, Instr_function* function, Allocator* allocator)
      : Instr(Instr_Kind::e_function_call, id), function(function),
        arguments(allocator)
    {
    }

    Instr_function_call(u32 id, Instr_function* function, Allocator* allocator,
                        anton::Slice<Instr* const> arguments)
      : Instr(Instr_Kind::e_function_call, id), function(function),
        arguments(allocator, anton::range_construct, arguments.begin(),
                  arguments.end())
    {
    }
  };

  UNARY_INSTR(Instr_convert_f2u, e_convert_f2u);
  UNARY_INSTR(Instr_convert_f2s, e_convert_f2s);
  UNARY_INSTR(Instr_convert_s2f, e_convert_s2f);
  UNARY_INSTR(Instr_convert_u2f, e_convert_u2f);
  UNARY_INSTR(Instr_uconvert, e_uconvert);
  UNARY_INSTR(Instr_sconvert, e_sconvert);
  UNARY_INSTR(Instr_fconvert, e_fconvert);
  UNARY_INSTR(Instr_convert_ptr2u, e_convert_ptr2u);
  UNARY_INSTR(Instr_convert_u2ptr, e_convert_u2ptr);

  struct Instr_composite_construct: public Instr {
    Instr* result_type;
    Array<Instr*> constituents;

    Instr_composite_construct(u32 id, Instr* result_type, Allocator* allocator)
      : Instr(Instr_Kind::e_composite_construct, id), result_type(result_type),
        constituents(allocator)
    {
    }

    Instr_composite_construct(u32 id, Instr* result_type, Allocator* allocator,
                              anton::Slice<Instr* const> constituents)
      : Instr(Instr_Kind::e_composite_construct, id), result_type(result_type),
        constituents(allocator, anton::range_construct, constituents.begin(),
                     constituents.end())
    {
    }
  };

  [[nodiscard]] Instr_composite_construct*
  make_instr_composite_construct(Allocator* allocator, u32 id,
                                 Instr* result_type);

  struct Instr_composite_extract: public Instr {
    Instr* result_type;
    Instr* composite;
    Array<u32> indices;

    Instr_composite_extract(u32 id, Instr* result_type, Instr* composite,
                            Allocator* allocator)
      : Instr(Instr_Kind::e_composite_extract, id), result_type(result_type),
        composite(composite), indices(allocator)
    {
    }

    Instr_composite_extract(u32 id, Instr* result_type, Instr* composite,
                            Allocator* allocator,
                            anton::Slice<u32 const> indices)
      : Instr(Instr_Kind::e_composite_extract, id), result_type(result_type),
        composite(composite), indices(allocator, anton::range_construct,
                                      indices.begin(), indices.end())
    {
    }
  };

  [[nodiscard]] Instr_composite_extract*
  make_instr_composite_extract(Allocator* allocator, u32 id, Instr* result_type,
                               Instr* composite);

  struct Instr_composite_insert: public Instr {
    Instr* result_type;
    Instr* composite;
    Instr* object;
    Array<u32> indices;

    Instr_composite_insert(u32 id, Instr* result_type, Instr* composite,
                           Instr* object, Allocator* allocator)
      : Instr(Instr_Kind::e_composite_insert, id), result_type(result_type),
        composite(composite), object(object), indices(allocator)
    {
    }

    Instr_composite_insert(u32 id, Instr* result_type, Instr* composite,
                           Instr* object, Allocator* allocator,
                           anton::Slice<u32 const> indices)
      : Instr(Instr_Kind::e_composite_insert, id), result_type(result_type),
        composite(composite), object(object),
        indices(allocator, anton::range_construct, indices.begin(),
                indices.end())
    {
    }
  };

  [[nodiscard]] Instr_composite_insert*
  make_instr_composite_insert(Allocator* allocator, u32 id, Instr* result_type,
                              Instr* composite, Instr* object);

  UNARY_INSTR(Instr_copy_object, e_copy_object);
  UNARY_INSTR(Instr_transpose, e_transpose);

  BINARY_INSTR(snegate, e_snegate);
  BINARY_INSTR(fnegate, e_fnegate);
  BINARY_INSTR(iadd, e_iadd);
  BINARY_INSTR(fadd, e_fadd);
  BINARY_INSTR(isub, e_isub);
  BINARY_INSTR(fsub, e_fsub);
  BINARY_INSTR(imul, e_imul);
  BINARY_INSTR(fmul, e_fmul);
  BINARY_INSTR(udiv, e_udiv);
  BINARY_INSTR(sdiv, e_sdiv);
  BINARY_INSTR(fdiv, e_fdiv);
  BINARY_INSTR(umod, e_umod);
  BINARY_INSTR(srem, e_srem);
  BINARY_INSTR(smod, e_smod);
  BINARY_INSTR(frem, e_frem);
  BINARY_INSTR(fmod, e_fmod);
  BINARY_INSTR(vec_times_scalar, e_vec_times_scalar);
  BINARY_INSTR(mat_times_scalar, e_mat_times_scalar);
  BINARY_INSTR(vec_times_mat, e_vec_times_mat);
  BINARY_INSTR(mat_times_vec, e_mat_times_vec);
  BINARY_INSTR(mat_times_mat, e_mat_times_mat);
  BINARY_INSTR(outer_product, e_outer_product);
  BINARY_INSTR(dot, e_dot);
  BINARY_INSTR(shr_logical, e_shr_logical);
  BINARY_INSTR(shr_arithmetic, e_shr_arithmetic);
  BINARY_INSTR(shl, e_shl); // ShiftLeftLogical
  BINARY_INSTR(bit_or, e_bit_or);
  BINARY_INSTR(bit_xor, e_bit_xor);
  BINARY_INSTR(bit_and, e_bit_and);
  BINARY_INSTR(bit_not, e_bit_not);
  BINARY_INSTR(logical_eq, e_logical_eq);
  BINARY_INSTR(logical_neq, e_logical_neq);
  BINARY_INSTR(logical_or, e_logical_or);
  BINARY_INSTR(logical_and, e_logical_and);
  BINARY_INSTR(logical_not, e_logical_not);

  struct Instr_select: public Instr {
    Instr* result_type;
    Instr* condition;
    Instr* operand1;
    Instr* operand2;

    Instr_select(u32 id, Instr* result_type, Instr* condition, Instr* operand1,
                 Instr* operand2)
      : Instr(Instr_Kind::e_select, id), result_type(result_type),
        condition(condition), operand1(operand1), operand2(operand2)
    {
    }
  };

  BINARY_INSTR(ieq, e_ieq); // IEqual
  BINARY_INSTR(ineq, e_ineq); // INotEqual
  BINARY_INSTR(ugt, e_ugt); // UGreaterThan
  BINARY_INSTR(sgt, e_sgt); // SGreaterThan
  BINARY_INSTR(uge, e_uge); // UGreaterThanEqual
  BINARY_INSTR(sge, e_sge); // SGreaterThanEqual
  BINARY_INSTR(ult, e_ult); // ULessThan
  BINARY_INSTR(slt, e_slt); // SLessThan
  BINARY_INSTR(ule, e_ule); // ULessThanEqual
  BINARY_INSTR(sle, e_sle); // SLessThanEqual
  BINARY_INSTR(foeq, e_foeq); // FOrdEqual
  BINARY_INSTR(fueq, e_fueq); // FUnordEqual
  BINARY_INSTR(foneq, e_foneq); // FOrdNotEqual
  BINARY_INSTR(funeq, e_funeq); // FUnordNotEqual
  BINARY_INSTR(folt, e_folt); // FOrdLessThan
  BINARY_INSTR(fult, e_fult); // FUnordLessThan
  BINARY_INSTR(fogt, e_fogt); // FOrdGreaterThan
  BINARY_INSTR(fugt, e_fugt); // FUnordGreaterThan
  BINARY_INSTR(fole, e_fole); // FOrdLessThanEqual
  BINARY_INSTR(fule, e_fule); // FUnordLessThanEqual
  BINARY_INSTR(foge, e_foge); // FOrdGreaterThanEqual
  BINARY_INSTR(fuge, e_fuge); // FUnordGreaterThanEqual

  UNARY_INSTR(Instr_dPdx, e_dPdx);
  UNARY_INSTR(Instr_dPdy, e_dPdy);
  UNARY_INSTR(Instr_fwidth, e_fwidth);
  UNARY_INSTR(Instr_dPdx_fine, e_dPdx_fine);
  UNARY_INSTR(Instr_dPdy_fine, e_dPdy_fine);
  UNARY_INSTR(Instr_fwidth_fine, e_fwidth_fine);
  UNARY_INSTR(Instr_dPdx_coarse, e_dPdx_coarse);
  UNARY_INSTR(Instr_dPdy_coarse, e_dPdy_coarse);
  UNARY_INSTR(Instr_fwidth_coarse, e_fwidth_coarse);

  struct Instr_phi: public Instr {
    Instr* result_type;
    Array<Instr*> operands;

    Instr_phi(u32 id, Instr* result_type, Allocator* allocator)
      : Instr(Instr_Kind::e_phi, id), result_type(result_type),
        operands(allocator)
    {
    }

    Instr_phi(u32 id, Instr* result_type, Allocator* allocator,
              anton::Slice<Instr* const> operands)
      : Instr(Instr_Kind::e_phi, id), result_type(result_type),
        operands(allocator, anton::range_construct, operands.begin(),
                 operands.end())
    {
    }
  };

  ID_INSTR(Instr_label, e_label);

  [[nodiscard]] Instr_label* make_instr_label(Allocator* allocator, u32 id);

  struct Instr_branch: public Instr {
    Instr_label* target;

    Instr_branch(Instr_label* target)
      : Instr(Instr_Kind::e_branch, 0), target(target)
    {
    }
  };

  [[nodiscard]] Instr_branch* make_instr_branch(Allocator* allocator,
                                                Instr_label* target);

  struct Instr_brcond: public Instr {
    Instr* condition;
    // TODO: Validate these labels are not the same.
    Instr_label* true_label;
    Instr_label* false_label;

    Instr_brcond(Instr* condition, Instr_label* true_label,
                 Instr_label* false_label)
      : Instr(Instr_Kind::e_brcond, 0), condition(condition),
        true_label(true_label), false_label(false_label)
    {
    }
  };

  [[nodiscard]] Instr_brcond* make_instr_brcond(Allocator* allocator,
                                                Instr* condition,
                                                Instr_label* true_label,
                                                Instr_label* false_label);

  struct Switch_Label {
    u64 literal;
    Instr_label* label;
  };

  struct Instr_switch: public Instr {
    Instr* selector;
    Instr_label* default_label;
    Array<Switch_Label> labels;

    Instr_switch(Instr* selector, Instr_label* default_label,
                 Allocator* allocator)
      : Instr(Instr_Kind::e_switch, 0), selector(selector),
        default_label(default_label), labels(allocator)
    {
    }
  };

  [[nodiscard]] Instr_switch* make_instr_switch(Allocator* allocator,
                                                Instr* selector,
                                                Instr_label* default_label);

  NOTHING_INSTR(return, e_return);

  struct Instr_return_value: public Instr {
    Instr* value;

    Instr_return_value(Instr* value)
      : Instr(Instr_Kind::e_return_value, 0), value(value)
    {
    }
  };

  [[nodiscard]] Instr_return_value*
  make_instr_return_value(Allocator* allocator, Instr* value);

  NOTHING_INSTR(terminate, e_terminate);
  NOTHING_INSTR(unreachable, e_unreachable);

#undef UNARY_INSTR
#undef BINARY_INSTR
#undef TYPED_INSTR
#undef ID_INSTR
#undef NOTHING_INSTR
} // namespace vush::spirv
