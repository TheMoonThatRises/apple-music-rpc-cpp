# Apple Music RPC CPP

Apple Music rich presence for Discord using C++.

Inspired by [Nextfire/apple-music-discord-rpc](https://github.com/NextFire/apple-music-discord-rpc).

## Features

- Zero external dependencies: fully self-contained, with a purpose-built Discord IPC layer (`discord_ipc_cpp`) instead of bundled third-party libraries
- Event-driven instead of polling: directly hooks into Apple Music through Scripting Bridge, meaning track changes are instantly pushed
- Survives Discord relaunch: detects Discord launch and termination, ensuring the current track is always relayed
- Lightweight executable: low private memory footprint when running (<9MB) with small executable size (<400KB)

## Install

To install, clone the repository recursively:

```bash
git clone --recurse-submodules https://github.com/TheMoonThatRises/apple-music-rpc-cpp
```

Then, run the install script.

```bash
./scripts/install.sh
```

> [!NOTE]
> Make sure to allow `apple_music_rpc_cpp` to access Automation to allow it to
> access Apple Music statuses.
