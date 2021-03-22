# NumPy in C++

This is an attempt to imitate [NumPy](https://numpy.org/) in C++.

This project provides not only an API for Python/NumPy-like array programming but also the `py++` tool, which enables you to see execution outcomes in a pretty Python-interpreter-like fashion, as shown below.

For example, let us suppose you have [this code](https://github.com/RyotaUshio/numpy/blob/main/test/example.cpp).

```c++:test/example.cpp
```

All you have to do to see the result is only type the following command.
```
$ ./py++ test_copy_move
```
`py++` will auto-generate & compile some codes, and then show you something like this:

![a](https://github.com/RyotaUshio/numpy/blob/main/fig/py++.png?raw=true)

