/*
  Copyright 2025 Peter Duanmu

  You should have received a copy of the GNU General Public License along
  with apple-music-rpc-cpp. If not, see <https://www.gnu.org/licenses/>.
*/

#import <Foundation/Foundation.h>

#include <string>

#import "include/itunes_api.h"

#include "include/music_types.hpp"

@implementation ITunesAPI

+ (void)get_itunes_result:(const std::string*)song_name
        artist:(const std::string*)artist_name
        album:(const std::string*)album_name
        callback:(t_itunes_songs_callback)callback {
  NSString* query = [NSString
    stringWithFormat:@"%@ %@ %@",
    [NSString stringWithUTF8String:song_name->c_str()],
    [NSString stringWithUTF8String:artist_name->c_str()],
    [NSString stringWithUTF8String:album_name->c_str()]
  ];
  NSString* encoded_query = [query
    stringByAddingPercentEncodingWithAllowedCharacters:[NSCharacterSet
      URLQueryAllowedCharacterSet
    ]
  ];

  NSString* base_path = @"https://itunes.apple.com";
  NSString* url_string = [NSString
    stringWithFormat:@"%@/search?term=%@&media=music&entity=song",
    base_path,
    encoded_query
  ];

  NSMutableURLRequest* url_request = [[NSMutableURLRequest alloc]
    initWithURL:[NSURL URLWithString:url_string]
  ];

  [url_request setHTTPMethod:@"GET"];

  NSURLSession* session = [NSURLSession sharedSession];

  NSURLSessionDataTask* data_task = [session
    dataTaskWithRequest:url_request
    completionHandler:^(NSData* data, NSURLResponse* response, NSError* error) {
      NSHTTPURLResponse* http_response = (NSHTTPURLResponse*) response;

      if (http_response.statusCode == 200) {
        std::string response_string((const char*)[data bytes], [data length]);

        ITunesSongResults itunes_results = ITunesSongResults::from_string(
          response_string
        );

        callback(itunes_results);
      } else {
        NSLog(@"Unable to retrieve iTunes API data: %@", error);
      }
    }
  ];

  [data_task resume];
}

@end
