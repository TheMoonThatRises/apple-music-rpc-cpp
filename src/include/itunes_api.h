/*
  Copyright 2026 Peter Duanmu

  You should have received a copy of the GNU General Public License along
  with apple-music-rpc-cpp. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef INCLUDE_ITUNES_API_H_
#define INCLUDE_ITUNES_API_H_

#import <Foundation/Foundation.h>

#include <string>

#include "include/callback_types.hpp"

@interface ITunesAPI : NSObject

+ (void)get_itunes_result:(const std::string*)song_name
        artist:(const std::string*)artist_name
        album:(const std::string*)album_name
        callback:(const t_itunes_songs_callback)callback;

@end

#endif  // INCLUDE_ITUNES_API_H_
