/*
  Copyright 2025 Peter Duanmu

  You should have received a copy of the GNU General Public License along
  with apple-music-rpc-cpp. If not, see <https://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <string>

#include <discord_ipc_cpp/discord_ipc_client.hpp>

#include "include/utils.hpp"
#include "include/music_types.hpp"
#include "include/objc_bridge.hpp"

using discord_ipc_cpp::DiscordIPCClient;

using objc_bridge::bind_music_player_info;
using objc_bridge::run_cf_main_loop;

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

  register_signal_callback_handler(client);

  bind_music_player_info([&client](const auto& info) {
    music_player_binder(client, info);
  });

  run_cf_main_loop();

  return 0;
}
