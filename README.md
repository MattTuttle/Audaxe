# Audaxe

Audaxe is an alternative way to load/play sounds in Haxe. It currently is in an alpha state but it supports a few different features on Mac/iPhone.

* Load Ogg Vorbis files
* Load XM/MOD/IT/S3M tracker formats
* Set loop points on the fly to allow for intros or dynamic looping
* Change volume/pan for individual channels and overall sound
* Modify playback rate of channels (simple pitch adjustment)

## Using Audaxe

The easiest way to install Audaxe is through haxelib.

```bash
haxelib git Audaxe https://github.com/MattTuttle/Audaxe.git
```

Then include Audaxe in your project.xml configuration within your own project.

```xml
<haxelib name="audaxe" />
```

See the [sample](sample/source/Main.hx) to see how to load sounds and play them.

## Hacking the Engine

If you want to hack on Audaxe you'll first need to compile the vendor libraries for your platform of choice.

### Compiling Vendor Libraries

Use the following commands to compile the vendor libraries for development builds.

```bash
cd project/vendor/src
make OS=darwin # osx
make OS=iphoneos
make OS=iphoneos ARM=v7
make OS=iphonesim
```

### Building the Engine and Sample Project

To build the Audaxe engine there are makefile targets setup for each platform. You can also build the project directly by running `haxelib run hxcpp Build.xml` within the project folder.

```bash
make osx
make ios
```

## Roadmap

Here are planned features for the full release of Audaxe.

* Android/Linux/Windows support
* Multiple sounds per channel (used for grouping)
* Channel filters (reverb/delay/low pass/high pass)
* Resample when loading to reduce cpu overhead

## Credits

Audaxe uses the following libraries:

* [OggVorbis](http://www.vorbis.com/)
* [RtAudio](http://www.music.mcgill.ca/~gary/rtaudio/)
* [DUMB](http://dumb.sourceforge.net/)

## MIT License

Copyright (C) 2013 Matt Tuttle

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
