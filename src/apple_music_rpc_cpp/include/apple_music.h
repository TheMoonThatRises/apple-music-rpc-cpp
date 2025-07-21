/*
  Copyright 2025 Peter Duanmu

  You should have received a copy of the GNU General Public License along
  with apple-music-rpc-cpp. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef APPLE_MUSIC_RPC_CPP_SRC_APPLE_MUSIC_RPC_CPP_INCLUDE_APPLE_MUSIC_H_
#define APPLE_MUSIC_RPC_CPP_SRC_APPLE_MUSIC_RPC_CPP_INCLUDE_APPLE_MUSIC_H_

#import <Foundation/Foundation.h>

#include "./music_types.hpp"

@interface NotificationCenterBinder : NSObject

@property(nonatomic) t_player_info_callback player_info_callback;

+ (NotificationCenterBinder*)shared;  // NOLINT(readability/casting)

- (id)init;
- (void)dealloc;

- (void)set_player_callback: (t_player_info_callback)callback;

- (void)receive_player_info_update:
  (NSNotification*)notification;  // NOLINT(readability/casting)

@end

#endif  // APPLE_MUSIC_RPC_CPP_SRC_APPLE_MUSIC_RPC_CPP_INCLUDE_APPLE_MUSIC_H_
