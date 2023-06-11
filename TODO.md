# To-dos:

- [ ] code cleanup
- [ ] switch to std::vector
- [ ] coherent coding style
- [ ] make saul::TextU store raw text
- [ ] Documentation
  - [X] extend README.md:
    - [X] additional dependency/install instructions
    - [X] project introduction
    - [X] outline project structure
  - [ ] make /docs folder:
    - [ ] document basic usage
    - [ ] examples
    - [ ] document structure
    - [ ] documentation guidelines
- [ ] new UI elements
  - [ ] Input fields
  - [X] checkboxes
  - [ ] sliders
- [ ] save/load states of window and its structs
- [ ] render to texture, that texture can be used as a camera linking to another Window class

# Notes

## Improving SAUL's workflow
By creating a base SAUL struct that has a void pointer to the actual struct, we can stop creating lists for each type of UI struct implemented into the library. This method would also make possible object properties. Although having multiple of these is redundant as of now. A rough example of how would this work:

```c++
struct property {
  saul::TYPE type,
  void* object
};
struct object {
  const char* name,
  std:list<property> properties,
};
```

The following conversation regarding this improvement was recorded:
```text
rather than having 5 std::list of structs
why not have a saulObj struct
that has the name
a void pointer
and a TYPE indicating the pointer's type
with this, you only need one list of objects
```

## debugging note

debugging flags would be macros with 2 to the power of X, where X is the logging level we wanna turn on.
This way, we can OR together flags.
The master-debug-flag would be a heap object. A pointer to it would be stored in the game object.

Update: Debugging updated to use bool[]

## miscellaneous note no1

Overwrite new operator with calloc to prevent many exploits

## 3D is unfeasible

Just this, don't forget it. OpenGL or Vulkan is better for 3d than SDL2, and smaller 3d rendering features are simply not worth the time and effort implementing.

## Event handling within class

The desired flow would be

1. poll events
2. see whether any event is a registered trigger for closing the window
3. call the destructor if yes

## Events

Maybe, pass function pointers to events so the main class can call a function when the Escape key is pressed and then that function will be passed the window class and other environment variables.

## Windows

The only files necessary are the .sln files and the .vcxproj files, [example](https://github.com/id-Software/Quake-III-Arena/tree/master/code/)

## Parameter template class

This would be to reduce the number or getter/setter function to 1 universal template. The basic idea is something along the lines of
```c++
class<type T> Parameter{
  private:
    T value;
  public:
    void setValue(T newvalue);
    T getValue();
}
class Window{
  private:
    // private functions and some internal variables that should not be set as a whole (debugflags)
  public:
    Parameter<bool> quitOnSignal;
    Paramater<unsigned int> activeScreen;
    // the usual stuff
}
```
However, this is not yet needed, as there are only 2 pairs of getter/setters that could be delegated.

## Dones
- [X] figure out a way to compile the library
- [X] implement it in the Makefile
- [X] migrate some more memory heavy objects to the heap:
  - [X] the windows class
  - [X] Textures
- [X] implement a logging system for execution times and malloc and free()
- [X] fix possible memory leaks
- [X] make the whole thing a bit more coherent
- [X] add more logging and debugging options:
  - [X] logging levels
  - [X] debug switch on runtime
- [X] Render:
  - [X] Texture
  - [X] TexturePrimitive
  - [X] Text
  - [X] Button
- [X] separate saul::render() for different saul elements
- [X] logging to file
- [X] basic event polling for Window class
  - [X] window close event trigger
  - [X] setting for when to close
  - [X] Button event implementation
- [X] Windows building
  - [X] Port code
  - [X] Get it to build
  - [X] place files needed for Windows in a /windows folder, build from solution from there
  - [X] update anything else that would need updating for Windows
