/*
  Copyright 2026 Peter Duanmu

  You should have received a copy of the GNU General Public License along
  with apple-music-rpc-cpp. If not, see <https://www.gnu.org/licenses/>.
*/

#import "include/objc_convert.h"

#import <Foundation/Foundation.h>

#include <string>
#include <optional>

NSString* to_nsstring(const std::string &str) {
  return [[NSString alloc] initWithBytes:str.data()
    length:str.size()
    encoding:NSUTF8StringEncoding];
}

std::optional<std::string> to_optional_string(id value) {
  if ([value isKindOfClass:[NSString class]]) {
    if (const char* utf8 = [(NSString*)value UTF8String]) {
      return std::string(utf8);
    }
  }

  return std::nullopt;
}
