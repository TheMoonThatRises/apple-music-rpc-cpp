/*
  Copyright 2025 Peter Duanmu

  You should have received a copy of the GNU General Public License along
  with apple-music-rpc-cpp. If not, see <https://www.gnu.org/licenses/>.
*/

#include <unistd.h>

#include <string>

#include "include/utils.hpp"

std::string find_discord_ipc_file() {
  std::string user_tmp_dir = getenv("TMPDIR");
  std::string base_ipc_name = "discord-ipc-";

  for (int i = 0; i < 10; ++i) {
    std::string test_path = user_tmp_dir + base_ipc_name + std::to_string(i);

    if (access(test_path.c_str(), R_OK) >= 0) {
      return test_path;
    }
  }

  return "";
}
