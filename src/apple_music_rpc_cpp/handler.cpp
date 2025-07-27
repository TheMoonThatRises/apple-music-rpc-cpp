/*
  Copyright 2025 Peter Duanmu

  You should have received a copy of the GNU General Public License along
  with apple-music-rpc-cpp. If not, see <https://www.gnu.org/licenses/>.
*/

#include <regex>
#include <string>
#include <utility>

#include "include/handler.hpp"
#include "include/utils.hpp"
#include "include/music_types.hpp"
#include "include/objc_bridge.hpp"

using objc_bridge::get_itunes_result;
using objc_bridge::get_music_playback_info;

void Handler::set_empty_presence() {
  if (_has_presence) {
    _client.set_empty_presence();

    _has_presence = false;
  }
}

void Handler::set_presence() {
  _client.set_presence(_presence);

  _has_presence = true;
}

void Handler::itunes_callback(const ITunesSongResults& result) {
  _song_result = std::move(result);

  const ITunesSong* song = nullptr;

  if (_song_result.result_count > 0) {
    std::regex remove_paren_regex("\\(.*\\)$");

    std::string match_album_lower = to_lower(_player_info.album.value());
    std::string match_track_lower = to_lower(_player_info.name.value());

    for (const auto& itunes_song : _song_result.results) {
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
    _presence.details_url = song->track_view_url;
    _presence.state_url = song->artist_view_url;
    _presence.assets->large_image = song->artwork_url_100;
    _presence.assets->large_url = song->collection_view_url;
  }

  set_presence();
}

void Handler::set_accurate_time() {
  if (_player_info.total_time.has_value()) {
    int song_length = std::round(_player_info.total_time.value() / 1000);
    int current_time = get_current_time_seconds();

    int current_song_time = std::round(get_music_playback_info());

    int start_time = 2 * get_current_time_seconds()
      - current_time - current_song_time;

    _presence.timestamps->start = start_time;
    _presence.timestamps->end = start_time + song_length;
  }
}

Handler::Handler(discord_ipc_cpp::DiscordIPCClient& client) :
_client(client), _presence({}), _player_info({}),
_song_result({}), _has_presence(true) {
  set_empty_presence();
}

void Handler::music_player_binder(const MusicPlayerInfo& player_info) {
  _player_info = std::move(player_info);

  if (_player_info.player_state == "Paused" ||
      _player_info.player_state == "Stopped"
  ) {
    set_empty_presence();

    return;
  }

  _presence = construct_presence(_player_info);

  if (_player_info.player_state == "Playing") {
    set_accurate_time();
  }

  if (!_player_info.name.has_value() || !_player_info.artist.has_value()) {
    set_presence();

    return;
  }

  get_itunes_result(
    _player_info.name.value(),
    _player_info.artist.value(),
    _player_info.album.value_or(""),
    [this](const auto& result) {
      this->itunes_callback(result);
    });
}
