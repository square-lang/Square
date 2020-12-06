# The Square programming language

[![Build status](https://github.com/square-lang/Square/workflows/build/badge.svg)](https://github.com/square-lang/Square/actions)
[![Gitpod ready-to-code](https://img.shields.io/badge/Gitpod-ready--to--code-blue?logo=gitpod)](https://gitpod.io/#https://github.com/square-lang/Square)

## What is Square

Square is a powerful programming language.

### Features of Square

* Simple Syntax
* Highly Portable
* Easy to learn
* Small

## How to run

```bash
cd src
$ a.exe ../examples/helloworld.squ
```

## Examples

Print "Hello World!" on the screen:

```square
print "Hello World!"
```

A program to check whether it is an even number:

```square
a := 10
# You can replace "==" with "is" #
if a % 2 is 0 -> {
    print "This is a even number"
}

else -> {
    print "This is a odd number"
}

```

Result:

```plaintext
This is a even number
```

## Contribution

Welcome to pull a request!

## TODO

* A tiny VM for square!

## License

Copyright (c) 2020 Stepfen Shawn

## The Author

Square was designed and developed by [Stepfen Shawn](https://github.com/StepfenShawn) in 2020.
