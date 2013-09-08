// nativePath = "C:\Documents and Settings\igor\Application Data\NewProject\Local Store\test.db"
/**
 * Может синфронный запросы делать
 *   пусть пока асинхронные птом будет потом
 * 
 * */
package  
{
  import flash.data.SQLConnection;
  import flash.data.SQLStatement;
  import flash.data.SQLResult;
  import flash.events.SQLErrorEvent;
  import flash.errors.SQLError;
  import flash.events.SQLEvent;
  import flash.filesystem.File;
	/**
	 * Класс для исследования возможностей SQLite базы данных

	 */
	public class SQLiteAPITrial 
	{
		// DB
		private var _dbFile:File;
		private var _conn:SQLConnection;
		private var _TableName:String = "TestTable";
		
		// onEvent
		// DB
		public function openDB():void {
		  _conn = new SQLConnection();
          _dbFile= File.applicationStorageDirectory.resolvePath("DBSample.db");
		  try {
            _conn.open(_dbFile);
            trace("the database was created successfully");
          }
          catch (error:SQLError) {
			trace("Error message:", error.message);
            trace("Details:", error.details);
          }
		}
		
		// Create Table
		public function createTable():void 	{
			var createStmt:SQLStatement = new SQLStatement();
            createStmt.sqlConnection = _conn;
            var sql:String =
              "CREATE TABLE IF NOT EXISTS "+ _TableName +  // сама таблица
              " (_ID_NOTE INTEGER PRIMARY KEY AUTOINCREMENT, ";  // первичный ключ
			
			// Именуем столбцы  
			sql += 
			  " TimePoint TEXT, " +  // отметка времени
              " TextContent TEXT " +  // текстовое содержание
              ")";
			  
			// Записываем комманду в отправляющий объект
            createStmt.text = sql;
			
			// в случае синхронного открытия используются исключения
			try {
              createStmt.execute();
              trace("Table created");
            }
            catch (error:SQLError) {
              trace("Error message:", error.message);
              trace("Details:", error.details);
			 }
		}
		
		/// /// ///
		///
		/// скорее всего будет просто функциями
		private var _selectStmt:SQLStatement;
		public function SQLiteAPITrial() { }
		
		// insert
		public function TestInsert( sql:String ):void 
		{
			var insertStmt:SQLStatement = new SQLStatement();
            insertStmt.sqlConnection = _conn;
			insertStmt.text = sql;
			
			// пытаемся выполнить
			try {
              insertStmt.execute();
              // accessing the data is shown in a subsequent code listing
             }
             catch (error:SQLError) {
              trace("error handling is shown in a subsequent code listing");
            }
		}
		
		// Select - Read
		public function TestSelect( column:String ):void 
		{
			_selectStmt = new SQLStatement();
			_selectStmt.sqlConnection = _conn;
			// define the SQL text
			var sql:String =
			  "SELECT "+ column + ", " + "TextContent " + 
			  "FROM " + _TableName + " " +
			  "WHERE " + column + " LIKE 'Bob%'";
			_selectStmt.text = sql;
			trace(sql);
			  
			//
			try {
              _selectStmt.execute();
              var result:SQLResult = _selectStmt.getResult();
              var numResults:int = result.data.length;
              for (var i:int = 0; i < numResults; i++) {
                var row:Object = result.data[i];
                var output:String = "TimePoint: " + row.TimePoint;
				output += " TextContent :" + row.TextContent;
                trace(output);
              }
            }
            catch (error:SQLError) {
                // Information about the error is available in the
                // error variable, which is an instance of
                // the SQLError class.
				trace( "select error" );
            }
	    }
		
		/// 
		public function TestUpdate ( column:String ) : void {
			_selectStmt = new SQLStatement();
			_selectStmt.sqlConnection = _conn;
			// define the SQL text
			var sql:String =
			  "UPDATE " + _TableName + " " +  // Имя таблицы
			    "SET " + //column + " = '', " + 
			    "TextContent = 'xxx' " + 
			  "WHERE " + column + " LIKE 'Bob%'";
			_selectStmt.text = sql;
			trace(sql);
			  
			//
			try {
              _selectStmt.execute();
            }
            catch (error:SQLError) {
                // Information about the error is available in the
                // error variable, which is an instance of
                // the SQLError class.
				trace( "update error" );
            }
		}
		
		//
		public function TestDelete() : void {
			
		}
	}  // public class SQLiteAPI...
}