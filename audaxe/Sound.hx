package audaxe;

#if neko
import neko.Lib;
#elseif cpp
import cpp.Lib;
#end
import sys.FileSystem;

class Sound
{
	private function new(handle:Dynamic)
	{
		this.handle = handle;
	}

	public static function loadOggVorbis(path:String):Sound
	{
		if (FileSystem.exists(path))
		{
			return new Sound(audaxe_sound_load_vorbis(path));
		}
		trace("[audaxe.Sound] There is no Sound asset at \"" + path + "\"");
		return null;
	}

	public static function loadTracker(path:String):Sound
	{
		if (FileSystem.exists(path))
		{
			return new Sound(audaxe_sound_load_tracker(path));
		}
		trace("[audaxe.Sound] There is no Sound asset at \"" + path + "\"");
		return null;
	}

	public function setLoop(start:Int = 0, end:Int = -1)
	{
		audaxe_sound_loop_points(handle, start, end);
	}

	public function play()
	{
		audaxe_sound_operation(handle, soundOperationPlay);
	}

	public function stop()
	{
		audaxe_sound_operation(handle, soundOperationStop);
	}

	public function pause()
	{
		audaxe_sound_operation(handle, soundOperationPause);
	}

	public function resume()
	{
		audaxe_sound_operation(handle, soundOperationResume);
	}

	public var handle:Dynamic;

	private static inline var soundOperationPlay:Int = 0;
	private static inline var soundOperationStop:Int = 1;
	private static inline var soundOperationPause:Int = 2;
	private static inline var soundOperationResume:Int = 3;

	private static var audaxe_sound_loop_points = Lib.load("audaxe", "audaxe_sound_loop_points", 3);
	private static var audaxe_sound_operation = Lib.load("audaxe", "audaxe_sound_operation", 2);
	private static var audaxe_sound_load_tracker = Lib.load("audaxe", "audaxe_sound_load_tracker", 1);
	private static var audaxe_sound_load_vorbis = Lib.load("audaxe", "audaxe_sound_load_vorbis", 1);
}
