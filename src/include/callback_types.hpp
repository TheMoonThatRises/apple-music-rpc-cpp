/*
  Copyright 2025 Peter Duanmu

  You should have received a copy of the GNU General Public License along
  with apple-music-rpc-cpp. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef APPLE_MUSIC_RPC_CPP_INCLUDE_CALLBACK_TYPES_HPP_
#define APPLE_MUSIC_RPC_CPP_INCLUDE_CALLBACK_TYPES_HPP_

#include <functional>

#include "./music_types.hpp"

typedef std::function<void(const MusicPlayerInfo&)> t_player_info_callback;

typedef std::function<void(const ITunesSongResults&)> t_itunes_songs_callback;

typedef std::function<void()> t_discord_launch_callback;

#endif  // APPLE_MUSIC_RPC_CPP_INCLUDE_CALLBACK_TYPES_HPP_
