x = 3;
y = 10;

print("x = " .. x .. " and y = " .. y);
-- this will print "x = 3 and y = 10"
-- while true do
	print("Calling add and subtract function...");
	a, b = addNSubYo(x, y, 5);
	-- this calls the C function "addAndSubtract()"
--end
print("a = " .. a .. " and b = " .. b);
-- this will print "a = 6 and b = 5"