copy ..\HydraIRC\Debug\HydraIRC.exe Release
copy ..\HydraIRC\Debug\HydraIRC.map "Debug Files\Debug"
copy ..\HydraIRC\Debug\HydraIRC.pdb "Debug Files\Debug"

copy ..\HydraIRC\Plugins\Default\Debug\Default.dll Release\Plugins
copy ..\HydraIRC\Plugins\Default\Debug\Plugin_Default.map "Debug Files\Debug\Plugins"
copy ..\HydraIRC\Plugins\Default\Debug\Plugin_Default.pdb "Debug Files\Debug\Plugins"

copy ..\HydraIRC\Plugins\WinAMP\Debug\WinAMP.dll Release\Plugins
copy ..\HydraIRC\Plugins\WinAMP\Debug\Plugin_WinAMP.map "Debug Files\Debug\Plugins"
copy ..\HydraIRC\Plugins\WinAMP\Debug\Plugin_WinAMP.pdb "Debug Files\Debug\Plugins"

copy ..\HydraIRC\Plugins\Alias\Debug\Alias.dll Release\Plugins
copy ..\HydraIRC\Plugins\Alias\Debug\Plugin_Alias.map "Debug Files\Debug\Plugins"
copy ..\HydraIRC\Plugins\Alias\Debug\Plugin_Alias.pdb "Debug Files\Debug\Plugins"

copy ..\HydraIRC\Plugins\Uptime\Debug\Uptime.dll Release\Plugins
copy ..\HydraIRC\Plugins\Uptime\Debug\Plugin_Uptime.map "Debug Files\Debug\Plugins"
copy ..\HydraIRC\Plugins\Uptime\Debug\Plugin_Uptime.pdb "Debug Files\Debug\Plugins"

copy ..\Docs\Docs.htm Release
copy ..\TODO.TXT Release
copy ..\CHANGES.TXT Release

pause
