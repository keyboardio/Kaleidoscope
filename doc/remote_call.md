# Remote Call

Author: Florian Fleissner (shinynoseglasses@gmail.com)

## Introduction

Kaleidoscope's remote call interface exports firmware functions for being
directly called from the host. It supports setting and retrieving values of 
global variables. Device and host thereby communicate via USB serial.

The remote call interface is meant as a resource saving and type safe
replacement of Focus event handlers.

This document is describes all aspects of the remote call interface.

It starts with a description of the motivation behind the development of 
the interface. Then the most important features of the interface are explained.
Before usage and implementation are described in detail, the most important
abstract concepts of the approach are explained. Then a
comparison of the interface's resource requirements is provided and a comparison
with the requirements of Focus event handlers. The document ends with a list
of possible enhancements.

## Motivation

Before the remote call infrastructure was implemented, the common way of 
data exchange between host and device was using the Focus interface and
by implementing Focus event handlers for plugins.

The focus interface comes with some shortcomings that the new approach
was supposed to fix, namely 

* its high amount of boiler plate code, 
* its comparably complex way of implementing event handlers,
* large resource consumption in terms of program memory,
* the absence of type safety in communication,
* the absence of value range checking with respect to the target data types and
* missing documentation (commands are stored in PROGMEM).

The remote call infrastructure does not intent to replace Focus entirely, it 
even uses it under the hood. It just intents to replace it in those 
applications where the use of Focus comes along with the above shortcomings.

The remote call interface does away with all shortcomings mentioned. It

* comes without boiler plate code (everything is tightly wrapped in well documented macros),
* has a very simple API,
* reduces PROGMEM consumption to approximately 20% of that of Focus event handlers,
* adds type safety and value range checks,
* encourages in-code documentation of the communication interface where
* documentation comes without runtime costs (no PROGMEM strings stored)
* and is exported with the overall interface description.

By delegating great parts of the communication overhead to the host, 
the remote call interface supports for a much larger set of functionality 
being exported for remote control than it would be possible by using
Focus event handlers.

## Features 

The previous section already names some of the new features of the 
remote call API and the most important differences to Focus event handlers.
But some of the new features deserve a closer look.

### Type safety

All standard `int*_t` and `uint*_t` data types
as well as `float` are supported as function arguments and return 
values (results) as well as for input variables. 

Any user input that is supposed to be passed to the device is automatically 
checked against the exported data types to see if types match and if values 
are within the supported range.

### Minimum possible resource consumption

No additional code is required for checking serial commands. As error handling, 
data type and value ranges are checked by the host, no device resources are
consumed by this task. This is in contrast to Focus event handlers that require
a lot of string comparisons to root communication to the sender/receiver (plugin)
on the device side. 

Thin wrapper functions serve as a standardized and resource-saving 
communication interface for exported core and plugin functions.

### Packaging

Exported functions and inputs can be hierarchically grouped as packages.
This packaging may but does not have to follow Kaleidoscope's 
namespace hierarchy. It thus possibly enables an alternative grouping of 
information compared to the core/plugin structure which might better suite
the view of a non-programmer user.

### Documentation of exported features

All entities (packages/functions/inputs/arguments/results) can be assigned
description (documentation) strings of arbitrary length without affecting
firmware binary size.

### Hierarchically shared information

Information such as update procedures that are meant to be called upon changing
input variables can be defined on package level and shared (inherited) by 
nested packages and those inputs that do not specify update procedures 
of their own.

### Wrapper functions for common tasks

Exporting remote call functions means defining simple wrapper functions 
using the remote call API. This is comparably simple compared to writing Focus
event handlers.

### In-place updates of input variables

For some tasks, where specific plugin members or global variables are evaluated
in every cycle, it is cheaper to modify the value of such variables in place
instead of implementing wrapper functions. 

In the terminology of the remote call interface such
variables are named *inputs*. The interface allows
specification of an update procedure to be called automatically 
whenever an input is modified. See the description of inputs in the *concepts*
section for more details.

### YAML export of remote call interface

A YAML representation of the remote call interface for a given firmware
configuration can be auto-generated using the *scan_packages* script.
Such YAML files are meant to be used by the *remote* tool to handle
type safe host-device communication. The same files can also be read by 
other tools if they implement the *remote* scripts functionality.
*Chrysalis* would e.g. be a candidate.

### Simple host-device communication

As the remote call API makes the RAM address of variables and functions
available to the host, communication of most plugins 
can be routed through the RemoteCall plugin that handles the host-device data 
exchange via a very simple and robust typed PEEK/POKE communication interface.

### Globally disabling the remote call infrastructure

Some users might not have any use for remote calls and 
prefer to use every byte of RAM and PROGMEM for other features.

The implementation of the remote call interface respects that. If it's not needed, 
we should not want to waste resources for it.

Thus, the overall remote call infrastructure can be disabled by defining
a single macro (`KALEIDOSCOPE_REMOTE_CALL_DISABLED`) at the top 
of the user's sketch.

This will not only disable functionality but exclude any pieces of the remote
call infrastructure from being compiled. Thus, any additional runtime overhead
is prevented.

