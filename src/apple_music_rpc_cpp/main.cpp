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

using discord_ipc_cpp::DiscordIPCClient;
using discord_ipc_cpp::ipc_types::RichPresence;

int main() {
  // std::string music_client_id = "773825528921849856";  // apple music
  std::string music_client_id = "1313374141960949831";  // spwifiy

  DiscordIPCClient client(music_client_id);

  bool ret = client.connect();

  if (!ret) {
    std::cout << "failed to connect to socket" << std::endl;

    return 1;
  }

  int song_duration = 203;
  int start_time = get_current_time_seconds();
  int end_time = get_current_time_seconds() + song_duration;

  ret = client.set_empty_presence();

  if (!ret) {
    std::cout << "Failed to set presence" << std::endl;

    return 1;
  }

  client.set_presence({
    .details = "Test song name",
    .state = "Test artist name",
    .assets = {
      .large_text = "Test album name",
      .large_image = "https://upload.wikimedia.org/wikipedia/commons/thumb/6/66/SMPTE_Color_Bars.svg/1200px-SMPTE_Color_Bars.svg.png"
    },
    .timestamps = {
      .start = start_time,
      .end = end_time
    },
    .type = RichPresence::listening
  });

  system("read");

  client.close();

  return 0;
}
