/*
  Copyright 2025 Peter Duanmu

  You should have received a copy of the GNU General Public License along
  with apple-music-rpc-cpp. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef APPLE_MUSIC_RPC_CPP_SRC_INCLUDE_UTILS_HPP_
#define APPLE_MUSIC_RPC_CPP_SRC_INCLUDE_UTILS_HPP_

#include <string>
#include <vector>

std::string find_discord_ipc_file();

std::string unescape_string(const std::string& input);
std::string escape_string(const std::string& input);

#endif  // APPLE_MUSIC_RPC_CPP_SRC_INCLUDE_UTILS_HPP_
