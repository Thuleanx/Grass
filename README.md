# Grass

I've been wanting to implement grass shaders in my games, and this project will serve as my attempt of it in opengl before I move to HLSL. Here's a rough outline of how the process will work:

Basic version:
1. We start with a flat plane mesh, the vertex shader need not do anything special with the mesh.
2. We will use Billboarding to model our grass, as opposed to a 3D grass model.
3. Use compute shader to generate the grass vertices, as well as additional information for aesthetic effects.
4. Use geometry shader to generate the grass blades billboards, one per vertex (i'm not totally sure if this would be geometry shader or if we can just use the compute shader for everything).
5. Use fragment shader to light our grass + give it pretty colors.

Interactivity:
1. Implement slight wind movement. 
2. Here, we need a character controller. We can simply have our camera follow a sphere hovering slightly above the plane, and map our controls to the sphere movement instead of the camera.
3. We want the grass to part as we move through it.
  - Simple version: Pass the character position to the grass's vertex shader (then geometry shader). Then displace the top vertices away from the character position. This works only for a fixed number of actors, and the blades don't convincingly sway as the player leaves.
  - Harder version if there's time: Have a distortion texture that stores the velocity of objects on the plane (brighter for higher velocity), and fades away. This is done by the CPU passing in the position of actors on the plane, and the compute buffer recalculating the distortion texture as the object moves each frame.
  
Optimizations + extra features:
1. Camera Fulstum Culling (on GPU side): the grass are only generated if they are in the view fulstrum, and only if they are a certain distance away.
2. Switch to using a grass mesh, then apply LOD (previously implemented in old project)
3. Randomize grass size and color.
4. Don't render the plane mesh, since the grass would cover it.

A couple of time-saving optimizations are:
1. Reuse code to generate varied geometry.
2. Reuse camera movements and boilerplate code.
3. Apply various simple filters on the grass.

Tutorials:
- Unity Grass series by Acerola: https://www.youtube.com/playlist?list=PLUKV95Q13e_U7nZFPIpiG9-z_3BdWcCOA 
- Grass Fields in Unity URP using Compute Shader: https://www.youtube.com/watch?v=DeATXF4Szqo
- Grass with URP using Geometry Shader: https://www.youtube.com/watch?v=YghAbgCN8XA
- Stylized Grass with URP https://danielilett.com/2021-08-24-tut5-17-stylised-grass/
- Recreating Zelda's grass in URP: https://www.youtube.com/watch?v=MeyW_aYE82s
- Interactive Foliage: https://www.youtube.com/watch?v=BhtNgHg8gqk

Rough plan of action:
- [x] Start with boilerplate code from realtime project. Draw a plane.
- [x] Use compute shader to generate a compute buffer of grass positions.
- [x] ~~Use geometry shader to generate a simple triangle on top of each position.~~
- [x] Check if the above renders.
- [x] Draw grass texture.

**At this point of the project, grass would be already visible. The rest are added features and optimizations**
- [x] ~~Switch geometry shader to generate 3 billboards for each grass.~~
- [x] Grass geometry in compute shader, multiple triangles per blade
- [x] ~~Fragment shader to sample the grass texture.~~
- [x] Vary the height of the grass, 
- [x] FXAA so the grass wont look bad when moving around
- [x] Color grass by height
- [ ] Skybox
- [ ] Switch camera movement from centering around a sphere at a fixed angle (because grass will definitely look really crappy from top down)
- [ ] Pass sphere position to shader, sway generated grass away from sphere position.
- [x] Add noise to grass height and color. Also add wind.
- [ ] Create a velocity texture and use compute buffer to dims it every frame. Then draw a circle mask around the character at the position of the controller, with intensity scaled by character velocity.
- [ ] Instead of dimming, use the mass-spring damper equation
- [ ] Depth of field
- [ ] Chunking
- [ ] Camera Frustrum Culling
