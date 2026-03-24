/*
  Copyright 2025 Peter Duanmu

  You should have received a copy of the GNU General Public License along
  with apple-music-rpc-cpp. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef INCLUDE_APPLE_MUSIC_H_
#define INCLUDE_APPLE_MUSIC_H_

#import <Foundation/Foundation.h>

#include <string>

#include "./music_types.hpp"

@interface AppleMusic : NSObject

+ (void)set_safe_string:(std::optional<std::string>*)field
        from_item:(NSString*)item;  // NOLINT(readability/casting)

+ (double)retrieve_playback_info;  // NOLINT(readability/casting)

+ (MusicPlayerInfo)retrieve_current_player_info;

@end

#endif  // INCLUDE_APPLE_MUSIC_H_
