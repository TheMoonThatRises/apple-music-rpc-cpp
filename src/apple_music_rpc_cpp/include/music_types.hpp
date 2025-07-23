/*
  Copyright 2025 Peter Duanmu

  You should have received a copy of the GNU General Public License along
  with apple-music-rpc-cpp. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef APPLE_MUSIC_RPC_CPP_SRC_APPLE_MUSIC_RPC_CPP_INCLUDE_MUSIC_TYPES_HPP_
#define APPLE_MUSIC_RPC_CPP_SRC_APPLE_MUSIC_RPC_CPP_INCLUDE_MUSIC_TYPES_HPP_

#include <functional>
#include <string>
#include <vector>

#include <discord_ipc_cpp/json.hpp>

typedef struct MusicPlayerInfo {
  std::string album;
  std::string artist;
  std::string composer;
  std::string genre;
  std::string library_persistent_id;
  std::string name;
  int64_t persistent_id;
  std::string player_state;
  int total_time;
} MusicPlayerInfo;

typedef struct ITunesSong {
  std::string wrapper_type;
  std::string kind;
  std::string artist_id;
  std::string collection_id;
  std::string track_id;
  std::string artist_name;
  std::string collection_name;
  std::string track_name;
  std::string collection_censored_name;
  std::string track_censored_name;
  std::string artist_view_url;
  std::string collection_view_url;
  std::string track_view_url;
  std::string preview_url;
  std::string artwork_url_30;
  std::string artwork_url_60;
  std::string artwork_url_100;
  double collection_price;
  double track_price;
  std::string release_date;
  std::string collection_explicitness;
  std::string track_explicitness;
  int disc_count;
  int disc_number;
  int track_count;
  int track_number;
  int64_t track_time_millis;
  std::string country;
  std::string currency;
  std::string primary_genre_name;
  bool is_streamable;

 public:
  static ITunesSong from_json(const discord_ipc_cpp::json::JSON& data);
} ITunesSong;

typedef struct ITunesSongResults {
  int result_count;
  std::vector<ITunesSong> results;

 public:
  static ITunesSongResults from_string(const std::string& data);
} ITunesSongResults;

typedef std::function<void(const MusicPlayerInfo&)> t_player_info_callback;

typedef std::function<void(const ITunesSongResults&)> t_itunes_songs_callback;

#endif  // APPLE_MUSIC_RPC_CPP_SRC_APPLE_MUSIC_RPC_CPP_INCLUDE_MUSIC_TYPES_HPP_
