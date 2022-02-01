// Speed up calls to hasOwnProperty
var hasOwnProperty = Object.prototype.hasOwnProperty;

function isEmpty(obj) {

    // null and undefined are "empty"
    if (obj == null) return true;

    // Assume if it has a length property with a non-zero value
    // that that property is correct.
    if (obj.length && obj.length > 0)    return false;
    if (obj.length === 0)  return true;

    // Otherwise, does it have any properties of its own?
    // Note that this doesn't handle
    // toString and toValue enumeration bugs in IE < 9
    for (var key in obj) {
        if (hasOwnProperty.call(obj, key)) return false;
    }

    return true;
}

//log(options['key']);
options['key'] = 'value2';
log(options);

var tmp = {'asf': 'asfd'};
log(tmp)
log(Object.keys(options));
for (var index in options) { 
  log(options);
   if (tmp.hasOwnProperty(index)) {
       var attr = tmp[index];
       log(attr);
   }
}

function Process() {
	var that = this;
	for (var key in that) {
		log(key);
	}
	
	log(that.options.toString());
	log(that.output.toString());
	log(isEmpty(that.output).toString());
	
	log("MSG: Show options");
	
	for (var fields in that.options) {
		log(fields);
	}
}