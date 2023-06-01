# Unlock maker

## Description
This README aims to present the UnlockMaker project. It will explain to you the organization of the development and the installation processes, for a user of the software and for a developer who want to work on the project. You will see here how to install the needed libraries to run the program : gtk, glib, cmocka and graphviz. This project is developed on CLion and uses a CMakeLists.txt for its compiling. Currently, the project is designed for Linux Ubuntu only. 
In this ReadMe, you will find 5 useful sections :
- File organization
- Git organization
- User installation
- Developer installation
- Unit tests

If you expect to use the UnlockMaker software to create an Unlock scenario and then print your card game, please take a look at the "Important warning about images" section below.

## File organization
This project's files are sorted into 3 main directories : Model, View, and Controller. Any file you will add to this project must be tied to one of these 3 directories and thus put in it.

## Git organization
For the moment, we're working on several different git branches:
- The branch main contains the main versions of the project
- The branch devModel contains the work in progress for the model, only on the console
- The branch devView contains the work in progress for the view, on the Graphical interface.

## Visuals
This section will be updated when visuals will get more advanced.

## User installation (on Linux Ubuntu)

If you want to install UnlockMaker software in order to use it for Unlock scenarios creation, you will need to respect the following installation procedure. Indeed, several libraries and packages are required by the software UnlockMaker to work correctly. Hence, you will find in this section a detailed installation process.

#### Package versions

As you will install tools, first update the package version on your computer :
```
$ sudo apt-get update
```
#### GTK4 + Glib

GTK4 and Glib libraries are required for the graphical interface of UnlockMaker software. 
To get them in order to use a gtk application, tap this commands on a terminal :
```
$ sudo apt-get install libgtk-4-1
$ sudo apt-get install glib-2.0
```

#### GraphViz

Graphviz library is required for graph displaying, which is a major feature of our software's graphical interface.
To install it on your computer, please follow these steps :

1. Download the latest .gz archive of graphviz on the website https://graphviz.org/download/source/

2. Extract the downloaded archive

3. Open a terminal in the extracted graphviz directory and tap the following commands :
```
$ ./configure
$ make
$ sudo make install
```
4. Graphviz should now be installed, you can therefore delete the archive and the extracted directory.

#### Libharu 

The open source libHaru library is required for pdf generation, which is an important feature enabling to print its deck of cards once the scenario is designed and images have been imported. This library is intern to the project, you will download it by cloning this git repository. However, it could be required to update zlib library which is used by libHaru. 
To update zlib on your computer, tap this command on a terminal :
```
$ sudo apt-get install zlib1g-dev
```
## Developer installation (on Linux Ubuntu)

If you want to contribute to the UnlockMaker software as a developer, it will be required to install several additional libraries. In this section, you will find the complementary installation process, depending if you want to work on the model or on the view of the project.

#### Libraries for model development

If you want to compile and run only the model of this project, the only required library is libHaru for pdf generation. In this case, you just have to update zlib as explained above and install CMocka whose installation process is explained below.

#### CMocka

CMocka library is required for running unit tests of the project.
To install it on your computer, please follow these steps :

1. Download the latest .tar.xz archive of cmocka on the website https://cmocka.org/

2. Open a terminal in the repertory containing the downloaded archive and extract it with the command :
```
$ tar -xf cmocka-1.1.5.tar.xz
```
3. Tap the following commands to install cmocka on your computer
```
$ mkdir cmocka-build
$ cd cmocka-build
$ cmake ../cmocka-1.1.5
$ make
$ sudo make install
```
#### Lcov

Lcov library is used to generate graphical measure of the code coverage by unit tests.
To install it on your computer, tap this command on a terminal :
```
$ sudo apt-get install lcov
```
#### Libraries for view development

If you want to work on the view of this project, you will need to install all the libraries mentioned in the User installation section. Moreover, you will have to install gtk4 package for developers, whose installation process is explained below.

#### GTK4 for developers

The gtk4 package used for development is separate from the package for using.
To install it on your computer, tap this command on a terminal :
```
$ sudo apt-get install libgtk-4-dev
```
## Unit tests

A major part of unit testing have been done for the model part of this project. If you want to work on it as a developer, you will likely have to design and run unit tests. In this section, you will find a detailed process if you want to run already designed unit tests of the model.

1. First, you have to follow the installation procedure for model development to have CMocka on your computer.

2. Then, you have to go into the file Model/TestsU/ProjectTestsU and to change the following variables located at the top of the .c file :
    - char* projectPath (in ProjectTestsU)
    - char* imagePath (in ProjectTestsU)

3. You have to set projectPath to a valid repertory path on your computer. This repertory will be used to install project repertories during unit testing, but everything will be automatically removed. 
You have to set imagePath to a valid existing image path on your computer. This image will be used during unit testing, but it won't be modified or removed.

4. Then, you have to go into the file CMakeLists.txt to change a target compilation variable located at the top of the file. You have to set UNI_TESTS_MODE as yes by replacing the "**no**" in the line set(UNI_TESTS_MODE no) by "**yes**". Then, you have to reload if the CMakeLists.txt asks you to do so. If you have correctly installed all required library, it should switch your compilation target to UnlockMakerTestsU.

5. Then, you have to compile and run the unit tests executable UnlockMakerTestsU.

6. Then, you can generate a graphical measure of the code coverage using gcov and lcov. To do so, open a terminal in the unlock-maker directory. Then, type the following commands in the terminal, it will generate you an index.html file in the cmake-build-debug repertory :
```
$ lcov --capture --directory cmake-build-debug/CMakeFiles/UnlockMakerTestsU.dir/Model --output-file coverage.info
$ genhtml coverage.info --output-directory cmake-build-debug
```
When running unit tests, you will have lots of error messages in stderr. However, it's normal, and it's not a cause of failure of the tests. Most of them are error messages about a file impossible to delete, but the reason is this file didn't exist on your computer when you ran the tests. There are useful messages for debugging.

## Important warning about images

When using UnlockMaker software, before importing any image in an Unlock scenario, please make sure to check the image is in the **JPEG** (.jpg) format. If not, you won't be able to generate a pdf with the complete card game at the end of your project, although the image would be renamed as .jpg in the repertories of your project.

## Project context and contributors

The creation of UnlockMaker software was a school project aiming to improve our skills in C programming language. It was initialized by a team of 5 INSA Rennes students : Dario Ergun, Louenn Fradet, Thomas Golitin, Yasser Miniya and Maxime Plumejeau. Our supervisor for this project was Karol Desnos.
