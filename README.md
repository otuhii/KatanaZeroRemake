<a name="readme-top"></a>

<!-- GENERAL GAME INFO -->
<br />
<div align="center">

  <h2 align="center">KATANA ZERO</h2>

  <p align="center">
    <strong>Original game : </strong>
    <a href="https://en.wikipedia.org/wiki/Katana_Zero"><strong>General info »</strong></a>
    ·
    <a href="https://www.youtube.com/watch?v=1GkqYgIKh98"><strong>Youtube video »<strong></a>
    <br />
    <br />
  </p>
</div>



<!-- ABOUT THE PROJECT -->
## About The Project

<img src="images_&_spritesheets/gitRepoImages/main.gif" alt="dancedancedance">
<img src="images_&_spritesheets/gitRepoImages/slowmo.gif" alt="dancedancedance" width=100%>
<img src="images_&_spritesheets/gitRepoImages/replay.gif" alt="dancedancedance" width=100%>
<img src="images_&_spritesheets/gitRepoImages/dancedancedance.gif" alt="dancedancedance">


## My version

* Basic fighting mechanics(regular melee range katana attack) performed by your mouse
* Interection with environment items(flowerpot, bottles) that can be picked up and then thrown
* Time slow ability(no visuals)
* Ability to pet a cat
* HUD(and other signifiers for example time left to pass the level)
* Smooth movement that feels nice
* Extended fighting mechanics(big range katana attack)
* Dust particles for running
* Time reverse animation at the end

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- GETTING STARTED -->
## Getting Started

Project is written in c++14

### Prerequisites
* Visual Studio 2022

### How to run the project

To run project, you clone repository, set up dependancies inside visual studio(game project depends on engine project inside vs solution) and just compile.

<!-- HOW TO PLAY -->
## How to play 

### Controls
* Movement is performed with w a s d keys: a and d keys move you to the sides, w key corresponds to jumping, s key makes you crouch
* a + s or d + s - you roll in either right or left direction
* To interact with environment objects(like flower pots or lamps or if you want to pet a cat) you need to press space
* When throwable object is picked, next attack(left click) will throw it
* In order to slow the time you need to press left shift

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- CLASS STRUCTURE -->
<div align="center">
  <h2 align="center">Class structure</h2>
</div>

## Object composition 
Object composition is applied within ScreenOverlay class, which manages my Hud output and lifetime. \
Most of my manager classes uses object composition owning a vector of specific type. 
 * ParticleManager owns two pool vectors for AttackParticle pointers and CosmeticParticle pointers.
 * EnemyManager owns vector of Enemy pointers.
 * SoundManager owns vector of SoundEffect pointers and SoundStream pointers
 * SpriteManager owns vector of Sprite pointers.

Also Map class owns three vectors of environment objects: EnvironmentActiveObject, EnvironmentCosmetiObject and InteractableObject. EnvironmentActiveObject instances and EnvironmentCosmeticObject instances are stored by value thus they are deleted automatically. But my InteractableObjects are stored via pointers because i use it to implement polymorphic behavior for player interactions with those objects.

## Object aggregation
To enforce memory safety and perform easy management of animations and visuals, classes like 'Entity', 'Player', 'ScreenOverlay', 'EnvironmentActiveObject', 'EnvironmentCosmeticObject' and various 'InteractableObject' subclasses use aggregation. 
They maintain pointer to 'Sprite' instances, allowing them to cycle through animations and draw themselves without owning the underlying asset lifetimes. All sprite allocations, updates and destructions are entirely controlled by 'SpriteManager'.

## Object association
I use association inside 'LevelManager', 'ParticleManager' and 'SoundManager'.
Those classes need to access data of each other to output correct data or store correct data for my replay mechanic.
So i maintain these relations:
* LevelManager <-> ParticleManager
* LevelManager <-> SoundManager


## Inheritance 
I have two inheritance trees:
* Entities: I have two main branches for that, which are 'Entity -> Player' and 'Entity -> Enemy -> EnemySubClass'. The base entity class functions are used as general interface for physics and collisions. The enemy base class includes generalized state machine and base AI for enemy instances(regular bfs pathfinding). To handle tracking mechanics, enemy base class maintains 'Entity' pointer to its current movement target(such as the 'Player'), so here it can also be easily used to track other entities than player. Specialized attack behaviors are then isolated inside subclasses of enemy type(Grunt class and Gangster class).
  <img src="images_&_spritesheets/gitRepoImages/EntityTree.png" alt="EntityTree">
* Interactable objects: InteractableObject class is abstract interface layout. This base class sets up basic possible interactions which then are extended inside child subclasses. In these child subclasses i handle specific animations, collisions and states for each subclass.
  <img src="images_&_spritesheets/gitRepoImages/InteractableTree.png" alt="InteractableObjectTree">

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- ACKNOWLEDGMENTS -->
## Acknowledgments

* https://github.com/nlohmann/json
* https://json.nlohmann.me/api/basic_json/
* https://github.com/Marcel-Rei/Prog-2-Unity-JSON-Exporter
* https://en.cppreference.com/
* https://github.com/UnderminersTeam/UndertaleModTool.git

