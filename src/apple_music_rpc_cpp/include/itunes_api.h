/*
  Copyright 2025 Peter Duanmu

  You should have received a copy of the GNU General Public License along
  with apple-music-rpc-cpp. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef APPLE_MUSIC_RPC_CPP_SRC_APPLE_MUSIC_RPC_CPP_INCLUDE_ITUNES_API_H_
#define APPLE_MUSIC_RPC_CPP_SRC_APPLE_MUSIC_RPC_CPP_INCLUDE_ITUNES_API_H_

#import <Foundation/Foundation.h>

#include <string>

#include "./music_types.hpp"

@interface ITunesAPI : NSObject

+ (void)get_itunes_result:(const std::string*)song_name
        artist:(const std::string*)artist_name
        album:(const std::string*)album_name
        callback:(const t_itunes_songs_callback)callback;

@end

#endif  // APPLE_MUSIC_RPC_CPP_SRC_APPLE_MUSIC_RPC_CPP_INCLUDE_ITUNES_API_H_