## Concepts

The remote call interface defines some concepts for the interaction between 
host and device, namely exported *remote call functions* and *inputs*.

#### Functions

An exported function is basically a way to make a C++ function accessible 
for being called remotely. It is characterized by the following information.

* function name
* function arguments (name and type)
* function return values/results (name and type) 
* function body (C++ code)
* description (optional)

The remote call API allows for functions to have multiple named return values.

No RAM or program memory is consumed neither for *function name* 
and *description* nor for names and type information of arguments and results.

Remote functions are called by supplying name-value pairs for all function
arguments together with the function's name to the *call* sub-command 
of the *remote* command line tool (see the usage section).

#### Inputs

Inputs can either be global variables or member variables of global class
objects. They are characterized by their

* name,
* type (global or member),
* update procedure (optional) and
* description (optional).

If specified, the update procedure (a void function without arguments) 
is expected to be called remotely after changing the input variable for the
changes to be come effective. For such inputs that are picked up during
every firmware cycle no update procedure is required.

Inputs are modified via the *set_input* sub-command of the *remote* command
line tool. The current value of a input can be queried using the *get_input* 
sub-command (see the usage section for more information).

#### Packages

The remote call interface exports any functions and inputs within package
namespaces. Packaging enable grouping functions and inputs hierarchically and
supplying descriptions and inheritable update procedures for a set of inputs.
Packaging is similar to C++ namespaces but adds some additional features
such as sharing documentation and inheritance of input update procedures.

#### Nesting Packages

Packages may be nested. When passed as arguments to the *remote* 
command line tool, function or input names need to be
specified using their complete package namespace path, 
e.g. as `TopLevelPackage::NestedPackage::aFunction`.

##### Update Procedures and package level inheritance

Whenever an input does not supply an update procedure and is not explicitly 
marked as not requiring an update, the *scan_packages* tool will traverse the 
input's package namespace path in direction of the package trees root
to find an appropriate update procedure.
This allows to define a default update procedure that may optionally be 
overridden by nested packages or by a dedicated update procedure that is 
defined by an input.

## Usage

### Adding remote call capability to Kaleidoscope code

Any export directives of the remote call interface must either be added
to C++ include files (only those that are available for being included in the 
Kaleidoscope sketch) or to the sketch .ino-file itself.

Make sure to include the header `kaleidoscope/remote_call.h` in every
header file that you intent to export functions or input variables for
remote call from.

#### Exporting functions

The following is a commented version of the exported methods of
Kaleidoscope's *LEDControl* plugin.

```cpp
#include "kaleidoscope/remote_call.h"

...

// Any remote call specifications must be wrapped by in
// an invocation of `KALEIDOSCOPE_REMOTE_CALL`. This allows any remote call
// exports to be globally disabled.
//
KALEIDOSCOPE_REMOTE_CALL(

   // Every function/input must be wrapped by at least one package level.
   // The root level must be specified by using the KRC_ROOT_PACKAGE(...)
   // function macro.
   //
   KRC_ROOT_PACKAGE(plugin,
   
      // All nested package levels are specified by invoking KRC_PACKAGE(...).
      //
      KRC_PACKAGE(LEDControl,
      
         // Export a function to set the LED color of an individual key.
         //
         KRC_F(setCrgbAt, // The function's name (its package namespace path is
                          // 'plugin::LEDControl::setCrgbAt').
                          
            // Signal the function to have no result variables.
            //
            KRC_NO_RESULTS,
            
            // The function has five call arguments that are specified
            // by type and name. The whole set of arguments must be wrapped
            // in brackets. Every argument pair must be separated by a comma 
            // and also be wrapped in individual brackets.
            //
            ((uint8_t, row), (uint8_t, col),
             (uint8_t, red), (uint8_t, green), (uint8_t, blue)),
             
            // The function body must also be wrapped in brackets. The two 
            // pointers 'args' and 'results' are always automatically predefined
            // and appropriately typed. Both point to the beginning of the
            // common (overlapping) storage area in RAM that is used
            // for all functions' arguments and result variables.
            //
            (
               LEDControl.setCrgbAt(KeyAddr{args->row, args->col}, 
                                    CRGB(args->red, args->green, args->blue));
            ),
                                  
            // The description text is optional.
            //
            KRC_DESCRIPTION("Sets the LED color of a single key")
         )
         
         //... more exported functions ...
         
         // Export a function to query the current LED mode. This function
         // has a non-void result pair and no arguments.
         //
         KRC_F(getMode, ((uint8_t, mode_id)), KRC_NO_ARGS,
            (results->mode_id = LEDControl.get_mode_index();),
            KRC_DESCRIPTION("Queries the LED mode")
         )
      )
   )
)
```

#### Exporting inputs

To export a global variable as an input, the variable must already have been 
defined.

