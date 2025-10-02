/*
  Copyright 2025 Peter Duanmu

  You should have received a copy of the GNU General Public License along
  with apple-music-rpc-cpp. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef APPLE_MUSIC_RPC_CPP_INCLUDE_HANDLER_HPP_
#define APPLE_MUSIC_RPC_CPP_INCLUDE_HANDLER_HPP_

#include <discord_ipc_cpp/discord_ipc_client.hpp>
#include <discord_ipc_cpp/ipc_types.hpp>

#include "./music_types.hpp"

class Handler {
 private:
  discord_ipc_cpp::DiscordIPCClient& _client;

  discord_ipc_cpp::ipc_types::RichPresence _presence;
  MusicPlayerInfo _player_info;
  ITunesSongResults _song_result;

  bool _has_presence;

 private:
  void set_empty_presence();
  void set_presence();

 protected:
  void itunes_callback(const ITunesSongResults& result);
  void set_accurate_time();

 public:
  explicit Handler(discord_ipc_cpp::DiscordIPCClient& client);

  void attempt_discord_connect();

  void music_player_binder(const MusicPlayerInfo& player_info);
};

#endif  // APPLE_MUSIC_RPC_CPP_INCLUDE_HANDLER_HPP_
