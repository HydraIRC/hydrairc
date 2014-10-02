
           ===================


           THIS IS OUT OF DATE


           ===================

Hmm, not much in the way of docs yet, sorry..

Try and compile the included plugin,  you may have to adjust include paths and the like, but it does work.

If you're missing files, let me know ASAP.

I compile this With Visual Studio .NET (VC7).  HydraIRC uses WTL, but the example plugin doesn't, but might still need the include files,  it links with
HydraIRCPlugin.lib which compiles as a seperate project.

Check out API_Docs.htm, though this is probably out of date and/or incorrect by now.

No other plugin API's are defined apart from "Basic" and "CommandProcessor".  Check out the respective include files for more information on that.

More API's and functions soon, please request the functions you need if they're
not available and I will add them, or add them yourself and I'll patch them in
if you send me a diff.

- Hydra