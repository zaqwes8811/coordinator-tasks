package as_src 
{
	import system.data.maps.MultiValueMap;
	import system.data.Collection;
	/**
	 * ...
	 * @author i
	 */
	public class TODODataBase implements ITODODataBase 
	{
		
		public function TODODataBase() 
		{
			_todoMap = new MultiValueMap();
		}
		
		/* INTERFACE as_src.ITODODataBase */
		private var _todoMap : MultiValueMap;
		public function CreateEntry(assumedKey:String, entity:TODOMapPairValue):int 
		{
			// сохраняем полученный ключ
			var workAssumedKey:String = assumedKey;
	
			// проверяем есть ли еще записи по переданному ключу
			var slice:Collection = _todoMap.getCollection( workAssumedKey );
			if( slice != null ) {
			  // если записей больше одной, то получаем старшее значение номера
			  if (slice.size() > 0) {
				var pair:TODOMapPairValue = slice.get(slice.size() - 1);
				
				// и вставляемому присваиваем номер, увеличенный на единицу
				entity._numberEntry = pair._numberEntry + 1;
			  }
			}
			// Вставляем компанент
			_todoMap.put( workAssumedKey, entity );

			// Показываем всю структуру ( debug )
			var mapArray:Array = _todoMap.getValues();
			for (var i:int = 0; i < mapArray.length; i++ ) { 
			  trace(mapArray[i]._numberEntry);
			}
			trace(mapArray); // проверяем границы индексов
			return 0;
		}
		
		//
		public function GetEntry( Key:String ) : Array {
		  // проверить бы входные данные
		  
		  // извлекаем
		  var slice:Collection = _todoMap.get( Key );
		  return slice.toArray();
		}
		
		// удаление элемента 
		public function DeleteEntry(Key:String):void 
		{
			
		}
		
		public function test(assumedKey:String):void 
		{
			// показать содержимое
			
			
		}
	}
}