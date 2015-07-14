# Graphical User Interface
The graphical interface is very simple to use, it allows you to create a processing chain,
to modify it and to store it.

## Load an existing chain
Loading of the chain is povided by the following command: `menu -> File -> Open..` or just use the shortcut `Ctrl+O`.

## Create a new chain

- To create a new chain you need the command `menu -> File -> New` or just use the shortcut `Ctrl+N`.
- You can add new processing steps by clicking on the button `Add Processing Step` and delete the currently selected processing step  by clicking on the button `Delete Processing Step` or just use the `Del`-button of your keyboard.
- For each step you have to select the category of the module, it uses, and also select the concrete module. The tooltips help you to select the right module by showing the description of each of the module. 
- When module has been chosen the parameters can be set. The tooltip explains the meaning of each of the parameter and also tells you whether the parameter is mandatory
- When module has been chosen the inputs can be set. The tooltip explains the meaning of each of the input and also tells you whether the parameter is mandatory. The inputs are output from other steps. You can choose to each of the inputs from which other step the output should be taken, and which concrete output.

## Store a chain
You can store the current version of the chain by the following command: `menu -> File -> Save As...` or just use the shortcut `Ctrl+Shift+S` or if the chain does already exist, and was only modified by the following command: `menu -> File -> Save` or just use the shortcut `Ctrl+S`. The chains are stored in `.yaml`-files. If your filename doesn't end by this suffix, it is added automatically.

## Undo/Redo changes
You can undo your changes by the following command: `menu -> Edit -> Undo` or just use the shortcut `Ctrl+Z`. You can redo your chnages by the following command: `menu -> Edit -> Redo` or just use the shortcut `Ctrl+Shift+Z`. The number of changes stored by the interface is unlimited. If you create a new file or open an existing file the changes stored will be deleted.

## Run/Stop the execution of the chain
You can run the chain by the following command: `menu -> Configuration -> Run` or just use the shortcut `Ctrl+R`. You can stop the execution of the chain by the following command: `menu -> Configuration -> Stop` or just use the shortcut `Ctrl+Shift+R`. Before the chain will be executed, it will be checked for validation. If there are any errors, they will be listed in the logger and also the steps, which force this errors will be marked by red color in the graphical visualization of the chain. If the validation was successfull, the chain will be executed. The graphical visualization markes the steps, which have been already finished by green color and the currently executed steps by blue color.

## Close windows
This feature can be used by the following command: `menu -> View -> Close windows` or just use the shortcut `Ctrl+W`. This feature allows you to close all the windows, which where opened by the modules during the execution by one click.

## Graphical Visualization
The graphical visualization shows all the existing steps of the current chain. The arrows between the single steps show their dependencies. The steps can be moved/reordered by the user in the way he wants. The steps can either be shown only by their names, or can also include their parameters and inputs, it is achieved by double clicking on the step. You can select the steo by moving your cursor on the step in the visualizaion. The steps get colored when running the chain see [Run/Stop](#runstop-the-execution-of-the-chain).

## Logger
The logger shows you the configuration errors, when tryiing to execute. If there are no errors, it allows the modules to write their progress, or other information during the execution.

## Progress Bar
The progress bar shows the progress of the execution. Each of the n modules, which has to be executed, gets 1/n part of the progress bar, which will be filled, when the step has been completed.

## Changes
The color of the pen, shown in the menu bar, tells you whether the current version of the file has been saved (color black), or not (color red). The graphical interface prevents you from creating new files, opening files, or closing the interface, in case you have unsaved changes. In this case you will always be asked, whether it was really wanted, or you firstly want to save your changes.
