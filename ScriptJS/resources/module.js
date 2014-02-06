var anotherModule = require("anotherModule");

var Util = {
    sort : function(a,b){
        return a > b ? a : b;
    },
    expose : function(){
        anotherModule.expose();
    },
    
    spotted : function(){
        anotherModule.spotted();
    },
    
    logWaldo : function(){
        anotherModule.report();
    }
}


module.exports = Util;