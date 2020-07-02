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

#ifdef KALEIDOSCOPE_VIRTUAL_BUILD

#include "kaleidoscope/simulator/log_protocol/Consumer_.h"
#include "kaleidoscope/simulator/log_protocol/Parser.h"
#include "kaleidoscope/simulator/log_protocol/Grammar.h"

#include <vector>
#include <map>
#include <exception>
#include <sstream>

#define PARSER_ERROR(...)                                               \
  {                                                                     \
    std::ostringstream o;                                               \
    o << "Log protocol v1 parser error in line " << line_id_ << ": "    \
      << __VA_ARGS__ << "." ;                                           \
    throw std::runtime_error{o.str()};                                  \
  }

namespace kaleidoscope {
namespace simulator {
namespace log_protocol {

#define COMMA ,

#define PROTOCOL_V1_ARRAY_INIT(NAME, ID) #NAME
#define PROTOCOL_V1_MAP_INIT(NAME, ID) { #NAME, ID}

const char *Parser::commandIdToString(uint8_t command_id) const {
  static const char *command_strings[] = {
    PROTOCOL_V1_COMMANDS(PROTOCOL_V1_ARRAY_INIT, COMMA,)
  };

  if (command_id >= sizeof(command_strings)) {
    PARSER_ERROR("Unable to find command for id " << (int)command_id)
  }

  LOG_PROTOCOL_DEBUG("Found command string \'" << command_strings[command_id]
                     << "\' for command id " << (int)command_id)

  return command_strings[command_id];
}

const char *Parser::subCommandIdToString(uint8_t sub_command_id) const {
  static const char *sub_command_strings[] = {
    PROTOCOL_V1_SUBCOMMANDS(PROTOCOL_V1_ARRAY_INIT, COMMA,)
  };

  if (sub_command_id >= sizeof(sub_command_strings)) {
    PARSER_ERROR("Unable to find subcommand for id " << (int)sub_command_id)
  }

  LOG_PROTOCOL_DEBUG("Found subcommand string \'" << sub_command_strings[sub_command_id]
                     << "\' for subcommand id " << (int)sub_command_id)

  return sub_command_strings[sub_command_id];
}

uint8_t Parser::commandStringToId(const char *s) const {
  static const std::map<std::string, uint8_t> command_ids = {
    PROTOCOL_V1_COMMANDS(PROTOCOL_V1_MAP_INIT, COMMA,)
  };

  auto it = command_ids.find(std::string{s});
  if (it == command_ids.end()) {
    return Command::none;
    PARSER_ERROR("Unable to find command id for command string \'" << s << '\'')
  }

  return it->second;
}

uint8_t Parser::subCommandStringToId(const char *s) const {
  static const std::map<std::string, uint8_t> sub_command_ids = {
    PROTOCOL_V1_SUBCOMMANDS(PROTOCOL_V1_MAP_INIT, COMMA,)
  };

  auto it = sub_command_ids.find(std::string{s});
  if (it == sub_command_ids.end()) {
    PARSER_ERROR("Unable to find subcommand id for command string \'" << s << '\'')
  }

  return it->second;
}

namespace {
void skipToNextNonWhitespace(std::istream &in) {
  do {
    char next = in.peek();
    if (std::isspace(next)) {
      in.ignore();
    } else {
      break;
    }
  } while (in);
}
}

uint8_t Parser::readCommandId(std::istream &line_stream) const {
  uint8_t command_id = 0;

  if (input_document_type_ == DocumentType::verbose) {
    static constexpr int buf_length = 1024;
    char buffer[buf_length];

    skipToNextNonWhitespace(line_stream);

    line_stream.get(buffer, buf_length, ' ');

    LOG_PROTOCOL_DEBUG("Command string: \'" <<  buffer << "\'")

    command_id = this->commandStringToId(buffer);
  } else {
    int command_id_int = 0;
    line_stream >> command_id_int;
    command_id = command_id_int;
  }

  LOG_PROTOCOL_DEBUG("Command id: " << (int)command_id)

  return command_id;
}

uint8_t Parser::readSubCommandId(std::istream &line_stream) const {
  uint8_t sub_command_id = 0;

  if (input_document_type_ == DocumentType::verbose) {
    static constexpr int buf_length = 1024;
    char buffer[buf_length];

    skipToNextNonWhitespace(line_stream);

    line_stream.get(buffer, buf_length, ' ');
    line_stream.ignore();

    LOG_PROTOCOL_DEBUG("Subcommand string: \'" <<  buffer << "\'")

    sub_command_id = this->subCommandStringToId(buffer);
  } else {
    int sub_command_id_int = 0;
    line_stream >> sub_command_id_int;
    sub_command_id = sub_command_id_int;
  }

  LOG_PROTOCOL_DEBUG("Subcommand id: " << (int)sub_command_id)

  return sub_command_id;
}

bool Parser::parseHeaderLine(std::string &line, Consumer_ &consumer) const {
  std::istringstream line_stream(line);
  line_stream >> std::skipws;

  uint8_t command_id = this->readCommandId(line_stream);

  switch (command_id) {
  case Command::firmware_id: {
    static constexpr int buf_length = 1024;
    char buffer[buf_length];
    line_stream.get(buffer, buf_length, '\"');
    line_stream.ignore();
    line_stream.get(buffer, buf_length, '\"');

    LOG_PROTOCOL_DEBUG("firmware_id: " << buffer)
    consumer.onFirmwareId(buffer);
    return true;
  }
  }

  return false;
}

void Parser::parseAction(std::istream &line_stream, Consumer_ &consumer) const {
  uint8_t sub_command_id = this->readSubCommandId(line_stream);

  int row = 255, col = 255;
  line_stream >> row >> col;

  switch (sub_command_id) {
  case SubCommand::key_pressed: {
    LOG_PROTOCOL_DEBUG("action key_pressed " << row << " " << col)
    consumer.onKeyPressed(row, col);
  }
  break;
  case SubCommand::key_released: {
    LOG_PROTOCOL_DEBUG("action key_released " << row << " " << col)
    consumer.onKeyReleased(row, col);
  }
  break;
  default:
    PARSER_ERROR("Insuitable action subcommand id " << (int)sub_command_id << " encountered")
  }
}

void Parser::parseReaction(std::istream &line_stream, Consumer_ &consumer) const {
  uint8_t sub_command_id = this->readSubCommandId(line_stream);

  switch (sub_command_id) {
  case SubCommand::hid_report: {
    int id = 255;
    int length = 0;
    line_stream >> id >> length;

    uint8_t data[255];
    for (uint8_t i = 0; i < length; ++i) {
      int tmp;
      line_stream >> tmp;
      data[i] = tmp;
    }

    LOG_PROTOCOL_DEBUG("reaction hid_report " << id << ' ' << length)
    if (debug_) {
      for (int i = 0; i < length; ++i) {
        std::cout << (int)data[i] << ' ';
      }
      std::cout << std::endl;
    }
    consumer.onHIDReport(id, length, data);
  }
  break;
  default:
    PARSER_ERROR("Insuitable reaction subcommand " << (int)sub_command_id << " encountered")
  }
}

bool Parser::parseBodyLine(std::string &line, Consumer_ &consumer) const {
  std::istringstream line_stream(line);
  line_stream >> std::skipws;

  uint8_t command_id = this->readCommandId(line_stream);

  switch (command_id) {
  case Command::start_cycle: {
    uint32_t cycle_id = 0;
    uint32_t start_time = 0;
    line_stream >> cycle_id >> start_time;
    LOG_PROTOCOL_DEBUG("start_cycle " << cycle_id << ' ' << start_time)
    consumer.onStartCycle(cycle_id, start_time);
  }
  break;
  case Command::action:
    this->parseAction(line_stream, consumer);
    break;
  case Command::reaction:
    this->parseReaction(line_stream, consumer);
    break;
  case Command::end_cycle: {
    uint32_t cycle_id = 0;
    uint32_t end_time = 0;
    line_stream >> cycle_id >> end_time;
    LOG_PROTOCOL_DEBUG("end_cycle " << cycle_id << ' ' << end_time)
    consumer.onEndCycle(cycle_id, end_time);
  }
  break;
  case Command::set_time: {
    uint32_t time = 0;
    line_stream >> time;
    LOG_PROTOCOL_DEBUG("set_time " << time)
    consumer.onSetTime(time);
  }
  break;
  case Command::cycle: {
    uint32_t cycle_id = 0;
    uint32_t start_time = 0;
    uint32_t end_time = 0;
    line_stream >> cycle_id >> start_time >> end_time;
    LOG_PROTOCOL_DEBUG("cycle " << cycle_id << ' ' << start_time << ' ' << end_time)
    consumer.onCycle(cycle_id, start_time, end_time);
  }
  break;
  case Command::cycles: {
    uint32_t start_cycles_id = 0;
    uint32_t start_cycles_time = 0;
    uint32_t n_cycles = 0;
    line_stream >> start_cycles_id >> start_cycles_time >> n_cycles;
    LOG_PROTOCOL_DEBUG("cycles " << start_cycles_id << ' ' << start_cycles_time
                       << ' ' << n_cycles)

    std::vector<uint32_t> cycle_durations;
    cycle_durations.resize(n_cycles);
    uint8_t last_time_offset = 0;
    for (uint32_t i = 0; i < n_cycles; ++i) {
      uint32_t cycle_id = start_cycles_id + i;
      line_stream >> cycle_durations[i];

      if (debug_) {
        std::cout << cycle_durations[i] << ' ';
      }
    }
    if (debug_) {
      std::cout << std::endl;
    }
    consumer.onCycles(start_cycles_id, start_cycles_time, cycle_durations);
  }
  break;
  default:
    PARSER_ERROR("Unknown command id " << (int)command_id << " encountered")
  }

  return false;
}

bool Parser::readNextLine(std::istream &in, std::string &line) {
  bool read_success = false;
  do {
    if (!std::getline(in, line)) {
      return false;
    }
    ++line_id_;

    if (!line.empty() && (line[0] != comment_symbol) && (line[0] != '.')) {
      break;
    }
  } while (1);

  return true;
}

void Parser::parse(std::istream &in, Consumer_ &consumer) {
  std::string line;

  this->readNextLine(in, line);

  bool is_header_line = this->parseHeaderLine(line, consumer);

  if (!is_header_line) {
    this->parseBodyLine(line, consumer);
  }

  while (this->readNextLine(in, line)) {
    ++line_id_;
    try {
      this->parseBodyLine(line, consumer);
    } catch (const std::runtime_error &e) {
      std::ostringstream out;
      out << e.what() << " Current line is \'" << line << "\'";
      throw std::runtime_error(out.str());
    }
  }
}

#define DEBUG_CC(...) LOG_PROTOCOL_DEBUG("Compression: " << __VA_ARGS__)

class CompressionConsumer : public Consumer_ {

