/*
  Copyright 2025 Peter Duanmu

  You should have received a copy of the GNU General Public License along
  with apple-music-rpc-cpp. If not, see <https://www.gnu.org/licenses/>.
*/

#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>

#include <string>
#include <optional>

#import "include/notif_binder.h"
#import "include/apple_music.h"

#include "include/callback_types.hpp"
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

    [[[NSWorkspace sharedWorkspace] notificationCenter]
      addObserver:self
      selector:@selector(receive_discord_launch_notif:)
      name:NSWorkspaceDidLaunchApplicationNotification
      object:nil
    ];
  }

  return self;
}

- (void)dealloc {
  [[NSDistributedNotificationCenter defaultCenter] removeObserver:self];

  [super dealloc];
}

- (void)set_player_callback:(t_player_info_callback)callback {
  self.player_info_callback = callback;
}

- (void)set_discord_callback:(t_discord_launch_callback)callback {
  self.discord_launch_callback = callback;
}

- (void)receive_player_info_update:(NSNotification*)notification {
  if (!self.player_info_callback) {
    return;
  }

  NSDictionary *userInfo = [notification userInfo];

  MusicPlayerInfo playerInfo {};

  if (userInfo) {
    [AppleMusic set_safe_string:&playerInfo.album
      from_item:userInfo[@"Album"]];
    [AppleMusic set_safe_string:&playerInfo.artist
      from_item:userInfo[@"Artist"]];
    [AppleMusic set_safe_string:&playerInfo.composer
      from_item:userInfo[@"Composer"]];
    [AppleMusic set_safe_string:&playerInfo.name
      from_item:userInfo[@"Name"]];
    [AppleMusic set_safe_string:&playerInfo.player_state
          from_item:userInfo[@"Player State"]];
    [AppleMusic set_safe_string:&playerInfo.library_persistent_id
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

- (void)receive_discord_launch_notif:(NSNotification*)notification {
  if (!self.discord_launch_callback) {
    return;
  }

  NSDictionary* userInfo = [notification userInfo];
  NSRunningApplication* app = userInfo[NSWorkspaceApplicationKey];

  NSString* bundleId = [app bundleIdentifier];

  NSString* regexSearchPattern = @"^com\\.hnc\\.discord.*$";
  NSRange searchRange = NSMakeRange(0, [bundleId length]);
  NSError *error = nil;
  NSRegularExpression *regex = [NSRegularExpression
    regularExpressionWithPattern:regexSearchPattern
    options:NSRegularExpressionCaseInsensitive
    error:&error
  ];

  if (error) {
    NSLog(@"Failed to create regular expression: %@", error);
    return;
  }

  NSTextCheckingResult *firstMatch = [regex
    firstMatchInString:bundleId
    options:0
    range:searchRange
  ];

  if (firstMatch) {
    self.discord_launch_callback();
  }
}

@end
