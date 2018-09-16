********************************************************************************

          ASCOT :: ASCII ANIMATION ENGINE :: [c] 2003 RESERVOIR GODS

********************************************************************************

-- USAGE

Ascot is driven by two files, one containing all the frames of ascii animation,
the other being a script file that controls the sequencing of this frames.

Ascot searches for these file names:

*.FRM    - ASCII animation frames
*.PI1    - 16 colour images
*.MOD    - musicmon iles
ANIM.SPT - Script file


-- FRAMES

The .FRM files contain all your frames of ASCII data.
Each frame is tagged so it can be used by the scripting engine.
Each frame should begin with an identifier in this form:

[ FRAME nameid ]

The 'nameid' field can then be referenced by the script.
The 'nameid' should contain no spaces.

The ASCII data for this frame is encompassed in curly brackets:

{
some sample ascii
}

You MUST always include the terminating curly bracket.


-- SCRIPT

The script controls how the frames are displayed.
The ASCOT rendering engine is based on a layering system.
You have 8 different layers which contain ASCII data.
These layers can be individually controlled and overlayed over each other.

The draw order of layers is important, layer 0 is the background and is drawn
first, subsequent layers are drawn sequentially on top of each other.

The space character is interpreted as being transparent, which allows
you an alpha like ablity to see through layers.

Layers can be turned on and off - the more layers you have enabled,
the more costly the rendering time will be.


-- FONT

ASCOT searches the directory or linkfile for a graphic called 'FONT8X8.PI1'
it uses this for all font printing. You can modify this graphic file to
change the appearance of the font.


-- MUSIC

ASCOT replays music mon .MOD files.
ASCOT scans the working directory for all .MOD files and loads them.
You can reference a musicmon file from script by its filename


-- PICTURES

Pictures should be in degas elite (.PI1) format.
ASCOT scans the working directory for all PI1 files and loads them into memory.
You can reference a PI1 file from the script by its filename.


-- DEVELOPMENT

During the development and debugging phase of your ASCOT project, you
should put all the needed data files (ANIM.SPT,*.FRM,*.MOD,*.PI1) into a
folder called UNLINK/PACKAGE1 which should reside in the same directory as 
ASCOT.TOS.
This allows easy editing of the data files.


-- RELEASE

When you have a ASCOT project ready for relase, you want to LINKFILE your
data files. This saves space in the distribution and will prevent people
modifying your data files.

To create a LINKFILE, you run MAKELINK.TTP passing it two arguments:
1) the name of the directory to compact into a linkfile
2) the name of the linkfile to create

example:

MAKELINK.TTP C:\ASCOT\UNLINK\PACKAGE1 C:\PACKAGE1.LNK

(this will create the linkfile 'PACKAGE1.LNK' from the directory
c:\ascot\unlink\package1)


Once you have create the linkfile, you can pack the linkfile with pack-ice
as a data file.

*DO NOT* pack any data files before you have linked them.

The linkfile for the release version MUST be called 'PACKAGE1.LNK'
and must reside in a folder called DATA.


Here is a complete list of script commands:

--------------------------------------------------------------------------------
	COLOUR		aIndex,aValue
--------------------------------------------------------------------------------

Sets palette colour aIndex to aValue.
aIndex is in the range 0-3, 0 being the background colour and 1-3 being
colours for text.
aValue is an STE colour value in the form 0xRGB, and ranges from 0x000 to 0xFFF


--------------------------------------------------------------------------------
	DRAW	aFrames
--------------------------------------------------------------------------------

Renders an image with the current settings.
Nothing will be displayed until you use the DRAW command!
aFrames is the number of VBLs to render for.
During this time, automatic effects such as scrolling will be updated.

--------------------------------------------------------------------------------
	END
--------------------------------------------------------------------------------

Terminates script processing


--------------------------------------------------------------------------------
	FADE_TORGB	aRGB,aFrameCount
--------------------------------------------------------------------------------

All colours in the palette fade towards colour 'aRGB'.
This process takes aFrameCount frames.
The fade will begin when the next 'DRAW' command is reached.

examples:

FADE_TORGB	0,10
(fades all colours to black over 10 frames)

FADE_TORGB	0xFFF,50
(fades all colours to white over 50 frames)


--------------------------------------------------------------------------------
	FADE_TOPICPAL	aPictureName,aFrameCount
--------------------------------------------------------------------------------

Fades to palette stored in PI1 image (aPictureName) over aFrameCount frames.
This action will begin when next 'DRAW' command is reached

examples:

FADE_TOPICPAL	BUNNY.PI1,30
(fades to palette of 'bunny.pi1' over 30 frames)


--------------------------------------------------------------------------------
	MUSICMON_PLAY	aSongName
--------------------------------------------------------------------------------

Plays musicmon tune from filename 'aSongName'

