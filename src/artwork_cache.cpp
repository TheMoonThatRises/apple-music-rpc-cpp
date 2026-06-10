/*
  Copyright 2026 Peter Duanmu

  You should have received a copy of the GNU General Public License along
  with apple-music-rpc-cpp. If not, see <https://www.gnu.org/licenses/>.
*/

#include "include/artwork_cache.hpp"

#include <cmath>
#include <format>
#include <fstream>
#include <map>
#include <vector>
#include <optional>
#include <string>
#include <sstream>

#include "include/music_types.hpp"

ITunesSong ArtworkCache::parse_cache_data(const std::string& data) {
  if (data.length() == 0) {
    return {};
  }

  std::vector<std::string> urls(4);
  std::stringstream data_stream(data);

  for (size_t i = 0; i < urls.size(); ++i) {
    std::string url_length_str;

    std::getline(data_stream, url_length_str, '.');

    size_t url_length = stol(url_length_str);

    urls[i].resize(url_length);
    data_stream.read(urls[i].data(), url_length);
  }

  return {
    .artist_view_url = urls[1],
    .collection_view_url = urls[3],
    .track_view_url = urls[0],
    .artwork_url_100 = urls[2]
  };
}

std::string ArtworkCache::encode_cache_data(const ITunesSong& data) {
  std::string track_view_url = data.track_view_url.value_or("");
  std::string artist_view_url = data.artist_view_url.value_or("");
  std::string artwork_url_100 = data.artwork_url_100.value_or("");
  std::string collection_view_url = data.collection_view_url.value_or("");

  if (
    track_view_url.length() == 0 &&
    artist_view_url.length() == 0 &&
    artwork_url_100.length() == 0 &&
    collection_view_url.length() == 0
  ) {
    return "";
  }

  return std::format(
    "{}.{}{}.{}{}.{}{}.{}",
    track_view_url.length(),
    track_view_url,
    artist_view_url.length(),
    artist_view_url,
    artwork_url_100.length(),
    artwork_url_100,
    collection_view_url.length(),
    collection_view_url);
}

// void ArtworkCache::read_cache_file() {

// }

// void ArtworkCache::write_cache_file() {

// }

std::string ArtworkCache::generate_key(
  const std::string& name,
  const std::string& artist,
  const std::string& album
) {
  return std::format("{};{};{}", name, artist, album);
}


ArtworkCache::ArtworkCache(
  const std::string& cache_file,
  size_t max_cache_size
) :
_cache_file(cache_file),
_max_cache_size(max_cache_size) {
  _recency_cache.reserve(_max_cache_size);

  // read_cache_file();
}

void ArtworkCache::add_artwork(
  const std::string& name,
  const std::string& artist,
  const std::string& album,
  const ITunesSong& result
) {
  std::string key = generate_key(name, artist, album);
  std::string value = encode_cache_data(result);

  if (_data_cache.count(key)) {
    _recency_cache.erase(
      std::find(_recency_cache.begin(), _recency_cache.end(), key));
  }

  _data_cache.insert_or_assign(key, value);
  _recency_cache.push_back(key);

  if (_recency_cache.size() > _max_cache_size) {
    size_t prune_to = std::round(_max_cache_size * 0.7);
    size_t remove_count = _recency_cache.size() - prune_to;

    auto first = _recency_cache.begin();
    auto last = first + remove_count;

    for (auto it = first; it != last; ++it) {
      _data_cache.erase(*it);
    }

    _recency_cache.erase(first, last);
  }
}

std::optional<ITunesSong> ArtworkCache::get_artwork(
  const std::string& name,
  const std::string& artist,
  const std::string& album
) {
  std::string key = generate_key(name, artist, album);

  auto it = _data_cache.find(key);

  if (it != _data_cache.end()) {
    _recency_cache.erase(
      std::find(_recency_cache.begin(), _recency_cache.end(), key));
    _recency_cache.push_back(key);

    return parse_cache_data(it->second);
  }

  return std::nullopt;
}
