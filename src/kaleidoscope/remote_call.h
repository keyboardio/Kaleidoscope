/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2013-2020  Keyboard.io, Inc.
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

// Check if we support remote call with this platform.
//
// To add a new platform, the following changes are required.
//   * implement platform specific versions of
//     _KRC_READ_FUNCTION_POINTER_ASM(FUNC) and
//     _KRC_READ_GLOBAL_VARIABLE_POINTER_ASM(VAR)
//     defined in this file.
//   * modify the include guards and error handling around those macros
//     to prevent errors being emitted for the new platform.
//
// We only warn if remote call isn't already disabled.
//
#ifndef KALEIDOSCOPE_REMOTE_CALL_DISABLED
#ifndef __AVR__
#pragma message "Warning: Remote call not implemented for your platform and thus disabled."
#define KALEIDOSCOPE_REMOTE_CALL_DISABLED
#endif // ifndef __AVR__
#endif // ifndef KALEIDOSCOPE_REMOTE_CALL_DISABLED

//##############################################################################
//
//
//                       Kaleidoscope remote call API
//
//
//##############################################################################

// All invocations of macros of the remote call API must be wrapped into
// an invocation of KALEIDOSCOPE_REMOTE_CALL(...).
//
#define KALEIDOSCOPE_REMOTE_CALL(...)                                          \
   _KALEIDOSCOPE_REMOTE_CALL(__VA_ARGS__)

// The outermost package level must be defined in namespace
// kaleidoscope::remote_call. Therefore, we supply a dedicated macro to
// define the outermost package level.
//
// NAME: The name of the package.
// ...:  Nested packages, functions or inputs.
//
#define KRC_ROOT_PACKAGE(NAME, ...)                                            \
   namespace kaleidoscope {                                             __NL__ \
   namespace remote_call {                                              __NL__ \
      KRC_PACKAGE(NAME, __VA_ARGS__)                                    __NL__ \
   } /* namespace remote_call */                                        __NL__ \
   } /* namespace kaleidoscope */

// Use the KRC_PACKAGE(....) macro to define any nested package levels.
//
// NAME: The name of the package.
// ...:  Nested packages, functions or inputs.
//
#define KRC_PACKAGE(NAME, ...)                                                 \
   _KRC_PACKAGE_AUX(NAME, __COUNTER__, __VA_ARGS__)

// Defines a package that is associated with a global class object.
//
// NAME: The name of the package.
// OBJECT: The full qualified name of the global object.
// ...:  Nested packages, functions or inputs.
//
#define KRC_OBJECT(NAME, OBJECT, ...)                                          \
   _KRC_PACKAGE_AUX(NAME, __COUNTER__,                                  __NL__ \
      typedef decltype(OBJECT) ClassType;                               __NL__ \
      constexpr auto* object = &OBJECT;                                 __NL__ \
                                                                        __NL__ \
      __VA_ARGS__                                                       __NL__ \
   )

// Defines a package that is associated with a global class object that is
// a Kaleidoscope plugin.
//
// NAME: The name of the package.
// PLUGIN: The full qualified name of the global plugin object.
// ...:  Nested packages, functions or inputs.
//
#define KRC_PLUGIN(NAME, PLUGIN, ...)                                          \
   KRC_OBJECT(NAME, PLUGIN, __VA_ARGS__)

// Adds a description to a package, function, argument, result or input.
//
// TEXT: The description text as a string literal.
//
#define KRC_DESCRIPTION(TEXT)                                                  \
   namespace _______info_______ {                                       __NL__ \
      extern const char description[];                                  __NL__ \
      const char description[] = TEXT;                                  __NL__ \
   } /* namespace _______info_______ */

//******************************************************************************
// Macros defining inputs
//******************************************************************************

