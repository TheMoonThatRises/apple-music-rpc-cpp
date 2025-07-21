/*
  Copyright 2025 Peter Duanmu

  You should have received a copy of the GNU General Public License along
  with apple-music-rpc-cpp. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef APPLE_MUSIC_RPC_CPP_SRC_APPLE_MUSIC_RPC_CPP_INCLUDE_MUSIC_TYPES_HPP_
#define APPLE_MUSIC_RPC_CPP_SRC_APPLE_MUSIC_RPC_CPP_INCLUDE_MUSIC_TYPES_HPP_

#include <string>
#include <functional>

typedef struct {
  std::string album;
  std::string artist;
  std::string composer;
  std::string genre;
  std::string library_persistent_id;
  std::string name;
  int64_t persistent_id;
  std::string player_state;
  int total_time;
} MusicPlayerInfo;

typedef std::function<void(const MusicPlayerInfo&)> t_player_info_callback;

#endif  // APPLE_MUSIC_RPC_CPP_SRC_APPLE_MUSIC_RPC_CPP_INCLUDE_MUSIC_TYPES_HPP_
