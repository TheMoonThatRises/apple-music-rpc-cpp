/*
  Copyright 2025 Peter Duanmu

  You should have received a copy of the GNU General Public License along
  with apple-music-rpc-cpp. If not, see <https://www.gnu.org/licenses/>.
*/

#include <unistd.h>

#include <map>
#include <string>
#include <vector>
#include <regex>

#include "include/utils.hpp"

namespace utils {
const std::map<std::string, std::string> _escape_key {
  {"\\\"", "\""}
};

std::string find_discord_ipc_file() {
  std::string user_tmp_dir = getenv("TMPDIR");
  std::string base_ipc_name = "discord-ipc-";

  for (int i = 0; i < 10; ++i) {
    std::string test_path = user_tmp_dir + base_ipc_name + std::to_string(i);

    if (access(test_path.c_str(), R_OK) >= 0) {
      return test_path;
    }
  }

  return "";
}

std::string unescape_string(const std::string& input) {
  std::string output = input;

  for (const auto& [key, value] : _escape_key) {
    output = std::regex_replace(output, std::regex(key), value);
  }

  return output;
}

std::string escape_string(const std::string& input) {
  std::string output = input;

  for (const auto& [key, value] : _escape_key) {
    output = std::regex_replace(output, std::regex(value), key);
  }

  return output;
}
}  // namespace utils
