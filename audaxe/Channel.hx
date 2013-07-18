package audaxe;

#if neko
import neko.Lib;
#elseif cpp
import cpp.Lib;
#end

class Channel
{

	public function new(handle:Dynamic)
	{
		__handle = handle;
		this.volume = 1;
		this.pan = 0;
		this.rate = 1;
	}

	public var sound(default, set):Sound;
	private function set_sound(value:Sound):Sound
	{
		if (value != sound)
		{
			audaxe_channel_set_sound(__handle, value.handle);
			sound = value;
		}
		return value;
	}

	public var pan(default, set):Float;
	private function set_pan(value:Float):Float
	{
		if (value < -1) value = -1;
		else if (value > 1) value = 1;

		if (value != pan)
		{
			audaxe_channel_pan(__handle, value);
			pan = value;
		}
		return value;
	}

	public var volume(default, set):Float;
	private function set_volume(value:Float):Float
	{
		if (value < 0) value = 0;

		if (value != volume)
		{
			audaxe_channel_volume(__handle, value);
			volume = value;
		}
		return value;
	}

	public var rate(default, set):Float;
	private function set_rate(value:Float):Float
	{
		if (value < 0) value = 0;

		if (value != rate)
		{
			audaxe_channel_rate(__handle, value);
			rate = value;
		}
		return value;
	}

	private var __handle:Dynamic;

	private static var audaxe_channel_pan = Lib.load("audaxe", "audaxe_channel_pan", 2);
	private static var audaxe_channel_rate = Lib.load("audaxe", "audaxe_channel_rate", 2);
	private static var audaxe_channel_volume = Lib.load("audaxe", "audaxe_channel_volume", 2);
	private static var audaxe_channel_set_sound = Lib.load("audaxe", "audaxe_channel_set_sound", 2);
}