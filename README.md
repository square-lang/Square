[![Build Status](https://travis-ci.org/square-lang/Square.svg?branch=master)](https://travis-ci.org/square-lang/Square)  
## What is Square
Square is a powerful programming language.
### Features of Square
* Simple Syntax
* Highly Portable
## How to run
```
$ cd src
$ ./a.exe ../examples/helloworld.squ
```
## Examples
A program to check whether it is an odd number:  
```
(
    lambda x : 
    if(x % 2 == 0)
    {
        print("This is a even number")
    }
    else
    {
        print("This is odd number")
    }
) <= 1
# Assign 1 to lambda's parameters #
```
Result:
```
This is odd number
```
## Contribution
Welcome to pull a request!
## License
Copyright (c) 2020 Stepfen Shawn
## The Author
Square was designed and developed by [Stepfen Shawn](https://github.com/StepfenShawn) in 2020.  