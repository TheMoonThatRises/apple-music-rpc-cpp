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

std::string clamp_string(const std::string& input) {
  std::string clamped = input.substr(
    0, std::max(static_cast<int>(input.length()), 125));

  if (input.length() >= 128 || clamped.length() <= 2) {
    clamped += "   ";
  }

  return clamped;
}

RichPresence construct_presence(const MusicPlayerInfo& player_info) {
  RichPresence presence = {
    .name = "Apple Music",
    .type = RichPresence::at_listening,
    .status_display_type = RichPresence::sdt_details,
    .assets = RichPresence::Assets {}
  };

  if (player_info.total_time.has_value()) {
    int player_time_s = std::round(player_info.total_time.value() / 1000.);
    int start_time = get_current_time_seconds();
    int end_time = get_current_time_seconds() + player_time_s;

    presence.timestamps = {
      .start = start_time,
      .end = end_time
    };
  }

  if (player_info.name.has_value()) {
    presence.details = clamp_string(player_info.name.value());
  } else {
    presence.details = "Unknown song";
  }

  if (player_info.artist.has_value()) {
    presence.state = clamp_string(player_info.artist.value());
  } else {
    presence.state = "Unknown artist";
  }

  if (player_info.album.has_value()) {
    presence.assets->large_text = clamp_string(player_info.album.value());
  } else {
    presence.assets->large_text = "Unknown album";
  }

  return presence;
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

  RichPresence base_presence = construct_presence(player_info);

  if (!player_info.name.has_value() || !player_info.artist.has_value()) {
    client.set_presence(base_presence);

    return;
  }

  get_itunes_result(
    player_info.name.value(),
    player_info.artist.value(),
    player_info.album.value_or(""),
    [&client, player_info](const auto& itunes_data) {
      RichPresence presence = construct_presence(player_info);

      const ITunesSong* song = nullptr;

      if (itunes_data.result_count > 0) {
        std::regex remove_paren_regex("\\(.*\\)$");

        std::string match_album_lower = to_lower(player_info.album.value());
        std::string match_track_lower = to_lower(player_info.name.value());

        for (const auto& itunes_song : itunes_data.results) {
          std::string collection_lower = to_lower(
            itunes_song.collection_name.value());
          std::string strip_collection = std::regex_replace(
            collection_lower, remove_paren_regex, "");
          std::string track_lower = to_lower(itunes_song.track_name.value());

          if ((collection_lower.find(match_album_lower) != std::string::npos ||
              strip_collection.find(match_album_lower) != std::string::npos) &&
              track_lower.find(match_track_lower) != std::string::npos
          ) {
            song = &itunes_song;

            break;
          }
        }
      }

      if (song) {
        presence.details_url = song->track_view_url;
        presence.state_url = song->artist_view_url;
        presence.assets->large_image = song->artwork_url_100;
        presence.assets->large_url = song->collection_view_url;
      }

      client.set_presence(presence);
    });
}
