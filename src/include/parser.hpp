/*
  Copyright 2025 Peter Duanmu

  You should have received a copy of the GNU General Public License along
  with apple-music-rpc-cpp. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef APPLE_MUSIC_RPC_CPP_SRC_INCLUDE_PARSER_HPP_
#define APPLE_MUSIC_RPC_CPP_SRC_INCLUDE_PARSER_HPP_

#include <string>

#include "./json.hpp"

namespace json {
class Parser {
 public:
  static JSON parse(const std::string& json);

 private:
  const std::string& _json;
  size_t _pos;

 private:
  explicit Parser(const std::string& json);

  void skip_whitespace();
  void expect(char item);

  JSON parse_value();

  JSON parse_object();
  JSON parse_array();
  std::string parse_string();
  JSON parse_number();
  JSON parse_literal();
};
}  // namespace json

#endif  // APPLE_MUSIC_RPC_CPP_SRC_INCLUDE_PARSER_HPP_
