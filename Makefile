all: ios

osx:
	cd project && haxelib run hxcpp Build.xml
	cd project && haxelib run hxcpp Build.xml -Ddebug
	cd project && haxelib run hxcpp Build.xml -DHXCPP_M64
	cd project && haxelib run hxcpp Build.xml -DHXCPP_M64 -Ddebug
	cd sample && haxelib run openfl test cpp

ios: ios-sim
	cd project && haxelib run hxcpp Build.xml -Diphoneos
	cd project && haxelib run hxcpp Build.xml -Diphoneos -Ddebug
	cd project && haxelib run hxcpp Build.xml -Diphoneos -DHXCPP_ARMV7
	cd project && haxelib run hxcpp Build.xml -Diphoneos -DHXCPP_ARMV7 -Ddebug
	cd sample && haxelib run openfl update ios -debug

ios-sim:
	cd project && haxelib run hxcpp Build.xml -Diphonesim
	cd project && haxelib run hxcpp Build.xml -Diphonesim -Ddebug

ios-lib:
	cd project/vendor/src && make OS=iphoneos ARM=v7
	cd project/vendor/src && make OS=iphonesim
	cd project/vendor/src && make OS=iphoneos
	cp project/vendor/lib/iPhone/*.a ndll/iPhone/

android:
	cd project && haxelib run hxcpp Build.xml -Dandroid
	cd sample && haxelib run openfl test android

clean:
	rm -rf project/obj
