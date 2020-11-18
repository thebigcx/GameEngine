# Game Engine
Engine for making games that run fast.

## Compiling
In your shell of choice:
```bash
vendor/premake/premake5 gmake Release
make
```
If you do not have gmake, swap 'gmake' for 'vs2019' to generate a .vsproj.

There should now be a static library in the bin/Debug/Engine directory. Copy this, along with Engine/include into your game.
Link the static library, and you should be good to go.

## Features

Current features include:
- 2D rendering of sprites, quads, and text
- Sound engine capable of playing 3D sounds
- Animation system
- Particle system
- Maths library (vec2, mat4, etc)
- Event, layer, and application management, all done behind the scenes