 public:

  static constexpr uint8_t max_grouped_cycles = 99;
  static constexpr uint8_t max_grouped_time_offset = 99;

  CompressionConsumer(const Parser &parser,
                      std::ostream &out,
                      bool debug)
    :  parser_{parser},
       out_{out},
       debug_{parser.protocol_.debug_}
  {}

  ~CompressionConsumer() {
    // There might be some unflushed content in the queue
    //
    this->flushCompressedCycles();
  }

  virtual void onFirmwareId(const char *firmware_id_string) override {

    DEBUG_CC("firmware_id \"" << firmware_id_string << '\"')

    this->startLine();
    this->outputCommand(Command::firmware_id);
    if (parser_.protocol_.quote_lines_) {
      out_ << " \\\"" << firmware_id_string << "\\\"";
    } else {
      out_ << " \"" << firmware_id_string << "\"";
    }
    this->endLine();
  }
  virtual void onStartCycle(uint32_t cycle_id, uint32_t
                            cycle_start_time) override {

    DEBUG_CC("start_cycle " << cycle_id << ' ' << cycle_start_time)

    // Store the information about the start of this cycle
    // in order to have it available later when, e.g.
    // cycles cannot be compressed.
    //
    last_cycle_id_ = cycle_id;
    last_cycle_start_time_ = cycle_start_time;

    cycle_content_encountered_ = false;

//          // To save space we allow at max max_grouped_cycles
//          // cycles to be grouped
//          //
//          if(cycle_end_offsets_.size() == max_grouped_cycles) {
//             this->flushCompressedCycles();
//          }

    if (cycle_end_offsets_.empty()) {
      cycles_start_time_ = cycle_start_time;
      cycles_start_id_ = cycle_id;
    }
  }
  virtual void onEndCycle(uint32_t cycle_id, uint32_t cycle_end_time) override {

    DEBUG_CC("end_cycle " << cycle_id << ' ' << cycle_end_time)

    if (!cycle_content_encountered_) {

      // The last cycle was an empty cycle.

//             auto offset = cycle_end_time - cycles_start_time_;
//
//             // Check the offset and make sure
//             // that time offsets do not exceed max_grouped_time_offset.
//             //
//             if(offset > max_grouped_time_offset) {
//                this->flushCompressedCycles();
//                cycles_start_time_ = last_cycle_start_time_;
//                cycles_start_id_ = cycle_id;
//             }

      // Recompute offset in case cycles were flushed.
      //
      auto offset = cycle_end_time - last_cycle_start_time_;

      if (offset > max_grouped_time_offset) {

        // If we end up here, offset equals the duration of
        // the last cycle.

        // If the duration exceeds max_grouped_time_offset,
        // we are not able to store this cycle in a compressed sequence.

        // Output the cycle as pair of start_cycle and end_cycle.
        //
        this->startLine();
        this->outputCommand(Command::start_cycle);
        out_ << ' ' << cycle_id << ' ' << last_cycle_start_time_;
        this->endLine();
        this->startLine();
        this->outputCommand(Command::end_cycle);
        out_ << ' ' << cycle_id << ' ' << cycle_end_time;
        this->endLine();
      } else {

        // Cycle duration ok

        cycle_end_offsets_.push_back((uint8_t)offset);
      }
    } else {

      // Cycle has content and thus cannot be compressed

      this->startLine();
      this->outputCommand(Command::end_cycle);
      out_ << ' ' << cycle_id << ' ' << cycle_end_time;
      this->endLine();
    }
  }
  virtual void onKeyPressed(uint8_t row, uint8_t col) override {
    DEBUG_CC("key_pressed " << (int)row << ' ' << (int)col)
    this->considerKeyAction(SubCommand::key_pressed, row, col);
  }
  virtual void onKeyReleased(uint8_t row, uint8_t col) override {
    DEBUG_CC("key_released " << (int)row << ' ' << (int)col)
    this->considerKeyAction(SubCommand::key_released, row, col);
  }
  virtual void onHIDReport(uint8_t id, int length, const uint8_t *data) override {

    DEBUG_CC("hid_report " << (int)id << ' ' << length)
    if (debug_) {
      for (int i = 0; i < length; ++i) {
        std::cout << (int)data[i] << ' ';
      }
      std::cout << std::endl;
    }

    this->flushAndStartUncompressedCycle();

    cycle_content_encountered_ = true;

    this->startLine();
    this->outputCommand(Command::reaction);
    out_ << ' ';
    this->outputSubCommand(SubCommand::hid_report);
    out_ << ' ' << (int)id << ' ' << length << ' ';
    for (int i = 0; i < length; ++i) {
      out_ << (int)data[i] << ' ';
    }
    this->endLine();
  }
  virtual void onSetTime(uint32_t time) override {
    DEBUG_CC("set_time " << time)
    this->startLine();
    this->outputCommand(Command::set_time);
    out_ << ' ' << time;
    this->endLine();
  }
  virtual void onCycle(uint32_t cycle_id, uint32_t cycle_start_time, uint32_t cycle_end_time) override {
    this->startLine();
    this->outputCommand(Command::cycle);
    out_ << ' ' << cycle_id << ' ' << cycle_start_time << ' ' << cycle_end_time << '\n';
    this->endLine();
  }
  virtual void onCycles(uint32_t start_cycle_id, uint32_t start_time_id,
                        const std::vector<uint32_t> &cycle_durations) override {
    this->startLine();
    this->outputCommand(Command::cycles);
    out_ << ' ' << start_cycle_id << ' ' << start_time_id << ' ' << cycle_durations.size() << ' ';
    for (const auto &duration : cycle_durations) {
      out_ << duration << ' ';
    }
    this->endLine();
  }

