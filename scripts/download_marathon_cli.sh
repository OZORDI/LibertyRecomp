#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

NAME="Marathon"
INSTALL_DIR="$SCRIPT_DIR/../tools/Marathon"
LINUX_URL="https://github.com/hyperbx/Marathon/releases/download/1.0.61/Marathon.CLI-linux-x64.tar.gz"
MAC_URL="https://github.com/hyperbx/Marathon/releases/download/1.0.61/Marathon.CLI-osx-x64.tar.gz"

if [ -f "$SCRIPT_DIR/../tools/Marathon/Marathon.CLI" ]; then
    echo "$NAME already downloaded"
    exit 0
fi

OS=$(uname -s)

cd "$INSTALL_DIR" || exit 1

echo "Downloading $NAME..."
case "$OS" in
    Linux*)
        curl -L "$LINUX_URL" -o "$NAME.tar.gz"
        ;;
    Darwin*)
        curl -L "$MAC_URL" -o "$NAME.tar.gz"
        ;;
    *)
        echo "Unsupported operating system: $OS"
        rm -rf "$TEMP_DIR"
        exit 1
        ;;
esac

if [ $? -ne 0 ]; then
    echo "Download failed"
    rm -rf "$TEMP_DIR"
    exit 1
fi

echo "Unpacking $NAME..."
tar -xzf "$NAME.tar.gz"

if [ $? -ne 0 ]; then
    echo "Unpacking failed"
    rm -rf "$TEMP_DIR"
    exit 1
fi

if [ $? -eq 0 ]; then
    echo "$NAME installed successfully to $INSTALL_DIR"
else
    echo "Installation failed"
    rm -rf "$TEMP_DIR"
    exit 1
fi

# Clean up
rm "$NAME.tar.gz"