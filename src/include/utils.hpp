/*
  Copyright 2025 Peter Duanmu

  You should have received a copy of the GNU General Public License along
  with apple-music-rpc-cpp. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef APPLE_MUSIC_RPC_CPP_SRC_INCLUDE_UTILS_HPP_
#define APPLE_MUSIC_RPC_CPP_SRC_INCLUDE_UTILS_HPP_

#include <map>
#include <string>
#include <vector>

namespace utils {
extern const std::map<std::string, std::string> _escape_key;

std::string find_discord_ipc_file();

std::string unescape_string(const std::string& input);
std::string escape_string(const std::string& input);
}  // namespace utils

#endif  // APPLE_MUSIC_RPC_CPP_SRC_INCLUDE_UTILS_HPP_