// Associates a global variable with an input.
//
// NAME: The input's name.
// VARIABLE: The full qualified name of the global variable.
// ...: Optional args, e.g. KRC_DESCRIPTION(....),
//                          KRC_UPDATE_THROUGH_GLOBAL_FUNCTION(....) or
//                          KRC_UPDATE_THROUGH_STATIC_MEMBER_FUNCTION(....)
//
#define KRC_GLOBAL(NAME, VARIABLE, ...)                                        \
   namespace _______inputs_______ {                                     __NL__ \
   _KRC_START_NAMESPACE(NAME)                                           __NL__ \
                                                                        __NL__ \
      namespace _______info_______ {                                    __NL__ \
         extern const void* address;                                    __NL__ \
         const void* address = &VARIABLE;                               __NL__ \
                                                                        __NL__ \
         extern const uint16_t size;                                    __NL__ \
         const uint16_t size = sizeof(VARIABLE);                        __NL__ \
                                                                        __NL__ \
         extern const uint8_t type;                                     __NL__ \
         const uint8_t type                                             __NL__ \
            = TypeNameTrait<decltype(VARIABLE)>::typeName();            __NL__ \
      } /* namespace _______info_______ */                              __NL__ \
                                                                        __NL__ \
      __VA_ARGS__                                                       __NL__ \
                                                                        __NL__ \
      _KRC_EXPORT_VARIABLE(VARIABLE)                                    __NL__ \
                                                                        __NL__ \
   _KRC_END_NAMESPACE(NAME)                                             __NL__ \
   } /* namespace _______inputs_______ */

// Defines a object or plugin member as an input.
//
// Important: This macro can only passed as variadic argument to a
// KRC_OBJECT(....) or KRC_PLUGIN(....) macro invocation.
//
// NAME: The input's name.
// MEMBER_NAME: The name of the object or plugin member variable.
// ...: Optional args, e.g. KRC_DESCRIPTION(....),
//                          KRC_UPDATE_THROUGH_GLOBAL_FUNCTION(....) or
//                          KRC_UPDATE_THROUGH_STATIC_MEMBER_FUNCTION(....)
//
#define KRC_MEMBER(NAME, MEMBER_NAME, ...)                                     \
   /* Export stuff for namepspace _______inputs_______ has already been */     \
   /* initialized in KRC_PACKAGE(...)                                   */     \
   /**/                                                                        \
   namespace _______inputs_______ {                                     __NL__ \
   _KRC_START_NAMESPACE(NAME)                                           __NL__ \
                                                                        __NL__ \
      namespace _______info_______ {                                    __NL__ \
         extern const void* address;                                    __NL__ \
         const void* address = &object->MEMBER_NAME;                    __NL__ \
                                                                        __NL__ \
         extern const uint16_t size;                                    __NL__ \
         const uint16_t size = sizeof(object->MEMBER_NAME);             __NL__ \
                                                                        __NL__ \
         extern const uint8_t type;                                     __NL__ \
         const uint8_t type                                             __NL__ \
            = TypeNameTrait<decltype(object->MEMBER_NAME)>              __NL__ \
                  ::typeName();                                         __NL__ \
      } /* namespace _______info_______ */                              __NL__ \
                                                                        __NL__ \
      __VA_ARGS__                                                       __NL__ \
   _KRC_END_NAMESPACE(NAME)                                             __NL__ \
   } /* namespace _______inputs_______ */


// Enables using a global function as update for an input.
//
// PROCEDURE: The full qualified name of a global procedure of type void (*)()
//
#define KRC_UPDATE_THROUGH_GLOBAL_FUNCTION(PROCEDURE)                          \
   _KRC_EXPORT_FUNCTION(PROCEDURE)                                             \
   _KRC_UPDATE(&PROCEDURE)

// Use a static class member function as update for an input.
//
// Important: This macro can only be passed as variadic argument to
//            macros KRC_OBJECT(....) and KRC_PLUGIN(....).
//
// STATIC_MEMBER_FUNCTION: The name of a static member function of type
//                         void (ClassType::*)() where ClassType is
//                         the type of the object or plugin object that
//                         was exported as a package by invoking KRC_OBJECT(....)
//                         or KRC_PLUGIN(....).
//
#define KRC_UPDATE_THROUGH_STATIC_MEMBER_FUNCTION(STATIC_MEMBER_FUNCTION)      \
   _KRC_EXPORT_FUNCTION(ClassType::STATIC_MEMBER_FUNCTION)                     \
   _KRC_UPDATE(&ClassType::STATIC_MEMBER_FUNCTION)

// Use the following macro to prevent an input inheriting the update procedure
// of its containing package.
//
#define KRC_NO_UPDATE                                                          \
   _KRC_UPDATE(&kaleidoscope::remote_call::_______noUpdate_______)

//******************************************************************************
// Macros exporting remote call functions
//******************************************************************************

