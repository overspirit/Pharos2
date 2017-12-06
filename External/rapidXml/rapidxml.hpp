#ifndef RAPIDXML_HPP_INCLUDED
#define RAPIDXML_HPP_INCLUDED

// Copyright (C) 2006, 2009 Marcin Kalicinski
// Version 1.13
// Revision $DateTime: 2009/05/13 01:46:17 $
//! \file rapidxml.hpp This file contains rapidxml parser and DOM implementation

// If standard library is disabled, user must provide implementations of required functions and typedefs
#if !defined(RAPIDXML_NO_STDLIB)
    #include <cstdlib>      // For std::size_t
    #include <cassert>      // For assert
    #include <new>          // For placement new
#endif

// Only include streams if not disabled
#ifndef RAPIDXML_NO_STREAMS
	#include <ostream>
	#include <iterator>
#endif

#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>

// On MSVC, disable "conditional expression is constant" warning (level 4). 
// This warning is almost impossible to avoid with certain types of templated code
#ifdef _MSC_VER
    #pragma warning(push)
    #pragma warning(disable:4127)   // Conditional expression is constant
#endif

#include "rapidxml_exceptions.hpp"
#include "rapidxml_define.hpp"
#include "rapidxml_lookup_tables.hpp"
#include "rapidxml_memory_pool.h"
#include "rapidxml_xml_base.h"
#include "rapidxml_xml_attribute.h"
#include "rapidxml_xml_node.h"
#include "rapidxml_xml_document.h"
#include "rapidxml_print.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_iterators.hpp"

// Undefine internal macros
#undef RAPIDXML_PARSE_ERROR

// On MSVC, restore warnings state
#ifdef _MSC_VER
    #pragma warning(pop)
#endif

#endif
