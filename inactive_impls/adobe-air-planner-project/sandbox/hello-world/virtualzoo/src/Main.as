// utf-8
/**
 * как передаются переменные
 * 
 * 
 * */
package 
{
	import flash.display.Sprite;
	import flash.events.Event;
	import zoo.VirtualZoo;
	
	/**
	 * ...
	 * @author i
	 */
	public class Main extends Sprite 
	{
		
		public function Main():void 
		{
			if (stage) init();
			else addEventListener(Event.ADDED_TO_STAGE, init);
		}
		///
		private function init(e:Event = null):void 
		{
			removeEventListener(Event.ADDED_TO_STAGE, init);
			// entry point
			var zoo = new zoo.VirtualZoo();
			trace("asdf");
		}
		
	}
	
}