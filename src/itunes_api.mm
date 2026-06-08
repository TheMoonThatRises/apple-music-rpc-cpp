/*
  Copyright 2026 Peter Duanmu

  You should have received a copy of the GNU General Public License along
  with apple-music-rpc-cpp. If not, see <https://www.gnu.org/licenses/>.
*/

#import "include/itunes_api.h"

#import <Foundation/Foundation.h>

#import "include/objc_convert.h"

#include <string>

#include "include/callback_types.hpp"
#include "include/music_types.hpp"

@implementation ITunesAPI

+ (void)get_itunes_result:(const std::string&)song_name
        artist:(const std::string&)artist_name
        album:(const std::string&)album_name
        callback:(t_itunes_songs_callback)callback {
  NSString* query_string = [NSString stringWithFormat:@"%@ %@ %@",
    to_nsstring(song_name),
    to_nsstring(artist_name),
    to_nsstring(album_name)
  ];

  NSDictionary* queryDictionary = @{
    @"term": query_string,
    @"media": @"music",
    @"entity": @"song"
  };

  NSURLComponents* url_components = [NSURLComponents
    componentsWithString:@"https://itunes.apple.com/search"
  ];
  NSMutableArray* queryItems = [NSMutableArray array];
  for (NSString* key in queryDictionary) {
      [queryItems addObject:[NSURLQueryItem queryItemWithName:key
        value:queryDictionary[key]
      ]];
  }
  url_components.queryItems = queryItems;

  NSMutableURLRequest* url_request = [[NSMutableURLRequest alloc]
    initWithURL:url_components.URL
  ];

  [url_request setHTTPMethod:@"GET"];

  NSURLSession* session = [NSURLSession sharedSession];

  NSURLSessionDataTask* data_task = [session dataTaskWithRequest:url_request
    completionHandler:^(NSData* data, NSURLResponse* response, NSError* error) {
      NSHTTPURLResponse* http_response = (NSHTTPURLResponse*) response;

      dispatch_async(dispatch_get_main_queue(), ^{
        if (http_response.statusCode == 200 && data.length > 0) {
          std::string response_string((const char*)[data bytes], [data length]);

          ITunesSongResults itunes_results = ITunesSongResults::from_string(
            response_string
          );

          callback(std::move(itunes_results));
        } else {
          NSLog(@"Unable to retrieve iTunes API data: %@", error);

          callback({});
        }
      });
    }
  ];

  [data_task resume];
}

@end
