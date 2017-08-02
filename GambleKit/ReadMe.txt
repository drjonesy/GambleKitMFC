IMPORTANT: To run applications built with GambleKit, you'll need to have installed OpenAL 
and MSVS Redistributables, which are included in the "Prerequisites" folder

GambleKit is a free open-source engine for easy building of slot machine gambling games with reels and GUI management.

It relyes basically on declarative programming style, so the programmer needs simply to declare, 
for example at creation time, the basic layout and behaviour of the game, and then leave the game flow, 
requiring as little additional intervention as possible.
Of course, a game can hardly go without any need of customizing the designed behaviours and 
it can be customized at will, having direct access to all its elements.
The goal of GambleKit is to provide the base for this kind of games, the ability to handle common
tasks declaratively with one or just a few lines of code, to leave only the very custom part of code for 
implementation by the programmer.
The basic idea being to code the very game logic and visuals immediately from the start and 
do it with as few lines of code as possible.
Even currently being implemented only for windows, GambleKit has little coupling with the OS and
 uses cross-platform tools with the prospect of easily increasing the number of supported platforms.

For technical info and guide, please refer to the "Introduction" file.
For a brief demonstration of the api, look at the "CustomGameLogic" directory in the solution.

GambleKit uses the following software products:
-OpenGL for its graphics http://www.opengl.org/
-OpenAL for its sound http://openal-soft.org/
-DevIL for image files decoding http://openil.sourceforge.net/

You can contact me at gamblekit@gmail.com