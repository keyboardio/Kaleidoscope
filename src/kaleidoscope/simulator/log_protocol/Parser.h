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

#ifdef KALEIDOSCOPE_VIRTUAL_BUILD

#include "kaleidoscope/simulator/log_protocol/Grammar.h"

#include <string>
#include <istream>

namespace kaleidoscope {
namespace simulator {
namespace log_protocol {

class Consumer_;

class Parser {
 public:
  static constexpr uint8_t protocol_version = log_protocol::protocol_version;

  Parser(const LogProtocol &protocol, uint8_t input_document_type, int start_line_id)
    :  protocol_{protocol},
       input_document_type_{input_document_type},
       line_id_{start_line_id},
       debug_{protocol.debug_}
  {}

  const char *commandIdToString(uint8_t command_id) const;
  const char *subCommandIdToString(uint8_t sub_command_id) const;

  uint8_t commandStringToId(const char *string) const;
  uint8_t subCommandStringToId(const char *string) const;

  uint8_t getInputDocumentType() const {
    return input_document_type_;
  }

  void parse(std::istream &in, Consumer_ &consumer);
  void compress(std::istream &in, std::ostream &out);

 private:

  uint8_t readCommandId(std::istream &line_stream) const;
  uint8_t readSubCommandId(std::istream &line_stream) const;

  bool readNextLine(std::istream &in, std::string &line);

  bool parseHeaderLine(std::string &line, Consumer_ &consumer) const;
  void parseAction(std::istream &line_stream, Consumer_ &consumer) const;
  void parseReaction(std::istream &line_stream, Consumer_ &consumer) const;
  bool parseBodyLine(std::string &line, Consumer_ &consumer) const;

 private:

  const LogProtocol &protocol_;
  uint8_t input_document_type_ = DocumentType::none;
  int line_id_ = 0;
  bool debug_ = false;

  friend class CompressionConsumer;
};

} // namespace log_protocol
} // namespace simulator
} // namespace kaleidoscope

#endif