examples:
MUSICMON_PLAY	ROCK.MOD
(plays musicmon tune 'ROCK.MOD)


--------------------------------------------------------------------------------
	MUSICMON_STOP
--------------------------------------------------------------------------------

Stops musicmon playback.


--------------------------------------------------------------------------------
	PAUSE	aFrames
--------------------------------------------------------------------------------

Pause for aFrames vertical blanks.


--------------------------------------------------------------------------------
	PIC	aPictureName
--------------------------------------------------------------------------------

Displays image 'aPictureName'.
Picture will be rendered on next 'DRAW' command as long as 'PIC_ON' has been set

examples:
PIC	LOGO.PI1
(sets picture to be 'logo.pi1')


--------------------------------------------------------------------------------
	PIC_SETPAL	aPictureName
--------------------------------------------------------------------------------

Sets palette to one stored in image 'aPictureName'

examples:
PIC_SETPAL	ATARI.PI1
(sets palette to the one in the degas file 'ATARI.PI1')


--------------------------------------------------------------------------------
	PIC_ON
--------------------------------------------------------------------------------

Enables rendering of pictures.


--------------------------------------------------------------------------------
	PIC_OFF
--------------------------------------------------------------------------------

Disables rendering of pictures.


--------------------------------------------------------------------------------
	RATE	aSpeed
--------------------------------------------------------------------------------

Sets the playback speed in number of VBLs, 0 being the fastest.


--------------------------------------------------------------------------------
	SCREEN_MODE	aMode
--------------------------------------------------------------------------------

Sets current screen resolution to either low res (0) or med res (1)

examples:

SCREEN_MODE	0
(sets screen into 320x200x16 ST Low mode)

SCREEN_MODE	1
(sets screen into 640x200x4 ST medium mode)


--------------------------------------------------------------------------------
	LAYER_COLOUR	aLayer,aColour
--------------------------------------------------------------------------------

Sets the text colour for aLayer.
aLayer is in the range 0-7
aColour is in the range 1-3, and references the palette set with
the COLOUR command.


--------------------------------------------------------------------------------
	LAYER_ON	aLayer
--------------------------------------------------------------------------------

Enables layer number aLayer.
When next DRAW command is issued, this layer will be rendered.

--------------------------------------------------------------------------------
	LAYER_OFF	aLayer
--------------------------------------------------------------------------------

Disables layer aLayer.
When next DRAW command is issued, this layer will not be rendered.

--------------------------------------------------------------------------------
	LAYER_FLIPX	aLayer,aFlag
--------------------------------------------------------------------------------

Sets horizontal flipping for layer aLayer.
If aFlag is 1, horizontal flipping will be enabled.
If aFlag is 0, horizontal flipping will be disabled.


--------------------------------------------------------------------------------
	LAYER_FLIPY	aLayer,aFlag
--------------------------------------------------------------------------------

Sets vertical flipping for layer aLayer.
If aFlag is 1, vertical flipping will be enabled.
If aFlag is 0, vertical flipping will be disabled.


--------------------------------------------------------------------------------
	LAYER_FRAME	aLayer,aFrameName
--------------------------------------------------------------------------------

Assigns frame aFrameName to layer aLayer.
aFrameName should correspond to a nameid of a frame in the ANIM.FRM file


--------------------------------------------------------------------------------
	LAYER_SCROLLX	aLayer,aSpeed
--------------------------------------------------------------------------------

Starts horizontal autoscroll on layer aLayer.
Every VBL, the value of aSpeed will be added to the layer's x co-ordinate.
aSpeed is a fixed point 16.16 number. 
The upper 16 bits represent the integer part, the lower 16 bits represent
the fractional part:

	0xiiiiffff

Some examples of fixed point numbers:

	0x00008000	=	0.5
	0x00010000	=	1.0
	0x00020000	=	2.0
	0x00028000	=	2.5
	0x00034000	=	3.25
	0xFFFF0000	=  -1.0
	0xFFFE0000	=  -2.0

If you want to stop scrolling, set the aSpeed value to zero.
You will also need to reset the layer's X position using the LAYER_X command
after you have finished your scrolling effect.

--------------------------------------------------------------------------------
	LAYER_SCROLLY	aLayer,aSpeed
--------------------------------------------------------------------------------

Starts vertical autoscroll on layer aLayer.
Every VBL, the value of aSpeed will be added to the layer's y co-ordinate.
aSpeed is a fixed point 16.16 number. 
If you want to stop scrolling, set the aSpeed value to zero.
You will also need to reset the layer's Y position using the LAYER_Y command
after you have finished your scrolling effect.

--------------------------------------------------------------------------------
	LAYER_X	aLayer,aX
--------------------------------------------------------------------------------

Sets the X co-ordinate of layer aLayer to aX.
This allows you to move layers around horizontally.

--------------------------------------------------------------------------------
	LAYER_Y	aLayer,aY
--------------------------------------------------------------------------------

Sets the Y co-ordinate of layer aLayer to aY.
This allows you to move layers around vertically






