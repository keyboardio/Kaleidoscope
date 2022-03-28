/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2013-2019  Keyboard.io, Inc.
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

#include "kaleidoscope_internal/type_traits/type_traits"  // IWYU pragma: keep
// IWYU pragma: no_include <type_traits>

namespace kaleidoscope {
namespace sketch_exploration {

struct EmptyPluginTypeList {
  typedef void Plugin;
  typedef EmptyPluginTypeList Next;
  static constexpr int size     = 0;
  static constexpr bool is_last = true;
};

template<typename _T>
struct BareType {
  typedef typename std::remove_const<
    typename std::remove_reference<_T>::type>::type Type;
};

template<int _id, typename _Plugin, typename... _MorePlugins>
struct PluginTypeList {
  typedef typename BareType<_Plugin>::Type Plugin;

  static constexpr int id = _id;

  typedef PluginTypeList<_id + 1, _MorePlugins...> Next;

  static constexpr int size     = 1 + Next::size;
  static constexpr bool is_last = false;
};

template<int _id, typename _Plugin>
struct PluginTypeList<_id, _Plugin> {
  typedef typename BareType<_Plugin>::Type Plugin;

  static constexpr int id = _id;

  typedef EmptyPluginTypeList Next;

  static constexpr int size     = 1;
  static constexpr bool is_last = true;
};

template<typename... _Plugins>
auto makePluginTypeList(const _Plugins &...p) -> PluginTypeList<0, _Plugins...> {}

template<typename _PluginTypeList, int _id>
struct Entry {
  typedef typename Entry<_PluginTypeList, _id - 1>::Next::Plugin Type;
  typedef typename Entry<_PluginTypeList, _id - 1>::Next::Next Next;

  static constexpr int id = Entry<_PluginTypeList, _id - 1>::id + 1;
  static_assert(id == _id, "");

  static constexpr bool is_last = Entry<_PluginTypeList, _id - 1>::Next::is_last;
};

template<typename _PluginTypeList>
struct Entry<_PluginTypeList, 0> {
  typedef typename _PluginTypeList::Plugin Type;
  typedef typename _PluginTypeList::Next Next;

  static constexpr int id       = 0;
  static constexpr bool is_last = _PluginTypeList::is_last;
};

template<typename _PluginTypeList, typename _WantedPlugin, int _id>
struct OccurrencesAux {
  static constexpr int value =
    (std::is_same<typename Entry<_PluginTypeList, _id>::Type, _WantedPlugin>::value)
      ? OccurrencesAux<_PluginTypeList, _WantedPlugin, _id - 1>::value + 1
      : OccurrencesAux<_PluginTypeList, _WantedPlugin, _id - 1>::value;
};

template<typename _PluginTypeList, typename _WantedPlugin>
struct OccurrencesAux<_PluginTypeList, _WantedPlugin, 0> {
  static constexpr int value = (std::is_same<typename Entry<_PluginTypeList, 0>::Type, _WantedPlugin>::value)
                                 ? 1
                                 : 0;
};

template<typename _PluginTypeList, typename _WantedPlugin>
struct Occurrences
  : public OccurrencesAux<_PluginTypeList, _WantedPlugin, _PluginTypeList::size - 1> {};

template<typename _PluginTypeList, typename _WantedPlugin, int _id>
struct PluginPositionAux {
  static constexpr int value =
    (PluginPositionAux<_PluginTypeList, _WantedPlugin, _id - 1>::value != -1)
      ? PluginPositionAux<_PluginTypeList, _WantedPlugin, _id - 1>::value
    : (std::is_same<typename Entry<_PluginTypeList, _id>::Type, _WantedPlugin>::value)
      ? _id
      : -1;
};

template<typename _PluginTypeList, typename _WantedPlugin>
struct PluginPositionAux<_PluginTypeList, _WantedPlugin, 0> {
  static constexpr int value = (std::is_same<typename Entry<_PluginTypeList, 0>::Type, _WantedPlugin>::value)
                                 ? 0
                                 : -1;
};

template<typename _PluginTypeList, typename _WantedPlugin>
struct PluginPosition
  : public PluginPositionAux<_PluginTypeList, _WantedPlugin, _PluginTypeList::size - 1> {};

template<typename _PluginTypeList, typename _WantedPlugin>
struct IsRegistered {
  static constexpr bool value = (PluginPosition<_PluginTypeList, _WantedPlugin>::value != -1);
};

template<typename _PluginTypeList>
struct Plugins__ {

  static constexpr int size = _PluginTypeList::size;

  // C++11 does not allow for template specialization to havven
  // in non-namespace scope. Thus, we define those templates
  // in namespace scope and then using declare their types here.
  //
  template<int _id>
  using Entry = Entry<_PluginTypeList, _id>;

  template<typename _PluginType>
  using Position = PluginPosition<_PluginTypeList, _PluginType>;

  template<typename _PluginType>
  using IsRegistered = IsRegistered<_PluginTypeList, _PluginType>;

  template<typename _PluginType>
  using Occurrences = Occurrences<_PluginTypeList, _PluginType>;
};

}  // namespace sketch_exploration
}  // namespace kaleidoscope

// clang-format off

#define _FIX_PLUGIN_TYPE_AND_NAME_AMBIGUITIES(T) T

#define _INIT_PLUGIN_EXPLORATION(...)                                          \
                                                                               \
  /* We have to define this type list in global namespace as some plugins      \
   * exhibit ambiguities between their type name and the name of               \
   * their global instance :(                                                  \
   */                                                                          \
  typedef decltype(                                                            \
    kaleidoscope::sketch_exploration::makePluginTypeList( __VA_ARGS__)         \
  ) Kaleidoscope_SketchExploration__ConcretePluginTypeList;                    \
                                                                               \
  namespace kaleidoscope {                                                     \
  namespace sketch_exploration {                                               \
    class Plugins : public Plugins__<                                          \
      Kaleidoscope_SketchExploration__ConcretePluginTypeList> {};              \
  } /* namespace sketch_exploration */                                         \
  } /* namespace kaleidoscope */

// Convenience macro that deal with the ugly typedef and typename stuff...
//
#define ENTRY_TYPE(PLUGINS, ID)                                                \
    typename PLUGINS::template Entry<ID>::Type

#define ENRTY_IS_LAST(PLUGINS, ID) PLUGINS::template Entry<ID>::is_last
#define PLUGIN_POSITION(PLUGINS, TYPE) PLUGINS::template Position<TYPE>::value
#define PLUGIN_IS_REGISTERED(PLUGINS, TYPE) PLUGINS::template IsRegistered<TYPE>::value
#define NUM_OCCURRENCES(PLUGINS, TYPE) PLUGINS::template Occurrences<TYPE>::value
