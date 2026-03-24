/*
  Copyright 2025 Peter Duanmu

  You should have received a copy of the GNU General Public License along
  with apple-music-rpc-cpp. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef INCLUDE_UTILS_HPP_
#define INCLUDE_UTILS_HPP_

#include <string>

#include <discord_ipc_cpp/discord_ipc_client.hpp>
#include <discord_ipc_cpp/ipc_types.hpp>

#include "./music_types.hpp"

int get_current_time_seconds();

std::string to_lower(const std::string& input);

std::string clamp_string(const std::string& input);

discord_ipc_cpp::ipc_types::RichPresence construct_presence(
  const MusicPlayerInfo& player_info);

void register_signal_callback_handler(
  discord_ipc_cpp::DiscordIPCClient& client);

#endif  // INCLUDE_UTILS_HPP_
