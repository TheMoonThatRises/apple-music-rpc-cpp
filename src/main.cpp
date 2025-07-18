/*
  Copyright 2025 Peter Duanmu

  You should have received a copy of the GNU General Public License along
  with apple-music-rpc-cpp. If not, see <https://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <format>
#include <string>

#include "include/client_connection.hpp"
#include "include/json.hpp"
#include "include/utils.hpp"

int main() {
  std::string discord_ipc_file = find_discord_ipc_file();
  // std::string music_client_id = "773825528921849856";  // apple music
  std::string music_client_id = "1313374141960949831";  // spwifiy

  JSON::JSONObject handshake_json = {
    {"v", JSON(1)},
    {"client_id", JSON(music_client_id)}
  };
  std::string handshake_data = JSON(handshake_json).to_string();

  std::cout << handshake_data << std::endl;

  ClientConnection client(discord_ipc_file);

  bool ret = client.connect();

  if (!ret) {
    std::cout << "failed to connect to socket" << std::endl;

    return 1;
  }

  ret = client.send_data(0, handshake_data);

  if (!ret) {
    std::cout << "failed to send packet data" << std::endl;

    return 1;
  }

  std::string data = client.recv_data();
  JSON recv_json = JSON::from_string(data);

  std::cout << data << std::endl;
  std::cout << recv_json.to_string() << std::endl;

  client.close();

  return 0;
}
