/*
  Copyright 2025 Peter Duanmu

  You should have received a copy of the GNU General Public License along
  with apple-music-rpc-cpp. If not, see <https://www.gnu.org/licenses/>.
*/

#include <sys/socket.h>
#include <unistd.h>

#include <string>
#include <vector>

#include "include/socket_client.hpp"

namespace websockets {
std::vector<char> SocketClient::encode_packet(
  int op,
  const std::string& data
) {
  int data_len = data.size();

  std::vector<char> packet(8 + data_len);

  std::memcpy(&packet[0], &op, 4);
  std::memcpy(&packet[4], &data_len, 4);
  std::memcpy(&packet[8], data.data(), data_len);

  return packet;
}

SocketClient::SocketClient(
  const std::string& socket_file)
: _socket_file(socket_file) {
  int opt = 1;

  _client_socket = ::socket(AF_UNIX, SOCK_STREAM, 0);
  setsockopt(_client_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  std::memset(&_server_addr, 0, sizeof(_server_addr));

  _server_addr.sun_family = AF_UNIX;
  std::strncpy(_server_addr.sun_path,
               _socket_file.c_str(),
               sizeof(_server_addr.sun_path) - 1);
}

SocketClient::~SocketClient() {
  close();
}

bool SocketClient::connect() {
  if (_client_socket < 0) {
    return false;
  }

  int ret = ::connect(_client_socket,
                      reinterpret_cast<const struct sockaddr*>(&_server_addr),
                      sizeof(struct sockaddr_un));

  return ret != -1;
}

bool SocketClient::close() {
  if (_client_socket >= 0) {
    ::close(_client_socket);

    _client_socket = -1;

    return true;
  } else {
    return false;
  }
}

bool SocketClient::send_data(int op_code, const std::string& data) {
  if (_client_socket < 0) {
    return false;
  }

  std::vector<char> packet = encode_packet(op_code, data);

  int ret = ::send(_client_socket, packet.data(), packet.size(), 0);

  return ret != -1;
}

std::string SocketClient::recv_data() {
  int data_len;
  std::vector<char> op_code(4), data_size(4), buffer;

  int ret = ::recv(_client_socket, op_code.data(), op_code.size(), 0);

  if (ret == -1) {
    return "";
  }

  ret = ::recv(_client_socket, data_size.data(), data_size.size(), 0);

  if (ret == -1) {
    return "";
  }

  std::memcpy(&data_len, data_size.data(), data_size.size());
  buffer.resize(data_len);

  ret = ::recv(_client_socket, buffer.data(), buffer.size(), 0);

  if (ret == -1) {
    return "";
  }

  return std::string(buffer.begin(), buffer.end());
}
}  // namespace websockets
