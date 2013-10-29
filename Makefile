all: osx

osx:
	cd project && haxelib run hxcpp Build.xml
	cd project && haxelib run hxcpp Build.xml -Dfulldebug
	cd project && haxelib run hxcpp Build.xml -DHXCPP_M64
	cd project && haxelib run hxcpp Build.xml -DHXCPP_M64 -Dfulldebug
	cd sample && haxelib run openfl test project.xml cpp

ios:
	cd project && haxelib run hxcpp Build.xml -Diphone -Diphoneos
	cd project && haxelib run hxcpp Build.xml -Diphone -Diphoneos -Dfulldebug
	cd project && haxelib run hxcpp Build.xml -Diphone -Diphoneos -DHXCPP_ARMV7
	cd project && haxelib run hxcpp Build.xml -Diphone -Diphoneos -DHXCPP_ARMV7 -Dfulldebug
	cd project && haxelib run hxcpp Build.xml -Diphone -Diphonesim
	cd project && haxelib run hxcpp Build.xml -Diphone -Diphonesim -Dfulldebug
	cd sample && haxelib run openfl update project.xml ios

android:
	cd project && haxelib run hxcpp Build.xml -Dandroid
	cd sample && haxelib run openfl test project.xml android

clean:
	rm -rf project/obj
