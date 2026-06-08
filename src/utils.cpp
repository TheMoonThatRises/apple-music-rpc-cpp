/*
  Copyright 2026 Peter Duanmu

  You should have received a copy of the GNU General Public License along
  with apple-music-rpc-cpp. If not, see <https://www.gnu.org/licenses/>.
*/

#include "include/utils.hpp"

#include <signal.h>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <ctime>
#include <string>

#include <discord_ipc_cpp/discord_ipc_client.hpp>
#include <discord_ipc_cpp/ipc_types.hpp>

#include "include/objc_bridge.hpp"

using discord_ipc_cpp::DiscordIPCClient;

using discord_ipc_cpp::ipc_types::RichPresence;

int64_t get_current_time_seconds() {
  auto now = std::chrono::system_clock::now();
  auto now_c = std::chrono::system_clock::to_time_t(now);

  return now_c;
}

std::string to_lower(const std::string& input) {
  std::string output(input);

  std::transform(
    output.begin(), output.end(), output.begin(),
    [](const auto& ch) {
      return std::tolower(static_cast<unsigned char>(ch));
    });

  return output;
}

std::string clamp_string(const std::string& input) {
  std::string clamped = input;

  if (clamped.length() <= 2) {
    clamped += std::string(3 - clamped.length(), ' ');
  } else if (clamped.length() >= 128) {
    clamped = clamped.replace(125, clamped.length(), "...");
  }

  return clamped;
}

RichPresence construct_presence(const MusicPlayerInfo& player_info) {
  RichPresence presence = {
    .name = "Apple Music",
    .type = RichPresence::at_listening,
    .status_display_type = RichPresence::sdt_state,
    .assets = RichPresence::Assets {}
  };

  if (player_info.total_time.has_value()) {
    int64_t player_time_s = std::round(player_info.total_time.value() / 1000.);
    int64_t start_time = get_current_time_seconds();
    int64_t end_time = get_current_time_seconds() + player_time_s;

    presence.timestamps = {
      .start = start_time,
      .end = end_time
    };
  }

  presence.details = clamp_string(player_info.name.value_or("Unknown song"));
  presence.state = clamp_string(player_info.artist.value_or("Unknown artist"));
  presence.assets->large_text = clamp_string(
    player_info.album.value_or("Unknown album"));

  return presence;
}

void register_signal_callback_handler(DiscordIPCClient& client) {
  static DiscordIPCClient* static_client = &client;

  signal(SIGINT, [](int signum) {
    static_client->close(true);

    exit(signum);
  });
}
