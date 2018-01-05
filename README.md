# WPFHostD3D11 and opengl
This program made in the following two steps by visual studio 2017.

1. make Win32 two independent Execution Projects.
    1. simple d3d-drawing by vertex&pixel shader.
    1. simple opengl-drawing by vertex & fragment shader.

1. make wpf(C#) project hosting two native window handles(HWND).
at this time, change step-1's execution projects to cli-classlibraries.
and then, add reference for the cli-classlibraries to wpf(C#) project.

# issue remaining
* on initial startup, secoundary hosted window is not drawn. no-wm_paint message.

# References
* [d3d-shader compile](http://www-fps.nifs.ac.jp/ito/memo/) Ito-san memo
* [d3d rendering setup process](http://yun.cup.com/directx11.html) Nanashi-soft-san info
* [opengl tutorial](http://www.opengl-tutorial.org) opengl-tutorial.org
