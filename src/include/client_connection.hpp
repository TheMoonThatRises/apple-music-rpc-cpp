/*
  Copyright 2025 Peter Duanmu

  You should have received a copy of the GNU General Public License along
  with apple-music-rpc-cpp. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef APPLE_MUSIC_RPC_CPP_SRC_INCLUDE_CLIENT_CONNECTION_HPP_
#define APPLE_MUSIC_RPC_CPP_SRC_INCLUDE_CLIENT_CONNECTION_HPP_

#include <sys/socket.h>
#include <sys/un.h>

#include <string>
#include <vector>

class ClientConnection {
 private:
    std::string _socket_file;

    int _client_socket;
    struct sockaddr_un _server_addr;

 protected:
    static std::vector<char> encode_packet(int op, const std::string& data);

 public:
    explicit ClientConnection(const std::string& socket_file);
    ~ClientConnection();

    bool connect();
    bool close();

    bool send_data(int op_code, const std::string& data);
    std::string recv_data();
};

#endif  // APPLE_MUSIC_RPC_CPP_SRC_INCLUDE_CLIENT_CONNECTION_HPP_
