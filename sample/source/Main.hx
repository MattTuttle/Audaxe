import flash.Lib;
import flash.display.Sprite;
import flash.filesystem.File;
import flash.events.KeyboardEvent;
import flash.events.TouchEvent;
import flash.events.Event;
import flash.ui.Keyboard;
import flash.ui.Multitouch;
import flash.ui.MultitouchInputMode;

import audaxe.Engine;
import audaxe.Sound;
import audaxe.Channel;

class Main extends Sprite
{

	public function new()
	{
		super();

		if (Lib.current.stage == null)
		{
			Lib.current.stage.addEventListener(Event.ADDED_TO_STAGE, onStage);
		}
		else
		{
			onStage();
		}
	}

	private function onStage(?e:Event)
	{
		channel = Engine.createChannel();
		// var sound = Sound.loadOggVorbis("assets/bgm.ogg");
		var sound = Sound.loadTracker("assets/storm.xm");
		if (sound != null)
		{
			sound.setLoop();
			channel.sound = sound;
			sound.play();
		}

		// channel = Engine.createChannel();
		// sound = Sound.loadTracker("assets/storm.xm");
		// if (sound != null)
		// {
		// 	channel.sound = sound;
		// 	sound.play();
		// }

		Lib.current.stage.addEventListener(KeyboardEvent.KEY_DOWN, onKeyDown);
		if (Multitouch.supportsTouchEvents)
		{
			Multitouch.inputMode = MultitouchInputMode.TOUCH_POINT;
			Lib.current.stage.addEventListener(TouchEvent.TOUCH_MOVE, onTouchMove);
		}
	}

	private function onTouchMove(e:TouchEvent)
	{
		var dx = e.localX / Lib.stage.stageWidth,
			dy = e.localY / Lib.stage.stageHeight;
		channel.pan = dx * 2 - 1;
		channel.rate = 2 - dy * 2;
	}

	private function onKeyDown(e:KeyboardEvent)
	{
		switch (e.keyCode)
		{
			case Keyboard.UP:
				channel.rate += 0.01;
			case Keyboard.DOWN:
				channel.rate -= 0.01;
			case Keyboard.LEFT:
				channel.pan -= 0.05;
			case Keyboard.RIGHT:
				channel.pan += 0.05;
			case Keyboard.SPACE:
				channel.rate = 1;
				channel.pan = 0;
		}
	}

	private var channel:Channel;

}