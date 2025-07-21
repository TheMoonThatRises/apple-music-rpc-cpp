/*
  Copyright 2025 Peter Duanmu

  You should have received a copy of the GNU General Public License along
  with apple-music-rpc-cpp. If not, see <https://www.gnu.org/licenses/>.
*/

#import "include/objc_bridge.hpp"
#import "include/apple_music.h"

#include "include/music_types.hpp"

namespace objc_bridge {
void bind_music_player_info(t_player_info_callback callback) {
  [[NotificationCenterBinder shared] set_player_callback:callback];
}

void run_cf_main_loop() {
  CFRunLoopRun();
}
}  // namespace ObjCBridge
