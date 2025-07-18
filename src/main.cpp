/*
  Copyright 2025 Peter Duanmu

  You should have received a copy of the GNU General Public License along
  with apple-music-rpc-cpp. If not, see <https://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <format>
#include <string>

#include "include/utils.hpp"
#include "include/client_connection.hpp"

int main() {
  std::string discord_ipc_file = find_discord_ipc_file();
  std::string music_client_id = "773825528921849856";
  std::string handshake_data = std::format(
    "{{\"v\":1,\"client_id\":\"{}\"}}", music_client_id);

  ClientConnection client(discord_ipc_file);

  client.connect();

  client.send_data(0, handshake_data);

  std::string data = client.recv_data();

  std::cout << data << std::endl;

  client.close();

  return 0;
}
