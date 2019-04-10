# Lab 3: The Banker's Algorithm

## Project architecture 

The project includes the following modules: 

- IO
  - TaskReader
  - TaskPrinter
- Manager
  - OptimisticManager
  - Banker
- DS
  - TaskTable (time taken; time waiting; % time waiting)

Create a data structure that stores statistics regarding each task for inter-module communications. 

## Algorithm for detecting safe state


## TODO
1. Make `cycle` a private data member of OptimisticManager, not a var in function.
2. Migrate final printing functions to `TaskPrinter.cc`.

## Zipping and testing
```
zip -r lab3.zip lab3c/ -x "*.git*" -x "*.ipch" -x "*.vscode*"
```