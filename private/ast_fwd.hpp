#pragma once

#include <anton/array.hpp>
#include <owning_ptr.hpp>

namespace vush {
    enum struct AST_Node_Type;
    struct AST_Node;
    struct Declaration;
    struct Statement;

    using Declaration_List = anton::Array<Owning_Ptr<Declaration>>;
    using Statement_List = anton::Array<Owning_Ptr<Statement>>;
} // namespace vush
