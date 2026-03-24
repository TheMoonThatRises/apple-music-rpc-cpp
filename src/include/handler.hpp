/*
  Copyright 2026 Peter Duanmu

  You should have received a copy of the GNU General Public License along
  with apple-music-rpc-cpp. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef INCLUDE_HANDLER_HPP_
#define INCLUDE_HANDLER_HPP_

#include <discord_ipc_cpp/discord_ipc_client.hpp>
#include <discord_ipc_cpp/ipc_types.hpp>

#include "include/music_types.hpp"

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

  void attempt_discord_connect(bool should_attempt);

  void music_player_binder(const MusicPlayerInfo& player_info);
};

#endif  // INCLUDE_HANDLER_HPP_
