/*
  Copyright 2025 Peter Duanmu

  You should have received a copy of the GNU General Public License along
  with apple-music-rpc-cpp. If not, see <https://www.gnu.org/licenses/>.
*/

#include <algorithm>
#include <string>
#include <vector>
#include <iostream>

#include <discord_ipc_cpp/json.hpp>
#include <discord_ipc_cpp/parser.hpp>

#include "include/music_types.hpp"

using discord_ipc_cpp::json::JSON;
using discord_ipc_cpp::json::JSONString;
using discord_ipc_cpp::json::JSONInt;
using discord_ipc_cpp::json::JSONDouble;
using discord_ipc_cpp::json::JSONLong;
using discord_ipc_cpp::json::JSONBool;
using discord_ipc_cpp::json::JSONArray;
using discord_ipc_cpp::json::Parser;

ITunesSong ITunesSong::from_json(const JSON& data) {
  return {
    .wrapper_type = data["wrapperType"].as<JSONString>(),
    .kind = data["kind"].as<JSONString>(),
    .artist_id = data["artistId"].as<JSONInt>(),
    .collection_id = data["collectionId"].as<JSONInt>(),
    .track_id = data["trackId"].as<JSONInt>(),
    .collection_name = data["collectionName"].as<JSONString>(),
    .track_name = data["trackName"].as<JSONString>(),
    .collection_censored_name = data["collectionCensoredName"].as<JSONString>(),
    .track_censored_name = data["trackCensoredName"].as<JSONString>(),
    .artist_view_url = data["artistViewUrl"].as<JSONString>(),
    .collection_view_url = data["collectionViewUrl"].as<JSONString>(),
    .track_view_url = data["trackViewUrl"].as<JSONString>(),
    .preview_url = data["previewUrl"].as<JSONString>(),
    .artwork_url_30 = data["artworkUrl30"].as<JSONString>(),
    .artwork_url_60 = data["artworkUrl60"].as<JSONString>(),
    .artwork_url_100 = data["artworkUrl100"].as<JSONString>(),
    .collection_price = data["collectionPrice"].as<JSONDouble>(),
    .track_price = data["trackPrice"].as<JSONDouble>(),
    .release_date = data["releaseDate"].as<JSONString>(),
    .collection_explicitness = data["collectionExplicitness"].as<JSONString>(),
    .track_explicitness = data["trackExplicitness"].as<JSONString>(),
    .disc_count = data["discCount"].as<JSONInt>(),
    .disc_number = data["discNumber"].as<JSONInt>(),
    .track_count = data["trackCount"].as<JSONInt>(),
    .track_number = data["trackNumber"].as<JSONInt>(),
    .track_time_millis = data["trackTimeMillis"].as<JSONInt>(),
    .country = data["country"].as<JSONString>(),
    .currency = data["currency"].as<JSONString>(),
    .primary_genre_name = data["primaryGenreName"].as<JSONString>(),
    .is_streamable = data["isStreamable"].as<JSONBool>()
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
