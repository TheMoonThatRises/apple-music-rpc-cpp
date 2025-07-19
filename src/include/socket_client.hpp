/*
  Copyright 2025 Peter Duanmu

  You should have received a copy of the GNU General Public License along
  with apple-music-rpc-cpp. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef APPLE_MUSIC_RPC_CPP_SRC_INCLUDE_SOCKET_CLIENT_HPP_
#define APPLE_MUSIC_RPC_CPP_SRC_INCLUDE_SOCKET_CLIENT_HPP_

#include <poll.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <string>
#include <optional>
#include <vector>

namespace websockets {
class SocketClient {
 private:
    std::string _socket_file;

    int _client_socket;
    struct sockaddr_un _server_addr;

    struct pollfd _fds[1];

 public:
    explicit SocketClient(const std::string& socket_file);
    ~SocketClient();

    bool connect();
    bool close();

    bool send_data(const std::vector<char>& data);
    std::optional<std::vector<char>> recv_data(int buffer_size);
    std::optional<std::vector<char>> recv_data(int buffer_size, int timeout);
};
}  // namespace websockets

#endif  // APPLE_MUSIC_RPC_CPP_SRC_INCLUDE_SOCKET_CLIENT_HPP_
