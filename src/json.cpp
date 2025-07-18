/*
  Copyright 2025 Peter Duanmu

  You should have received a copy of the GNU General Public License along
  with apple-music-rpc-cpp. If not, see <https://www.gnu.org/licenses/>.
*/

#include <algorithm>
#include <format>
#include <string>
#include <variant>
#include <vector>

#include "include/json.hpp"
#include "include/utils.hpp"

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

std::string JSON::to_string() const {
  std::string base;

  if (is<JSONObject>()) {
    JSONObject object = as<JSONObject>();

    base = "{";

    for (auto it = object.begin(); it != object.end(); ++it) {
      base += std::format(
        "\"{}\":{}", escape_string(it->first), it->second.to_string());

      if (std::next(it) != object.end()) {
        base += ",";
      }
    }

    base += "}";
  } else if (is<JSONArray>()) {
    JSONArray array = as<JSONArray>();

    base = "[";

    for (size_t i = 0; i < array.size(); ++i) {
      base += array[i].to_string();

      if (i < array.size() - 1) {
        base += ",";
      }
    }

    base += "]";
  } else if (is<std::string>()) {
    base = std::format("\"{}\"", escape_string(as<std::string>()));
  } else if (is<int>()) {
    base = std::to_string(as<int>());
  } else if (is<double>()) {
    base = std::to_string(as<double>());
  } else if (is<bool>()) {
    base = as<bool>() ? "true" : "false";
  } else if (is<std::nullptr_t>()) {
    base = "null";
  }

  return base;
}

JSON JSON::from_string(const std::string& json) {
  JSON base;
  size_t pos = 0;

  if (json[0] == '{') {
    base = JSON(JSON::JSONObject{});
  } else {
    base = JSON(JSON::JSONArray{});
  }

  ++pos;

  while (pos < json.length() - 1) {
    JSON tmp;
    std::string key;

    std::string search_str = json.substr(pos);

    if (base.is<JSONObject>()) {
      size_t colon_idx = search_str.find(':');
      key = unescape_string(search_str.substr(1, colon_idx - 2));

      pos += colon_idx + 1;
      search_str = json.substr(pos);
    }

    char search_item = json[pos];

    switch (search_item) {
      case '{':
      case '[': {
        char end_item = search_item == '{' ? '}' : ']';
        size_t start_counts = 0, end_counts = 0;
        size_t end_idx = 0;

        for (; end_idx < search_str.length(); ++end_idx) {
          if (search_str[end_idx] == search_item) {
            ++start_counts;
          } else if (search_str[end_idx] ==  end_item) {
            ++end_counts;
          }

          if (start_counts == end_counts) {
            break;
          }
        }

        ++end_idx;

        tmp = from_string(search_str.substr(0, end_idx));

        pos += end_idx;

        if (json[pos] == ',') {
          ++pos;
        }

        break;
      }
      default: {
        size_t end_comma = search_str.find_first_of(',');
        std::string match_item = search_str;

        if (end_comma != std::string::npos) {
          match_item.erase(end_comma);
        } else {
          match_item.pop_back();
        }

        switch (search_item) {
          case '"': {
            tmp = JSON(
              unescape_string(match_item.substr(1, match_item.length() - 2)));

            break;
          }
          case 'n': {
            tmp = JSON(nullptr);

            break;
          }
          case 't':
          case 'f': {
            tmp = JSON(match_item == "true" ? true : false);

            break;
          }
          default: {
            if (match_item.find('.') == std::string::npos) {
              tmp = JSON(stoi(match_item));
            } else {
              tmp = JSON(stod(match_item));
            }
          }
        }

        pos += match_item.length() + 1;
      }
    }

    if (base.is<JSONObject>()) {
      base[key] = tmp;
    } else {
      base.push_back(tmp);
    }
  }

  return base;
}
