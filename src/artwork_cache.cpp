/*
  Copyright 2026 Peter Duanmu

  You should have received a copy of the GNU General Public License along
  with apple-music-rpc-cpp. If not, see <https://www.gnu.org/licenses/>.
*/

#include "include/artwork_cache.hpp"

#include <malloc/malloc.h>

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

std::string ArtworkCache::generate_key(
  const std::string& name,
  const std::string& artist,
  const std::string& album
) {
  return std::format("{};{};{}", name, artist, album);
}


void ArtworkCache::removeNode(Node *node) {
  node->prev->next = node->next;
  node->next->prev = node->prev;

  if (node == _head) {
    if (node == node->next) {
      _head = nullptr;
    } else {
      _head = node->next;
    }
  }
}

void ArtworkCache::insertNodeHead(Node *node) {
  if (_head) {
    node->next = _head;
    node->prev = _head->prev;

    _head->prev->next = node;
    _head->prev = node;
  } else {
    node->prev = node;
    node->next = node;
  }

  _head = node;
}

void ArtworkCache::deallocateNode(Node *node) {
  node->next = nullptr;
  node->prev = nullptr;
  free(node);
}

ArtworkCache::ArtworkCache(size_t max_cache_size) :
_max_cache_size(max_cache_size),
_head(nullptr) {
}

ArtworkCache::~ArtworkCache() {
  for (auto [key, node] : _data_list) {
    removeNode(node);
    deallocateNode(node);
  }

  _data_list.clear();
  _head = nullptr;
}

void ArtworkCache::add_artwork(
  const std::string& name,
  const std::string& artist,
  const std::string& album,
  const ITunesSong& result
) {
  std::string key = generate_key(name, artist, album);
  std::string value = encode_cache_data(result);

  auto elements = _data_list.find(key);
  Node* node;

  if (elements != _data_list.end()) {
    node = elements->second;

    node->value = value;

    removeNode(node);
  } else {
    if (_data_list.size() >= _max_cache_size) {
      _data_list.erase(_head->prev->key);
      deallocateNode(_head->prev);
      removeNode(_head->prev);
    }

    node = static_cast<Node*>(calloc(1, sizeof(Node)));

    if (node) {
      node->key = key;
      node->value = value;

      _data_list.insert({ key, node });
    }
  }

  insertNodeHead(node);
}

std::optional<ITunesSong> ArtworkCache::get_artwork(
  const std::string& name,
  const std::string& artist,
  const std::string& album
) {
  std::string key = generate_key(name, artist, album);

  auto element = _data_list.find(key);

  if (element == _data_list.end()) {
    return std::nullopt;
  }

  Node* node = element->second;

  if (node != _head) {
    removeNode(node);
    insertNodeHead(node);
  }

  return parse_cache_data(node->value);
}
