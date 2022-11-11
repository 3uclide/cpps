# Class

## Examples

```c++
public class my_class: =
{
	my_var: int = 0;	
	my_func: (i: int) =
	{
		my_var = i;
	}
	
	my_sub_class: public class =
	{
	...
	}
}

public class my_override_class: public my_class =
{
...
}

public class my_override_multi_class: public my_class, public my_class2 =
{
...
}

public class my_template_class<_ T, int Size>: =
{
	my_var: T = ();
}

public class my_override_template_class<int Size>: public my_template_class<int, Size> =
{
...
}
```