// Exports a remote call function.
//
// NAME: The exported name of the function.
// SPECS: A non comma separated list of the following.
//    non-optional:
//         - arguments specification KRC_ARGUMENTS(....)/KRC_NO_ARGS
//         - results specification KRC_RESULTS(....)/KRC_NO_RESULTS
//         - KRC_FUNCTION_BODY(....) must occur after any arguments
//              or results specifications.
//    optional:
//         - description KRC_DESCRIPTION(....) of the function.
//
// Important: Any variadic arguments
//
#define KRC_FUNCTION(NAME, SPECS)                                              \
   /* Export stuff for namepspace _______function_______ */                    \
   /* has already been initialized in KRC_PACKAGE(...) */                      \
   /**/                                                                        \
   namespace _______function_______ {                                   __NL__ \
   _KRC_START_NAMESPACE(NAME)                                           __NL__ \
      SPECS                                                             __NL__ \
   _KRC_END_NAMESPACE(NAME)                                             __NL__ \
   } /* namespace _______function_______ */

// Defines function arguments.
//
// ...: A list of type-name-description pairs, individually wrapped
//      in braces. The description can be omitted.
//      e.g. KRC_ARGUMENTS((uint8_t, arg1),
//                         (uint16_t, arg2, KRC_DESCRIPTION("Some description")))
//
#define KRC_ARGUMENTS(...)                                                     \
   _KRC_START_NAMESPACE(_______arguments_______)                        __NL__ \
                                                                        __NL__ \
      struct StructType {                                               __NL__ \
         MAP(_KRC_IO_DATA_STRUCT_MEMBER,   __VA_ARGS__)                 __NL__ \
      };                                                                __NL__ \
                                                                        __NL__ \
      MAP(_KRC_IO_DATUM,   __VA_ARGS__)                                 __NL__ \
                                                                        __NL__ \
      _KRC_IO_DATA_ADD_DATA_TYPE(StructType)                            __NL__ \
                                                                        __NL__ \
   _KRC_END_NAMESPACE(_______arguments_______)

// Defines function results.
//
// ...: A list of type-name-description pairs, individually wrapped
//      in braces. The description can be omitted.
//      e.g. KRC_RESULTS((uint8_t, arg1),
//                       (uint16_t, arg2, KRC_DESCRIPTION("Some description")))
//
#define KRC_RESULTS(...)                                                       \
   _KRC_START_NAMESPACE(_______results_______)                          __NL__ \
                                                                        __NL__ \
      struct StructType {                                               __NL__ \
         MAP(_KRC_IO_DATA_STRUCT_MEMBER, __VA_ARGS__)                   __NL__ \
      };                                                                __NL__ \
                                                                        __NL__ \
      MAP(_KRC_IO_DATUM, __VA_ARGS__)                                   __NL__ \
                                                                        __NL__ \
      _KRC_IO_DATA_ADD_DATA_TYPE(StructType)                            __NL__ \
                                                                        __NL__ \
   _KRC_END_NAMESPACE(_______results_______)

// Helper macro to define a function without results. To be used instead of
// KRC_RESULTS(....).
//
#define KRC_NO_RESULTS ((uint8_t, _______void_______))

// Defines the body of a remote call wrapper function.
//
// ...: Any C++ code that defines the function body.
//
#define KRC_FUNCTION_BODY(...)                                                 \
   void functionBody() {                                                __NL__ \
      __VA_ARGS__                                                       __NL__ \
   }                                                                    __NL__ \
                                                                        __NL__ \
   _KRC_EXPORT_FUNCTION(functionBody)                                   __NL__ \
                                                                        __NL__ \
   namespace _______info_______ {                                       __NL__ \
      extern const kaleidoscope::remote_call::Callable callable;        __NL__ \
      const kaleidoscope::remote_call::Callable callable                __NL__ \
         = functionBody;                                                __NL__ \
   } /* namespace _______info_______ */

// Helper function macro to access function arguments
// (casts the global IO union to a pointer to the actual arguments struct).
//
// To be used in the C++ code supplied to KRC_FUNCTION_BODY(....), e.g. as
//
// auto args = KRC_ACCESS_ARGS();
//
#define KRC_ACCESS_ARGS()                                                      \
   static_cast<const _______arguments_______::StructType*>(             __NL__ \
      ::kaleidoscope::remote_call::_______function_io_______)

// Helper macro to define a function without arguments. To be used instead of
// KRC_ARGUMENTS(....).
//
#define KRC_NO_ARGS ((uint8_t, _______void_______))

