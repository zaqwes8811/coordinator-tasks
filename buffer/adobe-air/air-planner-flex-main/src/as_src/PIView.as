package as_src
{
	import flash.events.Event;
	import mx.containers.Panel;
	import mxml_src.*;
	
	/**
	 * ...
	 * @author i
	 */
	public class  PIView  // pattern-interface
	{
		private var _pOpe:mxml_src.OperatingPanel;  // ссыль на панель с содержанием
		private var _pAtr:mxml_src.AttributesPanel;
		private var _pCon:mxml_src.ContentPanel;  // ссыль на панель с содержанием
		
		///
		private var _db:ITODODataBase;  // полиморфный
		public function PIView() 
		{
		  // инициализируем указатель на базовый интерфейс
		  _db = new TODODataBase();// UnderDevelop();	
		}
		///
		public function getPanelAtrLink(op:OperatingPanel, pa:AttributesPanel, pc:ContentPanel) : void 
		{  _pOpe = op; _pAtr = pa; _pCon = pc; }
		
		/// /// ///
		///
		/// OnEvents
		public function onCreate(e:Event) : void 
		{
			_pAtr.priority.text = "high";
			var oneEntry:TODOMapPairValue = new TODOMapPairValue();  // runtime error
			
			// заносим в заметку соедржание
			oneEntry.setStringNote( _pCon.noteField.text );
			
			// записываем в базу данных
			_db.CreateEntry(_pOpe.accessKey.text, oneEntry);
		}
		public function onRead(e:Event) : void 
		{
		  trace("onRead()");
		  
		  // получаем !все что лежит по ключу в виде массива
		  var todoArray:Array = _db.GetEntry( _pOpe.accessKey.text );
		  trace(todoArray);
		  
		  // теперь элемент один и его уже можно отобразить
		  var oneTODO:TODOMapPairValue = todoArray[0];
		  
		  // выбираем элемент - нужно будет еще данных) и
		  // раскидываем по элементам управления
		  _pCon.noteField.text = oneTODO.getStringNote();
		    // текстовое поле
		  _pAtr.priority.text = (oneTODO.getPreority()).toString();
		    // преоритет
		}
		public function onUpdate(e:Event) : void 
		{
		  trace("onUpdate()");	
		}
		/// /// ///
		///
		/// core function
		private function fillEntry() : void 
		{
			
		}
	}
	
}