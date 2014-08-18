package 
{
  // UI
  import flash.display.Sprite;
  import flash.events.Event;
  import flash.events.MouseEvent;
  import flash.events.TextEvent;
  import flash.text.TextField;  // поле вывода
  /***/
	public class Main extends Sprite 
	{
		// UI
		private var Button_:SimpleButtonExample;
		private var ButtonTwo_:SimpleButtonExample;
		private var _db:SQLiteAPITrial;

		// Ui
		private function onButtonClick(event:Event):void {
            trace("Button Click()");
			_db.select( "firstName" );

        }
		private function onButtonTwoClick(event:Event):void {
            trace("Button Two Click()");
			_db.TestInsert();
        }
		private function init(e:Event = null):void 
		{
			removeEventListener(Event.ADDED_TO_STAGE, init);
			/// Button !! с масштабом проблемы
			Button_ = new SimpleButtonExample();
			addChild(Button_);
			Button_.addEventListener(MouseEvent.CLICK, onButtonClick);
			//
			ButtonTwo_ = new SimpleButtonExample();
			addChild(ButtonTwo_);			
			ButtonTwo_.x = 100;	ButtonTwo_.height = 60;	ButtonTwo_.width = 60;
			ButtonTwo_.addEventListener(MouseEvent.CLICK, onButtonTwoClick);
			// !! связаны с размером окна!!
		}
		
		/// /// ///
		///
		/// Main
		public function Main():void 
		{
			/// UI
			if (stage) init();
			else addEventListener(Event.ADDED_TO_STAGE, init);
			
			//
			_db = new SQLiteAPITrial();
			_db.openDB();
		}
	}
}