 private:

  void startLine() const {
    if (parser_.protocol_.quote_lines_) {
      out_ << '"';
    }
  }

  void endLine() const {
    if (parser_.protocol_.quote_lines_) {
      out_ << "\\n\"";
    }
    out_ << '\n';
  }

  void considerKeyAction(uint8_t action_id, uint8_t row, uint8_t col) {

    this->flushAndStartUncompressedCycle();

    cycle_content_encountered_ = true;

    this->startLine();
    this->outputCommand(Command::action);
    out_ << ' ';
    this->outputSubCommand(action_id);
    out_ << ' ' << (int)row << ' ' << (int)col;
    this->endLine();
  }

  void outputCommand(uint8_t command_id) {
    switch (parser_.protocol_.output_document_type_) {
    case DocumentType::verbose:
      out_ << parser_.commandIdToString(command_id);
      break;
    case DocumentType::compressed:
      out_ << (int)command_id;
      break;
    default: {
      std::ostringstream error_stream{};
      error_stream << "Error while compressing. Unknown document type " << parser_.protocol_.output_document_type_;
      throw std::runtime_error(error_stream.str().c_str());
    }
    }
  }

  void outputSubCommand(uint8_t sub_command_id) {
    switch (parser_.protocol_.output_document_type_) {
    case DocumentType::verbose:
      out_ << parser_.subCommandIdToString(sub_command_id);
      break;
    case DocumentType::compressed:
      out_ << (int)sub_command_id;
      break;
    default: {
      std::ostringstream error_stream{};
      error_stream << "Error while compressing. Unknown document type " << parser_.protocol_.output_document_type_;
      throw std::runtime_error(error_stream.str().c_str());
    }
    }
  }

