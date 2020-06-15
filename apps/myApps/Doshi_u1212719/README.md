/// Mit Doshi
/// U1212719

/// Running the project
1. If you have bin folder inside "..\myApps\Doshi_u1212719\" folder than run the Doshi_U1212719_debug.exe file to start the project else run the Doshi_U1212719.sln project and build the solution, it would create the .exe file for you to test.

/// Running Assignment 2
1. When you start the .exe you need to input any number from 1, 2 or 3
	- on giving input 1, A* on small graph will start executing, to exit from this state, give input -1 -1.
	- on giving input 2, A* on large graph will start executing, to exit from this state, give input -1 -1.
	- on giving input 3, openframework will lauch its window with dynamic world with obstacles (darker colored) and a blue colored player* 
	- click on any light colored node and the player will to try to go there if it finds a path

*after input 3, you cannot check A* on the graphs and need to restart the application.

/// C1083 - Cannot open include file

1. This issue comes up because the compiler can't find the specified files. To fix this, right click on the project solution file and click "Properties". Go to C/C++ > Additional Include Directories and add the following lines:

	- $(ProjectDir)src;
	- $(ProjectDir)PathFinding;
	- $(ProjectDir)PathFinding\Schemes;
	- $(ProjectDir)MovementAlgorithms\Dynamic;
	- $(ProjectDir)MovementAlgorithms\Kinematic;
	- $(ProjectDir);
