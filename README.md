# JayEngine

Welcome to JayEngine a 3D game engine created for eductional purposes by Josep Casanovas Vidal(Josef21296), me. I am 
currently studying at CITM (UPC).

GitHub: https://github.com/Josef212/JayEngine
Releases: https://github.com/Josef212/JayEngine/releases

Assignment instructions:
    ·Run the engine.
    ·Go to "FBX" menu "Load town" to load the teacher material.
    ·Hierarchy should be seen on left and inspector on right, if not check on "View" above to see how to open those 
    windows.
    ·Move around hierarchy menu and try inspector options, try moving scaling and rotating, etc.
    ·Engine create a default camera called "main camera". Select it and active "Culling", move the camera from inspector 
    and see if it culls...

-Instalation:
    ·Download latest release, extract files and run Jay_Engine.exe.
        or
    ·Download latest source code version and complie it by yourself.

-How to use it:
On the latest version of the engine you can basically load fbx on the engine and load all its information into a game object
and components organitzation.

·Move the camera with WASD RF.
·Rotate the camera using mouse right click and drag.

·Press F1 to see some debug info such as frustum from the camera.

##Menus

###File
    ·Quit: Just exit the engine.

###View
    ·Configuration: Open or close configuration window.
    ·Console: Open or close console window.
    ·Hierarchy: Open or close hierarchy window.
    ·Inspector: Open or close inspector window.

###GameObject
    ·Create empty game object: creates an empty game object.
    ·Primitives: Create basically primitives. (Working on)
    ·Camera: Create a new game object with a camera component.

###Components
    ·Add transform: Add transform component to selected game object (One transform for each game object. Added by default 
    to all game objects).
    ·Add mesh: Add mesh component to selected game object.
    ·Add material: Add material component to selected game object.
    ·Add camera: Add camera component to selected game object. (Can add as many as wanted but for now only main camera default 
    created will be used for frustum culling.).

###Help
    ·ImGui Demo: Open or close ImGui demo window.
    ·Engine documentation: Browse wiki page. (Working on)
    ·Check all releases: Browse releases page.
    ·Report a bug: Browse github issues page to report bugs. (Please if you find any bug help me and create a new issue.)
    ·3rd Party Documentation: Browse all 3rd party code the engine uses.
    ·Test LOG: Just a testing tool to check different log types on console.
    ·About: Open or close a window with some basic info.

###FBX
    ·Load mech.fbx: Loads a mecha fbx.
    ·Load town.fbx: Loads a town fbx asked for first delivery.

##Hierarchy
    ·Here you can see all the scene hierarchy with all game objects on the current scene.
    ·Double click or click on the arrow to open the object and see all its childs and/or select the game object to watch it 
    on the inspector.
    ·Selected object will be rendered with its mesh in wireframe on green over the texture or the raw mesh.

##Inspector
    ·ShowGrid: Enable or disable the grid.
    ·Name: Display the objects name. You can change it.

    ·Show enclosing AABB: Show or not object's and children's enclosing box.(Green box)
    ·Show oriented box: Show or not object's and children's oriented box.(Blue box)
    ·Delete: Delete's the object and all it's childs.

###Transform
    ·Checkbox: Enable or disable the component.
    ·Position: Set objects position by its x y z.
    ·Scale: Set objects scale by its x y z.
    ·Rotation: Set objects rotation by its x y z.

###Mesh
    ·Displays some info about the mesh.
    ·Wireframe: Display the mesh on wireframe.
    ·Normals: Render normals.

###Material
    ·Checkbox: Apply the material or not.
    ·Display small info about the component.
    ·Color: Show and let change the material color.
    ·Path: Show the path of the texture and double click on ti to show the texture.

###Camera
    ·Background: Show and change the background color. (Not working until engine camera use a component object)
    ·Near plane: Allows to change the near plane distance.
    ·Far plane: Allows to change the far plane distance.
    ·Field of view: Allows to cahnge FoV(Vertical).
    ·Culling: Enable or disable frustum culling. (Currently only applied with the main camera frustum)
    ·Make this active: Nothing for now.
    ·NOTE: Press 'F1' to show the frustum(Yellow).


##License

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
