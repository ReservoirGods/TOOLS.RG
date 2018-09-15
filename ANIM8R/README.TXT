[] ANIM8R USER'S MANUAL []
[]     ReservoirGods    []
[]        (c)2001       []

Seeing things is nice.
Moving things are nice.
Combine the two - the joy of 'seeing things'
with the pleasure of 'moving things' and you
have a winning combination.
Welcome to the world of ANIM8R.

ANIM8R comprised of 2 key components,
the command parser & the animation keyframer.

The command parser allows you to write your
own animation scripts offline in the text
editor of your choice, giving you total
control of the displayed images.

The animation keyframer handles the animation
playback, speed control & rendering.

[ MEET ME ]

ANIM8R works with two files;
* a source graphics file (DEGAS PI1 FORMAT)
* a command list (ASCII FORMAT)

[ PAINT ME ]

Graphics should be painted with or
exported to degas pi1 format.
Compressed PC1 pictures are NOT supported.

The sprites you want to see animated should
be on the top row of the image in a line
from right to left.

[ WRITE ME ]

The animation is controlled with a command
list text file.
This specifies the source graphics filename,
sprite width and height and animation sequence.

By default, ANIM8R loads 'ANIM.TXT' as its
command list file, but as its a .TTP you can
specify your own command list file as the
sole argument.

[ WATCH ME ]

ANIM8R loads up the command list, parses the
file, sets all parameters then loads the
graphics. The sprites are then displayed
at the specified speed in the desired
sequence.

[ ABUSE ME ]

Errors in the script are reported.
Press 'SPACE' to exit error screen.

[ USE ME ]

The command list is file consisting
of commands and parameters.
Commands are upper case and enclosed
in [SQUARE] brackets.

Parameters follow on the line 
immediately following the command.

Some commands take multiple parameters,
these must all appear on consecutive
separate lines.


[ COMMAND ME ]

Here are the available commands:

COMMAND     : [GRAPHIC]
PARAMETER   : filename
DESCRIPTION : degas pi1 file that
              contains animations

COMMAND     : [WIDTH]
PARAMETER   : sprite width
DESCRIPTION : width of sprite in pixels

COMMAND     : [HEIGHT]
PARAMETER   : sprite height
DESCRIPTION : height of sprite in pixels

COMMAND     : [X]
PARAMETER   : x offset to first sprite
DESCRIPTION : NOT SUPPORTED IN THIS VERSION

COMMAND     : [Y]
PARAMETER   : y offset to first sprite
DESCRIPTION : NOT SUPPORTED IN THIS VERSION

COMMAND     : [FRAMECOUNT]
PARAMETER   : number of frames
DESCRIPTION : total frames in animation sequence

COMMAND     : [FRAMES]
PARAMETER   : list of frame numbers
DESCRIPTION : this is the animation sequence
              each frame number should appear
              on its own separate line

COMMAND     : [TIME]
PARAMETER   : vbl count
DESCRIPTION : this is the animation speed
              parameter is the number of
			  vertical blanks to pause
			  between frames

COMMAND     : [EOF]
PARAMETER   : none
DESCRIPTION : marks end of file


If you have suggestions for other commands,
let me know.

Look at 'ANIM.TXT' to see how a command
list file should be layed out.

.MrPink.