/*
  Copyright 2025 Peter Duanmu

  You should have received a copy of the GNU General Public License along
  with apple-music-rpc-cpp. If not, see <https://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <string>

#include <discord_ipc_cpp/discord_ipc_client.hpp>

using discord_ipc_cpp::DiscordIPCClient;

int main() {
  // std::string music_client_id = "773825528921849856";  // apple music
  std::string music_client_id = "1313374141960949831";  // spwifiy

  DiscordIPCClient client(music_client_id);

  bool ret = client.connect();

  if (!ret) {
    std::cout << "failed to connect to socket" << std::endl;

    return 1;
  }

  system("read");

  client.close();

  return 0;
}
