# disney-streaming-homepage
Fun weekend project to dig into OpenGL for a media based, non-gaming application. Its still rough but I am continuing to work on it over time.

1. You can set the size of the window to either (1920x1080) or (960x540) by replacing a config file. You can also manually adjust the parameters within the config file; however, the rendering is not set up to correctly handler independent resolution rendering. 
2. The project uses a data cache for shaders, textures on config files. You can find the folders within their respective release folders
3. Be patient with the loading screen, adding a loading indicator added a bit more to my scope for now
4. There is minimal response to input. The tiles can be selected but there is no scrolling.
5. Compiled and ran in VS2019 utilizing C++ 11 to 14 features 

Controls for the Tile selection
"W" - Up
"A" - Left
"D" - Right
"S" - Down
