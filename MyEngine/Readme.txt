Half a year ago I decided that it would be a good idea to make my own game engine, it's not very useful since it doesn't support dynamic buffer updates yet (but I have done it in the past as well as exponential shadow mapping), it supports my own texture and font file formats, you just have to feed it an std::vector of texture names and it will load them, their mipmaps and create texture2darray, it also has a text class that creates text mesh out of string

Sharing code and executable

Graphics class is just a test class, some classes are not fully implemented yet, DirectX11 class was supposed to be window handling class but I didn't finish it
