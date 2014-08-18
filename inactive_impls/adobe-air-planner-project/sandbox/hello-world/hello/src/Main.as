package
{
  import flash.display.Sprite;
  import flash.display.SimpleButton;
  import flash.events.Event;
  import flash.text.TextField;
  import mx.core.ButtonAsset;
 
  /**
   * ...
   * @author Makarov Evgeny aka Budy
   */
  public class Main extends Sprite
  {
 
    public function Main():void
    {
      if (stage) init();
      else addEventListener(Event.ADDED_TO_STAGE, init);
    }
 
    private function init(e:Event = null):void
    {
      removeEventListener(Event.ADDED_TO_STAGE, init);
      // entry point
 
      var tf:TextField;
      tf = new TextField();
      tf.text = 'Hello, World!';
 
      addChild(tf);  // add to panel
	  var el:SimpleButton;
	  el = new SimpleButton();
	  el.width = 200;
	  el.height = 400;
	  el.name = 'asdf';
	  addChild(el);
    }
 
  }
 
}