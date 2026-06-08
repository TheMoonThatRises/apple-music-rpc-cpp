#!/bin/zsh -e

# cd to project root
cd "$(dirname "$0")"
cd ..

# clean build directory if exists
if [ -d build ]; then
  echo "Cleaning build directory"

  cmake --build build --target clean
else
  mkdir build
fi

echo "Compiling project"

# create build directory
cd build

# build project
cmake ..
make -j

# return to project root
cd -

# uninstall if launch agent already exists
./scripts/uninstall.sh

echo "Copying launch agent plist"

# copying launch agent
mkdir -p ~/Library/LaunchAgents/
cp -f io.github.themoonthatrises.apple-music-rpc-cpp.plist ~/Library/LaunchAgents/

# updating launch agent variables
plutil -replace EnvironmentVariables.TMPDIR -string "$TMPDIR" ~/Library/LaunchAgents/io.github.themoonthatrises.apple-music-rpc-cpp.plist
plutil -replace WorkingDirectory -string "$(pwd)" ~/Library/LaunchAgents/io.github.themoonthatrises.apple-music-rpc-cpp.plist

# loading launch agent
echo "Loading launch agent"
launchctl load ~/Library/LaunchAgents/io.github.themoonthatrises.apple-music-rpc-cpp.plist

echo "Installed successfully"