// Helper function macro to access function results
// (casts the global IO union to a pointer to the actual results struct).
//
// To be used in the C++ code supplied to KRC_FUNCTION_BODY(....), e.g. as
//
// auto args = KRC_ACCESS_RESULTS();
//
#define KRC_ACCESS_RESULTS()                                                   \
   static_cast<_______results_______::StructType*>(                     __NL__ \
      ::kaleidoscope::remote_call::_______function_io_______)

// A convenience version of the KRC_FUNCTION macro.
//
// NAME: The exported function name.
// BRACED_RESULTS_LIST: A braced list of type-name-description pairs,
//      individually wrapped in braces. The description can be omitted.
//      e.g. ((uint8_t, arg1), (uint16_t, arg2, KRC_DESCRIPTION("Some description")))
// BRACED_ARGUMENTS_LIST: A braced list of type-name-description pairs,
//      individually wrapped in braces. The description can be omitted.
//      e.g. ((uint8_t, arg1), (uint16_t, arg2, KRC_DESCRIPTION("Some description")))
// BRACED_BODY: C++ code that makes up the wrapper functions's body, enclosed in braces.
// ...: Optional description KRC_DESCRIPTION(....)
//
#define KRC_F(NAME, BRACED_RESULTS_LIST, BRACED_ARGUMENTS_LIST,                \
              BRACED_BODY, ...)                                                \
   KRC_FUNCTION(NAME,                                                          \
      KRC_RESULTS(UNWRAP BRACED_RESULTS_LIST)                                  \
      KRC_ARGUMENTS(UNWRAP BRACED_ARGUMENTS_LIST)                              \
      KRC_FUNCTION_BODY(                                                       \
         auto args = KRC_ACCESS_ARGS();                                 __NL__ \
         auto results = KRC_ACCESS_RESULTS();                           __NL__ \
         UNWRAP BRACED_BODY                                             __NL__ \
      )                                                                        \
      __VA_ARGS__                                                              \
   )

//##############################################################################
//
//
//                                    PRIVATE
//
//
//##############################################################################
// Everything that follows is considered as implementation details and is not
// part of Kaleidoscope's remote call API.
//##############################################################################

//******************************************************************************
// IO struct export
//******************************************************************************
// All remote call functions arguments and result values are stored in RAM.
// As Kaleidoscope is a single tasking system, we can allow all functions
// to store their arguments and result values in the same memory area.
//
// The following macros serve to define a global union that stores
// arguments and result values of all functions.
//
// This is achieved by using recursively defined template unions on
// every namespace level of packages and functions.

// Initializes IODataUnion collection on the level of every namespace.
//
#define _KRC_INIT_IO_DATA_EXPORT                                               \
   template<int _T>                                                     __NL__ \
   union IODataUnion                                                    __NL__ \
   {                                                                    __NL__ \
      IODataUnion<_T - 1> rest_;                                        __NL__ \
   };                                                                   __NL__ \
                                                                        __NL__ \
   template<>                                                           __NL__ \
   union IODataUnion<-1>                                                __NL__ \
   {                                                                    __NL__ \
      uint8_t dummy_; /* Union must have at least one member */         __NL__ \
   };

#define _KRC_IO_DATA_ADD_DATA_TYPE_AUX(DATA_TYPE, CNTR)                        \
   template<>                                                           __NL__ \
   union IODataUnion<CNTR>                                              __NL__ \
   {                                                                    __NL__ \
      DATA_TYPE data_;                                                  __NL__ \
      IODataUnion<CNTR - 1> rest_;                                      __NL__ \
   };

// Adds a data type to the global function arguments/results (IO data)
// union.
//
#define _KRC_IO_DATA_ADD_DATA_TYPE(DATA_TYPE) \
   _KRC_IO_DATA_ADD_DATA_TYPE_AUX(DATA_TYPE, __COUNTER__)

#define _KRC_IO_DATA_ADD_FROM_NAMESPACE_AUX(NAMESPACE, CNTR)                   \
   template<>                                                           __NL__ \
   union IODataUnion<CNTR>                                              __NL__ \
   {                                                                    __NL__ \
      NAMESPACE::IODataUnion<CNTR> nested_data_;                        __NL__ \
      IODataUnion<CNTR - 1> rest_;                                      __NL__ \
   };

