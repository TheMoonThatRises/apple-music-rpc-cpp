/*
  Copyright 2026 Peter Duanmu

  You should have received a copy of the GNU General Public License along
  with apple-music-rpc-cpp. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef INCLUDE_CALLBACK_TYPES_HPP_
#define INCLUDE_CALLBACK_TYPES_HPP_

#include <functional>

#include "include/music_types.hpp"

typedef std::function<void(MusicPlayerInfo)> t_player_info_callback;

typedef std::function<void(ITunesSongResults)> t_itunes_songs_callback;

typedef std::function<void()> t_discord_launch_callback;

#endif  // INCLUDE_CALLBACK_TYPES_HPP_
