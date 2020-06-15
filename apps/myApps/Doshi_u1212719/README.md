/// Mit Doshi
/// U1212719

/// Running the project
1. If you have bin folder inside "..\myApps\Doshi_u1212719\" folder than run the Doshi_U1212719_debug.exe file to start the project else run the Doshi_U1212719.sln project and build the solution, it would create the .exe file for you to test.

/// Running Assignment 2
1. When you start the .exe
2. Press F1 to hide/show the Decision Tree Boid (Destroyer Boid)
3. Press F2 to hide/show the Behavior Tree Boid (Killer Boid)
4. Press F3 to Create new boid that uses GOAP to kill the player

*after input 3, you cannot check A* on the graphs and need to restart the application.

/// C1083 - Cannot open include file

1. This issue comes up because the compiler can't find the specified files. To fix this, right click on the project solution file and click "Properties". Go to C/C++ > Additional Include Directories and add the following lines:

	- $(ProjectDir)src;
	- $(ProjectDir)PathFinding;
	- $(ProjectDir)PathFinding\Schemes;
	- $(ProjectDir)MovementAlgorithms\Dynamic;
	- $(ProjectDir)MovementAlgorithms\Kinematic;
	- $(ProjectDir);