// Adds the IO data union of a namespace to the IO data union of
// the surrounding namespace.
//
#define _KRC_IO_DATA_ADD_FROM_NAMESPACE(NAMESPACE) \
   _KRC_IO_DATA_ADD_FROM_NAMESPACE_AUX(NAMESPACE, __COUNTER__)

// Initializes the global IO data union
// (in namespace kaleidoscope::remote_call).
//
#define _KRC_GLOBAL_INIT_IO_DATA                                               \
   namespace kaleidoscope {                                             __NL__ \
   namespace remote_call {                                              __NL__ \
      _KRC_INIT_IO_DATA_EXPORT                                          __NL__ \
   } /* namespace remote_call */                                        __NL__ \
   } /* namespace kaleidoscope */

// Exports the global IO data union.
//
#define _KRC_GLOBAL_FINISH_IO_DATA                                             \
   namespace kaleidoscope {                                             __NL__ \
   namespace remote_call {                                              __NL__ \
      IODataUnion<__COUNTER__> _______function_io_union_______;         __NL__ \
      _KRC_EXPORT_VARIABLE(::kaleidoscope::remote_call                  __NL__ \
         ::_______function_io_union_______)                             __NL__ \
      void *const _______function_io_______                             __NL__ \
            = &_______function_io_union_______;                         __NL__ \
   } /* namespace remote_call */                                        __NL__ \
   } /* namespace kaleidoscope */

//******************************************************************************
// Enforced symbol export
//******************************************************************************
// During the linker stage, ld the linker will strip out any symbols (functions
// and global variables that are not referenced by any other function that is
// directly or indirectly called from main.
//
// To prevent symbols that we want to use for remote calls from being stripped
// we add dummy access instructions that are indirectly called from main.
// This is achieved by defining SymbolExporter template class specializations
// that do the assembler symbol access. The SymbolExporter classes are defined
// in a way that each specialization calls the apply() method of the specialization
// with the next lower integer template parameter. SymbolExporter<CNTR> ends
// the recursion.
//
// Every invocation of _KRC_EXPORT_FUNCTION or _KRC_EXPORT_VARIABLE defines a
// SymbolExporter class specialization whose integer value is defined by the
// continuously growing intrinsic __COUNTER__ macro. That way we can be sure that
// each counter value is only used once. 3rd party use of the __COUNTER__ macro
// leads to gaps in the range of integer template parameters of the SymbolExporter
// specializations. This can be safely ignored as those gaps cause the standard
// implementation of SymbolExporter being used that just calls the next lower
// specialization.
//
// A final use of the __COUNTER__ macro in KRC_FINISH_EXPORTS
// ensures that all the eval() static methods of all template specializations
// are called from a function kaleidoscope::remote_call::exportSymbols() that
// is invoked by void Runtime_::setup() which is in turn called indirectly
// by main().

// Initializes symbol export for a namespace level (package or function).
//
#define _KRC_INIT_SYMBOL_EXPORT                                                \
   template<int _T>                                                     __NL__ \
   struct SymbolExporter                                                __NL__ \
   {                                                                    __NL__ \
      __attribute__((always_inline))                                    __NL__ \
      static void eval() {                                              __NL__ \
         SymbolExporter<_T - 1>::eval();                                __NL__ \
      }                                                                 __NL__ \
   };                                                                   __NL__ \
                                                                        __NL__ \
   /* End recursion */                                                  __NL__ \
   /**/                                                                 __NL__ \
   template<>                                                           __NL__ \
   struct SymbolExporter<-1>                                            __NL__ \
   {                                                                    __NL__ \
      __attribute__((always_inline))                                    __NL__ \
      static void eval() {}                                             __NL__ \
   };

#define _KRC_EXPORT_SYMBOLS_OF_NAMESPACE_AUX(NAMESPACE, CNTR)                  \
   template<>                                                           __NL__ \
   struct SymbolExporter<CNTR>                                          __NL__ \
   {                                                                    __NL__ \
      __attribute__((always_inline))                                    __NL__ \
      static void eval() {                                              __NL__ \
         SymbolExporter<CNTR - 1>::eval();                              __NL__ \
         NAMESPACE::SymbolExporter<CNTR>::eval();                       __NL__ \
      }                                                                 __NL__ \
   };

#if __AVR__

