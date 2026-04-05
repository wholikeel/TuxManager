# Tux Manager

A Linux Task Manager alternative built with Qt6, inspired by the Windows Task Manager but designed to go further - providing deep visibility into system processes, performance metrics, users, and services.

## Memory view
![Screenshot](screenshots/readme.png)
## CPU view
![Screenshot](screenshots/cpu.png)
## GPU view
![Screenshot](screenshots/gpu.png)

## Building

### qmake

```bash
# cd to root of the repo and then:
mkdir build && cd build
qmake6 ../src
make -j$(nproc)
./tux-manager
```

## License

GPL-3.0-or-later
