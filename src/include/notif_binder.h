/*
  Copyright 2026 Peter Duanmu

  You should have received a copy of the GNU General Public License along
  with apple-music-rpc-cpp. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef INCLUDE_NOTIF_BINDER_H_
#define INCLUDE_NOTIF_BINDER_H_

#import <Foundation/Foundation.h>

#include "include/callback_types.hpp"

@interface NotificationCenterBinder : NSObject

@property(nonatomic) t_player_info_callback player_info_callback;
@property(nonatomic) t_discord_launch_callback discord_launch_callback;

+ (NotificationCenterBinder*)shared;  // NOLINT(readability/casting)

- (id)init;
- (void)dealloc;

- (void)set_player_callback:(t_player_info_callback)callback;
- (void)set_discord_callback:(t_discord_launch_callback)callback;

- (void)receive_player_info_update:
  (NSNotification*)notification;  // NOLINT(readability/casting)
- (void)receive_discord_launch_notif:
  (NSNotification*)notification;  // NOLINT(readability/casting)

@end

#endif  // INCLUDE_NOTIF_BINDER_H_
