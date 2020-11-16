## C++ Coding Style Conventions

Here it is a list with some of the conventions used in this project template:

Code element | Convention | Example
--- | :---: | ---
Defines | ALL_CAPS | `#define PLATFORM_DESKTOP`
Macros | ALL_CAPS | `#define MIN(a,b) (((a)<(b))?(a):(b))`
Variables | snake_case | `int screen_width = 0;`
Local variables | snake_case | `Vector2 player_position;`
Global variables | snake_case | `bool fullscreen = false;`
Constants | snake_case | `const int max_value = 8`
Pointers | MyType* pointer | `Texture2D* array;`
float values | always x.xf | `float value = 10.0f`
Operators | value1*value2 | `int product = value * 6;`
Operators | value1/value2 | `int division = value / 4;`
Operators | value1 + value2 | `int sum = value + 10;`
Operators | value1 - value2 | `int res = value - 5;`
Enum | TitleCase | `TextureFormat` 
Enum members | ALL_CAPS | `UNCOMPRESSED_R8G8B8`
Struct | TitleCase | `struct Texture2D`
Struct members |lowerCase | `texture.id` 
Functions | TitleCase | `InitWindow()`
Class | TitleCase | `class Player`
Class fields | snake_case | `Vector2 player_position`
Class methods | TitleCase | `GetPosition()`
Ternary Operator | (condition)? result1 : result2 | `printf("Value is 0: %s", (value == 0)? "yes" : "no");`

 - Project uses aligned braces or curly brackets:
```c
void SomeFunction()
{
   // TODO: Do something here!
}
```
