/*
  Copyright 2026 Peter Duanmu

  You should have received a copy of the GNU General Public License along
  with apple-music-rpc-cpp. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef INCLUDE_OBJC_BRIDGE_HPP_
#define INCLUDE_OBJC_BRIDGE_HPP_

#include <string>

#include "./callback_types.hpp"
#include "./music_types.hpp"

namespace objc_bridge {
void bind_music_player_info(t_player_info_callback);
void bind_discord_launch(t_discord_launch_callback);
double get_music_playback_info();
MusicPlayerInfo get_music_player_info();
void get_itunes_result(const std::string& song_name,
                       const std::string& artist_name,
                       const std::string& album_name,
                       t_itunes_songs_callback);
void run_cf_main_loop();
}  // namespace objc_bridge

#endif  // INCLUDE_OBJC_BRIDGE_HPP_
