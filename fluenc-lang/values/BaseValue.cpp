#include "BaseValue.h"
#include "Utility.h"

const BaseValue *BaseValue::forward(size_t id) const
{
    UNUSED(id);

    return this;
}