```cpp

extern uint8_t someGlobalVariable; // The variable to be exported as input.

extern void someUpdateFunc(); // An update procedure that is expected
                              // to be called by remote communication
                              // whenever the input has been modified.

KALEIDOSCOPE_REMOTE_CALL(

   KRC_PACKAGE(SomePackage,
   
      // Export a global variable as input.
      //
      KRC_GLOBAL(someGlobalVariable, // The variable name. Its package namespace
                                     // path is 'SomePackage::someGlobalVariable'.
                                     
         // The global variable to export. 
         //
         // Important: Specify all exported variables with complete 
         //            C++ namespace path (starting with :: 
         //            for the global namespace).
         //
         ::someGlobalVariable,
         
         // An (optional) description string.
         //
         KRC_DESCRIPTION("The red LED color portion")
         
         // The global function that is expected to be called remotely after the
         // input value has been modified.
         //
         KRC_UPDATE_THROUGH_GLOBAL_FUNCTION(::someUpdateFunc)
      )
   )
```

#### Exporting Kaleidoscope plugins as packages

To simplify exporting Kaleidoscope plugins as a package and to access 
the member variables of a global singleton instance of the plugin, 
an additional macro `KRC_Plugin(...)` is provided.

The following example assumes that there exists a plugin instance 
`kaleidoscope::plugin::MyPlugin`.

```cpp
KALEIDOSCOPE_REMOTE_CALL(
   KRC_PLUGIN(MyPlugin, // Remote call package name
      kaleidoscope::plugin::MyPlugin, // Global plugin instance
      KRC_DESCRIPTION("MyPlugin description") // optional description
      KRC_MEMBER(a, a_, // Exports plugin member a_ as a
         KRC_DESCRIPTION("a is ...") // optional
         // No update required
      )
      KRC_MEMBER(b, b_, 
         KRC_DESCRIPTION("b is ...")
         // Assumes the plugin class to have a static method 
         // void bUpdate()
         //
         KRC_UPDATE_THROUGH_STATIC_MEMBER_FUNCTION(bUpdate)
      )
      
      // Global functions can be added to the plugin package if necessary.
      //
      KRC_GLOBAL(someGlobalVariable, ::someGlobalVariable,
         KRC_UPDATE_THROUGH_GLOBAL_FUNCTION(::someUpdateFunc)
      )
   )
)
```

#### Inherited update procedures

Update procedures can be defined on package level. Every input that does not
explicitly specify an update procedure will inherit the update procedure
defined on the next higher package level (if available).

Exported inputs can specify the macro `KRC_NO_UPDATE` to explicitly 
prevent update procedure inheritance.

See the example sketch `examples/RemoteCall/RemoteCall.ino` for further
information.

#### Descriptions

Description text can be added on package level, to exported functions,
arguments, results and inputs. Description strings can have arbitrary size
at zero cost.

#### Disabling the remote call interface

The entire remote call interface can be disabled by adding the following
to the very top of the sketch, before any include directives.

```cpp
#define KALEIDOSCOPE_REMOTE_CALL_DISABLED
```

### scan_packages tool

To export the remote call interface, the readily build firmware and its 
build artifacts must manually be post-processed using the *scan_packages* tool.
This tool, a Python script, currently resides in the `bin/remote_call` folder
of the Kaleidoscope repository.

The following is an example off a typical usage.

```
./scan_packages \
   --binutils_dir=<path_to_arduino_installation>/arduino-1.8.10/hardware/tools/avr/bin/ \
   --binutils_prefix=avr- \
   --sketch_dir=/tmp/kaleidoscope-$USER/sketch/<sketch_dir> \
   --yaml_output_file=remote_call.yml
```

Please replace `<path_to_arduino_installation>` and `<sketch_dir>` accordingly.

This will write the extracted package information to a YAML file named `remote_call.yml`
in the current working directory.

Usage information is available via *scan_packages*' `-h` command line switch.

#### YAML export

The information generated by *scan_packages* is exported as YAML files.
One such YAML file represents the remote call interface of one specific 
configuration of the firmware, build by a specific compiler version for a 
specific device. It contains addressing information that is only valid on that
very device architecture. Because of that it is important to name the YAML files
appropriately. For more information, read the section about 
firmware checksums at the end of this document.

This is an example of possible YAML output of a remote call interface
specification. This information is generated by evaluating 
the information collected under the hood by the remote call API macros.

```yml
packages:
   - name: PackageLevel1
     description: PackageLevel1 description
     callable:
        address: 789
        symbol_mangled: _Z14updateMyPluginv
        symbol_unmangled: updateMyPlugin()
     packages:
      - name: PackageLevel2
        description: PackageLevel2 description
        callable:
           address: 789
           symbol_mangled: _Z14updateMyPluginv
           symbol_unmangled: updateMyPlugin()
        inputs:
         - name: blue
           description: The blue LED color portion
           data:
              address: 258
              base_address: 258
              offset: 0
              size: 1
              type: uint8_t
              symbol_unmangled: b
              symbol_mangled: b
           callable:
              address: 790
              symbol_mangled: _Z8someFuncv
              symbol_unmangled: someFunc()
              inherited: False
```

### Host-Device communication

The *remote* command line tool manages the communication between host and 
device. The Python script is meant to serve as a reference implementation.
It can easily be adapted to incorporate the same host-device communication 
functionality into other tools.

It is a simple command interpreter that validates and converts user input 
to a set of peek/poke/call transactions with the device.

