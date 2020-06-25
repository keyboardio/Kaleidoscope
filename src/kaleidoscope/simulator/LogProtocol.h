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

#include <stdint.h>

namespace kaleidoscope {
namespace simulator {
namespace log_protocol {

struct DocumentType {
  static constexpr uint8_t none = 0;
  static constexpr uint8_t verbose = 1;
  static constexpr uint8_t compressed = 2;
};

} // namespace log_protocol
} // namespace simulator
} // namespace kaleidoscope

#ifdef KALEIDOSCOPE_VIRTUAL_BUILD

#include <istream>
#include <ostream>
#include <iostream>

#define LOG_PROTOCOL_DEBUG(...)                                         \
  if(debug_) {                                                          \
    std::cout << "log-debug: " << __VA_ARGS__ << std::endl;             \
  }

namespace kaleidoscope {
namespace simulator {
namespace log_protocol {

namespace v1 {
class Parser;
class CompressionConsumer;
}

class Consumer_;

class LogProtocol {

 public:

  void parse(std::istream &in, Consumer_ &consumer);
  void parse(const char *document, Consumer_ &consumer);

  void compress(std::istream &in, std::ostream &out);
  void compress(const char *document, std::ostream &out);

  void setDebug(bool state = true) {
    debug_ = state;
  }
  void setOutputDocumentType(uint8_t document_type) {
    output_document_type_ = document_type;
  }
  void setQuoteLines(bool state = true) {
    quote_lines_ = state;
  }

 private:

  void determineDocumentVersion(std::istream &in);

 private:

  int document_type_ = 0;
  int protocol_version_ = 0;
  int line_id_ = 0;
  bool debug_ = false;
  uint8_t output_document_type_ = DocumentType::compressed;
  bool quote_lines_ = false;

  friend class v1::Parser;
  friend class v1::CompressionConsumer;
};

} // namespace log_protocol
} // namespace simulator
} // namespace kaleidoscope

#endif
