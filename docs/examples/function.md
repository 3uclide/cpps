# Function

Parameter are *in* by default

```c++
[function_name]<[templates]>: [[lambda_capture]]([parameters...]) -> [return_type] = 
{
    [function_body]
}
```

## Examples

```c++
// normal function
my_func: (i: int) -> int =
{
    return 0;
}

// lambda
my_lambda: [] (i: int) -> int =
{
...
}

// template function
my_template<_ T, int Size>: (i: T) -> int =
{
...
}

// template lambda function
my_template<_ T, int Size>: [] (i: T) -> int =
{
...
}

// in, copy, inout, out, move, forward
my_func: (param: int) -> void = {}
my_func: (in param: int) -> void = {}
my_func: (copy param: int) -> void = {}
my_func: (inout param: int) -> void = {}
my_func: (out param: int) -> void = {}
my_func: (move param: int) -> void = {}
my_func: (forward param: int) -> void = {}
```
