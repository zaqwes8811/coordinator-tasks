package my_cool_pkg 
{
	import flash.display.Stage;
  import flash.display.Sprite;
  import flash.display.Bitmap;
  import flash.events.Event;
  import flash.events.MouseEvent;
  import flash.ui.Mouse;
 
  /**
   * ...
   * @author Makarov Evgeny aka Budy
   */
 
  public class Cursor extends Sprite
  {
    // объявляем картинку с курсором
    [Embed(source = '../../res/cursor.png')] public var CursorIco:Class;
    // объект-курсор
    private var cursor:Bitmap;
 
    public function Cursor(_stage:Stage)
    {
      // добавим спрайт с курсором на сцену
      cursor = new CursorIco();
      _stage.addChild(cursor);
 
      // создаем методы обработки событий
      _stage.addEventListener(Event.MOUSE_LEAVE, evMouseLeave);
      _stage.addEventListener(MouseEvent.MOUSE_MOVE, evMouseMove);
 
      // скроем обычный курсор
      Mouse.hide();
    }
 
    private function evMouseLeave(e:Event):void
    {
      // метод вызывается, когда курсор покидает сцену
      cursor.visible = false; // скрываем спрайт курсора
    }
 
    private function evMouseMove(e:MouseEvent):void
    {
      // обработка перемещения курсора по сцене
      if (!cursor.visible) cursor.visible = true;
      cursor.x = mouseX - 5;
      cursor.y = mouseY - 2;
      // обновим сцену после перемещения
      // в этом случае курсор будет перемещаться быстрее
      e.updateAfterEvent();
    }
 

}