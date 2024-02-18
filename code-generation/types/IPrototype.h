#pragma once

#include "PrototypeField.h"

#include <vector>

class EntryPoint;
class Emitter;

namespace fluenc
{
    struct struct_node;
}

std::pair<EntryPoint, std::vector<structure_field_t>> default_fields(const EntryPoint &entryPoint, const Emitter &visitor, const fluenc::struct_node *prototype);
