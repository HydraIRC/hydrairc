copy ..\HydraIRC\Release\HydraIRC.exe Release
copy ..\HydraIRC\Release\HydraIRC.map "Debug Files\Release"

copy ..\HydraIRC\Plugins\Default\Release\Default.dll Release\Plugins
copy ..\HydraIRC\Plugins\Default\Release\Plugin_Default.map "Debug Files\Release\Plugins"

copy ..\HydraIRC\Plugins\WinAMP\Release\WinAMP.dll Release\Plugins
copy ..\HydraIRC\Plugins\WinAMP\Release\Plugin_Winamp.map "Debug Files\Release\Plugins"

copy ..\HydraIRC\Plugins\Alias\Release\Alias.dll Release\Plugins
copy ..\HydraIRC\Plugins\Alias\Release\Plugin_Alias.map "Debug Files\Release\Plugins"

copy ..\HydraIRC\Plugins\Uptime\Release\Uptime.dll Release\Plugins
copy ..\HydraIRC\Plugins\Uptime\Release\Plugin_Uptime.map "Debug Files\Release\Plugins"

copy ..\Docs\Docs.htm Release
copy ..\TODO.TXT Release
copy ..\CHANGES.TXT Release

pause