Here's an example that demonstrates how the remote tool can be used to set 
the LED color of a specific key-LED on the device.

```
cd <kaleidoscope_repo>/bin/remote_call
./remote \
   --kaleidoscope_repo_dir=$PWD/../../ \
   --yaml_model_file=<path_to_yaml_file> \
   call plugin::LEDControl::setCrgbAt row=1 col=2 red=255 green=0 blue=0
```

Please replace `<kaleidoscope_repo>` and `<path_to_yaml_file>` appropriately.

The *remote* tool requires commands to be specified after any settings.
In the above example the command `call` is supplied with the name of an
exported function (full package namespace path) and a number of arguments 
as name-value pairs. In general arguments may be specified in arbitrary order.

Please note that it may be necessary to define the device location via 
the `DEVICE` environment variable in order for the serial communication 
to work, e.g. `DEVICE=/dev/ttyACM<x>` where `<x>` must be replaced
with an integer number that is assigned by the system at the time
when the device is plugged in.

Further usage information is available via *remote*' `-h` command line switch.

## Implementation

The remote call interface is mostly based on a set of API function macros
that are situated in the include file `kaleidoscope/remote_call.h`.

Any public function macros start with the prefix `KRC_`. Other macros starting
with a leading underscore are considered as internal and may be subject to 
future changes.

In the following we will explain some important concepts of the remote call
implementation. Before the actual information is discussed, we take a short
detour and see how the linker works and how it applies garbage collection.
The latter is one of the most important concepts the remote call interface
is build upon.

### Linking

When the Kaleidoscope firmware is build, first all relevant compilation units
are compiled. They are then passed to the linker which combines them to 
generates the .elf firmware file in a separate processing step. 
While linking, any relocation information stored in object files is used
to generate actual locations, i.e. the addresses where symbols are going to
be stored. This can be seen by representing the names of internally and 
externally referenced symbols with the actual addresses. This step is called
location.

We will come back to location 

### Linker garbage collection



During this step, the linker can
determine which symbols (functions and variables) are not accessed (called)
directly or indirectly by the `main(...)` function. As not used by the final
firmware, all such symbols are considered as garbage and removed.

This mechanism helps to keep the firmware binary as small as possible as device 
resources are typically fairly restricted.

The same mechanism can elegantly be used to extract information about the 
firmware that is otherwise only available to the compiler. The idea is to 
store such information in symbols whose values are extracted before the 
final linker stage eliminates them during garbage collection.

We call such pieces of meta information *stowaways*.

### Exporting data/program layout information

#### Stowaways

Any information that is available at compile time and possibly reflects 
characteristics of the target platform is hard to access during normal builds 
if not instrumentalizing the compiler which is currently only supported by
the clang compiler suite.

But if such information is stored in variables that are exported from 
compiled object files, it can easily be retrieved using *binutils* tools 
that are typically shipped with the gcc compiler - at least for Arduino.

Let's look at a simple example that demonstrates how the runtime address 
of a global variable can be exported.

```cpp
// Define a global variable and optionally initialize it. We assume here
// that a_variable is protected against linker garbage collection due
// to being used in some place by the firmware.
//
uint8_t a_variable = 42;

// Mark the address symbol as extern to force the compiler to export it in
// the object file.
//
extern const void* a_variable_address;

// Store the address of a_variable. The pointer a_variable_address
// will later be removed during garbage collection.
//
const void* a_variable_address = &a_variable;
```

Let's assume we are given a set of build artifacts (the object files and the
.elf file). Furthermore, we have been told that the address of a specific
variable used by the firmware is stored at compile time in 
`a_variable_address`. But we are not told the name of the actual variable 
which in the example above is `a_variable`. 

Our task is to retrieve the runtime address of that variable based on the known
`a_variable_address`.

First parse the relocation records that are part of the output of `objdump -x` 
applied to the sketch compilation unit. This gives use the name of the 
symbol whose address is assigned to variable `a_variable_address` which is
of course `a_variable`.

Next we parse the linker map file that was generated during the final 
linker step that generated the firmware .elf file. There we can find the 
load address of `a_variable`.

This is fairly straight forward for intrinsic global variables. But for data 
members of global objects this is not that simple. The reason is that from
a technical point of view, object members are not symbols, only the objects
themselves are treated as symbols in post-compilation world.

That's why for members of global class objects we need to know the byte 
offset from the beginning of the memory area that is occupied by their 
containing object.

For members of global class objects, we also have to parse the member's 
byte offset from the beginning of the memory area that is occupied by the
object. Fortunately, this information can also be collected at compile time
and also be exported as stowaway data. So let's assume we know the 
`member_offset` of a specific class member.

We now can determine the runtime address (the actual address used 
during program execution) as follows.
```
runtime_address = load_address + member_offset - ram_start_address
```

For intrinsic typed global variables, `member_offset` is always zero.

Harvard architectures such as AVR, have different address spaces for RAM and
program memory. Those require a non zero `ram_start_address` to be subtracted
from the datum's load address. For the AVR architecture `ram_start_address` 
is equal to `0x800000`.

