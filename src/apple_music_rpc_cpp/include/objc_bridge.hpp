/*
  Copyright 2025 Peter Duanmu

  You should have received a copy of the GNU General Public License along
  with apple-music-rpc-cpp. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef APPLE_MUSIC_RPC_CPP_SRC_APPLE_MUSIC_RPC_CPP_INCLUDE_OBJC_BRIDGE_HPP_
#define APPLE_MUSIC_RPC_CPP_SRC_APPLE_MUSIC_RPC_CPP_INCLUDE_OBJC_BRIDGE_HPP_

#include <string>

#include "./music_types.hpp"

namespace objc_bridge {
void bind_music_player_info(t_player_info_callback);
void get_itunes_result(const std::string& song_name,
                       const std::string& artist_name,
                       const std::string& album_name,
                       t_itunes_songs_callback);
void run_cf_main_loop();
}  // namespace objc_bridge

#endif  // APPLE_MUSIC_RPC_CPP_SRC_APPLE_MUSIC_RPC_CPP_INCLUDE_OBJC_BRIDGE_HPP_
