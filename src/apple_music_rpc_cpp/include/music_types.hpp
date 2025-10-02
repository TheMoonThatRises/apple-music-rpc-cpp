/*
  Copyright 2025 Peter Duanmu

  You should have received a copy of the GNU General Public License along
  with apple-music-rpc-cpp. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef APPLE_MUSIC_RPC_CPP_INCLUDE_MUSIC_TYPES_HPP_
#define APPLE_MUSIC_RPC_CPP_INCLUDE_MUSIC_TYPES_HPP_

#include <functional>
#include <string>
#include <optional>
#include <vector>

#include <discord_ipc_cpp/json.hpp>

typedef struct MusicPlayerInfo {
  std::optional<std::string> album;
  std::optional<std::string> artist;
  std::optional<std::string> composer;
  std::optional<std::string> genre;
  std::optional<std::string> library_persistent_id;
  std::optional<std::string> name;
  std::optional<int64_t> persistent_id;
  std::optional<std::string> player_state;
  std::optional<int> total_time;
} MusicPlayerInfo;

typedef struct ITunesSong {
  std::optional<std::string> wrapper_type;
  std::optional<std::string> kind;
  std::optional<int> artist_id;
  std::optional<int> collection_id;
  std::optional<int> track_id;
  std::optional<std::string> artist_name;
  std::optional<std::string> collection_name;
  std::optional<std::string> track_name;
  std::optional<std::string> collection_censored_name;
  std::optional<std::string> track_censored_name;
  std::optional<std::string> artist_view_url;
  std::optional<std::string> collection_view_url;
  std::optional<std::string> track_view_url;
  std::optional<std::string> preview_url;
  std::optional<std::string> artwork_url_30;
  std::optional<std::string> artwork_url_60;
  std::optional<std::string> artwork_url_100;
  std::optional<double> collection_price;
  std::optional<double> track_price;
  std::optional<std::string> release_date;
  std::optional<std::string> collection_explicitness;
  std::optional<std::string> track_explicitness;
  std::optional<int> disc_count;
  std::optional<int> disc_number;
  std::optional<int> track_count;
  std::optional<int> track_number;
  std::optional<int> track_time_millis;
  std::optional<std::string> country;
  std::optional<std::string> currency;
  std::optional<std::string> primary_genre_name;
  std::optional<bool> is_streamable;

 public:
  static ITunesSong from_json(const discord_ipc_cpp::json::JSON& data);
} ITunesSong;

typedef struct ITunesSongResults {
  int result_count;
  std::vector<ITunesSong> results;

 public:
  static ITunesSongResults from_string(const std::string& data);
} ITunesSongResults;

#endif  // APPLE_MUSIC_RPC_CPP_INCLUDE_MUSIC_TYPES_HPP_