A runtime address thus computed is the actual value that is stored as a 
pointer at runtime. If this address is known, dereferencing it enables 
assignment to the right location in memory. An address known to the host 
can thus passed to the device via serial communication together with a
value to be assigned. The device then does the rest.

The really nice aspect of the stowaway approach is that we can thus access 
any information that is known at compile time by post processing the build 
artifacts. All instrumentation code that we have to add to make this possible 
will automatically be eliminated during garbage collection. This means that
no matter how restricted the device resources are in terms of PROGMEM, we
can export as much information as we like, as bloating the build artifacts
does not cause any changes to the size of the final firmware binary.

### Exporting function addresses

Exporting function addresses works very similar as exporting data addresses.
Only the `ram_start_address` needs to be omitted. Also the value of the 
runtime address of a function must be divided by a power of two 
as PROGMEM pointers may be counted as words instead of as bytes. 

For the AVR architectures this e.g. means that we have to divide by 
two.

#### Stowaway meta data export

Apart from exporting addresses of variables and functions or member offsets
for member data of global objects, we want to export other pieces of 
meta information that e.g. enable validation of input data on the host.

The following meta information is exported by the remote call API for every
variable (input/function argument/function result).

* name [string]
* address
* data size [no. bytes]
* data type [enum]
* description [string] (optional)

As already mentioned: None of this information generates any runtime costs.

It is important to note that the presented stowaway approach elegantly solves the
task of making platform dependent information accessible that is otherwise
hard to access as only known to the compiler. It also helps to eliminate
non-standardized compiler behavior as e.g. variable alignment.

Only the compiler e.g. knows about data type sizes and alignment requirements
of types such as `int`. By making load addresses available, we just do not
need to care for the compilers alignment decisions and any related issues
are avoided automatically.

And if we export the storage size and the acceptable value range of  
variables we have all we need to validate any input value that is meant to
be assigned to an exported variable.
We can e.g. check if a value for a variable of type `int8_t` is within the range 
[-128; 127] and reject it otherwise. All that can be done on the host where
resources are typically far less limited than on the device and thus safe
a lot of device resources as such validation operations are quite costly
if implemented for all supported data types.

### Type safe remote PEEK/POKE

To set the value of an exported global device variable, the host sends 
the variable's address, an enumerator value that codes the variables data 
type and the value to be assigned to the host. As the assigned value has 
been checked beforehand by the host, it is guaranteed to match the data type and
the acceptable value range.

Any values, regardless of their actual type, are transferred as 
unsigned data via the serial communication interface. The device 
casts them appropriately depending on the value of the data type enumerator
value it receives.

This very simple type safe PEEK/POKE-style transfer protocol safes a lot
of resources when compared to what Focus event handlers typically do.

### Remote function calls

When looking at the way we remotely peek and poke variable values, it 
is obvious that the device needs to allocate some program memory for 
the case distinction between data types as we have to distinguish between 
the data sizes of different data types and cast any values received via serial
communication to the appropriate value. This is not a big problem as the number
of different data types that we need to support is fairly limited.

When looking at functions, this is not the case. When browsing through the 
firmware code, one can find a very large number of different function signatures
(in C++ that means a combination of return value, arguments and `const` qualifiers).
If would not make any sense to define a mapping between function signatures and
enumerations values and then let the device do a case distinction and make it 
cast function pointers to appropriate type. This would be way too costly
in terms of resources and maintenance overhead.

And we would need to go this way as there is no portable way of calling
a function directly via its pointer. Some platforms use the stack to
pass function arguments, others use registers or even a combination of 
both.

Because of that, we have to adopt a completely different approach. Instead,
we use wrapper procedures (void functions without arguments) and exchange
function arguments and results via dedicated RAM areas.

Now, we have just a single function type, actually a procedure, i.e.
a function with void result and without any call arguments. Pointers to
such procedures can now be passed by the host to the device and there 
directly called.

For every wrapper procedure the host also knows the properties of the function
argument and result variables that it can access via peek/poke communication
as described above.

Calling a remote call function is, thus, a three step process.

1) The host remote-pokes the function arguments.
2) The host sends the address of the wrapper procedure to the device where it 
is called (this typically changes the return value variables)
3) The host peeks the return values on the device.

Here's a basic code example that would allow such type of communication. The
part of the code that deals with peek/poke/call has been omitted.

```cpp

uint8_t someDeviceFunction(uint16_t arg) {
   // does something with arg and returns a value...
}

uint16_t result, arg;

void someDeviceFunction_remoteCallWrapper() {

   // The argument `arg` has automagically been assigned a value by 
   // the calling instance
   //
   result = someDeviceFunction(arg);
   
   // The calling instance will take care to pass the value of `result`
   // back to the host.
}
```

### Shared results/args structs

One aspect that needs some additional thought is that fact that 
different functions might have different number and type of arguments 
and return values. Also if we export many different functions that
means that we have to spend a lot of RAM to store the functions' arguments and
return values. On an embedded system, RAM is one of the most limited
resources.

Fortunately, there is a simple solution - at least for most embedded systems.
A single tasking system can only execute one function at a time.
That implies that all functions' argument and results can share a common 
memory area.

