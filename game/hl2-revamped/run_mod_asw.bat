@echo off

set GameDir=%~dp0
set GameDir=%GameDir:~0,-1%
"C:\Program Files (x86)\Steam\steamapps\common\Alien Swarm\swarm.exe" -windowed -noborder -override_vpk -scripting -game "%GameDir%"