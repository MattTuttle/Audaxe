all: osx

osx:
	cd project/vendor/src && make OS=darwin
	lime rebuild . mac

ios:
	lime rebuild . ios
	cd sample && haxelib run openfl update ios

android:
	lime rebuild . android
	cd sample && haxelib run openfl test android

clean:
	rm -rf project/obj
