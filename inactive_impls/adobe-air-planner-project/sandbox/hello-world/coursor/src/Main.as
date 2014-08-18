package 
{
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.events.TextEvent;
	import flash.text.TextField;  // поле вывода
	
	/**
	 * ...
	 * @author i
	 */
	public class Main extends Sprite 
	{
		private var Ru2Do_:SimpleButtonExample;
		private var Do2Ru_:SimpleButtonExample;
		private var RuEdit_:TextField;
		
		///
		public function Main():void 
		{
			if (stage) init();
			else addEventListener(Event.ADDED_TO_STAGE, init);
		}
		
		private function init(e:Event = null):void 
		{
			removeEventListener(Event.ADDED_TO_STAGE, init);
			// entry point
			
			/// ru->
			Ru2Do_ = new SimpleButtonExample();
			addChild(Ru2Do_);
			Ru2Do_.addEventListener(MouseEvent.CLICK, onRu2DoClick);
			
			/// do->
			this.Do2Ru_ = new SimpleButtonExample();
			this.Do2Ru_.setPos(80, 40);
			addChild(this.Do2Ru_);
			this.Do2Ru_.addEventListener(MouseEvent.CLICK, this.onDo2RuClick);
			
			///
			this.RuEdit_ = new TextField();
			this.RuEdit_.height = 20;
			this.RuEdit_.width = 30;
			this.RuEdit_.text = 'asdf';
			this.RuEdit_.border = true;
			this.RuEdit_.borderColor = 0x00CCFF;
			this.RuEdit_.selectable = true;
			//this.RuEdit_.backgroundColor = 0x00CCFF;
			//this.RuEdit_.addEventListener(
			addChild(this.RuEdit_);
			
		}
		// onClickRu
		private function onRu2DoClick(event:Event):void {
			var value:uint = 10;
           this.RuEdit_.text = value.toString();
        }
		// onClickDo
		private function onDo2RuClick(event:Event):void {
			//this.Do2Ru_.height = 10;
		}
		
	}
	
}