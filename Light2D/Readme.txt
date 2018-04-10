This is a 2D lighting system which is a mixture of compute shaders and texture blits
Occluders are rendered to occlusion map, map is distorted using tangent, compute shader "shoots rays" looking for closest obstacle and writes it to shadowmap which is then used by a normal shader to render shadows

Move light with mouse, esc to quit