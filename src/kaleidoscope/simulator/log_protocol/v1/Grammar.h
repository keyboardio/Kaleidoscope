/* -*- mode: c++ -*-
 * kaleidoscope::simulator::log_protocol -- A log protocol for keyboard I/O
 * Copyright (C) 2019  noseglasses (shinynoseglasses@gmail.com)
 * Copyright (C) 2020  Keyboard.io, Inc
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

#include "kaleidoscope/simulator/LogProtocol.h"

#include <stdint.h>

// This is version 1 of the Aglais protocol
//
// Document grammar:
//
// <command_string_XXX> ::= (character string version of command)
// <command_id_XXX> ::= (uint8_t id version of command)
// <XXX_cmd> ::= <command_string_XXX> | <command_id_XXX>
//
// <cycle_id> ::= (uint32_t)
// <time> ::= (uint32_t)
// <time_offset_16> ::= (uint8_t)
// <row> ::= (uint8_t)
// <col> ::= (uint8_t)
// <report_id> ::= (uint8_t)
// <data_length> ::= (uint8_t)
// <report_bytes> ::= (uint8_t)... (<data_length> times)
// <document_type> ::= (uint8_t)
// <protocol_version> ::= (uint8_t)
// <version_id> ::= (uint8_t)
// <id_string> ::= (double quoted string)
// <lbr> ::= '\n'
// <comment> ::= '#' (any characters except for '\n') <lbr>
// <start_time> ::= <time>
// <end_time> ::= <time>
// <start_cycle> ::= <start_cycle_cmd> <cycle_id> <start_time>
// <key_pressed> ::= <key_pressed_cmd> <row> <col>
// <key_released> ::= <key_released_cmd> <row> <col>
// <action_subcommand> ::= <key_pressed> | <key_released>
// <action> ::= <action_cmd> <action_subcommand>
// <keyboard_report> ::= <keyboard_report_cmd> <report_id> <data_length> <report_bytes>
// <reaction_subcommand> ::= <keyboard_report>
// <reaction> ::= <reaction_cmd> <reaction_subcommand>
// <end_cycle> ::= <end_cycle_cmd> <cycle_id> <end_time>
// <cycle_ar> ::= <action> | <reaction>
// <cycle_content> ::= <cycle_ar> | <cycle_ar> <lbr> <cycle_content>
// <cycle_long> ::= <start_cycle> <lbr> <cycle_content> <lbr> <end_cycle>
// <set_time> ::= <set_time_cmd> <time>
// <cycle_short> ::= <cycle_cmd> <cycle_id>
// <cycle_duration> ::= (uint32_t)
// <cycle_durations> ::= <cycle_duration> | <cycle_duration> <cycle_durations> # must match <cycles_count>
// <cycles_start_time> ::= <time>
// <cycles_start_id> ::= (uint32_t)
// <cycles_count> ::= (uint32_t)
// <cycles> ::= <cycles_cmd> <cycles_start_id> <cycles_start_time> <cycles_count> <cycle_durations>
// <cycle> ::= <cycle_short> | <cycle_long> | <cycles>
// <cycle_data> ::= <cycle> | <cycle> <lbr> <cycle_data> | <comment>
// <firmeware_id> ::= <firmeware_id_cmd> <id_string>
// <header_content> ::= <firmeware_id>
// <header> ::= <document_type> <lbr> <protocol_version> <lbr> <header_content>
// <document> ::= <header> <lbr> <cycle_data>

// The value of <document_type> defines whether string- or
// id-versions of commands are used

namespace kaleidoscope {
namespace simulator {
namespace log_protocol {
namespace v1 {

#define PROTOCOL_V1_COMMANDS(OP, SEP, TERM_SEP)                         \
  OP(none, 0)SEP                                                        \
  OP(firmware_id, 1)SEP                                                 \
  OP(start_cycle, 2)SEP                                                 \
  OP(action, 3)SEP                                                      \
  OP(reaction, 4)SEP                                                    \
  OP(end_cycle, 5)SEP                                                   \
  OP(set_time, 6)SEP                                                    \
  OP(cycle, 7)SEP                                                       \
  OP(cycles, 8)TERM_SEP

#define PROTOCOL_UINT8_T_STRUCT_DEF(NAME, ID)   \
   static constexpr uint8_t NAME = ID;

struct Command  {
  PROTOCOL_V1_COMMANDS(PROTOCOL_UINT8_T_STRUCT_DEF,,)
};

#define PROTOCOL_V1_SUBCOMMANDS(OP, SEP, TERM_SEP)                      \
  OP(none, 0)SEP                                                        \
  OP(key_pressed, 1)SEP                                                 \
  OP(key_released, 2)SEP                                                \
  OP(hid_report, 3)TERM_SEP

struct SubCommand {
  PROTOCOL_V1_SUBCOMMANDS(PROTOCOL_UINT8_T_STRUCT_DEF,,)
};

constexpr uint8_t protocol_version = 1;

constexpr char comment_symbol = '#';

} // namespace v1
} // namespace log_protocol
} // namespace simulator
} // namespace kaleidoscope
