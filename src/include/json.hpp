/*
  Copyright 2025 Peter Duanmu

  You should have received a copy of the GNU General Public License along
  with apple-music-rpc-cpp. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef APPLE_MUSIC_RPC_CPP_SRC_INCLUDE_JSON_HPP_
#define APPLE_MUSIC_RPC_CPP_SRC_INCLUDE_JSON_HPP_

#include <map>
#include <string>
#include <variant>
#include <vector>

class JSON {
 public:
  using JSONObject = std::map<std::string, JSON>;
  using JSONArray = std::vector<JSON>;

 private:
  using JSONValue = std::variant<
    std::nullptr_t,
    std::string,
    int,
    double,
    bool,
    JSONArray,
    JSONObject
  >;

  JSONValue _value;

 public:
  JSON();
  explicit JSON(std::nullptr_t value);
  explicit JSON(const std::string& value);
  explicit JSON(const char* value);
  explicit JSON(int value);
  explicit JSON(double value);
  explicit JSON(bool value);
  explicit JSON(const JSONArray& value);
  explicit JSON(const JSONObject& value);

  JSON& operator[](const std::string& key);

  template<typename T>
  T as() const;

  template<typename T>
  bool is() const;

  void push_back(const JSON& item);

  std::string to_string() const;
  static JSON from_string(const std::string& json);
};

#endif  // APPLE_MUSIC_RPC_CPP_SRC_INCLUDE_JSON_HPP_
