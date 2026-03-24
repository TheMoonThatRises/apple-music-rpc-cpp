/*
  Copyright 2026 Peter Duanmu

  You should have received a copy of the GNU General Public License along
  with apple-music-rpc-cpp. If not, see <https://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <string>

#include <discord_ipc_cpp/discord_ipc_client.hpp>

#include "include/utils.hpp"
#include "include/handler.hpp"
#include "include/objc_bridge.hpp"

using discord_ipc_cpp::DiscordIPCClient;

using objc_bridge::bind_music_player_info;
using objc_bridge::bind_discord_launch;
using objc_bridge::run_cf_main_loop;

int main() {
  std::string music_client_id = "1430370823801274418";  // apple music

  DiscordIPCClient client(music_client_id);
  Handler handler(client);

  handler.attempt_discord_connect(false);

  std::cout << "Hooking into music and launch notifications" << std::endl;

  register_signal_callback_handler(client);

  bind_discord_launch([&handler]() {
    std::cout << "Detected Discord relaunching..." << std::endl;

    handler.attempt_discord_connect(true);
  });
  bind_music_player_info([&handler](const auto& info) {
    handler.music_player_binder(info);
  });

  run_cf_main_loop();

  return 0;
}