  void flushAndStartUncompressedCycle() {

    // Only enable flushing once per cycle when actions/reactions
    // appear.
    //
    if (cycle_content_encountered_) {
      return;
    }

    this->flushCompressedCycles();

    this->startLine();
    this->outputCommand(Command::start_cycle);
    out_ << ' ' << last_cycle_id_ << ' ' << last_cycle_start_time_;
    this->endLine();
  }

  void flushCompressedCycles() {

    if (cycle_end_offsets_.empty()) {
      return;
    }

    this->startLine();
    this->outputCommand(Command::cycles);
    out_ << ' ' << cycles_start_id_ << ' ' << cycles_start_time_ << ' '
         << cycle_end_offsets_.size() << ' ';
    for (const auto &end_offset : cycle_end_offsets_) {
      out_ << (int)end_offset << ' ';
    }
    this->endLine();

    cycle_end_offsets_.clear();
  }

 private:

  const Parser &parser_;
  std::ostream &out_;
  bool debug_ = false;

  bool cycle_content_encountered_ = false;
  uint32_t cycles_start_id_ = 0;
  uint32_t cycles_start_time_ = 0;
  uint32_t last_cycle_id_ = 0;
  uint32_t last_cycle_start_time_ = 0;
  std::vector<uint8_t> cycle_end_offsets_;
};

void Parser::compress(std::istream &in, std::ostream &out) {

  CompressionConsumer cc{*this, out, debug_};

  this->parse(in, cc);
}

} // namespace log_protocol
} // namespace simulator
} // namespace kaleidoscope

#endif
