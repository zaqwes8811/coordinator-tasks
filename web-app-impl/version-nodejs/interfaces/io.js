/**
 * IO-abstractions
 */


function BaseAccessor(username, password)  {}
BaseAccessor.prototype.makeBaseAuthGet = function (url, done, payload) { throw new Error("NI"); };
BaseAccessor.prototype.makeNoAuthGet = function (action) { throw new Error("NI"); };;
BaseAccessor.prototype.post = function (payload, action) { throw new Error("NI"); };


//@NoGAS
module.exports = {'BaseAccessor': BaseAccessor};
