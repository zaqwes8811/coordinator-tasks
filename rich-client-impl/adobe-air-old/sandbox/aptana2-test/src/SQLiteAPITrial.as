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
		
		// onEvent
		// DB
		public function openDB():void {
		  _conn = new SQLConnection();
          _conn.addEventListener(SQLEvent.OPEN, _openHandler);
          _conn.addEventListener(SQLErrorEvent.ERROR, _errorHandler);
          _dbFile= File.applicationStorageDirectory.resolvePath("DBSample.db");
          _conn.openAsync(_dbFile);  // ! Асинхронно
		}
		private function _openHandler(event:SQLEvent):void {
          trace("the database was created successfully");
        }
        private function _errorHandler(event:SQLErrorEvent):void {
          trace("Error message:", event.error.message);
          trace("Details:", event.error.details);
        }
		
		// Create Table
		public function createTable():void 	{
			var createStmt:SQLStatement = new SQLStatement();
            createStmt.sqlConnection = _conn;
            var sql:String =
              "CREATE TABLE IF NOT EXISTS TestTablex (" +
              " _ID_NOTE INTEGER PRIMARY KEY AUTOINCREMENT, " +
              " TimePoint TEXT, " +  // name - type - ограничения
              " TextContent TEXT, " +
              ")";
            createStmt.text = sql;
            createStmt.addEventListener(SQLEvent.RESULT, createResult);
            createStmt.addEventListener(SQLErrorEvent.ERROR, createError);
            createStmt.execute();
		}
		private function createResult(event:SQLEvent):void {
          trace("Table created");
        }
        private function createError(event:SQLErrorEvent):void {
          trace("Error message:", event.error.message);
          trace("Details:", event.error.details);
        }
		
		// скорее всего будет просто функциями
		//
		private var _selectStmt:SQLStatement;
		public function SQLiteAPITrial() { }
		// insert
		public function TestInsert():void 
		{
			var insertStmt:SQLStatement = new SQLStatement();
            insertStmt.sqlConnection = _conn;
			// define the SQL text
			var sql:String =
			  "INSERT INTO employees (firstName, lastName, salary) " +
              "VALUES ('Bob', 'Smith', 8000)";
            insertStmt.text = sql;
			// register listeners for the result and failure (status) events
			insertStmt.addEventListener(SQLEvent.RESULT, insertResult);
			insertStmt.addEventListener(SQLErrorEvent.ERROR, insertError);
			// execute the statement
			insertStmt.execute();
		}
		private function insertResult(event:SQLEvent):void
        {
			trace("INSERT statement succeeded");
		}

		private function insertError(event:SQLErrorEvent):void
		{ 
			trace("Error message:", event.error.message);
            trace("Details:", event.error.details);
        }
		// Select
		public function select( column:String ):void 
		{
			_selectStmt = new SQLStatement();
			_selectStmt.sqlConnection = _conn;
			// define the SQL text
			var sql:String =
			  "SELECT "+ column + " " +
			  "FROM employees";
			  _selectStmt.text = sql;
			  // register listeners for the result and error events
			  _selectStmt.addEventListener(SQLEvent.RESULT, selectResult);
			  _selectStmt.addEventListener(SQLErrorEvent.ERROR, selectError);
			  // execute the statement
			  _selectStmt.execute();
		}
		private function selectResult(event:SQLEvent):void
		{
          // access the result data
		  var result:SQLResult = _selectStmt.getResult();
		  var numRows:int = result.data.length;
		  for (var i:int = 0; i < numRows; i++)
		  {
			  var output:String = "";
              for (var columnName:String in result.data[i])
			  {
                 output += columnName + ": " + result.data[i][columnName] + "; ";
              }
              trace("row[" + i.toString() + "]\t", output);
          }
        }
        private function selectError(event:SQLErrorEvent):void
        {
          trace("Error message:", event.error.message);
		  trace("Details:", event.error.details);
        }
	}

}