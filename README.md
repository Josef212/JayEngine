# JayEngine

Welcome to JayEngine a 3D game engine created for eductional purposes by Josep Casanovas Vidal(Josef21296), me. I am 
currently studying at CITM (UPC).

GitHub: https://github.com/Josef212/JayEngine
Releases: https://github.com/Josef212/JayEngine/releases
Web: https://josef212.github.io/JayEngine/

Assignment 3 instructions:   
    · Before running the engine check "Data/Library/..." to see folders are empty and check again after running the engine to see how fbx have been imported.   
    · Some more fbx will be in Assets folder to see that engine can import many fbx.   
    · Run the engine and all fbx will be imported to the proper format.   
    · Hierarchy should be seen on left and inspector on right, if not check on "View" to see how to open those 
    windows.   
    · Move around hierarchy menu and try inspector options, try moving scaling and rotating, etc.   
    · Engine create a default camera called "main camera" and is the active camera by default so on play this will be the used camera.   
    · Click on "Play" to start and then click "Pause" or "Stop" to recover the editing status.   
    · On "View"->"Scene tree" then "Show"->"Show tree" to display the scene octree. Do the same again to stop showing it.   
    · On left under hierarchy there's a window will all resources. Click on them and then click to attach to attach resource to game objects. Try different ways.   
   
    · All drawed geometry is rendered using a default shader.    
    · You can create new shaders and edit them from outside the engine. (Working on still so may be some errors.)   
    · You can attach new shaders to materials and then  force default shader again.   

    · Video: https://www.youtube.com/watch?v=hS8AymfBZvU&feature=youtu.be   


-Instalation:
    · Download latest release, extract files and run Jay_Engine.exe.   
        or   
    · Download latest source code version and complie it by yourself.   

- How to use it:   
On the latest version of the engine all fbx on "Data/Assets/fbx/" are imported to own format (json, binary meshes and dds), 
you can load the prefabs already imported to the scene. Scene can be serielized into json. You can play and stop the engine.
Camera can cull optimitzated by an octree.   
   
Resource management makes only load once any texture or mesh.   
   
· Move the camera with WASD RF.   
· Rotate the camera using mouse right click and drag.   
· Orbit using mouse right click and drag + ALT(This is a bit strange and must be inproved).   
   
· Press F1 to see some debug info such as frustum from the camera(Also able to active on the checkbox next to play button).   
   
## Menus
   
### File
    ·TMP: Save scene: Manually saves the scene into "Data/Scenes/scene.json"
    ·TMP: Load scene: Manually load the scene from "Data/Scenes/scene.json"
    ·LoadConf: Load the configuration if have been saved before. (Not full functionality)
    ·SaveConf: Save configuration. (Not full functionality)
    ·Load: (Working on)
    ·Save: (Working on)
    ·Search files: (Working on)
    ·Quit: Just exit the engine.

### View
    ·Configuration: Open or close configuration window.
    ·Console: Open or close console window.
    ·Hierarchy: Open or close hierarchy window.
    ·Inspector: Open or close inspector window.
    ·Scene tree: Open or close tree window.

### GameObject
    ·Create empty game object: creates an empty game object.
    ·Primitives: Create basically primitives. (Working on)
    ·Camera: Create a new game object with a camera component.

### Components
    ·Add transform: Add transform component to selected game object (One transform for each game object. Added by default 
    to all game objects).
    ·Add mesh: Add mesh component to selected game object.
    ·Add material: Add material component to selected game object.
    ·Add camera: Add camera component to selected game object. (Can add as many as wanted but for now only main camera default 
    created will be used for frustum culling.).

### Help
    ·ImGui Demo: Open or close ImGui demo window.
    ·Engine documentation: Browse wiki page. (Working on)
    ·Check all releases: Browse releases page.
    ·Report a bug: Browse github issues page to report bugs. (Please if you find any bug help me and create a new issue.)
    ·3rd Party Documentation: Browse all 3rd party code the engine uses.
    ·Test LOG: Just a testing tool to check different log types on console.
    ·About: Open or close a window with some basic info.

## Hierarchy
    ·Here you can see all the scene hierarchy with all game objects on the current scene.
    ·Double click or click on the arrow to open the object and see all its childs and/or select the game object to watch it 
    on the inspector.
    ·Selected object will be rendered with its mesh in wireframe on green over the texture or the raw mesh.

## Inspector
    ·ShowGrid: Enable or disable the grid.
    ·Name: Display the objects name. You can change it.

    ·Show enclosing AABB: Show or not object's and children's enclosing box.(Green box)
    ·Show oriented box: Show or not object's and children's oriented box.(Blue box)
    ·Delete: Delete's the object and all it's childs.

### Transform
    ·Checkbox: Enable or disable the component.
    ·Position: Set objects position by its x y z.
    ·Scale: Set objects scale by its x y z.
    ·Rotation: Set objects rotation by its x y z.

### Mesh
    ·Displays some info about the mesh.
    ·Wireframe: Display the mesh on wireframe.
    ·Normals: Render normals.

### Material
    ·Checkbox: Apply the material or not.
    ·Display small info about the component.
    ·Color: Show and let change the material color.
    ·Path: Show the path of the texture and double click on ti to show the texture.

### Camera
    ·Background: Show and change the background color. (Not working until engine camera use a component object)
    ·Near plane: Allows to change the near plane distance.
    ·Far plane: Allows to change the far plane distance.
    ·Field of view: Allows to cahnge FoV(Vertical).
    ·Culling: Enable or disable frustum culling. (Currently only applied with the main camera frustum)
    ·Make this active: Nothing for now.
    ·NOTE: Press 'F1' to show the frustum(Yellow).

## Play menu
    ·Play: Start the game after saving the scene.
    ·Pause: Pause the game.
    ·Stop: Stops the game and recover the editor status.
    ·Show grid: Show or not the grid. On play it's hidden.
    ·Draw debug: Display debug info (Shortcut: F1).

## Time 
    ·Display real time clock and game clock and engine status (Editor, play or play). (Still working on to add more)

## Resources 
    ·For each resource type you can load it to scene or attach it to game objects components.
    ·For shaders you can create new ones and force recompile them.

## License

MIT License

Copyright (c) 2016 Josef21296

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
