/*
  Copyright 2025 Peter Duanmu

  You should have received a copy of the GNU General Public License along
  with apple-music-rpc-cpp. If not, see <https://www.gnu.org/licenses/>.
*/

#import <Foundation/Foundation.h>

#import "include/apple_music.h"

#include "include/music_types.hpp"

@implementation NotificationCenterBinder

- (id)init {
  self = [super init];

  if (self) {
    [[NSDistributedNotificationCenter defaultCenter]
      addObserver:self
      selector:@selector(receive_player_info_update:)
      name:@"com.apple.Music.playerInfo"
      object:nil
    ];
  }

  return self;
}

- (void)dealloc {
  [[NSDistributedNotificationCenter defaultCenter] removeObserver:self];

  [super dealloc];
}

- (void)set_player_callback: (t_player_info_callback)callback {
  self.player_info_callback = callback;
}

- (void)receive_player_info_update: (NSNotification*)notification {
  if (!self.player_info_callback) {
    return;
  }

  NSDictionary *userInfo = [notification userInfo];

  MusicPlayerInfo playerInfo {};

  if (userInfo) {
    playerInfo.album = [userInfo[@"Album"] UTF8String];
    playerInfo.artist = [userInfo[@"Artist"] UTF8String];
    playerInfo.composer = [userInfo[@"Composer"] UTF8String];
    playerInfo.name = [userInfo[@"Name"] UTF8String];
    playerInfo.player_state = [userInfo[@"Player State"] UTF8String];
    playerInfo.total_time = [userInfo[@"Total Time"] intValue];

    if (userInfo[@"Library PersistentID"]) {
      playerInfo.library_persistent_id = [
        userInfo[@"Library PersistentID"] UTF8String];
    }

    if (userInfo[@"PersistentID"]) {
      playerInfo.persistent_id = [userInfo[@"PersistentID"] longLongValue];
    }
  }

  self.player_info_callback(playerInfo);
}

@end
