/*
  Copyright 2025 Peter Duanmu

  You should have received a copy of the GNU General Public License along
  with apple-music-rpc-cpp. If not, see <https://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <format>
#include <string>

#include "include/discord_ipc_client.hpp"
#include "include/json.hpp"
#include "include/parser.hpp"

using discordipc::DiscordIPCClient;
using json::JSON;
using json::JSONObject;
using json::Parser;

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
