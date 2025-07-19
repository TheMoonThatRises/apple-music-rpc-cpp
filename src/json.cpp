/*
  Copyright 2025 Peter Duanmu

  You should have received a copy of the GNU General Public License along
  with apple-music-rpc-cpp. If not, see <https://www.gnu.org/licenses/>.
*/

#include <format>
#include <string>
#include <sstream>
#include <variant>
#include <vector>

#include "include/json.hpp"
#include "include/utils.hpp"

using utils::escape_string;

namespace json {
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

template<typename T>
bool JSON::is() const {
  return std::holds_alternative<T>(_value);
}

void JSON::push_back(const JSON& item) {
  std::get<JSONArray>(_value).push_back(item);
}

void JSON::stringify(std::ostream& os) const {
  if (is<JSONObject>()) {
    JSONObject object = as<JSONObject>();
    bool first_item = true;

    os << "{";

    for (const auto& [key, value] : object) {
      if (!first_item) {
        os << ",";
      } else {
        first_item = false;
      }

      os << "\"" << escape_string(key) << "\":";
      value.stringify(os);
    }

    os <<"}";
  } else if (is<JSONArray>()) {
    JSONArray array = as<JSONArray>();
    bool first_item = true;

    os << "[";

    for (const auto& item : array) {
      if (!first_item) {
        os << ",";
      } else {
        first_item = false;
      }

      item.stringify(os);
    }

    os << "]";
  } else if (is<std::string>()) {
    os << "\"" << escape_string(as<std::string>()) << "\"";
  } else if (is<int>()) {
    os << std::to_string(as<int>());
  } else if (is<double>()) {
    os << std::to_string(as<double>());
  } else if (is<bool>()) {
    os << (as<bool>() ? "true" : "false");
  } else if (is<std::nullptr_t>()) {
    os << "null";
  }
}

std::string JSON::to_string() const {
  std::stringstream os;
  stringify(os);
  return os.str();
}
}  // namespace json
