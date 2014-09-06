Conway
======

Conway's Game of Life running on a GBA, with some interesting features

Runs as fast as I could get it to run on the GBA given the requirement that we couldn't store any additional information about the simulation outside of the video buffer. I guess I could have done some simulation during the HBlank interrupts, but I don't think we were allowed to do that either.

Anyways, pressing Start will pause the simulation and give you a cursor to move around. Pressing A will paint 'live' tiles, and pressing B will paint 'dead' tiles. Pressing L while paused will force a buffer flip, which works as a makeshift undo in some cases, but is there because why not. Pressing R during pause will load pre-generated images in the format of the game of life.
