/*
  Copyright 2025 Peter Duanmu

  You should have received a copy of the GNU General Public License along
  with apple-music-rpc-cpp. If not, see <https://www.gnu.org/licenses/>.
*/

#include <signal.h>

#include <chrono>
#include <cmath>
#include <ctime>

#include <discord_ipc_cpp/discord_ipc_client.hpp>
#include <discord_ipc_cpp/ipc_types.hpp>

#include "include/utils.hpp"

using discord_ipc_cpp::DiscordIPCClient;
using discord_ipc_cpp::ipc_types::RichPresence;

int get_current_time_seconds() {
  auto now = std::chrono::system_clock::now();
  auto now_c = std::chrono::system_clock::to_time_t(now);

  return now_c;
}

void register_signal_callback_handler(DiscordIPCClient& client) {
  static DiscordIPCClient* static_client = &client;

  signal(SIGINT, [](int signum) {
    static_client->close();

    exit(signum);
  });
}

void music_player_binder(
  DiscordIPCClient& client,
  const MusicPlayerInfo& player_info
) {
  if (player_info.player_state == "Paused" ||
      player_info.player_state == "Stopped"
  ) {
    client.set_empty_presence();

    return;
  }

  int player_time_s = std::round(player_info.total_time / 1000.);
  int start_time = get_current_time_seconds();
  int end_time = get_current_time_seconds() + player_time_s;

  client.set_presence({
    .details = player_info.name,
    .state = player_info.artist,
    .assets = {
      .large_text = player_info.album,
    },
    .timestamps = {
      .start = start_time,
      .end = end_time
    },
    .type = RichPresence::listening
  });
}
