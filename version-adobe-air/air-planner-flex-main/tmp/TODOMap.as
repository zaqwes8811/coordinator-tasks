package  as_src
{
    import com.hexagonstar.structures.maps.HashMap;
	import com.hexagonstar.structures.IIterator;
	import system.data.maps.MultiValueMap;  // новый контейнер
	
	/**
	 * abs. : все управление ведется через текстовые ключи с датой, 
	 *   к которой привязана заметка
	 * notes : 
		 * при чтения можно пользоваться итераторами, поэтому
		 * ими нужно научится пользоваться (все для HashMap)
	 */
	
	public class TODOMap implements ITODOMap 
	{
		// Кэш для...
		private var map_:HashMap;  
		private var _map:MultiValueMap;  // со многими ключами
		
		// Констркутр. Стркутура данных будет загрузаться извне
		public function TODOMap() {
			// создаем кэш по умолчанию
			map_ = new HashMap(10);
			_map = new MultiValueMap();
			
			// загрузить активное окно из..
			// 1.SQLite
			// 2. vk..
		}
		
		// Iface
		public function addItem( key:String, value:TODOMapPairValue ) : Boolean	{
			return map_.put(key, value);  // может не решение
		}
		public function removeItem( key:String ) : Boolean {
			// возвращает пока что что-то странное
			map_.remove(key);
			return true;
		}
		public function getTODOValue( key:String ) : TODOMapPairValue {
			// if
			return map_.getValue( key );
	    }
		public function ShowAll() : void {
			// Проходим по всему контейнеру
			var i:IIterator = map_.iterator;
			while (i.hasNext) {
				var n:Array = i.next;
				
				// получаем ключ
				trace(n[0]);
				
				// снимаем копию с объекта 
				var oneTODO:TODOMapPairValue = n[1];
				
				// обрабатываем объект
				trace(oneTODO.getStringNote());
			}
		}
    }
}