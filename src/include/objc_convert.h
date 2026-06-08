/*
  Copyright 2026 Peter Duanmu

  You should have received a copy of the GNU General Public License along
  with apple-music-rpc-cpp. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef INCLUDE_OBJC_CONVERT_H_
#define INCLUDE_OBJC_CONVERT_H_

#import <Foundation/Foundation.h>

#include <string>
#include <optional>

NSString* to_nsstring(const std::string &str);
std::optional<std::string> to_optional_string(id value);

#endif  // INCLUDE_OBJC_CONVERT_H_
