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
A program to check whether it is an even number:  
```
a := 10
# You can replace "==" with "is" #
if a % 2 is 0 -> {
    print("This is a even number")
}

else -> {
    print("This is a odd number")
}
```
Result:
```
This is even number
```  
A program to print 1~100 on the screen
```
# print number from 1 to 100 #
i := 1
loop when i <= 100 -> {
    print(i)
    # TODO: use inc(i) #
    i := i + 1
}
```
## Contribution
Welcome to pull a request!
## License
Copyright (c) 2020 Stepfen Shawn
## The Author
Square was designed and developed by [Stepfen Shawn](https://github.com/StepfenShawn) in 2020.  