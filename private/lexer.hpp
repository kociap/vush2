#pragma once

#include <anton/expected.hpp>
#include <anton/iterators/zip.hpp>
#include <anton/string7_view.hpp>
#include <diagnostics.hpp>
#include <vush/types.hpp>

namespace vush {
  enum struct Token_Kind {
    identifier,
    comment,
    whitespace,
    // keywords
    kw_if,
    kw_else,
    kw_switch,
    kw_default,
    kw_for,
    kw_while,
    kw_do,
    kw_return,
    kw_break,
    kw_continue,
    kw_discard,
    kw_from,
    kw_struct,
    kw_import,
    kw_var,
    kw_mut,
    kw_settings,
    kw_reinterpret,
    // separators
    tk_brace_open,
    tk_brace_close,
    tk_lbracket,
    tk_rbracket,
    tk_lparen,
    tk_rparen,
    tk_langle,
    tk_rangle,
    tk_semicolon,
    tk_colon,
    tk_comma,
    tk_dot,
    tk_double_quote,
    tk_at,
    tk_plus,
    tk_minus,
    tk_asterisk,
    tk_slash,
    tk_percent,
    tk_amp,
    tk_pipe,
    tk_hat,
    tk_bang,
    tk_tilde,
    tk_equals,
    // literals
    lt_bin_integer,
    lt_dec_integer,
    lt_hex_integer,
    lt_float,
    lt_string,
    lt_bool,
  };

  struct Token {
    Token_Kind type;
    anton::String7_View value;
  };

  struct Token_Source_Info {
    i64 offset;
    i64 line;
    i64 column;
    i64 end_offset;
    i64 end_line;
    i64 end_column;
  };

  struct Lexed_Source {
  public:
    Array<Token> tokens;
    Array<Token_Source_Info> token_sources;

  public:
    Lexed_Source(Array<Token> tokens, Array<Token_Source_Info> token_sources);

    using token_iterator = anton::Zip_Iterator<Token*, Token_Source_Info*>;
    using const_token_iterator = anton::Zip_Iterator<Token const*, Token_Source_Info const*>;

    token_iterator begin();
    token_iterator end();
    const_token_iterator begin() const;
    const_token_iterator end() const;
    const_token_iterator cbegin() const;
    const_token_iterator cend() const;
  };

  [[nodiscard]] anton::Expected<Lexed_Source, Error>
  lex_source(Context const& ctx, anton::String_View source_path, anton::String7_View source);
} // namespace vush