The following code example explains how this can be implemented in the most
simple way. Let's assume we have two different functions `f1` and `f2` that
we both want to export and that we want to share their arguments and results.

```cpp
uint8_t f1(uint16_t arg) { ... }

cRGB f2(uint8_t arg1, uint8_t arg2) { ... }

struct F1Args {
   uint16_t arg;
};

struct F1Results {
   uint8_t res;
};

struct F2Args {
   uint8_t arg1, arg2;
};

struct F2Results {
   uint8_t r, g, b;
};

union SharedIOData {
   F1Args f1args;
   F1Results results;
   F2Args f2args;
   F2Results f2results;
};

// shared_io_data is just large enough to hold all sets of function arguments 
// and results.
//
SharedIOData shared_io_data;

void f1_remoteCallWrapper() { ... }

void f2_remoteCallWrapper() {

   // Cast the shared buffer to the appropriate struct type for arguments 
   // and results.
   //
   // Important: The memory area pointed to by `results` overlaps with 
   //            the area pointed to by `args`. Because of that,
   //            all arguments must be read from RAM before results can be
   //            assigned.
   //
   auto args = static_cast<F1Args *>(&shared_io_data);
   auto results = static_cast<F1Results *>(&shared_io_data);
   
   cRGB r_tmp = f2(args->arg1, args->arg2);
   
   results->r = cRGB.r;
   results->g = cRGB.g;
   results->b = cRGB.b;
}

```

Although the wrapper procedure `f2_remoteCallWrapper` seems to contain 
a lot of code, it actually compiles to a very small number of instructions when
an optimizing compiler like gcc is applied. This makes our function wrapper
approach pretty cheap in terms of PROGMEM.

A nice side effect of passing arguments/results via memory to wrapper
procedures is that we can have more than one function return value. But 
on the other hand in some cases we couldn't do without this multi return value
approach. This is because we have to export every intrinsic data type member 
of call arguments and return values individually. Instead of e.g. passing 
a `cRGB` struct, we have to pass all three color values individually.
   
It would be very tedious to hand code the shared union approach for more than
two exported functions. Because of that, the remote call API uses template 
union specialization to automatically define a global `SharedIOData` union
type for **all** functions exported for remote call. All that is nicely hidden
behind the macros of the remote call API.

### Protecting exported function against linker garbage collection

There is another problematic aspect of remote call wrapper procedures like 
`f1_remoteCallWrapper` from previous section's example. They are only 
meant to be used for remote calls and they are only called via a pointer 
to the procedure (by the RemoteCall plugin). The procedure's symbol (name)
is not used in any part of the code that is called directly or indirectly
by the firmware's `main(...)` function.

This makes the remote call wrapper procedures premium candidate for
being regarded as garbage by the linker.

There are some hacker tricks to prevent garbage collection for individual
symbols, e.g. by using custom linker scripts or by setting symbols' visibility 
to default by using gcc compiler attributes. 

Unfortunately both approaches have severe drawbacks. The former would require 
very ugly tweaking of Kaleidoscope's build system and the
latter is not portable as e.g. not supported by AVR.

So the only solution to prevent a symbol from being garbage collected is
to have the firmware actually use it. In this context the term *use* 
can be replaced by *call* for function and by *assign* or *read* for variable 
symbols.

But calling a function unintendedly during runtime, just to protect it against 
garbage collection? Adding dummy reads or writes to exported variables?
That sounds stupid and can be expected to cause side effects if an exported 
function is not stateless.

Fortunately, for the linker not to eliminate a symbol, *use* can also mean 
to read the address of the symbol. Reading and doing nothing with the
symbol already suffices to prevent the symbol from being eliminated.

But this causes another problem as such a kind of operation will 100% sure
be optimized away by the compiler that can easily see that the operation 
has no effect.

Luckily, in C/C++ there is still a way out of this dilemma, which is using 
inline assembler. Inline assembler code is neither touched nor eliminated 
by the compiler. Thus, by loading a the pointer to a symbol that we want 
to protect through an inline assembler operation, we are on the safe side. 
It even suffices to just load one byte of the symbol's address.

Here's a avr-gcc code example that accesses the lower byte of 
the address of the wrapper procedure `f1_remoteCallWrapper`.

```cpp
asm volatile("ldi r26, lo8(%0)" :: "p" (f1_remoteCallWrapper));
```

This assembles to only two bytes that store the opcode and address of the 
load instruction. We thus have to invest exactly two bytes of PROGMEM 
for every symbol that we intent to protected against garbage collected.
Not for free but not very costly either.

To automatically protect procedures exported via the remote call API 
against garbage collection, we use a combination of template specialization 
and recursive namespace traversal. Everything is again nicely hidden
behind the API's macros.

See the implementation of `_KRC_EXPORT_SYMBOLS_OF_NAMESPACE_AUX`
in `kaleidoscope/remote_call.h` for details about this approach.

### Leave it to the sketch

One important object of the design of the remote call API was to allow the 
the entire system to be disabled. 

