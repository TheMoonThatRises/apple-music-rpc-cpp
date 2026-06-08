/*
  Copyright 2026 Peter Duanmu

  You should have received a copy of the GNU General Public License along
  with apple-music-rpc-cpp. If not, see <https://www.gnu.org/licenses/>.
*/

#import "include/apple_music.h"

#import <Foundation/Foundation.h>
#import <bridge/include/Music.h>

#import "include/objc_convert.h"

#include <string>
#include <optional>

#include "include/music_types.hpp"

@implementation AppleMusic

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

    playerInfo.album = to_optional_string([currentTrack album]);
    playerInfo.artist = to_optional_string([currentTrack artist]);
    playerInfo.name = to_optional_string([currentTrack name]);
    playerInfo.total_time = [currentTrack duration] * 1000;
  }

  return playerInfo;
}

@end