#define _KRC_READ_FUNCTION_POINTER_ASM(FUNC)                                   \
   "ldi r26, lo8(%0)" :: "p" (FUNC)
#define _KRC_READ_GLOBAL_VARIABLE_POINTER_ASM(VAR)                             \
   "" : "+r" (VAR)
#else
#error Please define appropriate assembly code that reads the pointer of a function
#endif

// Collects symbol export of a namespace and ensures that it will
// be considered by the surrounding namespace.
//
#define _KRC_EXPORT_SYMBOLS_OF_NAMESPACE(NAMESPACE) \
   _KRC_EXPORT_SYMBOLS_OF_NAMESPACE_AUX(NAMESPACE, __COUNTER__)

#define _KRC_EXPORT_FUNCTION_AUX(FUNC, CNTR)                                   \
   template<>                                                           __NL__ \
   struct SymbolExporter<CNTR>                                          __NL__ \
   {                                                                    __NL__ \
      __attribute__((always_inline))                                    __NL__ \
      static void eval() {                                              __NL__ \
         asm volatile(                                                  __NL__ \
            _KRC_READ_FUNCTION_POINTER_ASM(FUNC));                      __NL__ \
                                                                        __NL__ \
         SymbolExporter<CNTR - 1>::eval();                              __NL__ \
      }                                                                 __NL__ \
   };

// Exports a function. By issuing an assembler load command of the
// function address, we prevent the function to be garbage collected.
//
#define _KRC_EXPORT_FUNCTION(FUNC)                                             \
   _KRC_EXPORT_FUNCTION_AUX(FUNC, __COUNTER__)

#define _KRC_EXPORT_VARIABLE_AUX(VAR, CNTR)                                    \
   template<>                                                           __NL__ \
   struct SymbolExporter<CNTR>                                          __NL__ \
   {                                                                    __NL__ \
      __attribute__((always_inline))                                    __NL__ \
      static void eval() {                                              __NL__ \
         asm volatile(_KRC_READ_GLOBAL_VARIABLE_POINTER_ASM(VAR));      __NL__ \
                                                                        __NL__ \
         SymbolExporter<CNTR - 1>::eval();                              __NL__ \
      }                                                                 __NL__ \
   };

// Exports a global variable. By issuing an assembler load command of the
// variable address, we prevent the variable to be garbage collected.
//
#define _KRC_EXPORT_VARIABLE(VAR)                                              \
   _KRC_EXPORT_VARIABLE_AUX(VAR, __COUNTER__)

// Initialize symbol export in the outermost
// namespace kaleidoscope::remote_call.
//
#define _KRC_GLOBAL_INIT_SYMBOL_EXPORT                                         \
   namespace kaleidoscope {                                             __NL__ \
   namespace remote_call {                                              __NL__ \
      _KRC_INIT_SYMBOL_EXPORT                                           __NL__ \
   } /* namespace remote_call */                                        __NL__ \
   } /* namespace kaleidoscope */

// Provide a function exportSymbols() that is called by the runtimes
// initialization method in order to prevent symbol garbage collection.
//
#define _KRC_GLOBAL_FINISH_SYMBOL_EXPORTS                                      \
   namespace kaleidoscope {                                             __NL__ \
   namespace remote_call {                                              __NL__ \
      void exportSymbols() {                                            __NL__ \
         SymbolExporter<__COUNTER__>::eval();                           __NL__ \
      }                                                                 __NL__ \
   } /* namespace remote_call */                                        __NL__ \
   } /* namespace kaleidoscope */

//******************************************************************************

// At the begin of a namespace (package or function) we initialize symbol export
// and IO data union collection.
//
#define _KRC_START_NAMESPACE(NAMESPACE)                                        \
   namespace NAMESPACE {                                                __NL__ \
      _KRC_INIT_SYMBOL_EXPORT                                           __NL__ \
      _KRC_INIT_IO_DATA_EXPORT

// At the end of every namespace we make the surrounding namespace collect
// the symbol export and IO data union information of the nested namespace.
//
#define _KRC_END_NAMESPACE(NAMESPACE)                                          \
   } /* namespace NAMESPACE */                                          __NL__ \
   _KRC_EXPORT_SYMBOLS_OF_NAMESPACE(NAMESPACE)                          __NL__ \
   _KRC_IO_DATA_ADD_FROM_NAMESPACE(NAMESPACE)

