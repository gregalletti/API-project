# API-project
Source code of the Algorithms and Principles of Computer Science project course at PoliMi.

## Target
C99 implementation of a Non-deterministic Single Tape Turing Machine. 

## Built With
* [CLion](https://www.jetbrains.com/clion/) - The IDE used
* [Valgrind](http://valgrind.org/) - The leak check tool used
* [GDB Online](https://www.onlinegdb.com/) - Used to debug 


## Result
The following code provides an efficient simulator of the TM in terms of time and space complexity. Every input file is formatted in the same way, with the declaration of the transition function, acceptant states, max number of pass (to prevent an infinite loop situation) and a list of input strings. The program provides an output with one character for each input string.

## Running the program
The only thing to do is to compile the code
```
gcc -o progFinale progFinale.c
```
and run the executable with an input file, for example the one in this repository
```
progFinale < inputEx.txt
```
or, if you use Linux 
```
./progFinale < ./inputEx.txt
```

## Authors

* **Gregorio Galletti** - [gregalletti](https://github.com/gregalletti)
