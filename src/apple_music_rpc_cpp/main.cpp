/*
  Copyright 2025 Peter Duanmu

  You should have received a copy of the GNU General Public License along
  with apple-music-rpc-cpp. If not, see <https://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <string>

#include <discord_ipc_cpp/discord_ipc_client.hpp>
#include <discord_ipc_cpp/ipc_types.hpp>

#include "include/utils.hpp"
#include "include/music_types.hpp"
#include "include/objc_bridge.hpp"

using discord_ipc_cpp::DiscordIPCClient;
using discord_ipc_cpp::ipc_types::RichPresence;

int main() {
  std::string music_client_id = "773825528921849856";  // apple music

  DiscordIPCClient client(music_client_id);

  bool ret = client.connect();

  if (!ret) {
    std::cout << "failed to connect to socket" << std::endl;

    return 1;
  }

  ret = client.set_empty_presence();

  if (!ret) {
    std::cout << "Failed to set presence" << std::endl;

    return 1;
  }

  objc_bridge::bind_music_player_info(
    [&](const MusicPlayerInfo& player_info) {
      if (player_info.player_state == "Paused") {
        client.set_empty_presence();

        return;
      }

      int player_time_s = player_info.total_time / 1000;
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
    });

  objc_bridge::run_cf_main_loop();

  client.close();

  return 0;
}
