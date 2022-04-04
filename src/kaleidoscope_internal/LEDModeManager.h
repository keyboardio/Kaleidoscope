/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2013-2018  Keyboard.io, Inc.
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

#include <Arduino.h>  // for PROGMEM, memcpy_P, pgm_read_byte
#include <stddef.h>   // for size_t
#include <stdint.h>   // for uint8_t

#include "kaleidoscope/macro_helpers.h"                    // for __NL__, ADD_TEMPLATE_BRACES_0
#include "kaleidoscope/plugin.h"                           // for Plugin
#include "kaleidoscope/plugin/LEDMode.h"                   // for LEDMode, LEDControl
#include "kaleidoscope/plugin/LEDModeInterface.h"          // for LEDModeInterface
#include "kaleidoscope_internal/array_like_storage.h"      // IWYU pragma: keep
#include "kaleidoscope_internal/type_traits/has_method.h"  // for DEFINE_HAS_METHOD_TRAITS

#if defined(KALEIDOSCOPE_VIRTUAL_BUILD) || defined(ARDUINO_ARCH_STM32)
#include <new>  // for operator new
#else

// To enable placement new, we need to supply a global operator
// function.
//
inline void *operator new(size_t, void *__p) throw() {
  return __p;
}
#endif

namespace kaleidoscope {

namespace plugin {
// Forward declarations to avoid header inclusions
class LEDControl;        // IWYU pragma: keep
class LEDModeInterface;  // IWYU pragma: keep

}  // namespace plugin

namespace internal {
namespace led_mode_management {

template<bool T>
struct Bool2Type {};

// Functions of this type allocate a new LED mode within a global
// buffer and give it access to it's parent plugin.
// This allows a transient LED mode to access member data of its parent
// plugin, which can help to reduce its RAM footprint.
// Some transient LED modes might ignore the parent pointer entirely, e.g. if
// they do not have a configurable internal state.
//
// We use a factory function instead of a polymorphic
// class to generate LED modes. By this means we can avoid the additional
// cost of vtables which safes some program memory.
// Conceptually, the factory function approach is similar to having
// a polymorphic class with only one method, that is accessed directly
// instead of the detour via the vtable.
//
typedef kaleidoscope::plugin::LEDMode *(*LEDModeFactoryFunc)(void *raw_buffer,
                                                             kaleidoscope::plugin::LEDModeInterface *parent_plugin,
                                                             uint8_t mode_id);

template<typename ParentPlugin__>
inline void registerLEDModeActivated(Bool2Type<true>,
                                     ParentPlugin__ *parent_plugin,
                                     uint8_t mode_id) {
  parent_plugin->registerLEDModeActivated(mode_id);
}

template<typename ParentPlugin__>
inline void registerLEDModeActivated(Bool2Type<false>,
                                     ParentPlugin__ *parent_plugin,
                                     uint8_t mode_id) {
  // Noop
}

DEFINE_HAS_METHOD_TRAITS(Plugin,
                         /* registerLEDModeActivated not templated */ (),
                         (),
                         registerLEDModeActivated,
                         void,
                         (uint8_t led_mode_id))

// A templated implementation of LEDModeFactoryFunc.
//
// Please note that the actual type of ParentPlugin__ is only
// known to the context where the function is invoked or in our case
// the function pointer to generateLEDMode is generated (at compile time).
//
// The caller can be completely agnostic of ParentPlugin__ an just pass
// a pointer to kaleidoscope::Plugin for argument parent_plugin.
//
// The function generateLEDMode knows the actual type of parent_plugin.
// Or, more precise, we can rely on the fact that we store a pointer to a plugin
// of appropriate type together with the generateLEDMode-pointer in
// an LEDModeFactory instance in PROGMEM. Thus, generateLEDMode will only
// be called with a parent_plugin of appropriate type.
//
template<typename ParentPlugin__>
static kaleidoscope::plugin::LEDMode *
generateLEDMode(void *raw_buffer,
                kaleidoscope::plugin::LEDModeInterface *parent_plugin,
                uint8_t mode_id) {
  // We know the type of the parent plugin via the template parameter
  // ParentPlugin__, thus it is safe to cast to the actual type.
  //
  auto parent_plugin_actual = static_cast<ParentPlugin__ *>(parent_plugin);

  // Types defined by template parameters like ParentPlugin__ must
  // be declared explicitly using the "typename" keyword.
  // Because of this lengthy syntax, we use a typedef to
  // shorten the constructor call below.
  //
  typedef typename ParentPlugin__::TransientLEDMode TLM;

  // Generate a transient LED mode within the LED mode buffer.
  //
  auto led_mode_ptr = new (raw_buffer) TLM{parent_plugin_actual};

  constexpr bool accesses_transient_led_mode = Plugin_HasMethod_registerLEDModeActivated<ParentPlugin__>::value;

  // Register the newly created LED mode with its parent plugin.
  // Please note that this call is optimized away by the compiler
  // for all those plugins that do not reimplement registerLEDModeActivated.
  //
  registerLEDModeActivated(Bool2Type<accesses_transient_led_mode>(),
                           parent_plugin_actual,
                           mode_id);

  return led_mode_ptr;
}

// A data structure that contains all the information that is required
// to generate a transient LED mode. Instances of this class are stored
// in an array-like data structure in PROGMEM.
//
struct LEDModeFactory {

