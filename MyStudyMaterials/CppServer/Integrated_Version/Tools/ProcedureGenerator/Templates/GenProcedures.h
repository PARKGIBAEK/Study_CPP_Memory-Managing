#pragma once
#include "Types.h"
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include "DBBind.h"

{%- macro gen_procedures() -%} {% include 'Procedure.h' %} {% endmacro %}

namespace SP
{
	{{gen_procedures() | indent}}
};