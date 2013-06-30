package audaxe;

#if neko
import neko.Lib;
#elseif cpp
import cpp.Lib;
#end

class Engine
{

	public static function createChannel():Channel
	{
		return new Channel(audaxe_engine_create_channel(handle));
	}

	public static var volume(default, set):Float;
	private static function set_volume(value:Float):Float
	{
		if (value != volume)
		{
			audaxe_engine_volume(value);
			volume = value;
		}
		return value;
	}

	private static var handle(get, never):Dynamic;
	private static inline function get_handle():Dynamic
	{
		if (_handle == null)
		{
			_handle = audaxe_engine_init();
		}
		return _handle;
	}
	private static var _handle:Dynamic;

	private static var audaxe_engine_init = Lib.load("audaxe", "audaxe_engine_init", 0);
	private static var audaxe_engine_volume = Lib.load("audaxe", "audaxe_engine_volume", 1);
	private static var audaxe_engine_create_channel = Lib.load("audaxe", "audaxe_engine_create_channel", 1);
}