Disabling the remote call infrastructure is achieved by defining a 
flag macro at the top of the firmware sketch .ino file. As no other compilation 
unit will ever see the sketch, any configuration that is defined in the sketch 
is not available to other compilation units at compile time.

In consequence this means that all code that is related to remote call export 
must be part of the sketch's compilation unit. 

For being able to export inputs and functions from any other
part of the firmware, regardless if core or plugins, remote call 
export specifications must be restricted to headers as those can be included 
by the sketch (compilation unit).

On the other hand that means, that we have to prevent other compilation units 
to compile the same code which would clearly lead to multiply defined symbols 
and in consequence to linker errors.

Because of that, we have to ensure that in any other compilation units
any remote call macro invocations will evaluate empty. This is achieved
by wrapping all remote call API invocations in an invocation of 
macro `KALEIDOSCOPE_REMOTE_CALL(...)`. This macro is then 
conditionally empty or non-empty based on the flag macro `KALEIDOSCOPE_SKETCH`.
As the latter is only defined in the sketch compilation unit 
`KALEIDOSCOPE_REMOTE_CALL(...)` only there will be defined non-empty and 
any wrapped API calls will thus only be compiled in the sketch.

### Disabling remote call entirely

Disabling the remote call infrastructure entirely means function macro 
`KALEIDOSCOPE_REMOTE_CALL(...)` being an empty macro for all compilation units.
This is caused by `KALEIDOSCOPE_REMOTE_CALL_DISABLED` being defined at 
the top of the sketch.

## Resource Consumption

The remote call interface can replace Focus event handlers by a set of 
thin wrappers around existing firmware functions. The resulting reduction of 
PROGMEM consumption is demonstrated by means of a comparison between 
the Focus event handler of the `LEDPaletteTheme` plugin with an 
equivalent set of remote call function exports.

The original Focus code reads as follows.

```cpp
EventHandlerResult LEDPaletteTheme::onFocusEvent(const char *command) {
  if (!Runtime.has_leds)
    return EventHandlerResult::OK;

  const char *cmd = PSTR("palette");

  if (::Focus.handleHelp(command, cmd))
    return EventHandlerResult::OK;

  if (strcmp_P(command, cmd) != 0)
    return EventHandlerResult::OK;

  if (::Focus.isEOL()) {
    for (uint8_t i = 0; i < 16; i++) {
      cRGB color;

      color = lookupPaletteColor(i);
      ::Focus.send(color);
    }
    return EventHandlerResult::EVENT_CONSUMED;
  }

  uint8_t i = 0;
  while (i < 16 && !::Focus.isEOL()) {
    cRGB color;

    ::Focus.read(color);
    color.r ^= 0xff;
    color.g ^= 0xff;
    color.b ^= 0xff;

    Runtime.storage().put(palette_base_ + i * sizeof(color), color);
    i++;
  }
  Runtime.storage().commit();

  ::LEDControl.refreshAll();

  return EventHandlerResult::EVENT_CONSUMED;
}
```

The first part of this handler deals with providing command help. This is 
something that does not cause any code in the remote call version as 
any documentation is exported via stowaways and thus not part of the
final firmware binary.

The LEDPaletteTheme's Focus event handler method compiles to 284 bytes 
of PROGMEM.

An equivalent set of exported functions that enable querying and 
setting palette colors individually reads as follows.

It compiles in total to only 56 bytes of PROGMEM and requires four bytes of RAM
(for function arguments and results) that are shared with any other 
exported remote call functions.

```cpp
KALEIDOSCOPE_REMOTE_CALL(
   KRC_ROOT_PACKAGE(plugin,
      KRC_PACKAGE(LEDPaletteTheme,
         KRC_F(getPaletteColor, 
               ((uint8_t, red), (uint8_t, green), (uint8_t, blue)),
               ((uint8_t, palette_index)),
            (
             cRGB color;
             color = LEDPaletteTheme.lookupPaletteColor(args->palette_index);
             results->red = color.r;
             results->green = color.g;
             results->blue = color.b;
            ),
            KRC_DESCRIPTION("Retrieves the RGB value of a palette color")
         )
         KRC_F(setPaletteColor, KRC_NO_RESULTS,
               ((uint8_t, palette_index), (uint8_t, red), (uint8_t, green), (uint8_t, blue)),
            (
              LEDPaletteTheme.setPaletteColor(args->palette_index,
                 cRGB{args->red, args->green, args->blue});
            ),
            KRC_DESCRIPTION("Sets the RGB value of a palette color")
         )
         KRC_F(commitPalette, KRC_NO_RESULTS, KRC_NO_ARGS,
            (
              Runtime.storage().commit();
              ::LEDControl.refreshAll();
            ),
            KRC_DESCRIPTION("Sets the RGB value of a palette color")
         )
      )
   )
)
```

Comparing 284 to 56 bytes means an amount of 228 bytes of PROGMEM 
being saved or a reduction of PROGMEM consumed by this feature by roughly 80%.

When taking a closer look at the above example, one will notice that both
solutions do not do exactly the same thing. The original Focus-based version
retrieves and sets all palette colors at the same time while the remote call-based
version allows for individual colors to be set. That's why it has to 
export a function `commitPalette` that is meant to be called for changes 
to palette values to become effective. 

