# PlatformerGL
### ISART DIGITAL : School Project - GP1 - Guillaume Chiclet/Kevin Pawlonka

---

<!-- TABLE OF CONTENTS -->
<details open="open">
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#screenshots">Screenshots</a></li>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
        <li><a href="#how-to-launch">How to launch</a></li>
      </ul>
    </li>
    <li>
      <a href="#features-&-usage">Features & Usage</a></li>
    <li>
      <a href="#details">Details</a>
      <ul>
        <li><a href="#comments">Comments</a></li>
        <li><a href="#versionning">Versionning</a></li>
        <li><a href="#author">Author</a></li>
      </ul>
    </li>
  </ol>
</details>

<!-- ABOUT THE PROJECT -->
# About The Project

The goal of the project is to link both graphic and physic engine to make a platformer with it.

## Built with
The graphic engine was built with :
* OpenGL 3.3 with GLAD -> Window creation & Rendering process
* STB Image -> Texture loading
* ImGui -> User/Debug interface

<!-- GETTING STARTED -->

# Getting Started

Before getting started, you need to install the engine.

## Prerequisites

>If you don't have the compiled version:

You need to have Visual Studio 2019 installed on your computer.

>If you have the compiled version:

You can skip to <a href="#how-to-launch">How to launch</a> !

## Installation

You then need to compile the solution with **Visual Studio 2019** in Release mode

You can test it directly from Visual Studio, but you can also launch it from executable located in x64/Release/


## How to launch
Execute : **engine.exe**

---

# Features & Usage

Currently, you can do all sorts of things with the ImGui interface that is implemented.
---

First of all, there is the ResourceManager Manager.

You can create/load/save/close scenes with it.

---
Then, if you have a loaded scene, you can access the Shader Loader & the Model Loader.
You absolutely need a shader specified to load a model.
The model is then added to the current scene.

If you try to load a model that is already loaded, it will be faster because the shaders, meshes and textures are already loaded in the ResourceManager.

---
After that, there is the current scene parameters.

You can access all the parameters of your models, lights and camera from here.

* Camera :
  You can change its move speed, the field of view, and the near & far clipping planes.

* Lights :
  You can enable lights, change their colors and intensities (with the attenuation).

* Models :

  You can scroll between the different models that are present in your scene, move them via their transform, and modify their subMeshes individually (if they are visible or not, if they receive light a certain way with the materials).

  You can also set a Parent Transform.

* Scene Saver :
  
  You can press F5 while playing to save the current state of the game and everything that comes with it. You can of course after saving your game, load it or create a totally new game.

* Player :

  You have a playable model, that can be controlled with WASD and the space keys.

* Physics :

  There is a lot of implemented Physics in the game, with colliders and collisions.

## Versioning

We did use GIT for versioning.

## Authors

* **Guillaume Chiclet**
* **Kevin Pawlonka**
