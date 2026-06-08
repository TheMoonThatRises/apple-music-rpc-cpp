#!/bin/zsh -e

if [ -e ~/Library/LaunchAgents/io.github.themoonthatrises.apple-music-rpc-cpp.plist ]; then
  echo "Unloading launch agent"
  launchctl unload ~/Library/LaunchAgents/io.github.themoonthatrises.apple-music-rpc-cpp.plist

  echo "Remove launch agent plist"
  rm -f ~/Library/LaunchAgents/io.github.themoonthatrises.apple-music-rpc-cpp.plist

  echo "Uninstall success"
else
  echo "No installation detected"
fi
