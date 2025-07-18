/*
  Copyright 2025 Peter Duanmu

  You should have received a copy of the GNU General Public License along
  with apple-music-rpc-cpp. If not, see <https://www.gnu.org/licenses/>.
*/

#include <format>
#include <string>
#include <regex>
#include <variant>

#include "include/json.hpp"

JSON::JSON() : _value(JSONObject{}) {}

JSON::JSON(std::nullptr_t value) : _value(value) {}

JSON::JSON(const std::string& value) : _value(value) {}

JSON::JSON(const char* value) : _value(std::string(value)) {}

JSON::JSON(int value) : _value(value) {}

JSON::JSON(double value) : _value(value) {}

JSON::JSON(bool value) : _value(value) {}

JSON::JSON(const JSONArray& value) : _value(value) {}

JSON::JSON(const JSONObject& value) : _value(value) {}

JSON& JSON::operator[](const std::string& key) {
  return std::get<JSONObject>(_value)[key];
}

template<typename T>
T JSON::as() const {
  return std::get<T>(_value);
}

std::string JSON::to_string() const {
  std::string base;

  if (std::holds_alternative<JSONObject>(_value)) {
    JSONObject object = as<JSONObject>();

    base = "{";

    for (auto it = object.begin(); it != object.end(); ++it) {
      base += std::format("\"{}\":{}", it->first, it->second.to_string());

      if (std::next(it) != object.end()) {
        base += ",";
      }
    }

    base += "}";
  } else if (std::holds_alternative<JSONArray>(_value)) {
    JSONArray array = as<JSONArray>();

    base = "[";

    for (size_t i = 0; i < array.size(); ++i) {
      base += array[i].to_string();

      if (i < array.size() - 1) {
        base += ",";
      }
    }

    base += "]";
  } else if (std::holds_alternative<std::string>(_value)) {
    base = std::format("\"{}\"", as<std::string>());
  } else if (std::holds_alternative<int>(_value)) {
    base = std::to_string(as<int>());
  } else if (std::holds_alternative<double>(_value)) {
    base = std::to_string(as<double>());
  } else if (std::holds_alternative<bool>(_value)) {
    base = as<bool>() ? "true" : "false";
  } else {
    base = "null";
  }

  return base;
}

JSON JSON::from_string(const std::string& json) {
  JSON base = JSON();



  return base;
}
