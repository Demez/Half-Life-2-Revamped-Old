@echo off

set GameDir=%~dp0
set GameDir=%GameDir:~0,-1%
"D:\SteamLibrary\steamapps\common\Alien Swarm Reactive Drop\reactivedrop.exe" -sw -game "%GameDir%"