# Unlock maker

## Description
This README aims to introduce new people to be able to work on the Unlock Project. You will see here how to install the needed libraries to run the program : gtk and glib. This project is developped on CLion and uses a CMakeLists for its compiling.

## File organization
This project's files are sorted into 3 main directories : Model, View, and Controller. Any file you will add to this project must be tied to one of these 3 directories and thus put in it.

## Git organization
For the moment, we're working on several different git branches.
- The branch main contains the main versions of the project
- The branch devModel contains the work in progress for the model, only on the console
- The branch devView contains the work in progress for the view, on the Graphical interface.

## Visuals
This section will be updated when visuals will get more advanced.

## Installation
GTK4 (Linux OS):
If you want to modify the application, as a developer, and be able to build and run a project or just use the application we have created, the gtk4 libraries are required.
To get them you need to install the gtk4 packages.

1.) Open a terminal.

2.a) If you expect to develop an application you will need gtk4 and his dependencies. Tap these commands on the terminal:
- 'sudo apt-get install libgtk-4-dev'
- 'sudo apt-get install glib-2.0'
	
2.b) if you expect to use a gtk application, tap this command on the terminal:
- 'sudo apt-get install libgtk-4-1'

3.) Enter your login to authorize the downloading of the gtk4 packtages.

