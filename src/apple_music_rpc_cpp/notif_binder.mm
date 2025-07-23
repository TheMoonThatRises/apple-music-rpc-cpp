/*
  Copyright 2025 Peter Duanmu

  You should have received a copy of the GNU General Public License along
  with apple-music-rpc-cpp. If not, see <https://www.gnu.org/licenses/>.
*/

#import <Foundation/Foundation.h>

#include <string>

#import "include/notif_binder.h"

#include "include/music_types.hpp"

@implementation NotificationCenterBinder

+ (NotificationCenterBinder*)shared {
  static NotificationCenterBinder* _shared = nil;

  if (!_shared) {
    _shared = [[NotificationCenterBinder alloc] init];
  }

  return _shared;
}

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

- (void)set_safe_string:(std::string*)field from_item:(id)item {
  if (item && [item isKindOfClass:[NSString class]]) {
    *field = [(NSString*)item UTF8String];
  }
}

- (void)set_player_callback:(t_player_info_callback)callback {
  self.player_info_callback = callback;
}

- (void)receive_player_info_update:(NSNotification*)notification {
  if (!self.player_info_callback) {
    return;
  }

  NSDictionary *userInfo = [notification userInfo];

  MusicPlayerInfo playerInfo {};

  if (userInfo) {
    [self set_safe_string:&playerInfo.album from_item:userInfo[@"Album"]];
    [self set_safe_string:&playerInfo.artist from_item:userInfo[@"Artist"]];
    [self set_safe_string:&playerInfo.composer from_item:userInfo[@"Composer"]];
    [self set_safe_string:&playerInfo.name from_item:userInfo[@"Name"]];
    [self set_safe_string:&playerInfo.player_state
          from_item:userInfo[@"Player State"]];
    [self set_safe_string:&playerInfo.library_persistent_id
          from_item:userInfo[@"Library PersistentID"]];

    if (userInfo[@"Total Time"]) {
      playerInfo.total_time = [userInfo[@"Total Time"] intValue];
    }

    if (userInfo[@"PersistentID"]) {
      playerInfo.persistent_id = [userInfo[@"PersistentID"] longLongValue];
    }
  }

  self.player_info_callback(playerInfo);
}

@end