// For all namespaces that are opened and closes several times, such
// as e.g. _______inputs_______, we need to initialize the
// export stuff only once.
//
#define _KRC_INIT_EXPORT_STUFF_FOR_NAMESPACE(NAMESPACE)                        \
   namespace NAMESPACE {                                                __NL__ \
      _KRC_INIT_SYMBOL_EXPORT                                           __NL__ \
      _KRC_INIT_IO_DATA_EXPORT                                          __NL__ \
   }

#define _KRC_FINALIZE_EXPORT_STUFF_FOR_NAMESPACE(NAMESPACE)                    \
   _KRC_EXPORT_SYMBOLS_OF_NAMESPACE(NAMESPACE)                                 \
   _KRC_IO_DATA_ADD_FROM_NAMESPACE(NAMESPACE)

#define _KRC_PACKAGE_NAMESPACE_NAME(NAME, CNTR)                                \
   GLUE4(_______package, CNTR, _______, NAME)

// Defines a package level (packages may be hierarchically structured).
//
#define _KRC_PACKAGE_AUX(NAME, CNTR, ...)                                      \
   _KRC_START_NAMESPACE(_KRC_PACKAGE_NAMESPACE_NAME(NAME, CNTR))        __NL__ \
                                                                        __NL__ \
      _KRC_INIT_EXPORT_STUFF_FOR_NAMESPACE(_______inputs_______)        __NL__ \
      _KRC_INIT_EXPORT_STUFF_FOR_NAMESPACE(_______function_______)      __NL__ \
                                                                        __NL__ \
      /* The tag variable is only required to simplify regex */         __NL__ \
      /* parsing of packages                                 */         __NL__ \
      /**/                                                              __NL__ \
      extern const uint8_t _______package_______;                       __NL__ \
      const uint8_t _______package_______ = 0;                          __NL__ \
                                                                        __NL__ \
      __VA_ARGS__                                                       __NL__ \
                                                                        __NL__ \
      _KRC_FINALIZE_EXPORT_STUFF_FOR_NAMESPACE(_______inputs_______)    __NL__ \
      _KRC_FINALIZE_EXPORT_STUFF_FOR_NAMESPACE(_______function_______)  __NL__ \
                                                                        __NL__ \
   _KRC_END_NAMESPACE(_KRC_PACKAGE_NAMESPACE_NAME(NAME, CNTR))

// Associates an update procedure with an input.
//
// PROCEDURE: A full qualified pointer to a procedure of type void (*)()
//            e.g. for a function
//
//            namespace some_namespace {
//               void someUpdateFunction() {...}
//            }
//
//            This would be invoked as
//               _KRC_UPDATE(&some_namespace::someUpdateFunction)
//
#define _KRC_UPDATE(PROCEDURE)                                                 \
   namespace _______info_______ {                                       __NL__ \
      extern const kaleidoscope::remote_call::Callable callable;        __NL__ \
      const kaleidoscope::remote_call::Callable callable                __NL__ \
         = PROCEDURE;                                                   __NL__ \
   } /* namespace _______info_______ */

// An auxiliary macro that is used both to define function arguments and
// results.
//
#define _KRC_IO_DATUM_AUX(TYPE, NAME, ...)                                     \
   namespace NAME {                                                     __NL__ \
      namespace _______info_______ {                                    __NL__ \
         extern const uint16_t offset;                                  __NL__ \
         const uint16_t offset                                          __NL__ \
            = (uint16_t)offsetof(StructType, NAME);                     __NL__ \
                                                                        __NL__ \
         extern const uint16_t size;                                    __NL__ \
         const uint16_t size                                            __NL__ \
            = sizeof(decltype(StructType{}.NAME));                      __NL__ \
                                                                        __NL__ \
         extern const uint8_t type;                                     __NL__ \
         const uint8_t type                                             __NL__ \
            = TypeNameTrait<decltype(StructType{}.NAME)>                __NL__ \
                  ::typeName();                                         __NL__ \
      } /* namespace _______info_______ */                              __NL__ \
                                                                        __NL__ \
      __VA_ARGS__                                                       __NL__ \
   } /* namespace NAME */

#define _KRC_IO_DATUM(...) _KRC_IO_DATUM_AUX __VA_ARGS__

#define _KRC_IO_DATA_STRUCT_MEMBER_AUX(TYPE, NAME, ...)                        \
   TYPE NAME;