  // Don't try to make this a class with a constructor. This
  // will turn it into a non POD which cannot be used as
  // return type of a constexpr.

  bool isPersistentLEDMode() const {
    return !generate_led_mode_;
  }

  kaleidoscope::plugin::LEDMode *getPersistentLEDMode() const {
    return static_cast<kaleidoscope::plugin::LEDMode *>(parent_plugin_);
  }

  kaleidoscope::plugin::LEDMode *generateTransientLEDMode(
    uint8_t *buffer, uint8_t mode_id) const {
    return (*generate_led_mode_)(buffer, parent_plugin_, mode_id);
  }

  bool isAssociatedWithPlugin(kaleidoscope::plugin::LEDModeInterface *plugin) const {
    return parent_plugin_ == plugin;
  }

  kaleidoscope::plugin::LEDModeInterface *parent_plugin_;
  LEDModeFactoryFunc generate_led_mode_;
};

// The traits class remove_pointer is part of the C++ standard library
// but not present on Arduino.
//
template<class T>
struct remove_pointer {
  typedef T type;
};
template<class T>
struct remove_pointer<T *> {
  typedef T type;
};

enum PluginType {
  PluginType_NoLEDMode,
  PluginType_PersistentLEDMode,
  PluginType_TransientLEDMode
};

// The following three functions enable determining the PluginType
// at compile time by examining a constexpr pointer to one of the
// global plugin instances (pointers to global variables/objects are
// constexpr).
//
static constexpr int ledModePluginType(kaleidoscope::plugin::LEDMode *) {
  return PluginType_PersistentLEDMode;
}

static constexpr int ledModePluginType(kaleidoscope::plugin::LEDModeInterface *) {
  return PluginType_TransientLEDMode;
}

// Invoked for all plugins that inherit neither from LEDMode
// nor from LEDModeInterface.
//
static constexpr bool ledModePluginType(void *) {
  return PluginType_NoLEDMode;
}

// The following traits classes are used to distinguish between three cases
//
// 1) A plugin is unrelated to LEDModes (i.e. non of the other two applies).
//
// 2) A plugin is an old syle persistent LEDMode (plugin is derived from
//    LED mode = persistent LED mode)
//
// 3) A plugin exports a LED mode with dynamic lifetime (= transient LED mode)

// Checks if a plugin is related to LED modes in any kind of ways.
// This traits check if for a plugin
// an entry in the plugin factories array needs to be reserved.
//
constexpr bool pluginControlsLEDMode(int led_mode_plugin_type) {
  return led_mode_plugin_type != PluginType_NoLEDMode;
}

// The template GenerateLEDModeFactory generates LED mode factories
// in three different ways, depending on the type of a plugin (PluginType)

// Generates a dummy factory (discarded at compile time) for
// those plugins that are unrelated to LED modes. This template
// is selected for LEDModePluginType__ == NoLEDMode.
//
template<int LEDModePluginType__>
struct GenerateLEDModeFactory {

  static constexpr LEDModeFactory apply(kaleidoscope::Plugin * /* non LED mode plugin*/) {
    return LEDModeFactory{nullptr, nullptr};
  }
};

template<>
struct GenerateLEDModeFactory<PluginType_TransientLEDMode> {

  // This version of apply must be templated as we must know the actual
  // type of plugin as the instanciatioin of template function
  // generateLEDMode depends upon it.
  //
  template<typename Plugin__>
  static constexpr LEDModeFactory apply(Plugin__ *plugin) {
    return LEDModeFactory{
      plugin,
      generateLEDMode<Plugin__>  // pointer to template instantiation of
      // generateLEDMode<...>
    };
  }
};

template<>
struct GenerateLEDModeFactory<PluginType_PersistentLEDMode> {

