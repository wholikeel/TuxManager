# Tux Manager - Packaging

This directory contains Linux packaging scripts for Tux Manager.

## Supported targets

- Debian/Ubuntu (`.deb`) via `package-deb.sh`
- Fedora/RHEL/Alma/Rocky (`.rpm` + `.src.rpm`) via `package-rpm.sh`

## Dependencies

### Debian/Ubuntu

Required build tools/packages:

```bash
sudo apt-get install build-essential debhelper dpkg-dev pkg-config qt6-base-dev
```

Notes:
- The script accepts either `qmake6` or `qmake`.
- Qt5 can also work if your distro provides it (`qtbase5-dev`).

### Fedora/RHEL family

Required build tools/packages:

```bash
sudo dnf install rpm-build rsync git pkgconf-pkg-config qt6-qtbase-devel
```

Notes:
- The script can also build with Qt5 if only that is available (`qt5-qtbase-devel`).
- The script uses `rpmbuild` and creates both binary RPM and source RPM.

## Usage

From repo root:

### Build DEB

```bash
cd packaging
./package-deb.sh
```

Optional:

```bash
./package-deb.sh --qt /path/to/qt/bin
./package-deb.sh --version 1.2.3
```

Output (in `packaging/output/`):
- `tux-manager_<version>_<arch>.deb`
- optionally `tux-manager-dbgsym_<version>_<arch>.deb`

### Build RPM

```bash
cd packaging
./package-rpm.sh
```

Optional:

```bash
./package-rpm.sh --qt /path/to/qt/bin
./package-rpm.sh --version 1.2.3
```

Output (in `packaging/output/`):
- `tux-manager-<version>-1.*.rpm`
- `tux-manager-<version>-1.*.src.rpm`

## Install

### Debian/Ubuntu

```bash
sudo dpkg -i packaging/output/tux-manager_*.deb
sudo apt-get install -f
```

### Fedora/RHEL family

```bash
sudo dnf install packaging/output/tux-manager-*.rpm
```
