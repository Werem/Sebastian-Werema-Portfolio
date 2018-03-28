This is showing a simple shader that renders volumetric clouds, the look I achieved is not really what I wanted but it's not so bad, this is because I avoided a lot of calculations that would improve quality.

I focused more on the gpu side of things so start of the program takes a while because it's generating 3D perlin noise and 3D wolrey noise textures on the cpu

Unity profiler shows it takes 0.01-0.02ms per frame to render the sky alone, stopwatch measurement from PreRender to PostRender is displayed in program.

Use WSAD to move, mouse to rotate the camera, E and Q to change sky coverage.