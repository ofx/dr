```cpp
//    _____            _     _ _____                         
//   |  __ \          (_)   | |  __ \                        
//   | |  | |_ __ ___  _  __| | |__) |__ _  ___ ___ _ __ ___ 
//   | |  | | '__/ _ \| |/ _` |  _  // _` |/ __/ _ | '__/ __|
//   | |__| | | | (_) | | (_| | | \ | (_| | (_|  __| |  \__ \
//   |_____/|_|  \___/|_|\__,_|_|  \_\__,_|\___\___|_|  |___/
//                                                           
//                                                           
```

#About

DroidRacers project
by Marlon Etheredge <http://www.marlonetheredge.name>

#Short Description

Research project at Amsterdam University of Applied Sciences

Infinite racing 2d racing game controlled by the accelerometer of Android powered devices.
Gameplay inspired by Geometry Wars.

#Running

Supplied with this project is the Visual Studio Project, VC++ must be used to compile the source of the application.

This game relies on the following libraries:

* Chipmunk physics: http://chipmunk-physics.net/
* Hge: http://www.moddb.com/engines/haafs-game-engine
* muParser: http://muparser.beltoforion.de/
* libqrencode: http://fukuchi.org/works/qrencode/index.html

#Development

Excluded from this repository are assets used by the game.

#Todo

* Fix memory leaks (the debug output on program termination will give some pointers)
* Increase performance of levelgrid implementation (deletion of a grid causes framedrops)
* Implement more weapons and bullets
* Currently ships with muParser, would be cool to build weapon and bullet generation based on this library
* dr.ini is loaded and read, but the list of variables may be extended to tune gameplay and such 
