/*
  Copyright 2026 Peter Duanmu

  You should have received a copy of the GNU General Public License along
  with apple-music-rpc-cpp. If not, see <https://www.gnu.org/licenses/>.
*/

#include "include/music_types.hpp"

#include <algorithm>
#include <string>
#include <vector>
#include <iostream>

#include <discord_ipc_cpp/json.hpp>
#include <discord_ipc_cpp/parser.hpp>

using discord_ipc_cpp::json::JSON;
using discord_ipc_cpp::json::JSONString;
using discord_ipc_cpp::json::JSONInt;
using discord_ipc_cpp::json::JSONDouble;
using discord_ipc_cpp::json::JSONLong;
using discord_ipc_cpp::json::JSONBool;
using discord_ipc_cpp::json::JSONArray;
using discord_ipc_cpp::json::Parser;

ITunesSong ITunesSong::from_json(const JSON& data) {
  auto get = [&data]<typename T>(const std::string& key) {
    return data.safe_at(key).value_or(JSON()).safe_as<T>();
  };  // NOLINT(readability/braces)

  return {
    // cppcheck-suppress internalAstError
    .wrapper_type = get.operator()<JSONString>("wrapperType"),
    .kind = get.operator()<JSONString>("kind"),
    .artist_id = get.operator()<JSONInt>("artistId"),
    .collection_id = get.operator()<JSONInt>("collectionId"),
    .track_id = get.operator()<JSONInt>("trackId"),
    .collection_name = get.operator()<JSONString>("collectionName"),
    .track_name = get.operator()<JSONString>("trackName"),
    .collection_censored_name = get.template
      operator()<JSONString>("collectionCensoredName"),
    .track_censored_name = get.template
      operator()<JSONString>("trackCensoredName"),
    .artist_view_url = get.operator()<JSONString>("artistViewUrl"),
    .collection_view_url = get.operator()<JSONString>("collectionViewUrl"),
    .track_view_url = get.operator()<JSONString>("trackViewUrl"),
    .preview_url = get.operator()<JSONString>("previewUrl"),
    .artwork_url_30 = get.operator()<JSONString>("artworkUrl30"),
    .artwork_url_60 = get.operator()<JSONString>("artworkUrl60"),
    .artwork_url_100 = get.operator()<JSONString>("artworkUrl100"),
    .collection_price = get.operator()<JSONDouble>("collectionPrice"),
    .track_price = get.operator()<JSONDouble>("trackPrice"),
    .release_date = get.operator()<JSONString>("releaseDate"),
    .collection_explicitness = get.
      operator()<JSONString>("collectionExplicitness"),
    .track_explicitness = get.operator()<JSONString>("trackExplicitness"),
    .disc_count = get.operator()<JSONInt>("discCount"),
    .disc_number = get.operator()<JSONInt>("discNumber"),
    .track_count = get.operator()<JSONInt>("trackCount"),
    .track_number = get.operator()<JSONInt>("trackNumber"),
    .track_time_millis = get.operator()<JSONInt>("trackTimeMillis"),
    .country = get.operator()<JSONString>("country"),
    .currency = get.operator()<JSONString>("currency"),
    .primary_genre_name = get.operator()<JSONString>("primaryGenreName"),
    .is_streamable = get.operator()<JSONBool>("isStreamable"),
  };
}

// cppcheck-suppress unusedFunction
ITunesSongResults ITunesSongResults::from_string(const std::string& data) {
  JSON json_data = Parser::parse(data);

  int result_count = json_data["resultCount"].as<JSONInt>();

  JSONArray results_json = json_data["results"].as<JSONArray>();
  std::vector<ITunesSong> results(result_count);

  std::transform(
    results_json.cbegin(), results_json.cend(), results.begin(),
    [](const auto& item) { return ITunesSong::from_json(item); });

  return {
    .result_count = result_count,
    .results = results
  };
}
