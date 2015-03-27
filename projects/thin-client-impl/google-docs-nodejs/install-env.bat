rem need install Node.js and add path
call npm config delete proxy 
call npm config delete https-proxy 

rem dependences
call npm install mocha
call npm install googleapis
rem call npm install request
call npm install underscore
call npm install chai
rem call npm install net-snmp