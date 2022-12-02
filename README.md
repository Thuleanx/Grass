# Grass

I've been wanting to implement grass shaders in my games, and this project will serve as my attempt of it in opengl before I move to HLSL. Here's a rough outline of how the process will work:

Basic version:
1. We start with a flat plane mesh, the vertex shader need not do anything special with the mesh.
2. We will use Billboarding to model our grass, as opposed to a 3D grass model.
3. Use compute shader to generate the grass vertices, as well as additional information for aesthetic effects.
4. Use geometry shader to generate the grass blades billboards, one per vertex.
5. Use fragment shader to light our grass + give it pretty colors.

Interactivity:
1. Implement slight wind movement. 
2. Here, we need a character controller. We can simply have our camera follow a sphere hovering slightly above the plane, and map our controls to the sphere movement instead of the camera.
3. We want the grass to part as we move through it.
  - Simple version: Pass the character position to the grass's vertex shader (then geometry shader). Then displace the top vertices away from the character position. This works only for a fixed number of actors, and the blades don't convincingly sway as the player leaves.
  - Harder version if there's time: Have a distortion texture that stores the velocity of objects on the plane (brighter for higher velocity), and fades away. This is done by the CPU passing in the position of actors on the plane, and the compute buffer recalculating the distortion texture as the object moves each frame.
  
Optimizations + extra features:
1. Camera Fulstum Culling: the grass are only generated if they are in the view fulstrum, and only if they are a certain distance away.
2. Switch to using a grass mesh, then apply LOD (previously implemented in old project)
3. Randomize grass size and color.
4. Don't render the plane mesh, since the grass would cover it.

A couple of time-saving optimizations are:
1. Reuse code to generate varied geometry.
2. Reuse camera movements and boilerplate code.
3. Apply various simple filters on the grass.
