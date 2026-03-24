/*
  Copyright 2025 Peter Duanmu

  You should have received a copy of the GNU General Public License along
  with apple-music-rpc-cpp. If not, see <https://www.gnu.org/licenses/>.
*/

#import <Foundation/Foundation.h>

#include <string>
#include <optional>

#import "include/apple_music.h"
#import "bridge/include/Music.h"

#include "include/music_types.hpp"

@implementation AppleMusic

+ (void)set_safe_string:(std::optional<std::string>*)field from_item:(id)item {
  if (item && [item isKindOfClass:[NSString class]]) {
    *field = [(NSString*)item UTF8String];
  } else {
    *field = std::nullopt;
  }
}

+ (double)retrieve_playback_info {
  MusicApplication* music = [SBApplication
    applicationWithBundleIdentifier:@"com.apple.Music"
  ];

  if ([music playerState] == MusicEPlSPlaying) {
    return [music playerPosition];
  } else {
    return 0.0;
  }
}

+ (MusicPlayerInfo)retrieve_current_player_info {
  MusicApplication* music = [SBApplication
    applicationWithBundleIdentifier:@"com.apple.Music"
  ];

  MusicPlayerInfo playerInfo {};

  if (!music.isRunning) {
    playerInfo.player_state = "Stopped";

    return playerInfo;
  }

  playerInfo.player_state = [music playerState] == MusicEPlSPlaying
    ? "Playing"
    : "Stopped";

  if ([music playerState] == MusicEPlSPlaying) {
    MusicTrack* currentTrack = [music currentTrack];

    [AppleMusic set_safe_string:&playerInfo.album
      from_item:[currentTrack album]];
    [AppleMusic set_safe_string:&playerInfo.artist
      from_item:[currentTrack artist]];
    [AppleMusic set_safe_string:&playerInfo.name
      from_item:[currentTrack name]];
    playerInfo.total_time = [currentTrack duration] * 1000;
  }

  return playerInfo;
}

@end