The reader might agree that the latter version (remote call) is more versatile.

For the sake of fairness against the Focus based solution it is important to
mention that the RemoteCall plugin requires an additional 554 bytes of PROGMEM.

So let's see how the net reduction of PROGMEM consumption would look like.

Currently the stock firmware dedicates ~1200 bytes to Focus handlers. Let's 
assume that all that binary code could also be reduced to approximately 20%
by using remote call exports, this would leave us with roughly 240 bytes.

We now need to add the 554 bytes for the RemoteCall plugin without which
the exported remote calls would be useless, this would sum up to ~800 bytes
of PROGMEM being used for serial communication.

The size of the stock firmware binary could thus be reduced by ~400 bytes 
by switching from Focus event handlers to remote call exports.

The more serial communication with purpose of remote control we would 
add in the future, the more significant these savings will become.

## Future enhancements/TODOs

There are some possible features of the remote call interface that are 
currently not easy to achieve but might be added in the future when 
Kaleidoscope possibly switches to an alternative build system. 

### Firmware checksums

The remote call API provides advanced features such as type safety 
and value range checks at minimum resource overhead. This is achieved at the
cost of separating the interface specification (YAML-file) from the firmware 
binary. Only when having access to a matching YAML-file, remote communication
can safely be established.

To make sure that a YAML-interface definition exactly matches the 
configuration of the firmware running on the device, a firmware checksum 
could be used during a communication handshake. This checksum would be computed
based on the firmware object file and then stored in the final firmware binary
(using binutils).

This feature is already implemented by currently disabled.

The reason is that we cannot add the *scan_packages* post processing step 
to Kaleidoscope's Arduino build system. That's because *scan_packages*
is a Python script and Python cannot be assumed to be available on all
build platforms.

If the build system would be Python driven, which would make Python generally 
available, we could simply run *scan_packages* as an additional build step 
between the generation of the firmware .elf file and the objcopy step that 
generates the .hex file.

An alternative would be to re-implement *scan_packages* in a (script) language
that is supported on all platforms. Currently there's no such ubiquitous
language available.

The computed firmware checksum would be stored on the device and in the 
YAML file. Any tool that re-implements the remote call communication 
between host and device in the same way as it is done by the *remote* tool
could then verify the interface compatibility during a serial 
communication handshake.

### Replace focus-test

Currently the *remote* tool relies on Focus and uses the shell script 
`Kaleidoscope/bin/focus-test` under the hood. 

To replace this external call, it would be preferable to add the functionality 
of *focus-test* to *remote_call*'s Python implementation and 
also to auto-detect the `DEVICE` address which is currently not done by 
*focus-test*.

### Interface definition stored in the firmware

At first glance this sounds weird. Isn't the remote call interface designed
in a way to be as resource friendly as possible? True. And doesn't storing
the interface definition on the device mean that we loose a lot of PROGMEM?
Wasn't transfering as much information and work as possible to the host the
point of all this? True and not true...

Not all devices have highly limited resources. For those devices where 
we can afford it, it would be easy to store the entire YAML file or a 
compressed version of its content in PROGMEM. The host could then request it via 
serial communication and use it to safely communicate.

Storing the YAML file content in the firmware binary could e.g. done the following way:

1) Build all objects and link the firmware .elf file.
2) Post-process with *scan_packages* and extract the interface definition.
3) Compress the extracted interface definition.
4) Generate a simple code file that only defines a global array `interface_definition` that is large
   enough to hold the compressed interface definition.
5) Link the firmware again, considering the additional object holding the array.
6) Post-process again with *scan_packages*. The package information will be slightly different (addresses) but of same size.
7) Replace the content of array `interface_definition` by the actual compressed interface definition, using `objcopy --update-section`.

This would of course require a build system that is flexible enough to
add the additional steps.

Any remote call tool running on the host would then query first if an
on-board version of the interface definition is available and only 
try reading it from a file otherwise.

By this means we would not need a rather complex implementation that stores
the interface definition in a C++ data structures. No code on the device 
would be required to service its information to the host.

Instead, the host queries the compressed interface definition, uncompresses it
and then starts communicating. 

That way remote calls would be independent from a interface description in a file
but still not too much resources would be wasted.

Tests show that a 244 lines YAML file that requires roughly 8.6K storage can be
zip-compressed to ~1.1K.

### Enable virtual address spaces

The current implementation of the remote call API assumes devices to work with 
direct addressing.

Devices using virtual addressing could be supported as well. This would 
mean to replace absolute symbol address export with relative addresses (offsets).
Those offsets could be computed at compile time with respect to a pivot 
variable whose address could also be determined at runtime. That way,
knowing the runtime address of the pivot variable and adding another variables
offset address, it would theoretically become possible to determine the 
absolute addresses of exported symbols even though program and data would
reside at different places in memory for every firmware run.

## Additional Documentation

Apart from this document, the remote call API is documented in the file
`kaleidoscope/remote_call.h`. There's also a set of example sketches available 
under `examples/RemoteCall` in the Kaleidoscope repository.