// Defines the member of an IO data struct (function argument or result).
//
#define _KRC_IO_DATA_STRUCT_MEMBER(...)                                        \
   _KRC_IO_DATA_STRUCT_MEMBER_AUX __VA_ARGS__

namespace kaleidoscope {
namespace remote_call {
extern void exportSymbols();
} // namespace remote_call
} // namespace kaleidoscope

#ifdef KALEIDOSCOPE_SKETCH
#ifndef KALEIDOSCOPE_REMOTE_CALL_DISABLED

#if 0
// Make all members of all classes available for export.
// Note: This is a very dirty hack but avoids a lot of boilerplate friend
//       declarations. Also this is only happening in the sketch object
//       but not in any other objects where the classes are used.
//
#define private public
#define protected public
#endif

#include "kaleidoscope/macro_map.h"
#include "kaleidoscope/macro_helpers.h"

namespace kaleidoscope {
namespace remote_call {

void exportSymbols();
extern void * const _______function_io_______;

typedef void (*Callable)();
extern const uint8_t pointer_size;
const uint8_t pointer_size = sizeof(void*);

void _______noUpdate_______() {}

template<typename _T>
struct TypeNameTrait {
};

#define _KRC_MEMBER_TYPE_TRAIT(TYPE, TYPE_ID)                                  \
   template<>                                                           __NL__ \
   struct TypeNameTrait<TYPE>                                           __NL__ \
   {                                                                    __NL__ \
      constexpr static uint8_t typeName() {                             __NL__ \
         return TYPE_ID;                                                __NL__ \
      }                                                                 __NL__ \
   };

_KRC_MEMBER_TYPE_TRAIT(uint8_t,     0)
_KRC_MEMBER_TYPE_TRAIT(uint16_t,    1)
_KRC_MEMBER_TYPE_TRAIT(uint32_t,    2)
_KRC_MEMBER_TYPE_TRAIT(int8_t,      3)
_KRC_MEMBER_TYPE_TRAIT(int16_t,     4)
_KRC_MEMBER_TYPE_TRAIT(int32_t,     5)
_KRC_MEMBER_TYPE_TRAIT(float,       6)

#undef _KRC_MEMBER_TYPE_TRAIT

} // namespace remote_call
} // namespace kaleidoscope

#define KALEIDOSCOPE_REMOTE_CALL_INIT                                          \
   _KRC_GLOBAL_FINISH_SYMBOL_EXPORTS                                           \
   _KRC_GLOBAL_FINISH_IO_DATA

// A soon as we find a way to incorporate remote call package scan during
// the build process, we can store a firmware checksum in the firmware elf file.
//
// #define KALEIDOSCOPE_REMOTE_CALL_HAVE_FIRMWARE_CHECKSUM
//
#ifdef KALEIDOSCOPE_REMOTE_CALL_HAVE_FIRMWARE_CHECKSUM
namespace kaleidoscope {
namespace remote_call {
extern const PROGMEM int8_t firmware_checksum[8];
const PROGMEM int8_t firmware_checksum[8] = { 0 };
} // namespace remote_call
} // namespace kaleidoscope
#endif

_KRC_GLOBAL_INIT_SYMBOL_EXPORT
_KRC_GLOBAL_INIT_IO_DATA

#define _KALEIDOSCOPE_REMOTE_CALL(...) __VA_ARGS__

#else // #ifndef KALEIDOSCOPE_REMOTE_CALL_DISABLED

// If the remote call module is disabled, we need at least an empty
// symbol export function.
//
#define KALEIDOSCOPE_REMOTE_CALL_INIT                                          \
   namespace kaleidoscope {                                             __NL__ \
   namespace remote_call {                                              __NL__ \
      void exportSymbols() {}                                           __NL__ \
   } /* namespace remote_call */                                        __NL__ \
   } /* namespace kaleidoscope */

#define _KALEIDOSCOPE_REMOTE_CALL(...)

#endif // #ifndef KALEIDOSCOPE_REMOTE_CALL_DISABLED

#else

// An empty version of the _KALEIDOSCOPE_REMOTE_CALL macro
// is required to be used in all non-sketch compilation units.
//
#define _KALEIDOSCOPE_REMOTE_CALL(...)

#endif // #ifdef KALEIDOSCOPE_SKETCH

#define _KRC_EXPORT_SYMBOLS \
   kaleidoscope::remote_call::exportSymbols();
