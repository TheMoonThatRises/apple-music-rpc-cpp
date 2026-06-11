/*
  Copyright 2026 Peter Duanmu

  You should have received a copy of the GNU General Public License along
  with apple-music-rpc-cpp. If not, see <https://www.gnu.org/licenses/>.
*/

#include "include/handler.hpp"

#include <iostream>
#include <regex>
#include <cmath>
#include <thread>
#include <string>
#include <utility>

#include "include/artwork_cache.hpp"
#include "include/utils.hpp"
#include "include/music_types.hpp"
#include "include/objc_bridge.hpp"

using objc_bridge::get_itunes_result;
using objc_bridge::get_music_playback_info;
using objc_bridge::get_music_player_info;

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

void Handler::update_presence_assets(const ITunesSong& song) {
  _presence.details_url = song.track_view_url;
  _presence.state_url = song.artist_view_url;
  _presence.assets->large_image = song.artwork_url_100;
  _presence.assets->large_url = song.collection_view_url;
}

void Handler::itunes_callback(ITunesSongResults result) {
  static const std::regex remove_paren_regex("\\(.*\\)$");

  ITunesSongResults song_result = std::move(result);

  std::string name = _player_info.name.value_or("");
  std::string artist = _player_info.artist.value_or("");
  std::string album = _player_info.album.value_or("");

  const ITunesSong* song = nullptr;

  if (song_result.result_count > 0) {
    std::string match_album_lower = to_lower(album);
    std::string match_track_lower = to_lower(name);

    for (const auto& itunes_song : song_result.results) {
      std::string collection_lower = to_lower(
        itunes_song.collection_name.value_or(""));
      std::string strip_collection = std::regex_replace(
        collection_lower, remove_paren_regex, "");
      std::string track_lower = to_lower(itunes_song.track_name.value_or(""));

      if ((collection_lower.find(match_album_lower) != std::string::npos ||
          strip_collection.find(match_album_lower) != std::string::npos) &&
          track_lower.find(match_track_lower) != std::string::npos
      ) {
        song = &itunes_song;

        break;
      }
    }
  }

  _artworkCache.add_artwork(name, artist, album, song ? *song : ITunesSong{});

  if (song) {
    update_presence_assets(*song);
  }

  set_presence();
}

void Handler::set_accurate_time() {
  if (_player_info.total_time.has_value()) {
    int song_length = std::round(_player_info.total_time.value() / 1000);
    int64_t current_time = get_current_time_millis() / 1000;

    int current_song_time = std::round(get_music_playback_info());

    int64_t start_time = 2 * (get_current_time_millis() / 1000)
      - current_time - current_song_time;

    _presence.timestamps->start = start_time;
    _presence.timestamps->end = start_time + song_length;
  }
}

Handler::Handler(
  discord_ipc_cpp::DiscordIPCClient& client,
  ArtworkCache& artworkCache
) :
_artworkCache(artworkCache),
_client(client),
_presence({}),
_player_info({}),
_has_presence(true) {}

void Handler::attempt_discord_connect(bool should_attempt) {
  int max_attempts = should_attempt ? 5 : 1;
  int attempts = 0;

  while (attempts < max_attempts) {
    std::cout << "Attempting to connect to Discord "
              << "(" << attempts + 1 << "/" << max_attempts << ")"
              << "..." << std::endl;

    bool ret = _client.connect();

    if (ret) {
      std::cout << "Successfully connected to Discord" << std::endl;

      while (!_client.has_successful_auth()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(700));
      }

      music_player_binder(get_music_player_info());

      break;
    } else {
      ++attempts;

      std::this_thread::sleep_for(std::chrono::seconds(2 * attempts));
    }
  }

  if (attempts >= max_attempts) {
    std::cout << "Failed to connect to Discord... "
              << "Will attempt to connect on next application launch"
              << std::endl;
  }
}

void Handler::music_player_binder(const MusicPlayerInfo& player_info) {
  _player_info = player_info;

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

  std::string name = _player_info.name.value_or("");
  std::string artist = _player_info.artist.value_or("");
  std::string album = _player_info.album.value_or("");

  std::optional<ITunesSong> result = _artworkCache.get_artwork(
    name, artist, album);

  if (result.has_value()) {
    update_presence_assets(result.value());
    set_presence();
  } else {
    get_itunes_result(
      name,
      artist,
      album,
      [this](auto result) {
        this->itunes_callback(std::move(result));
      });
  }
}