  static constexpr LEDModeFactory apply(kaleidoscope::plugin::LEDModeInterface *plugin) {

    // For persistent LED modes, we use the LED factory to simply store
    // the plugin pointer (a persistent LED mode is itself the LEDMode).
    // Thus, no factory function is required.
    //
    return LEDModeFactory{plugin, nullptr};
  }
};

// The following template TransientLEDModeSize is used to determine
// the size of a transient LED mode. It is only active for dynamic
// LED modes. For all other plugins, it simply returns zero.
//
template<typename PluginPtr__, int LEDModePluginType__>
struct TransientLEDModeSize {
  static constexpr size_t value = 0;
};

template<typename PluginPtr__>
struct TransientLEDModeSize<PluginPtr__, PluginType_TransientLEDMode> {
  typedef typename remove_pointer<PluginPtr__>::type Plugin__;
  static constexpr size_t value = sizeof(typename Plugin__::TransientLEDMode);
};

// This helper class serves to determine the maximum memory footprint
// of any transient LED mode in the sketch.
// It implements a template type recursion
// that examins all specified LED modes. For this calculation
// it is no problem that also the type NoLEDMode is considered
// in this check as it is an empty class (size == 1 byte) and
// thus does not affect the maximum size computation.
//
template<typename PluginPtr__, typename... MorePluginPtrs__>
struct TransientLEDModeMaxSize {
  static constexpr size_t this_size = TransientLEDModeSize<PluginPtr__, ledModePluginType(PluginPtr__())>::value;

  static constexpr size_t nested_size = TransientLEDModeMaxSize<MorePluginPtrs__...>::value;

  static constexpr size_t value = (this_size > nested_size) ? this_size : nested_size;
};

// Specialization to end recursion
//
template<typename PluginPtr__>
struct TransientLEDModeMaxSize<PluginPtr__> {
  static constexpr size_t value = TransientLEDModeSize<PluginPtr__, ledModePluginType(PluginPtr__())>::value;
};

}  // namespace led_mode_management

class LEDModeManager {
 public:
  // Everything in this class private on purpose.
  //
  // Only the class LEDControl is supposed to gain
  // access to any inventory of this class to ensure that
  // the handling of transient LED modes is safely handled
  // by a well defined central instance.
  //
  friend class kaleidoscope::plugin::LEDControl;

  static uint8_t numLEDModes();

#if 0
  // This method could be used in rare cases
  // where a stack allocation of a LED mode may not be avoided.
  //
  template<typename ParentPlugin__>
  static auto
  generateLEDModeTemporary(void *raw_buffer,
                           ParentPlugin__ *parent_plugin
                          ) -> typename ParentPlugin__::TransientLEDMode * {
    auto led_mode = static_cast<typename ParentPlugin__::TransientLEDMode *>(
                      led_mode_management::generateLEDMode<ParentPlugin__>(
                        raw_buffer,
                        parent_plugin,
                        0 /*dummy*/
                      )
                    );
    // dummy to disable access to transient LED modes through LEDControl
    parent_plugin->registerLEDModeActivated(255);

    return led_mode;
  }
#endif

 private:
  // For the sake of convenience make type LEDModeFactory
  // available in class namespace
  //
  typedef led_mode_management::LEDModeFactory LEDModeFactory;

  static kaleidoscope::plugin::LEDMode *getLEDMode(uint8_t mode_id);

  static void retreiveLEDModeFactoryFromPROGMEM(uint8_t mode_id,
                                                LEDModeFactory &fac);

  static void setupPersistentLEDModes();

  // Persistent LED mode plugins are derived from kaleidoscope::plugin::LEDMode.
  // The standard dictates that for them this more specialized overload
  // of setupLEDMode is supposed to be called instead of the more
  // general one below.
  //
  static void setupLEDMode(kaleidoscope::plugin::LEDMode *persistent_led_mode) {
    persistent_led_mode->setup();
  }

  static void setupLEDMode(kaleidoscope::Plugin * /*not_a_persistent_led_mode*/) {}

  static uint8_t led_mode_buffer_[];
};

}  // namespace internal
}  // namespace kaleidoscope

// clang-format off

// Some auxiliary macros that are mapped to the list of
// plugins defined via KALEIDOSCOPE_INIT_PLUGINS follow.

// Evaluates to a boolean value that signals whether
// a plugin is related to LED modes and thus needs to be considered
// during setup of the LED mode factory array.
//
#define _LED_MODE_MANAGEMENT__PLUGIN_CONTROLS_LED_MODE(PLUGIN)                 \
   pluginControlsLEDMode(ledModePluginType(&::PLUGIN))

// Generates a LEDModeFactory for each plugin. For all those
// plugins that do not export transient LED modes, a nullptr-initialized
// factory is created. This does not mean a performance penalty, as
// those empty factories are then directly discarded, still at compile time.
//
#define _LED_MODE_MANAGEMENT__GENERATE_LED_MODE_FACTORY(PLUGIN)                \
   kaleidoscope::internal::led_mode_management                          __NL__ \
      ::GenerateLEDModeFactory<ledModePluginType(&::PLUGIN)>::apply(&::PLUGIN)

