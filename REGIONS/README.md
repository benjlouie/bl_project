REGIONS
=======

Experiment attempting to partition space in a grid filled with objects.

Uses SDL2 for display and interaction.

Grid Legend:
```
White - empty cell
Bright orange - corner cell
Solid color (mult cells) - part of one object (every object has a different color)
Light grey - path between two corners
blue - where two corners meet on a path
Bright Green - new cell that's not been added yet (see controls)
Purple line - shows how a corner connects to a blue cell
```




Controls:
```
Left Mouse Button
--toggle cell (bright green / white)
--hold to "paint"
Right Mouse Button
--toggle cell (bright green / white)
--hold and drag to toggle area
Middle Mouse Button
--Run algorithm
```

Unfinished, but functional.
