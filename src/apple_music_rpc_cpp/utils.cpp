/*
  Copyright 2025 Peter Duanmu

  You should have received a copy of the GNU General Public License along
  with apple-music-rpc-cpp. If not, see <https://www.gnu.org/licenses/>.
*/

#include <signal.h>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <ctime>
#include <string>
#include <regex>

#include <discord_ipc_cpp/discord_ipc_client.hpp>
#include <discord_ipc_cpp/ipc_types.hpp>

#include "include/utils.hpp"
#include "include/objc_bridge.hpp"

using discord_ipc_cpp::DiscordIPCClient;

using discord_ipc_cpp::ipc_types::RichPresence;

using objc_bridge::get_itunes_result;

int get_current_time_seconds() {
  auto now = std::chrono::system_clock::now();
  auto now_c = std::chrono::system_clock::to_time_t(now);

  return now_c;
}

std::string to_lower(const std::string& input) {
  std::string output(input);

  std::transform(
    output.begin(), output.end(), output.begin(),
    [](const auto& ch) { return std::tolower(ch); });

  return output;
}

void register_signal_callback_handler(DiscordIPCClient& client) {
  static DiscordIPCClient* static_client = &client;

  signal(SIGINT, [](int signum) {
    static_client->close();

    exit(signum);
  });
}

void music_player_binder(
  DiscordIPCClient& client,
  const MusicPlayerInfo& player_info
) {
  if (player_info.player_state == "Paused" ||
      player_info.player_state == "Stopped"
  ) {
    client.set_empty_presence();

    return;
  }

  get_itunes_result(
    player_info.name, player_info.artist, player_info.album,
    [&client, player_info](const auto& itunes_data) {
      if (itunes_data.result_count <= 0) {
        return;
      }

      int player_time_s = std::round(player_info.total_time / 1000.);
      int start_time = get_current_time_seconds();
      int end_time = get_current_time_seconds() + player_time_s;

      RichPresence base_presence = {
        .name = "Apple Music",
        .type = RichPresence::at_listening,
        .timestamps = RichPresence::Timestamps {
          .start = start_time,
          .end = end_time
        },
        .status_display_type = RichPresence::sdt_name,
        .details = player_info.name,
        .state = player_info.artist,
        .assets = RichPresence::Assets {
          .large_text = player_info.album,
        }
      };

      const ITunesSong* song;

      std::regex remove_paren_regex("\\(.*\\)$");

      std::string match_album_lower = to_lower(player_info.album);
      std::string match_track_lower = to_lower(player_info.name);

      for (const auto& itunes_song : itunes_data.results) {
        std::string collection_lower = to_lower(itunes_song.collection_name);
        std::string strip_collection = std::regex_replace(
          collection_lower, remove_paren_regex, "");
        std::string track_lower = to_lower(itunes_song.track_name);

        if ((collection_lower.find(match_album_lower) != std::string::npos ||
             strip_collection.find(match_album_lower) != std::string::npos) &&
            track_lower.find(match_track_lower) != std::string::npos
        ) {
          song = &itunes_song;

          break;
        }
      }

      if (song) {
        base_presence.details_url = song->track_view_url;
        base_presence.state_url = song->artist_view_url;
        base_presence.assets->large_image = song->artwork_url_100;
        base_presence.assets->large_url = song->collection_view_url;
      }

      client.set_presence(base_presence);
    });
}
