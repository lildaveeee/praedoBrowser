#!/bin/bash
echo "PraedoBrowser Installer - Setting up dependencies..."
if command -v apt-get >/dev/null; then
    sudo apt-get update
    sudo apt-get install -y qt5-webengine qt5-webengine-widgets libxcb-xinerama0 libxkbcommon-x11 libxcb-cursor0 libxcb-keysyms1
elif command -v dnf >/dev/null; then
    sudo dnf install -y qt5-qtwebengine libxcb libxkbcommon-x11
elif command -v pacman >/dev/null; then
    sudo pacman -S --noconfirm qt5-webengine libxkbcommon-x11
else
    echo "Unsupported package manager. Please install Qt WebEngine manually."
    exit 1
fi
mkdir -p ~/.praedoBrowser
cp -r praedoBrowser ~/.praedoBrowser/
echo "[Desktop Entry]
Name=PraedoBrowser
Exec=$HOME/.praedoBrowser/praedoBrowser
Type=Application
Icon=$HOME/.praedoBrowser/icon.png
Categories=Internet;" > ~/.local/share/applications/PraedoBrowser.desktop
chmod +x ~/.praedoBrowser/praedoBrowser
echo "Installation complete! You can start PraedoBrowser by running: $HOME/.praedoBrowser/praedoBrowser"
