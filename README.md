# quiver_ripper
An archive extractor tool for the DOS game Quiver (1997).

Pardon the cringey code, but I made this a while ago; though given that it serves its purpose just fine and the fact that it's
more or less comprehensible as it is, I think that no refactoring is necessary.

## Usage
	quiver_ripper G.bin
where G.bin is the game's archive name passed as an argument(in Windows you can also use the drag-and-drop method).
Once the extractor has done its thing, you should have the extracted resources in a subfolder
located in the same path of the archive (if you didn't use the drag-and-drop method, you should see the subfolder's path
printed as soon as the extraction is done, just in case).

## Extracted material
- Textures
- Sprites
- Various graphics (fonts, menus, HUDs, etc)
- Palette
- Sounds
- Music

All picture-related material is extracted as .TGA pictures, which can be viewed fine with
IrfanView (albeit without transparency), SLADE3, MtPaint, and possibly other image viewers I'm unaware of;
if that's still a problem for you, Ken Silverman's PNGOUT is your friend :)

As for sounds and music it's a bit trickier since the game uses Diamondware STK as sound library, with its own custom formats
(.DWD for sounds, .DWM for music), with DWD which appears to be derived from .VOC and .DWM which is MIDI converted into
a custom format which seems to make use of OPL commands; if you're that interested, Dynamo128's packaged resources
(which you can find [here](https://forum.zdoom.org/viewtopic.php?f=37&t=50106)) include PLAYDWD.EXE and PLAYDWM.EXE tools
(part of Diamondware STK library), which work under DOSBox and play those custom formats just fine.

If you want to convert .DWM files back to MIDI, I explained how to do that [here](https://forum.zdoom.org/viewtopic.php?p=949676#p949676).

As for .DWD(sound) files, I might look them up at some point, even though in all honesty they didn't seem they were worth the effort.

That's all, for now :)
