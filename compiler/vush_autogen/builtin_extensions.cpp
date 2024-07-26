// This file has been autogenerated.
// Do not modify manually.
//

#include <vush_ast/ast.hpp>
#include <vush_ir/ir.hpp>

namespace vush {
  ir::Instr_ext_call* select_ext(Allocator* const allocator, i64 const id,
                                 ir::Type* const type,
                                 ast::Expr_Call const* const expr)
  {
    anton::String_View const identifier = expr->identifier.value;
    bool const result_is_fp = ast::is_fp_based(*expr->evaluated_type);
    bool const result_is_sint =
      ast::is_signed_integer_based(*expr->evaluated_type);
    switch(anton::hash(identifier)) {
    case anton::hash("radians"):
      return ir::make_instr_ext_call(allocator, id, ir::Ext_Kind::e_radians,
                                     type, expr->source_info);
    case anton::hash("degrees"):
      return ir::make_instr_ext_call(allocator, id, ir::Ext_Kind::e_degrees,
                                     type, expr->source_info);
    case anton::hash("sin"):
      return ir::make_instr_ext_call(allocator, id, ir::Ext_Kind::e_sin, type,
                                     expr->source_info);
    case anton::hash("cos"):
      return ir::make_instr_ext_call(allocator, id, ir::Ext_Kind::e_cos, type,
                                     expr->source_info);
    case anton::hash("tan"):
      return ir::make_instr_ext_call(allocator, id, ir::Ext_Kind::e_tan, type,
                                     expr->source_info);
    case anton::hash("asin"):
      return ir::make_instr_ext_call(allocator, id, ir::Ext_Kind::e_asin, type,
                                     expr->source_info);
    case anton::hash("acos"):
      return ir::make_instr_ext_call(allocator, id, ir::Ext_Kind::e_acos, type,
                                     expr->source_info);
    case anton::hash("atan"):
      return ir::make_instr_ext_call(allocator, id, ir::Ext_Kind::e_atan, type,
                                     expr->source_info);
    case anton::hash("sinh"):
      return ir::make_instr_ext_call(allocator, id, ir::Ext_Kind::e_sinh, type,
                                     expr->source_info);
    case anton::hash("cosh"):
      return ir::make_instr_ext_call(allocator, id, ir::Ext_Kind::e_cosh, type,
                                     expr->source_info);
    case anton::hash("tanh"):
      return ir::make_instr_ext_call(allocator, id, ir::Ext_Kind::e_tanh, type,
                                     expr->source_info);
    case anton::hash("asinh"):
      return ir::make_instr_ext_call(allocator, id, ir::Ext_Kind::e_asinh, type,
                                     expr->source_info);
    case anton::hash("acosh"):
      return ir::make_instr_ext_call(allocator, id, ir::Ext_Kind::e_acosh, type,
                                     expr->source_info);
    case anton::hash("atanh"):
      return ir::make_instr_ext_call(allocator, id, ir::Ext_Kind::e_atanh, type,
                                     expr->source_info);
    case anton::hash("pow"):
      return ir::make_instr_ext_call(allocator, id, ir::Ext_Kind::e_pow, type,
                                     expr->source_info);
    case anton::hash("exp"):
      return ir::make_instr_ext_call(allocator, id, ir::Ext_Kind::e_exp, type,
                                     expr->source_info);
    case anton::hash("log"):
      return ir::make_instr_ext_call(allocator, id, ir::Ext_Kind::e_log, type,
                                     expr->source_info);
    case anton::hash("exp2"):
      return ir::make_instr_ext_call(allocator, id, ir::Ext_Kind::e_exp2, type,
                                     expr->source_info);
    case anton::hash("log2"):
      return ir::make_instr_ext_call(allocator, id, ir::Ext_Kind::e_log2, type,
                                     expr->source_info);
    case anton::hash("sqrt"):
      return ir::make_instr_ext_call(allocator, id, ir::Ext_Kind::e_sqrt, type,
                                     expr->source_info);
    case anton::hash("inv_sqrt"):
      return ir::make_instr_ext_call(allocator, id, ir::Ext_Kind::e_inv_sqrt,
                                     type, expr->source_info);
    case anton::hash("abs"):
      if(result_is_fp) {
        return ir::make_instr_ext_call(allocator, id, ir::Ext_Kind::e_fabs,
                                       type, expr->source_info);
      } else if(result_is_sint) {
        return ir::make_instr_ext_call(allocator, id, ir::Ext_Kind::e_iabs,
                                       type, expr->source_info);
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    case anton::hash("sign"):
      if(result_is_fp) {
        return ir::make_instr_ext_call(allocator, id, ir::Ext_Kind::e_fsign,
                                       type, expr->source_info);
      } else if(result_is_sint) {
        return ir::make_instr_ext_call(allocator, id, ir::Ext_Kind::e_isign,
                                       type, expr->source_info);
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    case anton::hash("floor"):
      return ir::make_instr_ext_call(allocator, id, ir::Ext_Kind::e_floor, type,
                                     expr->source_info);
    case anton::hash("trunc"):
      return ir::make_instr_ext_call(allocator, id, ir::Ext_Kind::e_trunc, type,
                                     expr->source_info);
    case anton::hash("round"):
      return ir::make_instr_ext_call(allocator, id, ir::Ext_Kind::e_round, type,
                                     expr->source_info);
    case anton::hash("round_even"):
      return ir::make_instr_ext_call(allocator, id, ir::Ext_Kind::e_round_even,
                                     type, expr->source_info);
    case anton::hash("ceil"):
      return ir::make_instr_ext_call(allocator, id, ir::Ext_Kind::e_ceil, type,
                                     expr->source_info);
    case anton::hash("fract"):
      return ir::make_instr_ext_call(allocator, id, ir::Ext_Kind::e_fract, type,
                                     expr->source_info);
    case anton::hash("mod"):
      return ir::make_instr_ext_call(allocator, id, ir::Ext_Kind::e_fmod, type,
                                     expr->source_info);
    case anton::hash("min"):
      if(result_is_fp) {
        return ir::make_instr_ext_call(allocator, id, ir::Ext_Kind::e_fmin,
                                       type, expr->source_info);
      } else if(result_is_sint) {
        return ir::make_instr_ext_call(allocator, id, ir::Ext_Kind::e_imin,
                                       type, expr->source_info);
      } else {
        return ir::make_instr_ext_call(allocator, id, ir::Ext_Kind::e_umin,
                                       type, expr->source_info);
      }
    case anton::hash("max"):
      if(result_is_fp) {
        return ir::make_instr_ext_call(allocator, id, ir::Ext_Kind::e_fmax,
                                       type, expr->source_info);
      } else if(result_is_sint) {
        return ir::make_instr_ext_call(allocator, id, ir::Ext_Kind::e_imax,
                                       type, expr->source_info);
      } else {
        return ir::make_instr_ext_call(allocator, id, ir::Ext_Kind::e_umax,
                                       type, expr->source_info);
      }
    case anton::hash("clamp"):
      if(result_is_fp) {
        return ir::make_instr_ext_call(allocator, id, ir::Ext_Kind::e_fclamp,
                                       type, expr->source_info);
      } else if(result_is_sint) {
        return ir::make_instr_ext_call(allocator, id, ir::Ext_Kind::e_iclamp,
                                       type, expr->source_info);
      } else {
        return ir::make_instr_ext_call(allocator, id, ir::Ext_Kind::e_uclamp,
                                       type, expr->source_info);
      }
    case anton::hash("length"):
      return ir::make_instr_ext_call(allocator, id, ir::Ext_Kind::e_length,
                                     type, expr->source_info);
    case anton::hash("distance"):
      return ir::make_instr_ext_call(allocator, id, ir::Ext_Kind::e_distance,
                                     type, expr->source_info);
    case anton::hash("dot"):
      return ir::make_instr_ext_call(allocator, id, ir::Ext_Kind::e_dot, type,
                                     expr->source_info);
    case anton::hash("cross"):
      return ir::make_instr_ext_call(allocator, id, ir::Ext_Kind::e_cross, type,
                                     expr->source_info);
    case anton::hash("normalize"):
      return ir::make_instr_ext_call(allocator, id, ir::Ext_Kind::e_normalize,
                                     type, expr->source_info);
    case anton::hash("faceforward"):
      return ir::make_instr_ext_call(allocator, id, ir::Ext_Kind::e_faceforward,
                                     type, expr->source_info);
    case anton::hash("reflect"):
      return ir::make_instr_ext_call(allocator, id, ir::Ext_Kind::e_reflect,
                                     type, expr->source_info);
    case anton::hash("matrix_comp_mult"):
      return ir::make_instr_ext_call(
        allocator, id, ir::Ext_Kind::e_mat_comp_mult, type, expr->source_info);
    case anton::hash("outer_product"):
      return ir::make_instr_ext_call(
        allocator, id, ir::Ext_Kind::e_outer_product, type, expr->source_info);
    case anton::hash("transpose"):
      return ir::make_instr_ext_call(allocator, id, ir::Ext_Kind::e_transpose,
                                     type, expr->source_info);
    case anton::hash("determinant"):
      return ir::make_instr_ext_call(allocator, id, ir::Ext_Kind::e_mat_det,
                                     type, expr->source_info);
    case anton::hash("inverse"):
      return ir::make_instr_ext_call(allocator, id, ir::Ext_Kind::e_mat_inv,
                                     type, expr->source_info);
    case anton::hash("less_than"):
      if(result_is_fp) {
        ANTON_UNREACHABLE("no fp ext");
      } else if(result_is_sint) {
        ANTON_UNREACHABLE("no sint ext");
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    case anton::hash("less_than_equal"):
      if(result_is_fp) {
        ANTON_UNREACHABLE("no fp ext");
      } else if(result_is_sint) {
        ANTON_UNREACHABLE("no sint ext");
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    case anton::hash("greater_than"):
      if(result_is_fp) {
        ANTON_UNREACHABLE("no fp ext");
      } else if(result_is_sint) {
        ANTON_UNREACHABLE("no sint ext");
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    case anton::hash("greater_than_equal"):
      if(result_is_fp) {
        ANTON_UNREACHABLE("no fp ext");
      } else if(result_is_sint) {
        ANTON_UNREACHABLE("no sint ext");
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    case anton::hash("equal"):
      if(result_is_fp) {
        ANTON_UNREACHABLE("no fp ext");
      } else if(result_is_sint) {
        ANTON_UNREACHABLE("no sint ext");
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    case anton::hash("not_equal"):
      if(result_is_fp) {
        ANTON_UNREACHABLE("no fp ext");
      } else if(result_is_sint) {
        ANTON_UNREACHABLE("no sint ext");
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    case anton::hash("any"):
      if(result_is_fp) {
        ANTON_UNREACHABLE("no fp ext");
      } else if(result_is_sint) {
        ANTON_UNREACHABLE("no sint ext");
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    case anton::hash("all"):
      if(result_is_fp) {
        ANTON_UNREACHABLE("no fp ext");
      } else if(result_is_sint) {
        ANTON_UNREACHABLE("no sint ext");
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    case anton::hash("not"):
      if(result_is_fp) {
        ANTON_UNREACHABLE("no fp ext");
      } else if(result_is_sint) {
        ANTON_UNREACHABLE("no sint ext");
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    case anton::hash("uadd_carry"):
      if(result_is_fp) {
        ANTON_UNREACHABLE("no fp ext");
      } else if(result_is_sint) {
        ANTON_UNREACHABLE("no sint ext");
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    case anton::hash("usub_borrow"):
      if(result_is_fp) {
        ANTON_UNREACHABLE("no fp ext");
      } else if(result_is_sint) {
        ANTON_UNREACHABLE("no sint ext");
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    case anton::hash("umul_extended"):
      if(result_is_fp) {
        ANTON_UNREACHABLE("no fp ext");
      } else if(result_is_sint) {
        ANTON_UNREACHABLE("no sint ext");
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    case anton::hash("imul_extended"):
      if(result_is_fp) {
        ANTON_UNREACHABLE("no fp ext");
      } else if(result_is_sint) {
        ANTON_UNREACHABLE("no sint ext");
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    case anton::hash("bitfield_extract"):
      if(result_is_fp) {
        ANTON_UNREACHABLE("no fp ext");
      } else if(result_is_sint) {
        ANTON_UNREACHABLE("no sint ext");
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    case anton::hash("bitfield_insert"):
      if(result_is_fp) {
        ANTON_UNREACHABLE("no fp ext");
      } else if(result_is_sint) {
        ANTON_UNREACHABLE("no sint ext");
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    case anton::hash("bitfield_reverse"):
      if(result_is_fp) {
        ANTON_UNREACHABLE("no fp ext");
      } else if(result_is_sint) {
        ANTON_UNREACHABLE("no sint ext");
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    case anton::hash("bit_count"):
      if(result_is_fp) {
        ANTON_UNREACHABLE("no fp ext");
      } else if(result_is_sint) {
        ANTON_UNREACHABLE("no sint ext");
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    case anton::hash("find_LSB"):
      if(result_is_fp) {
        ANTON_UNREACHABLE("no fp ext");
      } else if(result_is_sint) {
        ANTON_UNREACHABLE("no sint ext");
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    case anton::hash("find_MSB"):
      if(result_is_fp) {
        ANTON_UNREACHABLE("no fp ext");
      } else if(result_is_sint) {
        ANTON_UNREACHABLE("no sint ext");
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    case anton::hash("texture_size"):
      return ir::make_instr_ext_call(allocator, id, ir::Ext_Kind::e_tex_size,
                                     type, expr->source_info);
    case anton::hash("texture_query_lod"):
      return ir::make_instr_ext_call(
        allocator, id, ir::Ext_Kind::e_tex_query_lod, type, expr->source_info);
    case anton::hash("texture_query_levels"):
      return ir::make_instr_ext_call(allocator, id,
                                     ir::Ext_Kind::e_tex_query_levels, type,
                                     expr->source_info);
    case anton::hash("texture_samples"):
      return ir::make_instr_ext_call(allocator, id, ir::Ext_Kind::e_tex_samples,
                                     type, expr->source_info);
    case anton::hash("texture"):
      return ir::make_instr_ext_call(allocator, id, ir::Ext_Kind::e_tex, type,
                                     expr->source_info);
    case anton::hash("texture_proj"):
      return ir::make_instr_ext_call(allocator, id, ir::Ext_Kind::e_tex_proj,
                                     type, expr->source_info);
    case anton::hash("texture_lod"):
      return ir::make_instr_ext_call(allocator, id, ir::Ext_Kind::e_tex_lod,
                                     type, expr->source_info);
    case anton::hash("texture_offset"):
      return ir::make_instr_ext_call(allocator, id, ir::Ext_Kind::e_tex_off,
                                     type, expr->source_info);
    case anton::hash("texel_fetch"):
      return ir::make_instr_ext_call(allocator, id, ir::Ext_Kind::e_texel_fetch,
                                     type, expr->source_info);
    case anton::hash("texel_fetch_offset"):
      return ir::make_instr_ext_call(allocator, id,
                                     ir::Ext_Kind::e_texel_fetch_off, type,
                                     expr->source_info);
    case anton::hash("texture_proj_offset"):
      return ir::make_instr_ext_call(
        allocator, id, ir::Ext_Kind::e_tex_proj_off, type, expr->source_info);
    case anton::hash("texture_lod_offset"):
      return ir::make_instr_ext_call(allocator, id, ir::Ext_Kind::e_tex_lod_off,
                                     type, expr->source_info);
    case anton::hash("texture_proj_lod"):
      return ir::make_instr_ext_call(
        allocator, id, ir::Ext_Kind::e_tex_proj_lod, type, expr->source_info);
    case anton::hash("texture_proj_lod_offset"):
      return ir::make_instr_ext_call(allocator, id,
                                     ir::Ext_Kind::e_tex_proj_lod_off, type,
                                     expr->source_info);
    case anton::hash("texture_grad"):
      return ir::make_instr_ext_call(allocator, id, ir::Ext_Kind::e_tex_grad,
                                     type, expr->source_info);
    case anton::hash("texture_grad_offset"):
      return ir::make_instr_ext_call(
        allocator, id, ir::Ext_Kind::e_tex_grad_off, type, expr->source_info);
    case anton::hash("texture_proj_grad"):
      return ir::make_instr_ext_call(
        allocator, id, ir::Ext_Kind::e_tex_proj_grad, type, expr->source_info);
    case anton::hash("texture_proj_grad_offset"):
      return ir::make_instr_ext_call(allocator, id,
                                     ir::Ext_Kind::e_tex_proj_grad_off, type,
                                     expr->source_info);
    case anton::hash("texture_gather"):
      return ir::make_instr_ext_call(allocator, id, ir::Ext_Kind::e_tex_gather,
                                     type, expr->source_info);
    case anton::hash("texture_gather_offset"):
      return ir::make_instr_ext_call(
        allocator, id, ir::Ext_Kind::e_tex_gather_off, type, expr->source_info);
    case anton::hash("texture_gather_offsets"):
      return ir::make_instr_ext_call(allocator, id,
                                     ir::Ext_Kind::e_tex_gather_offs, type,
                                     expr->source_info);
    case anton::hash("atomic_add"):
      if(result_is_fp) {
        ANTON_UNREACHABLE("no fp ext");
      } else if(result_is_sint) {
        ANTON_UNREACHABLE("no sint ext");
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    case anton::hash("atomic_min"):
      if(result_is_fp) {
        ANTON_UNREACHABLE("no fp ext");
      } else if(result_is_sint) {
        ANTON_UNREACHABLE("no sint ext");
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    case anton::hash("atomic_max"):
      if(result_is_fp) {
        ANTON_UNREACHABLE("no fp ext");
      } else if(result_is_sint) {
        ANTON_UNREACHABLE("no sint ext");
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    case anton::hash("atomic_and"):
      if(result_is_fp) {
        ANTON_UNREACHABLE("no fp ext");
      } else if(result_is_sint) {
        ANTON_UNREACHABLE("no sint ext");
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    case anton::hash("atomic_or"):
      if(result_is_fp) {
        ANTON_UNREACHABLE("no fp ext");
      } else if(result_is_sint) {
        ANTON_UNREACHABLE("no sint ext");
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    case anton::hash("atomic_xor"):
      if(result_is_fp) {
        ANTON_UNREACHABLE("no fp ext");
      } else if(result_is_sint) {
        ANTON_UNREACHABLE("no sint ext");
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    case anton::hash("atomic_exchange"):
      if(result_is_fp) {
        ANTON_UNREACHABLE("no fp ext");
      } else if(result_is_sint) {
        ANTON_UNREACHABLE("no sint ext");
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    case anton::hash("atomic_comp_swap"):
      if(result_is_fp) {
        ANTON_UNREACHABLE("no fp ext");
      } else if(result_is_sint) {
        ANTON_UNREACHABLE("no sint ext");
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    case anton::hash("image_size"):
      if(result_is_fp) {
        ANTON_UNREACHABLE("no fp ext");
      } else if(result_is_sint) {
        ANTON_UNREACHABLE("no sint ext");
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    case anton::hash("image_samples"):
      if(result_is_fp) {
        ANTON_UNREACHABLE("no fp ext");
      } else if(result_is_sint) {
        ANTON_UNREACHABLE("no sint ext");
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    case anton::hash("image_load"):
      if(result_is_fp) {
        ANTON_UNREACHABLE("no fp ext");
      } else if(result_is_sint) {
        ANTON_UNREACHABLE("no sint ext");
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    case anton::hash("image_store"):
      if(result_is_fp) {
        ANTON_UNREACHABLE("no fp ext");
      } else if(result_is_sint) {
        ANTON_UNREACHABLE("no sint ext");
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    case anton::hash("image_atomic_add"):
      if(result_is_fp) {
        ANTON_UNREACHABLE("no fp ext");
      } else if(result_is_sint) {
        ANTON_UNREACHABLE("no sint ext");
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    case anton::hash("image_atomic_min"):
      if(result_is_fp) {
        ANTON_UNREACHABLE("no fp ext");
      } else if(result_is_sint) {
        ANTON_UNREACHABLE("no sint ext");
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    case anton::hash("image_atomic_max"):
      if(result_is_fp) {
        ANTON_UNREACHABLE("no fp ext");
      } else if(result_is_sint) {
        ANTON_UNREACHABLE("no sint ext");
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    case anton::hash("image_atomic_and"):
      if(result_is_fp) {
        ANTON_UNREACHABLE("no fp ext");
      } else if(result_is_sint) {
        ANTON_UNREACHABLE("no sint ext");
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    case anton::hash("image_atomic_or"):
      if(result_is_fp) {
        ANTON_UNREACHABLE("no fp ext");
      } else if(result_is_sint) {
        ANTON_UNREACHABLE("no sint ext");
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    case anton::hash("image_atomic_xor"):
      if(result_is_fp) {
        ANTON_UNREACHABLE("no fp ext");
      } else if(result_is_sint) {
        ANTON_UNREACHABLE("no sint ext");
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    case anton::hash("image_atomic_exchange"):
      if(result_is_fp) {
        ANTON_UNREACHABLE("no fp ext");
      } else if(result_is_sint) {
        ANTON_UNREACHABLE("no sint ext");
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    case anton::hash("image_atomic_comp_swap"):
      if(result_is_fp) {
        ANTON_UNREACHABLE("no fp ext");
      } else if(result_is_sint) {
        ANTON_UNREACHABLE("no sint ext");
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    case anton::hash("dFdx"):
      if(result_is_fp) {
        ANTON_UNREACHABLE("no fp ext");
      } else if(result_is_sint) {
        ANTON_UNREACHABLE("no sint ext");
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    case anton::hash("dFdy"):
      if(result_is_fp) {
        ANTON_UNREACHABLE("no fp ext");
      } else if(result_is_sint) {
        ANTON_UNREACHABLE("no sint ext");
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    case anton::hash("dFdx_fine"):
      if(result_is_fp) {
        ANTON_UNREACHABLE("no fp ext");
      } else if(result_is_sint) {
        ANTON_UNREACHABLE("no sint ext");
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    case anton::hash("dFdy_fine"):
      if(result_is_fp) {
        ANTON_UNREACHABLE("no fp ext");
      } else if(result_is_sint) {
        ANTON_UNREACHABLE("no sint ext");
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    case anton::hash("dFdx_coarse"):
      if(result_is_fp) {
        ANTON_UNREACHABLE("no fp ext");
      } else if(result_is_sint) {
        ANTON_UNREACHABLE("no sint ext");
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    case anton::hash("dFdy_coarse"):
      if(result_is_fp) {
        ANTON_UNREACHABLE("no fp ext");
      } else if(result_is_sint) {
        ANTON_UNREACHABLE("no sint ext");
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    case anton::hash("fwidth"):
      if(result_is_fp) {
        ANTON_UNREACHABLE("no fp ext");
      } else if(result_is_sint) {
        ANTON_UNREACHABLE("no sint ext");
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    case anton::hash("fwidth_fine"):
      if(result_is_fp) {
        ANTON_UNREACHABLE("no fp ext");
      } else if(result_is_sint) {
        ANTON_UNREACHABLE("no sint ext");
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    case anton::hash("fwidth_coarse"):
      if(result_is_fp) {
        ANTON_UNREACHABLE("no fp ext");
      } else if(result_is_sint) {
        ANTON_UNREACHABLE("no sint ext");
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    case anton::hash("interpolate_at_centroid"):
      if(result_is_fp) {
        ANTON_UNREACHABLE("no fp ext");
      } else if(result_is_sint) {
        ANTON_UNREACHABLE("no sint ext");
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    case anton::hash("interpolate_at_sample"):
      if(result_is_fp) {
        ANTON_UNREACHABLE("no fp ext");
      } else if(result_is_sint) {
        ANTON_UNREACHABLE("no sint ext");
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    case anton::hash("interpolate_at_offset"):
      if(result_is_fp) {
        ANTON_UNREACHABLE("no fp ext");
      } else if(result_is_sint) {
        ANTON_UNREACHABLE("no sint ext");
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    case anton::hash("barrier"):
      if(result_is_fp) {
        ANTON_UNREACHABLE("no fp ext");
      } else if(result_is_sint) {
        ANTON_UNREACHABLE("no sint ext");
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    case anton::hash("memory_barrier"):
      if(result_is_fp) {
        ANTON_UNREACHABLE("no fp ext");
      } else if(result_is_sint) {
        ANTON_UNREACHABLE("no sint ext");
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    case anton::hash("memory_barrier_atomic_counter"):
      if(result_is_fp) {
        ANTON_UNREACHABLE("no fp ext");
      } else if(result_is_sint) {
        ANTON_UNREACHABLE("no sint ext");
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    case anton::hash("memory_barrier_buffer"):
      if(result_is_fp) {
        ANTON_UNREACHABLE("no fp ext");
      } else if(result_is_sint) {
        ANTON_UNREACHABLE("no sint ext");
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    case anton::hash("memory_barrier_shared"):
      if(result_is_fp) {
        ANTON_UNREACHABLE("no fp ext");
      } else if(result_is_sint) {
        ANTON_UNREACHABLE("no sint ext");
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    case anton::hash("memory_barrier_image"):
      if(result_is_fp) {
        ANTON_UNREACHABLE("no fp ext");
      } else if(result_is_sint) {
        ANTON_UNREACHABLE("no sint ext");
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    case anton::hash("group_memory_barrier"):
      if(result_is_fp) {
        ANTON_UNREACHABLE("no fp ext");
      } else if(result_is_sint) {
        ANTON_UNREACHABLE("no sint ext");
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    case anton::hash("subpass_load"):
      if(result_is_fp) {
        ANTON_UNREACHABLE("no fp ext");
      } else if(result_is_sint) {
        ANTON_UNREACHABLE("no sint ext");
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    case anton::hash("any_invocation"):
      if(result_is_fp) {
        ANTON_UNREACHABLE("no fp ext");
      } else if(result_is_sint) {
        ANTON_UNREACHABLE("no sint ext");
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    case anton::hash("all_invocations"):
      if(result_is_fp) {
        ANTON_UNREACHABLE("no fp ext");
      } else if(result_is_sint) {
        ANTON_UNREACHABLE("no sint ext");
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    case anton::hash("all_invocations_equal"):
      if(result_is_fp) {
        ANTON_UNREACHABLE("no fp ext");
      } else if(result_is_sint) {
        ANTON_UNREACHABLE("no sint ext");
      } else {
        ANTON_UNREACHABLE("no uint ext");
      }
    default:
      ANTON_UNREACHABLE("incorrect builtin name");
    }
  }
} // namespace vush
