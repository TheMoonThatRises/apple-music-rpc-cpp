/*
  Copyright 2026 Peter Duanmu

  You should have received a copy of the GNU General Public License along
  with apple-music-rpc-cpp. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef INCLUDE_ARTWORK_CACHE_HPP_
#define INCLUDE_ARTWORK_CACHE_HPP_

#include <map>
#include <vector>
#include <string>
#include <optional>

#include "include/music_types.hpp"

class ArtworkCache {
 private:
  const size_t _max_cache_size;

  std::vector<std::string> _recency_cache;
  std::map<std::string, std::string> _data_cache;

 protected:
  static ITunesSong parse_cache_data(const std::string& data);
  static std::string encode_cache_data(const ITunesSong& data);

  static std::string generate_key(
    const std::string& name,
    const std::string& artist,
    const std::string& album);

 public:
  ArtworkCache(size_t max_cache_size);

  void add_artwork(
    const std::string& name,
    const std::string& artist,
    const std::string& album,
    const ITunesSong& result);

  std::optional<ITunesSong> get_artwork(
    const std::string& name,
    const std::string& artist,
    const std::string& album);
};

#endif  // INCLUDE_ARTWORK_CACHE_HPP_
