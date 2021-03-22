# watermarble_simulator
An OpenGL based simulator based on "Mathematical Marbling" by Shufang Lu et al.

This repo is a class project. Feel free to download it.
The paper can be accessed from this link: https://ieeexplore.ieee.org/document/5887299

The paper has very clear mathematical formula, and I applied it to GLSL shader.
Some detail about the code:
- Programming language: C++
- Library used: OpenGL, Glut, and AntTweakBar (for the customization window)

The demo is available for download in the "release" section.

How to use the program:
1. Left click to add new drop of color
2. Hold right click and drag it to the direction desired
3. Customization window:
- Layer: choose the drop, layer 0 for first drop, layer 1 for second drop, and so on
- Radius: drop radius
- Center: the drop center x, and center y. Use this to re-position the drop
- Color: change drop's color, floating point from 0 to 1 is used
- Marble selection: select the marbling type, please refer to Mathematical Marbling by Shufang Lu et al.