// Retreive the pointer type of the exported transient LED mode of a plugin.
//
#define _LED_MODE_MANAGEMENT__PLUGIN_PTR_TYPE(PLUGIN)                          \
   decltype(&::PLUGIN)

#define _LED_MODE_MANAGEMENT__SETUP_STATIC_LED_MODE(PLUGIN)                    \
   LEDModeManager::setupLEDMode(&::PLUGIN);

#define _INIT_LED_MODE_MANAGER(...)                                            \
   namespace kaleidoscope { /* NOLINT(build/namespaces) */              __NL__ \
   namespace internal { /* NOLINT(build/namespaces) */                  __NL__ \
   namespace led_mode_management { /* NOLINT(build/namespaces) */       __NL__ \
                                                                        __NL__ \
      /* Setup the array-like data structure that stores                __NL__ \
       * LEDModeFactories */                                            __NL__ \
      typedef kaleidoscope::internal::ArrayLikeStorage<                 __NL__ \
               LEDModeFactory,                                          __NL__ \
               /* Map the list of global plugin                         __NL__ \
                * objects to a list of led mode                         __NL__ \
                * factories */                                          __NL__ \
               MAP_LIST(                                                __NL__ \
                  /* Generate a series of boolean values. True for      __NL__ \
                   * each plugin that is related to LED modes,          __NL__ \
                   * false otherwise. */                                __NL__ \
                  _LED_MODE_MANAGEMENT__PLUGIN_CONTROLS_LED_MODE,       __NL__ \
                  __VA_ARGS__                                           __NL__ \
               )                                                        __NL__ \
      > ArrayType;                                                      __NL__ \
                                                                        __NL__ \
      /* Generate the actual instance of template class                 __NL__ \
       * TypedPluginArray and initialize it with a list of              __NL__ \
       * LEDModeFactories.                                              __NL__ \
       */                                                               __NL__ \
      const PROGMEM ArrayType led_mode_factories(                       __NL__ \
               MAP_LIST(                                                __NL__ \
                  _LED_MODE_MANAGEMENT__GENERATE_LED_MODE_FACTORY,      __NL__ \
                  __VA_ARGS__                                           __NL__ \
               )                                                        __NL__ \
         );                                                             __NL__ \
   } /* end namespace led_mode_management */                            __NL__ \
                                                                        __NL__ \
      /* Store the number of LEDModeFactories.                          __NL__ \
       */                                                               __NL__ \
      const PROGMEM uint8_t num_led_modes                               __NL__ \
              = led_mode_management::ArrayType::n_entries;              __NL__ \
                                                                        __NL__ \
      uint8_t LEDModeManager::numLEDModes() {                           __NL__ \
         return pgm_read_byte(&num_led_modes);                          __NL__ \
      }                                                                 __NL__ \
                                                                        __NL__ \
      void LEDModeManager::retreiveLEDModeFactoryFromPROGMEM(           __NL__ \
              uint8_t mode_id,                                          __NL__ \
              LEDModeFactory &fac) {                                    __NL__ \
         memcpy_P(&fac,                                                 __NL__ \
            &reinterpret_cast<const LEDModeFactory*>(                   __NL__ \
               &led_mode_management::led_mode_factories                 __NL__ \
            )[mode_id],                                                 __NL__ \
            sizeof(LEDModeFactory)                                      __NL__ \
         );                                                             __NL__ \
      }                                                                 __NL__ \
                                                                        __NL__ \
      static constexpr size_t max_led_mode_size                         __NL__ \
         = led_mode_management::TransientLEDModeMaxSize<                  __NL__ \
              MAP_LIST(                                                 __NL__ \
                 _LED_MODE_MANAGEMENT__PLUGIN_PTR_TYPE,                 __NL__ \
                 __VA_ARGS__                                            __NL__ \
              )                                                         __NL__ \
           >::value;                                                    __NL__ \
                                                                        __NL__ \
      /* This buffer is dimensioned in a way that it can hold           __NL__ \
       * the largest of all transient LED modes defined in the sketch.    __NL__ \
       */                                                               __NL__ \
      uint8_t LEDModeManager::led_mode_buffer_[max_led_mode_size];      __NL__ \
                                                                        __NL__ \
      void LEDModeManager::setupPersistentLEDModes() {                      __NL__ \
              MAP(                                                      __NL__ \
                  _LED_MODE_MANAGEMENT__SETUP_STATIC_LED_MODE,          __NL__ \
                  __VA_ARGS__                                           __NL__ \
              )                                                         __NL__ \
      }                                                                 __NL__ \
   } /* end namespace internal */                                       __NL__ \
   } /* end namespace kaleidoscope */
